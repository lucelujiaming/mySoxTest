#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef WIN32
# include <windows.h>
#include <time.h>
#include <process.h>
#else
#include <sys/time.h>
#include <pthread.h>
#endif
#include "modbus.h"
#include "board.h"
#include "list.h"

#ifdef WIN32
extern unsigned __int64 get_tick_ms(void);
#else
extern unsigned long long get_tick_ms(void);
#endif

#define MAX_ERROR_COUNT 3
#define MODBUS_READ_BLOCK_SIZE 64

//#define DEBUG_LIST
#ifdef DEBUG_LIST
static int mem_count;
#endif

typedef struct {
    list_node_t node;
    unsigned short addr;

    unsigned char endian:1;
    unsigned char is_force_update:1;
    unsigned char is_mutli_err:1;
    unsigned char err_cnt;

    unsigned short val;
    unsigned short val_w;
} element_t;

typedef struct {
    list_node_t node;
    unsigned char addr;

    unsigned int refresh_ms;
#ifdef WIN32
    unsigned __int64 next_update;
	unsigned __int64 last_err;
#else
    unsigned long long next_update;
	unsigned long long last_err;
#endif
	unsigned int spent_time_ms;

    list_head_t DO; // 00001, list of element_t.
    list_head_t DI; // 10001
    list_head_t INPUT; // 30001
    list_head_t HOLD; // 40001
} device_t;

typedef struct {
    list_node_t node;
    device_t *dev;
    element_t *reg;
} req_t;

#ifdef WIN32
#define LOCK_WRITE_QUEUE while (c->write_queue_lock != 0) Sleep(10); c->write_queue_lock = 1
#else
#define LOCK_WRITE_QUEUE while (c->write_queue_lock != 0) usleep(10*1000); c->write_queue_lock = 1
#endif
#define UNLOCK_WRITE_QUEUE c->write_queue_lock = 0
typedef struct
{
    modbus_t *ctx_modbus;
#ifdef WIN32
	HANDLE ctx_thread;
#else
	pthread_t ctx_thread;
#endif
    unsigned char ctx_thread_running;
    unsigned char ctx_added;
    unsigned int retry_delay_ms;

    list_head_t devices; // list of device_t

    int write_queue_lock;
    list_head_t write_queue; // list of req_t
} context_t;
static context_t context_table[MAX_MODBUSTCPSLAVE_NUM];

static device_t* get_next_read_device(context_t *c)
{
	device_t *device = NULL;
	list_node_t *dev_node = NULL;
#ifdef WIN32
    unsigned __int64 curr_tick = get_tick_ms();
    unsigned __int64 next_tick = curr_tick;
#else
    unsigned long long curr_tick = get_tick_ms();
    unsigned long long next_tick = curr_tick;
#endif

    dev_node = c->devices.first;
    while (c->ctx_thread_running != 0 && dev_node != NULL) {
#ifdef WIN32
		device = (device_t*)(dev_node) ; 
#else
        device = list_entry_safe(dev_node, device_t);
#endif
        if (device->next_update < next_tick)
            next_tick = device->next_update;
        dev_node = dev_node->next;
    }

    if (c->ctx_thread_running != 0 && next_tick != curr_tick) {
        dev_node = c->devices.first;
        while (dev_node != NULL) {
#ifdef WIN32
			device = (device_t*)(dev_node) ; 
#else
			device = list_entry_safe(dev_node, device_t);
#endif
            if (device->next_update == next_tick) {
#ifdef WIN32
                device->next_update = get_tick_ms() + (unsigned __int64)device->refresh_ms;
#else
                device->next_update = get_tick_ms() + (unsigned long long)device->refresh_ms;
#endif
                return device;
            }
            dev_node = dev_node->next;
        }
    }

    return NULL;
}

static inline void exec_one_reg_write_from_list(context_t *c)
{
	req_t *req = NULL;
	device_t *device = NULL;
	element_t *element = NULL;
	unsigned char devid = '\0';
	unsigned short address = '\0';

	list_node_t *node = NULL;
	
    if (c->ctx_thread_running != 0 && c->write_queue.len > 0) {
        LOCK_WRITE_QUEUE;
        node = list_get(&c->write_queue);
#ifdef WIN32
		req = (req_t*)(node) ; 
#else
		req = list_entry_safe(node, req_t);
#endif
        UNLOCK_WRITE_QUEUE;
        device = req->dev;
        element = req->reg;
		
        if (element->err_cnt != MAX_ERROR_COUNT) {
            devid = device->addr;
            address = element->addr;
            if (address > 0 && address < 10000) {
                modbus_set_slave(c->ctx_modbus, devid);
                modbus_write_bit(c->ctx_modbus, address - 1, element->val_w);
            } else if (address > 40000 && address < 50000) {
                modbus_set_slave(c->ctx_modbus, devid);
                modbus_write_register(c->ctx_modbus, address - 40001, element->val_w);
            }
        } else {
            element->is_force_update = 1;
        }
        free(req);
#ifdef DEBUG_LIST
        mem_count--;
#endif
    }
}

static inline void exec_single_element_read(context_t *c, device_t *device, element_t *element)
{
    unsigned char devid = device->addr;
    unsigned short address = element->addr;
    if (address > 0 && address < 10000) {
        unsigned char v;
        modbus_set_slave(c->ctx_modbus, devid);
        if (modbus_read_bits(c->ctx_modbus, address - 1, 1, &(v)) > 0) {
            element->val = v;
            element->err_cnt = 0;
        } else {
            if (element->err_cnt < MAX_ERROR_COUNT) element->err_cnt ++;
        }
    } else if (address > 10000 && address < 20000) {
        unsigned char v;
        modbus_set_slave(c->ctx_modbus, devid);
        if (modbus_read_input_bits(c->ctx_modbus, address - 10001, 1, &(v)) > 0) {
            element->val = v;
            element->err_cnt = 0;
        } else {
            if (element->err_cnt < MAX_ERROR_COUNT) element->err_cnt ++;
        }
    } else if (address > 30000 && address < 40000) {
        unsigned short v;
        modbus_set_slave(c->ctx_modbus, devid);
        if (modbus_read_input_registers(c->ctx_modbus, address - 30001, 1, &(v)) > 0) {
            element->val = v;
            element->err_cnt = 0;
        } else {
            if (element->err_cnt < MAX_ERROR_COUNT) element->err_cnt ++;
        }
    } else if (address > 40000 && address < 50000) {
        unsigned short v;
        modbus_set_slave(c->ctx_modbus, devid);
        if (modbus_read_registers(c->ctx_modbus, address - 40001, 1, &(v)) > 0) {
            element->val = v;
            element->err_cnt = 0;
        } else {
            if (element->err_cnt < MAX_ERROR_COUNT) element->err_cnt ++;
        }
    }
}

static inline void exec_one_list_read(context_t *c, device_t *device, list_head_t *list)
{
	element_t *curr_element = NULL;
    list_node_t *curr = list->first;
	
    while (c->ctx_thread_running != 0 && curr != NULL) {
#ifdef WIN32
		curr_element = (element_t*)(curr) ; 
#else
		curr_element = list_entry_safe(curr, element_t);
#endif
        exec_single_element_read(c, device, curr_element);
        exec_one_reg_write_from_list(c);
        curr = curr->next;
    }
}

static inline int is_device_offline(device_t *device)
{
	element_t *element = NULL;
    list_node_t *reg_node = device->DO.first;
    while (reg_node != NULL) {
#ifdef WIN32
		element = (element_t*)(reg_node) ; 
#else
		element = list_entry_safe(reg_node, element_t);
#endif
        if (element->err_cnt != MAX_ERROR_COUNT) return 0;
        reg_node = reg_node->next;
    }
    reg_node = device->DI.first;
    while (reg_node != NULL) {
#ifdef WIN32
		element = (element_t*)(reg_node) ; 
#else
		element = list_entry_safe(reg_node, element_t);
#endif
        if (element->err_cnt != MAX_ERROR_COUNT) return 0;
        reg_node = reg_node->next;
    }
    reg_node = device->INPUT.first;
    while (reg_node != NULL) {
#ifdef WIN32
		element = (element_t*)(reg_node) ; 
#else
		element = list_entry_safe(reg_node, element_t);
#endif
        if (element->err_cnt != MAX_ERROR_COUNT) return 0;
        reg_node = reg_node->next;
    }
    reg_node = device->HOLD.first;
    while (reg_node != NULL) {
#ifdef WIN32
		element = (element_t*)(reg_node) ; 
#else
		element = list_entry_safe(reg_node, element_t);
#endif
        if (element->err_cnt != MAX_ERROR_COUNT) return 0;
        reg_node = reg_node->next;
    }
    return 1;
}

static inline void exec_one_device_read(context_t *c, device_t *device)
{
#ifdef WIN32
    unsigned __int64 delta ;
#else
    unsigned long long delta ;
#endif
	element_t *element = NULL;
	unsigned char devid = '\0';
	unsigned short address = '\0';
	list_node_t *reg_node = NULL;
	
    if (is_device_offline(device) == 1) {
        
        if (device->DO.first != NULL) {
#ifdef WIN32
			element = (element_t*)(device->DO.first) ; 
#else
            element = list_entry_safe(device->DO.first, element_t);
#endif
        } else if (device->DI.first != NULL) {
#ifdef WIN32
			element = (element_t*)(device->DI.first) ; 
#else
            element = list_entry_safe(device->DI.first, element_t);
#endif
        } else if (device->INPUT.first != NULL) {
#ifdef WIN32
			element = (element_t*)(device->INPUT.first) ; 
#else
            element = list_entry_safe(device->INPUT.first, element_t);
#endif
        } else if (device->HOLD.first != NULL) {
#ifdef WIN32
			element = (element_t*)(device->HOLD.first) ; 
#else
            element = list_entry_safe(device->HOLD.first, element_t);
#endif
        }
        if (element != NULL) {
            exec_single_element_read(c, device, element);
            if (is_device_offline(device) == 1) {
#ifdef WIN32
                device->next_update = device->next_update - (unsigned __int64)device->refresh_ms + c->retry_delay_ms;
#else
                device->next_update = device->next_update - (unsigned long long)device->refresh_ms + c->retry_delay_ms;
#endif
            } else {

				if (c->ctx_thread_running != 0)
                    exec_one_list_read(c, device, &device->DO);
                if (c->ctx_thread_running != 0)
                    exec_one_list_read(c, device, &device->DI);
                if (c->ctx_thread_running != 0)
                    exec_one_list_read(c, device, &device->INPUT);
                if (c->ctx_thread_running != 0)
                    exec_one_list_read(c, device, &device->HOLD);
                reg_node = device->DO.first;
                while (reg_node != NULL) {
#ifdef WIN32
					element = (element_t*)(reg_node) ; 
#else
                    element = list_entry_safe(reg_node, element_t);
#endif
                    if (element->err_cnt == 0 && element->is_force_update == 1) {
                        devid = device->addr;
                        address = element->addr;
                        modbus_set_slave(c->ctx_modbus, devid);
                        modbus_write_bit(c->ctx_modbus, address - 1, element->val_w);
                        element->is_force_update = 0;
                    }
                    reg_node = reg_node->next;
                }
                reg_node = device->HOLD.first;
                while (reg_node != NULL) {
#ifdef WIN32
					element = (element_t*)(reg_node) ; 
#else
                    element = list_entry_safe(reg_node, element_t);
#endif
                    if (element->err_cnt == 0 && element->is_force_update == 1) {
                        devid = device->addr;
                        address = element->addr;
                        modbus_set_slave(c->ctx_modbus, devid);
                        modbus_write_register(c->ctx_modbus, address - 40001, element->val_w);
                        element->is_force_update = 0;
                    }
                    reg_node = reg_node->next;
                }
#ifdef WIN32
                device->next_update = get_tick_ms() + (unsigned __int64)device->refresh_ms;
#else
                device->next_update = get_tick_ms() + (unsigned long long)device->refresh_ms;
#endif
            }
        }
        device->spent_time_ms = 0;
        return;
    }

#ifdef WIN32
    delta = get_tick_ms();
#else
    delta = get_tick_ms();
#endif
    if (c->ctx_thread_running != 0)
        exec_one_list_read(c, device, &device->DO);
    if (c->ctx_thread_running != 0)
        exec_one_list_read(c, device, &device->DI);
    if (c->ctx_thread_running != 0)
        exec_one_list_read(c, device, &device->INPUT);
    if (c->ctx_thread_running != 0)
        exec_one_list_read(c, device, &device->HOLD);

    if (is_device_offline(device) == 1) {
#ifdef WIN32
        device->next_update = device->next_update - (unsigned __int64)device->refresh_ms + c->retry_delay_ms;
#else
        device->next_update = device->next_update - (unsigned long long)device->refresh_ms + c->retry_delay_ms;
#endif
        device->spent_time_ms = 0;
    } else {
        // 统计设备消费时间。
        delta = get_tick_ms() - delta;
        device->spent_time_ms = (unsigned int)delta;
    }
}

static void* thread_modbus_tcp_update(void* arg)
{
	element_t *element = NULL;
	device_t* next_read_device = NULL;
    list_node_t *dev_node = NULL;
    device_t *device = NULL;
	list_node_t *reg_node = NULL;
	req_t *req = NULL;
		
    context_t *c = (context_t *)arg;

    // wait for added event.
    while (c->ctx_thread_running != 0 && c->ctx_added == 0)
    {
 #ifdef WIN32
		Sleep(10); 
#else
		usleep(10*1000);
#endif
    }

    while (c->ctx_thread_running != 0)
    {
        next_read_device = get_next_read_device(c);
        if (next_read_device != NULL) {
            exec_one_device_read(c, next_read_device);
        } else if (c->write_queue.len > 0) {
            exec_one_reg_write_from_list(c);
        } else {
 #ifdef WIN32
			Sleep(10); 
#else
			usleep(10*1000);
#endif
        }
    }

#ifdef DEBUG_LIST
    {
        dev_node = c->devices.first;
        while (dev_node != NULL) {
#ifdef WIN32
			device = (device_t*)(dev_node) ; 
#else
			device = list_entry_safe(dev_node, device_t);
#endif
            printf("Device [%d]\n", device->addr);
            reg_node = device->DO.first;
            while (reg_node != NULL) {
#ifdef WIN32
				element = (element_t*)(reg_node) ; 
#else
				element = list_entry_safe(reg_node, element_t);
#endif
                printf("  %d\n", element->addr);
                reg_node = reg_node->next;
            }
            reg_node = device->DI.first;
            while (reg_node != NULL) {
#ifdef WIN32
				element = (element_t*)(reg_node) ; 
#else
				element = list_entry_safe(reg_node, element_t);
#endif
                printf("  %d\n", element->addr);
                reg_node = reg_node->next;
            }
            reg_node = device->INPUT.first;
            while (reg_node != NULL) {
#ifdef WIN32
				element = (element_t*)(reg_node) ; 
#else
				element = list_entry_safe(reg_node, element_t);
#endif
                printf("  %d\n", element->addr);
                reg_node = reg_node->next;
            }
            reg_node = device->HOLD.first;
            while (reg_node != NULL) {
#ifdef WIN32
				element = (element_t*)(reg_node) ; 
#else
				element = list_entry_safe(reg_node, element_t);
#endif
                printf("  %d\n", element->addr);
                reg_node = reg_node->next;
            }
            dev_node = dev_node->next;
        }
    }
#endif

    // free device regs
    {
        dev_node = list_get(&c->devices);
        while (dev_node) {
#ifdef WIN32
			device = (device_t*)(dev_node) ; 
#else
			device = list_entry_safe(dev_node, device_t);
#endif
            // DO
            reg_node = list_get(&device->DO);
            while (reg_node) {
#ifdef WIN32
				element = (element_t*)(reg_node) ; 
#else
				element = list_entry_safe(reg_node, element_t);
#endif
                free(element);
#ifdef DEBUG_LIST
                mem_count--;
#endif
                reg_node = list_get(&device->DO);
            }
            // DI
            reg_node = list_get(&device->DI);
            while (reg_node) {
#ifdef WIN32
				element = (element_t*)(reg_node) ; 
#else
				element = list_entry_safe(reg_node, element_t);
#endif
                free(element);
#ifdef DEBUG_LIST
                mem_count--;
#endif
                reg_node = list_get(&device->DI);
            }
            // INPUT
            reg_node = list_get(&device->INPUT);
            while (reg_node) {
#ifdef WIN32
				element = (element_t*)(reg_node) ; 
#else
				element = list_entry_safe(reg_node, element_t);
#endif
                free(element);
#ifdef DEBUG_LIST
                mem_count--;
#endif
                reg_node = list_get(&device->INPUT);
            }
            // HOLD
            reg_node = list_get(&device->HOLD);
            while (reg_node) {
#ifdef WIN32
				element = (element_t*)(reg_node) ; 
#else
				element = list_entry_safe(reg_node, element_t);
#endif
                free(element);
#ifdef DEBUG_LIST
                mem_count--;
#endif
                reg_node = list_get(&device->HOLD);
            }
            // final free the device.
            free(device);
#ifdef DEBUG_LIST
            mem_count--;
#endif
            dev_node = list_get(&c->devices);
        }
        memset(&c->devices, 0, sizeof(list_head_t));
    }

    if (c->write_queue.len) {
        list_node_t *node = list_get(&c->write_queue);
        while (node) {
#ifdef WIN32
			req = (req_t*)(node) ; 
#else
			req = list_entry_safe(node, req_t);
#endif
            free(req);
#ifdef DEBUG_LIST
            mem_count--;
#endif
            node = list_get(&c->write_queue);
        }
        memset(&c->write_queue, 0, sizeof(list_head_t));
    }
#ifdef DEBUG_LIST
    printf("MEM COUNT %d\n", mem_count);
#endif

    c->ctx_added = 0;

    modbus_close(c->ctx_modbus);
    modbus_free(c->ctx_modbus);
    c->ctx_modbus = NULL;

#ifndef WIN32
    pthread_exit(NULL);
#endif

    return (void*)NULL;
}

int tcp_read(int ctx_idx, int device_addr, int addr, float *buf, int len)
{
    device_t *device = NULL;
	element_t *next_element = NULL;
	element_t *element = NULL;

	float *p = NULL;

    // read device elapsed time.
    if (addr == 0) {
        context_t *c = &context_table[ctx_idx];
        list_node_t *dev_node = c->devices.first;
        while (dev_node != NULL) {
#ifdef WIN32
			device = (device_t*)(dev_node) ; 
#else
			device = list_entry_safe(dev_node, device_t);
#endif
            if (device->addr == device_addr) {
                if (is_device_offline(device) == 1) {
                    return 0;
                }
                if (device->spent_time_ms > 65535) {
                    buf[0] = 65535;
                } else {
                    buf[0] = (float)device->spent_time_ms;
                }
                return 1;
            }
            dev_node = dev_node->next;
        }
        return 0;
    }

    element = (element_t *)device_addr;
    if (element != NULL && element->addr == addr) {
        if (element->err_cnt == 0) {
            if (len == 1) {
                // int
                short val = (short)element->val;
                buf[0] = (float)val;
                return 1;
            }
            if (len == 2) {
                // word
                buf[0] = (float)element->val;
                return 1;
            }
            if (len == 3) {
                // dint
#ifdef WIN32
				next_element = (element_t*)(element->node.next) ; 
#else
				next_element = list_entry_safe(element->node.next, element_t);
#endif
                if (next_element != NULL && next_element->addr == addr + 1) {
                    int data = 0;
                    if (element->endian != 0) {
                        unsigned int _data = (element->val << 16) | next_element->val;
                        data = (int)_data;
                    } else {
                        unsigned int _data = (next_element->val << 16) | element->val;
                        data = (int)_data;
                    }
                    buf[0] = (float)data;
                    return 2;
                }
            }
            if (len == 4) {
                // dword
#ifdef WIN32
				next_element = (element_t*)(element->node.next) ; 
#else
				next_element = list_entry_safe(element->node.next, element_t);
#endif
                if (next_element != NULL && next_element->addr == addr + 1) {
                    unsigned int data = 0;
                    if (element->endian != 0) {
                        data = (element->val << 16) | next_element->val;
                    } else {
                        data = (next_element->val << 16) | element->val;
                    }
                    buf[0] = (float)data;
                    return 2;
                }
            }
            if (len == 5) {
                // real
#ifdef WIN32
				next_element = (element_t*)(element->node.next) ; 
#else
				next_element = list_entry_safe(element->node.next, element_t);
#endif
                if (next_element != NULL && next_element->addr == addr + 1) {
                    unsigned int data = 0;
                    if (element->endian != 0) {
                        data = (element->val << 16) | next_element->val;
                    } else {
                        data = (next_element->val << 16) | element->val;
                    }
                    p = (float *)((char *)&data);
                    buf[0] = *p;
                    return 2;
                }
            }
        }
    }

    return -1;
}

static inline void write_queue_put(int ctx_idx, element_t *element)
{
	device_t *curr_device = NULL;
	element_t *curr_element = NULL;
	req_t *_req = NULL;
    context_t *c = &context_table[ctx_idx];
    list_head_t *reg_head = NULL;
	list_node_t *req_node = NULL;
    int is_exist = 0;
    // find the device.
    list_node_t *dev_node = c->devices.first;
    device_t *device = NULL;
    while (dev_node != NULL && device == NULL) {
#ifdef WIN32
		curr_device = (device_t*)(dev_node) ; 
#else
		curr_device = list_entry_safe(dev_node, device_t);
#endif
        if (element->addr > 0 && element->addr < 10000) {
            reg_head = &curr_device->DO;
        } else if (element->addr > 10000 && element->addr < 20000) {
            reg_head = NULL;//&curr_device->DI;
        } else if (element->addr > 30000 && element->addr < 40000) {
            reg_head = NULL;//&curr_device->INPUT;
        } else if (element->addr > 40000 && element->addr < 50000) {
            reg_head = &curr_device->HOLD;
        }
        if (reg_head != NULL) {
            list_node_t *reg_node = reg_head->first;
            while (reg_node != NULL) {
#ifdef WIN32
				curr_element = (element_t*)(reg_node) ; 
#else
				curr_element = list_entry_safe(reg_node, element_t);
#endif
                if (curr_element == element) {
                    device = curr_device;
                    break;
                }
                reg_node = reg_node->next;
            }
        }
        dev_node = dev_node->next;
    }

    if (device == NULL) {
        return;
    }

    LOCK_WRITE_QUEUE;
    req_node = c->write_queue.first;
    while (req_node != NULL) {
#ifdef WIN32
		_req = (req_t*)(req_node) ; 
#else
		_req = list_entry_safe(req_node, req_t);
#endif
        if (_req->dev == device && _req->reg == element) {
            is_exist = 1;
            break;
        }
        req_node = req_node->next;
    }
    if (is_exist == 0) {
        req_t *req = malloc(sizeof(req_t));
        if (req != NULL) {
            memset(req, 0, sizeof(req_t));
            req->dev = device;
            req->reg = element;
            list_put(&c->write_queue, &req->node);
#ifdef DEBUG_LIST
            mem_count++;
#endif
        }
    }
    UNLOCK_WRITE_QUEUE;
}

int tcp_write(int ctx_idx, int device_addr, int addr, float *buf, int len)
{
	element_t *next_element = NULL;
	element_t *element =  NULL;
	unsigned int data = 0;
	unsigned int _data = 0;
	short short_data = 0;
    int result = -1;

    if ((addr > 10000 && addr < 20000) || (addr > 30000 && addr < 40000)) {
        return result;
    }

    element = (element_t *)device_addr;
    if (element != NULL && element->addr == addr) {
        if (addr < 20000) {
            if (buf[0] == 0) {
                element->val_w = 0;
            } else {
                element->val_w = 1;
            }
            if (element->err_cnt != MAX_ERROR_COUNT) {
                write_queue_put(ctx_idx, element);
                result = 1;
            } else {
                element->is_force_update = 1;
            }
        } else {
            if (len == 1) {
                // int
                short_data = (short)buf[0];
                element->val_w = (unsigned short)short_data;
                if (element->err_cnt != MAX_ERROR_COUNT) {
                    write_queue_put(ctx_idx, element);
                    result = 1;
                } else {
                    element->is_force_update = 1;
                }
            }
            if (len == 2) {
                // word
                element->val_w = (unsigned short)buf[0];
                if (element->err_cnt != MAX_ERROR_COUNT) {
                    write_queue_put(ctx_idx, element);
                    result = 1;
                } else {
                    element->is_force_update = 1;
                }
            }
            if (len == 3) {
                // dint
#ifdef WIN32
				next_element = (element_t*)(element->node.next) ; 
#else
				next_element = list_entry_safe(element->node.next, element_t);
#endif
                if (next_element != NULL && next_element->addr == addr + 1) {
                    data = (int)buf[0];
                    if (element->endian != 0) {
                        _data = (unsigned int)data;
                        element->val_w = (_data >> 16) & 0xFFFF;
                        next_element->val_w = (_data & 0xFFFF);
                    } else {
                        _data = (unsigned int)data;
                        next_element->val_w = (_data >> 16) & 0xFFFF;
                        element->val_w = (_data & 0xFFFF);
                    }
                    if (element->err_cnt != MAX_ERROR_COUNT) {
                        write_queue_put(ctx_idx, element);
                        write_queue_put(ctx_idx, next_element);
                        result = 2;
                    } else {
                        element->is_force_update = 1;
                        next_element->is_force_update = 1;
                    }
                }
            }
            if (len == 4) {
                // dword
#ifdef WIN32
				next_element = (element_t*)(element->node.next) ; 
#else
				next_element = list_entry_safe(element->node.next, element_t);
#endif
                if (next_element != NULL && next_element->addr == addr + 1) {
                    data = (unsigned int)buf[0];
                    if (element->endian != 0) {
                        element->val_w = (data >> 16) & 0xFFFF;
                        next_element->val_w = (data & 0xFFFF);
                    } else {
                        next_element->val_w = (data >> 16) & 0xFFFF;
                        element->val_w = (data & 0xFFFF);
                    }
                    if (element->err_cnt != MAX_ERROR_COUNT) {
                        write_queue_put(ctx_idx, element);
                        write_queue_put(ctx_idx, next_element);
                        result = 2;
                    } else {
                        element->is_force_update = 1;
                        next_element->is_force_update = 1;
                    }
                }
            }
            if (len == 5) {
                // real
#ifdef WIN32
				next_element = (element_t*)(element->node.next) ; 
#else
				next_element = list_entry_safe(element->node.next, element_t);
#endif
                if (next_element != NULL && next_element->addr == addr + 1) {
                    data = *((unsigned int *)buf);
                    if (element->endian != 0) {
                        element->val_w = (data >> 16) & 0xFFFF;
                        next_element->val_w = (data & 0xFFFF);
                    } else {
                        next_element->val_w = (data >> 16) & 0xFFFF;
                        element->val_w = (data & 0xFFFF);
                    }
                    if (element->err_cnt != MAX_ERROR_COUNT) {
                        write_queue_put(ctx_idx, element);
                        write_queue_put(ctx_idx, next_element);
                        result = 2;
                    } else {
                        element->is_force_update = 1;
                        next_element->is_force_update = 1;
                    }
                }
            }
        }
    }

    return result;
}

static inline void list_ordered_put(list_head_t *head, element_t *element)
{
	element_t *first_elem = NULL;
    list_node_t *prev = NULL;
	list_node_t *reg_node = NULL;
	element_t *next_elem = NULL;

    if (head->len == 0) {
        list_put(head, &element->node);
    } else {
#ifdef WIN32
		first_elem = (element_t*)(head->first) ; 
#else
		first_elem = list_entry_safe(head->first, element_t);
#endif
        if (first_elem->addr > element->addr) {
            list_put_begin(head, &element->node);
        } else {
            reg_node = head->first;
            while (reg_node != NULL) {
                if (reg_node->next == NULL) {
                    prev = reg_node;
                    break;
                }
#ifdef WIN32
				next_elem = (element_t*)(reg_node->next) ; 
#else
				next_elem = list_entry_safe(reg_node->next, element_t);
#endif
                if (next_elem->addr > element->addr) {
                    prev = reg_node;
                    break;
                }
                reg_node = reg_node->next;
            }
            if (prev == head->first) {
                list_put(head, &element->node);
            } else {
                head->len++;
                element->node.next = prev->next;
                prev->next = &element->node;
                if (prev == head->last) {
                    head->last = &element->node;
                }
            }
        }
    }

#ifdef LIST_CHECK
    list_check(head);
#endif
}

int tcp_add(int ctx_idx, int device_addr, int addr, int len, int refreshms)
{
	device_t *_device = NULL;
	list_node_t *reg_node = NULL;
	
    int register_node_addr = 0;
    int is_new_node = 1;
    // search the list to check is the node is exist.
    list_head_t *reg_head = NULL;

    device_t *device = NULL;
	list_node_t *dev_node = NULL;
	element_t *element = NULL;

    context_t *c = &context_table[ctx_idx];
    int endian = (device_addr >> 8) & 0xFF;
    device_addr = device_addr & 0xFF;

    if (device_addr == 0 && addr == 0 && len == 0 && refreshms == 0) {
        // add END.
        c->ctx_added = 1;
        return 0;
    } else if (addr == 0 && len == 0) {
        // add device node.
        // find the device by device_addr
        device_t *device = NULL;
        list_node_t *dev_node = c->devices.first;
        while (dev_node != NULL) {
#ifdef WIN32
			_device = (device_t*)(dev_node) ; 
#else
			_device = list_entry_safe(dev_node, device_t);
#endif
            if (_device->addr == device_addr) {
                device = _device;
                break;
            }
            dev_node = dev_node->next;
        }
        if (device != NULL) {
            return -1;
        }
        // if not found, create it.
        device = malloc(sizeof(device_t));
        if (device == NULL) {
            return -1;
        }
#ifdef DEBUG_LIST
        mem_count++;
#endif
        memset(device, 0, sizeof(device_t));
        device->addr = device_addr;
        device->refresh_ms = refreshms;
        list_put(&c->devices, &device->node);
        return 0;
    }

    // add register node
    // find the device by device_addr
    dev_node = c->devices.first;
    while (dev_node != NULL) {
#ifdef WIN32
		_device = (device_t*)(dev_node) ; 
#else
		_device = list_entry_safe(dev_node, device_t);
#endif
        if (_device->addr == device_addr) {
            device = _device;
            break;
        }
        dev_node = dev_node->next;
    }

    if (device == NULL) {
        return 0;
    }

    if (addr > 0 && addr < 10000) {
        reg_head = &device->DO;
    } else if (addr > 10000 && addr < 20000) {
        reg_head = &device->DI;
    } else if (addr > 30000 && addr < 40000) {
        reg_head = &device->INPUT;
    } else if (addr > 40000 && addr < 50000) {
        reg_head = &device->HOLD;
    }
    if (reg_head != NULL) {
        reg_node = reg_head->first;
        while (reg_node != NULL) {
#ifdef WIN32
			element = (element_t*)(reg_node) ; 
#else
			element = list_entry_safe(reg_node, element_t);
#endif
            if (element->addr == addr) {
                register_node_addr = (int)element;
                is_new_node = 0;
                break;
            }
            reg_node = reg_node->next;
        }
    }
    if (is_new_node == 1 && reg_head != NULL) {
        element_t *element = malloc(sizeof(element_t));
        if (element == NULL) {
            return 0;
        }
#ifdef DEBUG_LIST
        mem_count++;
#endif
        memset(element, 0, sizeof(element_t));
        element->addr = addr;
        element->endian = (endian == 0 ? 0 : 1);
        list_ordered_put(reg_head, element);
        register_node_addr = (int)element;
    }

    return register_node_addr;
}

int tcp_open(char *ip, int port)
{
    int i;
    int ctx_idx = -1;
    context_t *c;
	modbus_t* ctx = NULL;

    for (i=0; i<MAX_MODBUSTCPSLAVE_NUM; i++) {
        c = &context_table[i];
        if(c->ctx_modbus == NULL && c->ctx_thread_running == 0){
            ctx_idx = i;
            break;
        }
    }

    if(ctx_idx < 0 || ctx_idx >= MAX_MODBUSTCPSLAVE_NUM){
        return -1;
    }

    c = &context_table[ctx_idx];
    if(c->ctx_modbus != NULL || c->ctx_thread_running != 0){
        return -1;
    }

    ctx = modbus_new_tcp(ip, port);
    if(ctx == NULL){
        printf("new tcp err !!! \n");
        return -1;
    }

    modbus_set_debug(ctx, FALSE);
    modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL);

    c->ctx_modbus = ctx;
    memset(&c->devices, 0, sizeof(list_head_t));

    c->ctx_thread_running = 1;
#ifdef WIN32
	c->ctx_thread =
			(HANDLE)_beginthreadex(NULL, 0, c, thread_modbus_tcp_update, 0, NULL);
#else
    pthread_create(&(c->ctx_thread), NULL, thread_modbus_tcp_update, c);
#endif // WIN32

    return ctx_idx;
}

int tcp_close(int ctx_idx)
{
    context_t *c = &context_table[ctx_idx];
    if (c->ctx_modbus == NULL || c->ctx_thread_running == 0) {
        return -1;
    }

    c->ctx_thread_running = 0;
#ifdef WIN32
	CloseHandle( c->ctx_thread );  
#else
    pthread_join(c->ctx_thread, NULL);
#endif // WIN32

    while (c->ctx_modbus != NULL) {
 #ifdef WIN32
		Sleep(10); 
#else
		usleep(10*1000);
#endif
    }

    return 0;
}

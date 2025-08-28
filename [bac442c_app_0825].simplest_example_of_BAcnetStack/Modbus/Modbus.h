#ifndef __MODBUS_H__
#define __MODBUS_H__

#include <stdint.h>
#include <stdbool.h>

#define MODBUS_BROADCAST_ADDR	0
#define MODBUS_RTU_MAX_FRAME	255

//define exception code
#define ILLEGAL_FUNCTION	0x01
#define ILLEGAL_DATA_ADDR	0x02
#define ILLEGAL_DATA_VALUE	0x03
#define S_DEVICE_FAILURE	0x04
#define ACK					0x05
#define S_DEVICE_BUSY		0x06
#define NACK				0x07
#define MEMORY_PARITY_ERROR	0x08

//define function code
#define CMD_READ_OUTPUT_COIL		0x01
#define CMD_READ_INPUT_COIL			0x02
#define CMD_READ_HOLDING_REGISTER	0x03
#define CMD_READ_INPUT_REGISTER		0x04
#define CMD_FORCE_SINGLE_COIL		0x05
#define CMD_PRESET_SINGLE_REGISTER  0x06
#define CMD_READ_EXCEPTION_STATUS	0x07
#define CMD_DIAGNOSTICS				0x08

#define CMD_FETCH_COMM_EVENT_COUNTER	0x0B
#define CMD_FETCH_COMM_EVENT_LOG		0x0C
#define CMD_PROGRAM_CONTROLLER			0x0D
#define CMD_POLL_CONTROLLER				0x0E
#define CMD_FORCE_MULTIPLE_COILS		0x0F
#define CMD_PRESET_MULTIPLE_REGISTERS	0x10
#define CMD_REPORT_SLAVE_ID				0x11

#define CMD_READ_GENERAL_REFERENCE		0x14
#define CMD_WRITE_GENERAL_REFERENCE		0x15
#define CMD_MASK_WRITE_REGISTER			0x16
#define CMD_MASK_READ_REGISTER			0x17

//User define command
#define CMD_READ_ADDRESS				0x41
#define CMD_WRITE_ADDRESS				0x42
#define CMD_READ_CONFIG					0x43
#define CMD_WRITE_CONFIG				0x44
///////////////////////////////////////////////////
#define OCOIL_BASE	(uint16_t)1
#define OCOIL_TOP	(uint16_t)9999
#define OCOIL_EXT_BASE	(uint32_t)1
#define OCOIL_EXT_TOP	(uint32_t)65535

#define ICOIL_BASE	(uint16_t)10001
#define ICOIL_TOP	(uint16_t)19999
#define ICOIL_EXT_BASE	(uint32_t)100001
#define ICOIL_EXT_TOP	(uint32_t)165535

#define IREG_BASE	(uint16_t)30001
#define IREG_TOP	(uint16_t)39999
#define IREG_EXT_BASE	(uint32_t)300001
#define IREG_EXT_TOP	(uint32_t)365535

#define HREG_BASE	(uint16_t)40001
#define HREG_TOP	(uint16_t)49999
#define HREG_EXT_BASE	(uint32_t)400001
#define HREG_EXT_TOP	(uint32_t)465535

#define INVALID_ADDR	(uint32_t)0xFFFFFFFF

typedef enum
{
	OCOIL_TYPE= 0,
	ICOIL_TYPE= 1,
	IREG_TYPE= 3,
	HREG_TYPE= 4,
	INVALID_TYPE= -1
}REG_TYPE;

typedef enum
{
	MB_NULL= 0,
	MB_RTU= 1,
	MB_ASCII= 2,
	MB_TCP= 3
}MB_TYPE;

typedef struct 
{
	uint32_t unit_addr;
	uint8_t reg_type;
	uint16_t reg_addr;
	uint16_t reg_pts;
}REG_ADDRESS;	//10

uint16_t mb_rtu_get_crc(uint8_t *buf, uint8_t len);

int16_t mb_frame_npdu_decode(MB_TYPE bus, 
														uint8_t *pdu, 
														uint8_t *frame, 
														int16_t frm_len, 
														uint16_t *invokeID);

int16_t mb_frame_npdu_encode(MB_TYPE bus, 
														uint8_t *frame, 
														uint8_t *pdu, 
														int16_t pdu_len, 
														uint16_t invokeID);

#endif

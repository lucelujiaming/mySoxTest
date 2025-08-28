/**************************************************************************
*
* Copyright (C) 2005 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "address.h"
#include "bacdef.h"
#include "bacapp.h"
#include "datalink.h"
#include "bacdcode.h"
#include "npdu.h"
#include "apdu.h"
#include "tsm.h"
#include "device.h"
#include "arf.h"
#include "awf.h"
#include "rp.h"
#include "wp.h"
#include "handlers.h"
#include "bacfile.h"

#include "variables.h"
#include "mb_var_def.h"
#include "modbus.h"
#include "flashupdate.h"

#define MAX_BACFILE    4

#define MAX_BACBET_FILE_ITEM    4
#define MAX_DEVICE_FILE_ITEM    25
#define MAX_UPDATE_FILE_ITEM    7

#define MAX_FILE_BUFF        MAX_DEVICE_FILE_ITEM
uint16_t file_buff[MAX_FILE_BUFF];

typedef struct
{
    uint32_t offset;
    uint16_t addr;
} BACNET_FILE_ITEM;

typedef struct 
{
    uint32_t instance;
    char *name;
    char *descript;
    char *type;
    uint16_t records;
    const BACNET_FILE_ITEM* item;
} BACNET_FILE_LIST;

const BACNET_FILE_ITEM File_BACNet_ItemList[MAX_BACBET_FILE_ITEM]=
{
    {0, HR_INSTANCE_L},
    {1, HR_INSTANCE_H},
    {2, HR_MAX_INFO_FRAME},
    {3, HR_MAX_MASTER},
};

const BACNET_FILE_ITEM File_Device_ItemList[MAX_DEVICE_FILE_ITEM]=
{
    {0, HR_UNIT_MAC},
    {1, HR_USART_BAUD},
    {2, HR_USART_TYPE},
    
    {3, HR_DUCT_DP_COEFF},
    {4, HR_DUCT_DP_OFFSET},
    {5, HR_DUCT_AREA},

    {6, HR_LOCAL_TEMP_TYPE},
    {7, HR_LOCAL_TEMP_OFFSET},
    {8, HR_LOCAL_TEMP_FUNCTION},
    
    {9, HR_HTG_TYPE},
    
    {10, HR_HTG_VAVE_TYPE},
    {11, HR_AUX_FAN_TYPE},
    
    {12, HR_CLG_DB},
    {13, HR_CLG_PB},
    {14, HR_CLG_I},
    
    {15, HR_HTG_DB},
    {16, HR_HTG_PB},
    {17, HR_HTG_I},    
    
    {18, HR_DAT_DB},    
    {19, HR_DAT_PB},    
    {20, HR_DAT_I},    
    
    {21, HR_FLOW_DB},
    {22, HR_FLOW_PB},    
    {23, HR_FLOW_I},    
    {24, HR_FLOW_D},    
};

const BACNET_FILE_ITEM File_Update_ItemList[MAX_UPDATE_FILE_ITEM]=
{
    {0, HR_UPD_CMD},
    {1, HR_UPD_STATUS},
    {2, HR_UPD_FILE_SIZE_L},
    {3, HR_UPD_FILE_SIZE_H},    
    {4, HR_UPD_FILE_CRC},
    {5, HR_UPD_TIME_STATMP_L},
    {6, HR_UPD_TIME_STATMP_H},
};


const BACNET_FILE_LIST BAC_File_List[MAX_BACFILE] = 
{
    {0, "Firmware", "MAIN Firmware", "MAIN", 0, NULL},
    {1, "BACnet Config", "bacnet config", "EEPROM", MAX_BACBET_FILE_ITEM, &File_BACNet_ItemList[0]},
  {2, "Device Config", "device config", "EEPROM", MAX_DEVICE_FILE_ITEM, &File_Device_ItemList[0]},
    {94, "Update", "firmware update", "EEPROM", MAX_UPDATE_FILE_ITEM, &File_Update_ItemList[0]},
};

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int bacfile_Properties_Required[] = {
    PROP_OBJECT_IDENTIFIER,
    PROP_OBJECT_NAME,
    PROP_OBJECT_TYPE,
    PROP_FILE_TYPE,
    PROP_FILE_SIZE,
    PROP_MODIFICATION_DATE,
    PROP_ARCHIVE,
    PROP_READ_ONLY,
    PROP_FILE_ACCESS_METHOD,
    -1
};

static const int bacfile_Properties_Optional[] = {
    PROP_DESCRIPTION,
    -1
};

static const int bacfile_Properties_Proprietary[] = {
    -1
};

void BACfile_Property_Lists(
    const int **pRequired,
    const int **pOptional,
    const int **pProprietary)
{
    if (pRequired)
        *pRequired = bacfile_Properties_Required;
    if (pOptional)
        *pOptional = bacfile_Properties_Optional;
    if (pProprietary)
        *pProprietary = bacfile_Properties_Proprietary;

    return;
}

uint32_t bacfile_index_to_instance(
    unsigned index)
{
    uint32_t instance = BACNET_MAX_INSTANCE + 1;

    if(index >= MAX_BACFILE)
        return instance;
    
    return BAC_File_List[index].instance;
}

uint32_t bacfile_instance_to_index(
    unsigned instance)
{
    uint32_t index = MAX_BACFILE;
    uint16_t i;

    for(i= 0; i< MAX_BACFILE; i++)
    {
        if(BAC_File_List[i].instance == instance)
        {
            return i;
        }
    }

    return index;
}

char *bacfile_name(
    uint32_t instance)
{

    uint32_t index;
    
    index= bacfile_instance_to_index(instance);
    
    if(index < MAX_BACFILE)
    {
        return ((char*)(&BAC_File_List[index].name[0]));
    }

  return NULL;
}

char *bacfile_type(
    uint32_t instance)
{
    uint32_t index;
    
    index= bacfile_instance_to_index(instance);
    
    if(index < MAX_BACFILE)
    {
        return ((char*)(&BAC_File_List[index].type[0]));
    }

  return NULL;
}


char *bacfile_descript(
    uint32_t instance)
{
    uint32_t index;
    
    index= bacfile_instance_to_index(instance);
    
    if(index < MAX_BACFILE)
    {
        return ((char*)(&BAC_File_List[index].descript[0]));
    }

  return NULL;
}

bool bacfile_valid_instance(
    uint32_t object_instance)
{
    uint32_t index;
    
    index= bacfile_instance_to_index(object_instance);
    
    if(index < MAX_BACFILE)
    {
        return true;
    }

  return false;
}

uint32_t bacfile_count(
    void)
{
   return MAX_BACFILE;
}

unsigned bacfile_file_size(
    uint32_t object_instance)
{
    uint32_t index;
    
    index= bacfile_instance_to_index(object_instance);
    
    if(index < MAX_BACFILE)
    {
        return (BAC_File_List[index].records*sizeof(uint16_t));
    }

  return 0;
}

bool bacfile_readonly(
    uint32_t object_instance)
{
    uint32_t index;
    
    index= bacfile_instance_to_index(object_instance);
    
    if(index < MAX_BACFILE)
    {
        if(object_instance== 0)
        {
            return true;
        }
    }

  return false;
}

void bacfile_read_record(
    uint32_t object_instance, 
    uint32_t record_start,
    uint32_t record_count,
    uint16_t *len)
{
    
}

/* return the number of bytes used, or -1 on error */
int bacfile_read_property(
    BACNET_READ_PROPERTY_DATA * rpdata)
{
    int apdu_len = 0;   /* return value */
    BACNET_CHARACTER_STRING char_string;
    BACNET_DATE bdate;
    BACNET_TIME btime;
    uint8_t *apdu = NULL;

    if ((rpdata == NULL) || (rpdata->application_data == NULL) ||
        (rpdata->application_data_len == 0)) 
    {
        return 0;
    }
    apdu = rpdata->application_data;
    switch (rpdata->object_property) 
    {
        case PROP_OBJECT_IDENTIFIER:
            apdu_len =
                encode_application_object_id(&apdu[0], OBJECT_FILE,
                rpdata->object_instance);
            break;
        case PROP_OBJECT_NAME:
            characterstring_init_ansi(&char_string, bacfile_name(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_TYPE:
            characterstring_init_ansi(&char_string, bacfile_type(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_DESCRIPTION:
            characterstring_init_ansi(&char_string, bacfile_descript(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_FILE_TYPE:
                        characterstring_init_ansi(&char_string, bacfile_type(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_FILE_SIZE:
            apdu_len =
                encode_application_unsigned(&apdu[0],
                bacfile_file_size(rpdata->object_instance));
            break;
        case PROP_MODIFICATION_DATE:
            /* FIXME: get the actual value instead of April Fool's Day */
            bdate.year = 2020;  /* AD */
            bdate.month = 1;    /* 1=Jan */
            bdate.day = 1;      /* 1..31 */
            bdate.wday = 1;     /* 1=Monday */
            apdu_len = encode_application_date(&apdu[0], &bdate);
            /* FIXME: get the actual value */
            btime.hour = 0;
            btime.min = 0;
            btime.sec = 0;
            btime.hundredths = 0;
            apdu_len += encode_application_time(&apdu[apdu_len], &btime);
            break;
        case PROP_ARCHIVE:
            /* 12.13.8 Archive
               This property, of type BOOLEAN, indicates whether the File
               object has been saved for historical or backup purposes. This
               property shall be logical TRUE only if no changes have been
               made to the file data by internal processes or through File
               Access Services since the last time the object was archived.
             */
            /* FIXME: get the actual value: note it may be inverse... */
            apdu_len = encode_application_boolean(&apdu[0], true);
            break;
        case PROP_READ_ONLY:
            /* FIXME: get the actual value */
                        apdu_len = encode_application_boolean(&apdu[0], bacfile_readonly(rpdata->object_instance));
            break;
        case PROP_FILE_ACCESS_METHOD:
                        apdu_len =
                encode_application_enumerated(&apdu[0], FILE_STREAM_ACCESS);
            break;
        default:
            rpdata->error_class = ERROR_CLASS_PROPERTY;
            rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
            apdu_len = -1;
            break;
    }

    return apdu_len;
}

/* returns true if successful */
bool bacfile_write_property(
    BACNET_WRITE_PROPERTY_DATA * wp_data)
{
    bool status = false;        /* return value */
    int len = 0;
    BACNET_APPLICATION_DATA_VALUE value;

    if (!bacfile_valid_instance(wp_data->object_instance)) 
        {
        wp_data->error_class = ERROR_CLASS_OBJECT;
        wp_data->error_code = ERROR_CODE_UNKNOWN_OBJECT;
        return false;
    }
    /*  only array properties can have array options */
    if (wp_data->array_index != BACNET_ARRAY_ALL) 
        {
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
        return false;
    }
    
        /* decode the some of the request */
    len =
        bacapp_decode_application_data(wp_data->application_data,
        wp_data->application_data_len, &value);
    if (len < 0) 
        {
        /* error while decoding - a value larger than we can handle */
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
        return false;
    }
        
    /* FIXME: len < application_data_len: more data? */
    switch (wp_data->object_property) {
        case PROP_ARCHIVE:
            /* 12.13.8 Archive
               This property, of type BOOLEAN, indicates whether the File
               object has been saved for historical or backup purposes. This
               property shall be logical TRUE only if no changes have been
               made to the file data by internal processes or through File
               Access Services since the last time the object was archived. */
            status =
                WPValidateArgType(&value, BACNET_APPLICATION_TAG_BOOLEAN,
                &wp_data->error_class, &wp_data->error_code);
            if (status) {
                if (value.type.Boolean) {
                    /* FIXME: do something to wp_data->object_instance */
                } else {
                    /* FIXME: do something to wp_data->object_instance */
                }
            }
            break;
        case PROP_FILE_SIZE:
            /* If the file size can be changed by writing to the file,
               and File_Access_Method is STREAM_ACCESS, then this property
               shall be writable. */
            status =
                WPValidateArgType(&value, BACNET_APPLICATION_TAG_UNSIGNED_INT,
                &wp_data->error_class, &wp_data->error_code);
            if (status) {
                /* FIXME: do something with value.type.Unsigned
                   to wp_data->object_instance */
            }
            break;
        case PROP_OBJECT_IDENTIFIER:
        case PROP_OBJECT_NAME:
        case PROP_OBJECT_TYPE:
        case PROP_DESCRIPTION:
        case PROP_FILE_TYPE:
        case PROP_MODIFICATION_DATE:
        case PROP_READ_ONLY:
        case PROP_FILE_ACCESS_METHOD:
            wp_data->error_class = ERROR_CLASS_PROPERTY;
            wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
            break;
        default:
            wp_data->error_class = ERROR_CLASS_PROPERTY;
            wp_data->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
            break;
    }

    return status;
}

uint32_t bacfile_instance(
    char *name)
{
    uint32_t index = 0;
    uint32_t instance = BACNET_MAX_INSTANCE + 1;

    /* linear search for name match */
    while (BAC_File_List[index].name) 
        {
        if (strcmp(BAC_File_List[index].name, name) == 0) 
                {
            instance = BAC_File_List[index].instance;
            break;
        }
        index++;
    }

    return instance;
}

#if MAX_TSM_TRANSACTIONS
/* this is one way to match up the invoke ID with */
/* the file ID from the AtomicReadFile request. */
/* Another way would be to store the */
/* invokeID and file instance in a list or table */
/* when the request was sent */
uint32_t bacfile_instance_from_tsm(
    uint8_t invokeID)
{
    BACNET_NPDU_DATA npdu_data = { 0 }; /* dummy for getting npdu length */
    BACNET_CONFIRMED_SERVICE_DATA service_data = { 0 };
    uint8_t service_choice = 0;
    uint8_t *service_request = NULL;
    uint16_t service_request_len = 0;
    BACNET_ADDRESS dest;        /* where the original packet was destined */
    uint8_t apdu[MAX_PDU] = { 0 };      /* original APDU packet */
    uint16_t apdu_len = 0;      /* original APDU packet length */
    int len = 0;        /* apdu header length */
    BACNET_ATOMIC_READ_FILE_DATA data = { 0 };
    uint32_t object_instance = BACNET_MAX_INSTANCE + 1; /* return value */
    bool found = false;

    found =
        tsm_get_transaction_pdu(invokeID, &dest, &npdu_data, &apdu[0],
        &apdu_len);
    if (found) 
        {
        if (!npdu_data.network_layer_message && npdu_data.data_expecting_reply
            && (apdu[0] == PDU_TYPE_CONFIRMED_SERVICE_REQUEST)) 
                {
            len =
                apdu_decode_confirmed_service_request(&apdu[0], apdu_len,
                &service_data, &service_choice, &service_request,
                &service_request_len);
                    
            if (service_choice == SERVICE_CONFIRMED_ATOMIC_READ_FILE) 
                        {
                len =
                    arf_decode_service_request(service_request,
                    service_request_len, &data);
                if (len > 0) 
                                {
                    if (data.object_type == OBJECT_FILE)
                        object_instance = data.object_instance;
                }
            }
        }
    }

    return object_instance;
}
#endif

bool bacfile_read_data(
    BACNET_ATOMIC_READ_FILE_DATA * data)
{
    // char *pFilename = NULL;
    bool found = false;
    // size_t len = 0;
    uint16_t start, count;
    BACNET_FILE_ITEM *item;
    uint16_t max_record;
    uint16_t i;
    uint32_t index;
    
    index= bacfile_instance_to_index(data->object_instance);
    if(index < MAX_BACFILE)
    {
        found = true;
    
        if(index== 0)
        {
            octetstring_truncate(&data->fileData, 0);
            data->endOfFile = true;
        }
        else
        {
            item= (BACNET_FILE_ITEM *)BAC_File_List[index].item;
            max_record= BAC_File_List[index].records;
            
            for(i= 0; i< max_record; i++)
            {
                file_buff[i]= HoldingReg[item[i].addr];
            }
            
            start= data->type.stream.fileStartPosition;
            count= data->type.stream.requestedOctetCount;
            
            if(start >= max_record*sizeof(int16_t))
            {
                octetstring_truncate(&data->fileData, 0);
                data->endOfFile = true;
            }
            else
            {
                if(start + count >= max_record*sizeof(int16_t))
                {
                    count= max_record*sizeof(int16_t) - start;
                    data->endOfFile = true;
                }
                memmove(octetstring_value(&data->fileData), (uint8_t*)file_buff + start, count);
                
                octetstring_truncate(&data->fileData, count);
            }
        }
    } 
    else 
    {
        octetstring_truncate(&data->fileData, 0);
        data->endOfFile = true;
    }

    return found;
}

bool bacfile_write_stream_data(
    BACNET_ATOMIC_WRITE_FILE_DATA * data)
{
    // char *pFilename = NULL;
    bool found = false;
    size_t len = 0;
    uint16_t start, count;
    BACNET_FILE_ITEM *item;
    uint16_t max_record;
    uint16_t i;
    uint32_t index;
    
    index= bacfile_instance_to_index(data->object_instance);
    if(index < MAX_BACFILE)
    {
        found = true;
        
        if(index== 0)
        {
            len= USR_Flash_FillBuffer(octetstring_value(&data->fileData), 
                                                    data->type.stream.fileStartPosition, 
                                                    octetstring_length(&data->fileData));
            
            if(len== 0)
            {
                found = false;
            }
        }
        else
        {
            item= (BACNET_FILE_ITEM *)BAC_File_List[index].item;
            max_record= BAC_File_List[index].records;
            
            for(i= 0; i< max_record; i++)
            {
                file_buff[i]= HoldingReg[item[i].addr];
            }

            start= data->type.stream.fileStartPosition;
            count= octetstring_length(&data->fileData);
            
            if (data->type.stream.fileStartPosition != -1) 
            {
                if(start < max_record*sizeof(int16_t))
                {
                    if(start + count > max_record*sizeof(int16_t))
                    {
                        count= max_record*sizeof(int16_t) - start;
                    }
                    memmove((uint8_t*)file_buff + start, octetstring_value(&data->fileData), count);
                    
                    start/= sizeof(uint16_t);
                    
                    for(i= 0; i< count / sizeof(uint16_t); i++)
                    {
                        VAR_SetIntValue(item[start + i].addr, file_buff[start + i], 0);
                    }
                }
            }
            else
            {
                
            }
        }
    }

  return found;
}

bool bacfile_read_ack_stream_data(
    uint32_t instance,
    BACNET_ATOMIC_READ_FILE_DATA * data)
{
    bool found = false;
    char *pFilename = NULL;

    pFilename = bacfile_name(instance);
    if (pFilename) 
        {
            found = true;
        }

    return found;
}

void bacfile_init(
    void)
{
}

/**************************************************************************
*
* Copyright (C) 2012 Steve Karg <skarg@users.sourceforge.net>
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

/* Multi-state Value Objects */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "bacdef.h"
#include "bacdcode.h"
#include "bacenum.h"
#include "bacapp.h"
#include "config.h"     /* the custom stuff */
#include "rp.h"
#include "wp.h"
#include "msv.h"
#include "handlers.h"

#include "variables.h"
#include "mb_var_def.h"

#define BMSV_MAX_QTY        16

#ifndef BMSV_MAX_QTY
    #define BMSV_MAX_QTY 0
#endif

const BAC_VAR_SET Var_BMSV_Table[BMSV_MAX_QTY]=
{
    
    {HR_EFFECT_OCC,                 0, "effect_occ",                 "Effect Occ Status",     UNITS_NO_UNITS},
    {HR_SCHEDUULE_OCC,             1, "schedule_occ",            "Schedule Occupancy ", UNITS_NO_UNITS},
    {HR_AHU_OCC_REQ,                 2, "ahu_occ_req",             "AHU Occ Require",     UNITS_NO_UNITS}, 
    {HR_AHU_OCC_ST,                 3, "ahu_occ_status",         "AHU Occ Status",     UNITS_NO_UNITS},     
    {HR_EFFECT_HVAC_MODE,     4, "effect_mode",             "Effect HVAC Mode", UNITS_NO_UNITS}, 
    {HR_HVAC_MODE_OVRD,         5, "mode_override",            "HVAC Mode Override", UNITS_NO_UNITS}, 
    {HR_COMMISSION_OVRD,         6, "commission_ovrd",     "Commissioning Override",     UNITS_NO_UNITS}, 
    
    {HR_LOCAL_TEMP_TYPE,         7, "local_temp_sensor_type",         "Local Temp Sensor Type",     UNITS_NO_UNITS}, 
    {HR_LOCAL_TEMP_FUNCTION,     8, "local_temp_function",         "Local Temp Function",     UNITS_NO_UNITS},     
    {HR_HTG_TYPE,                     9, "reheating_type",             "Reheating Equipment Type", UNITS_NO_UNITS}, 
    
    {HR_HTG_VAVE_TYPE,             10, "analog_output_type",        "Analg Output Signal Type", UNITS_NO_UNITS}, 
    {HR_AUX_FAN_TYPE,             11, "aux_fan_type",         "Aux. Fan Type",     UNITS_NO_UNITS}, 
    {HR_STAT_OCC,                     12, "stat_occ",         "Stat Occupancy Mode Set", UNITS_NO_UNITS}, 
    
    {HR_USART_BAUD,                 101, "serial_baud",     "Serial Port Baud Rate",         UNITS_NO_UNITS}, 
    {HR_USART_PARITY,             102, "serial_parity", "Serial Port Parity",             UNITS_NO_UNITS}, 
    {HR_USART_TYPE,                 103, "serial_protocol",     "Serial Protocol",             UNITS_NO_UNITS}, 
};

const BAC_MSV_STATE Var_MSV_State[BMSV_MAX_QTY]=
{
    {0, 1},
    {0, 2},
    {0, 2},
    {0, 2},
    {1, 7},
    {1, 7},
    {2, 11},
    
    {3, 4},
    {4, 3},
    {5, 2},
    {6, 2},
    {7, 2},
    {0, 2},
    
    {8, 4},
    {9, 2},
    {10, 2},
};
/* NULL part of the array */
#define MSV_MULTISTATE_NULL (0)
/* how many states? 1 to 254 states - 0 is not allowed. */
//#ifndef MSV_NUMBER_OF_STATES
//#define MSV_NUMBER_OF_STATES (5)
//#endif

/* Here is our Present Value */
//static uint8_t Present_Value[BMSV_MAX_QTY];
/* Writable out-of-service allows others to manipulate our Present Value */
static bool MSV_Out_Of_Service[BMSV_MAX_QTY];

const char *MSV_State_Text_0[2]=
{
    "unoccupied", 
    "occupied"
};

const char *MSV_State_Text_1[7]=
{
    "heating", 
    "cooling",
    "warm-up",
    "cool-down",
    "set-back",
    "set-up",
    "purge"
};

const char *MSV_State_Text_2[11]=
{
    "flow_zero", 
    "flow_clg_max",
    "flow_min",
    "damper_close",
    "damper_open",
    "htg_stage_off",
    "htg_stage_on",
    "fan_stop",
    "fan_start",
    "force damper",
    "force flow"
};

const char *MSV_State_Text_3[4]=
{
    "ntc-10k B3950", 
    "ntc-20k",
    "ntc-10k-II",
    "ntc-10k-III",
};

const char *MSV_State_Text_4[3]=
{
    "zone air temp", 
    "supply air temp",
    "discharge air temp",
};

const char *MSV_State_Text_5[2]=
{
    "stage", 
    "modulating",
};

const char *MSV_State_Text_6[2]=
{
    "0-10V", 
    "2-10V",
};

const char *MSV_State_Text_7[2]=
{
    "serial fan", 
    "parallel fan",
};

//HR_USART_BAUD
const char *MSV_State_Text_8[4]=
{
    "9.6k bps",
    "19.2k bps",
    "38.4k bps",
    "76.8k bps",
};

//HR_USART_PARITY
const char *MSV_State_Text_9[2]=
{
    "odd",
    "even",
};

//HR_USART_TYPE
const char *MSV_State_Text_10[2]=
{
    "mstp master",
    "mstp slave",
};

const char** MSV_TextList[11]=
{
    MSV_State_Text_0,
    MSV_State_Text_1,
    MSV_State_Text_2,
    MSV_State_Text_3,
    MSV_State_Text_4,
    MSV_State_Text_5,
    MSV_State_Text_6,
    MSV_State_Text_7,
    MSV_State_Text_8,
    MSV_State_Text_9,
    MSV_State_Text_10
};

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int Properties_Required[] = {
    PROP_OBJECT_IDENTIFIER,
    PROP_OBJECT_NAME,
    PROP_OBJECT_TYPE,
    PROP_PRESENT_VALUE,
    PROP_STATUS_FLAGS,
    PROP_EVENT_STATE,
    PROP_OUT_OF_SERVICE,
    PROP_NUMBER_OF_STATES,
    -1
};

static const int Properties_Optional[] = {
    PROP_DESCRIPTION,
    PROP_STATE_TEXT,
    -1
};

static const int Properties_Proprietary[] = {
    -1
};

void Multistate_Value_Property_Lists(
    const int **pRequired,
    const int **pOptional,
    const int **pProprietary)
{
    if (pRequired)
        *pRequired = Properties_Required;
    if (pOptional)
        *pOptional = Properties_Optional;
    if (pProprietary)
        *pProprietary = Properties_Proprietary;

    return;
}

void Multistate_Value_Init(
    void)
{
    // uint16_t i;
    
  return;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the index */
/* that correlates to the correct instance number */
unsigned Multistate_Value_Instance_To_Index(
    uint32_t object_instance)
{
    unsigned index = BMSV_MAX_QTY;
    unsigned i;

    for(i= 0; i< BMSV_MAX_QTY; i++)
    {
        if(object_instance== Var_BMSV_Table[i].obj_instance)
        {
            return i;
        }
    }
    
    return index;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */
uint32_t Multistate_Value_Index_To_Instance(
    unsigned index)
{
    if (index < BMSV_MAX_QTY)
    {
        index= Var_BMSV_Table[index].obj_instance;
        return index;
    }
     
    return BMSV_MAX_QTY;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then count how many you have */
unsigned Multistate_Value_Count(
    void)
{
    return BMSV_MAX_QTY;
}

bool Multistate_Value_Valid_Instance(
    uint32_t object_instance)
{
    unsigned index = 0; /* offset from instance lookup */

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < BMSV_MAX_QTY) 
    {
        return true;
    }

    return false;
}

uint32_t Multistate_Value_Present_Value(
    uint32_t object_instance)
{
    uint32_t value = (uint32_t)NULL;
    unsigned index = 0; /* offset from instance lookup */

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < BMSV_MAX_QTY) 
    {
        value = VAR_GetRealValue(Var_BMSV_Table[index].addr); 
        
        if(101== object_instance)
        {
            switch(value)
            {
                case 8:
                    value= 1;
                    break;
                case 16:
                    value= 2;
                    break;
                case 32:
                    value= 3;
                    break;
                case 64:
                    value= 4;
                    break;
                default:
                    break;
            }
        }
    }

    return value;
}

bool Multistate_Value_Present_Value_Set(
    uint32_t object_instance,
    uint32_t value)
{
    bool status = false;
    unsigned index = 0; /* offset from instance lookup */

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < BMSV_MAX_QTY) 
    {
        if(101== object_instance)
        {
            switch(value)
            {
                case 1:
                    value= 8;
                    break;
                case 2:
                    value= 16;
                    break;
                case 3:
                    value= 32;
                    break;
                case 4:
                    value= 64;
                    break;
                default:
                    break;
            }
        }
        
        VAR_SetIntValue(Var_BMSV_Table[index].addr, value, 0);
        status= true;
    }

    return status;
}

bool Multistate_Value_Out_Of_Service(
    uint32_t object_instance)
{
    bool status = false;
    unsigned index = 0;

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < BMSV_MAX_QTY) 
    {
        status = MSV_Out_Of_Service[index];
        status= true;
    }

    return status;
}

void Multistate_Value_Out_Of_Service_Set(
    uint32_t object_instance,
    bool value)
{
    unsigned index = 0;

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < BMSV_MAX_QTY) 
    {
        MSV_Out_Of_Service[index] = value;
    }

    return;
}

//    static char *Multistate_Value_Description(
//        uint32_t object_instance)
//    {
//      char* text_string;   /* okay for single thread */
//        unsigned int index;
//
//        index = Multistate_Value_Instance_To_Index(object_instance);
//        if (index < BMSV_MAX_QTY) 
//        {
//        text_string= (char*)Var_BMSV_Table[index].description;    
//            return text_string;
//        }
//        return NULL;
//    }

bool Multistate_Value_Description_Set(
    uint32_t object_instance,
    char *new_name)
{
    return false;
}

uint16_t Multistate_Value_MaxState(uint32_t object_instance)
{
    unsigned int index;

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < BMSV_MAX_QTY) 
    {
        return Var_MSV_State[index].max_state;
    }

  return 0;
}

char* Multistate_Value_Name(
    uint32_t object_instance)
{
  char* text_string;   /* okay for single thread */
    unsigned int index;

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < BMSV_MAX_QTY) 
    {
    text_string= (char*)Var_BMSV_Table[index].name;    
        return text_string;
    }

   return NULL;
}

char* Multistate_Value_Descript(
    uint32_t object_instance)
{
  char* text_string;   /* okay for single thread */
    unsigned int index;

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < BMSV_MAX_QTY) 
    {
    text_string= (char*)Var_BMSV_Table[index].description;    
        return text_string;
    }

   return NULL;
}

/* note: the object name must be unique within this device */
bool Multistate_Value_Name_Set(
    uint32_t object_instance,
    char *new_name)
{
    bool status = false;        /* return value */

    return status;
}

static char *Multistate_Value_State_Text(
    uint32_t object_instance,
    uint32_t state_index)
{
    unsigned index = 0; /* offset from instance lookup */
    char *pName = NULL; /* return value */
    uint8_t str_idx;

    index = Multistate_Value_Instance_To_Index(object_instance);
    
    if (index < BMSV_MAX_QTY 
        && state_index < Var_MSV_State[index].max_state) 
    {
        str_idx= Var_MSV_State[index].state_index;
        
        pName= (char*)MSV_TextList[str_idx][state_index];
    }

    return pName;
}

/* note: the object name must be unique within this device */
bool Multistate_Value_State_Text_Set(
    uint32_t object_instance,
    uint32_t state_index,
    char *new_name)
{
    return false;;
}


/* return apdu len, or BACNET_STATUS_ERROR on error */
int Multistate_Value_Read_Property(
    BACNET_READ_PROPERTY_DATA * rpdata)
{
    int len = 0;
    int apdu_len = 0;   /* return value */
    BACNET_BIT_STRING bit_string;
    BACNET_CHARACTER_STRING char_string;
    uint32_t present_value = 0;
    // unsigned object_index = 0;
    unsigned i = 0;
    uint8_t *apdu = NULL;
    uint16_t value;

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
                encode_application_object_id(&apdu[0],
                OBJECT_MULTI_STATE_VALUE, rpdata->object_instance);
            break;
            /* note: Name and Description don't have to be the same.
               You could make Description writable and different */
        case PROP_OBJECT_NAME:
            characterstring_init_ansi(&char_string, Multistate_Value_Name(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_DESCRIPTION:
            characterstring_init_ansi(&char_string, Multistate_Value_Descript(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_TYPE:
            apdu_len =
                encode_application_enumerated(&apdu[0],
                OBJECT_MULTI_STATE_VALUE);
            break;
        case PROP_PRESENT_VALUE:
            present_value =
                Multistate_Value_Present_Value(rpdata->object_instance);
                apdu_len = encode_application_unsigned(&apdu[0], present_value);
            break;
        case PROP_STATUS_FLAGS:
            /* note: see the details in the standard on how to use these */
            bitstring_init(&bit_string);
            bitstring_set_bit(&bit_string, STATUS_FLAG_IN_ALARM, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_FAULT, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OVERRIDDEN, false);
            
            bitstring_set_bit(&bit_string, STATUS_FLAG_OUT_OF_SERVICE,
                    Multistate_Value_Out_Of_Service(rpdata->object_instance));
            apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
            break;
        case PROP_EVENT_STATE:
            /* note: see the details in the standard on how to use this */
            apdu_len =
                encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
            break;
        case PROP_OUT_OF_SERVICE:
            apdu_len = encode_application_boolean(&apdu[0], 
                    Multistate_Value_Out_Of_Service(rpdata->object_instance));
            break;
        case PROP_NUMBER_OF_STATES:
                        value= Multistate_Value_MaxState(rpdata->object_instance);
            apdu_len = encode_application_unsigned(&apdu[apdu_len], value);
            break;
        case PROP_STATE_TEXT:
                    if (rpdata->array_index == 0) 
                    {
                            /* Array element zero is the number of elements in the array */
                            value= Multistate_Value_MaxState(rpdata->object_instance);
                            apdu_len =
                                    encode_application_unsigned(&apdu[0], value);
                    } 
                    else 
                        if (rpdata->array_index == BACNET_ARRAY_ALL) 
                        {
                            /* if no index was specified, then try to encode the entire list */
                            /* into one packet. */
                            // object_index = 
                            Multistate_Value_Instance_To_Index(rpdata->object_instance);
                            
                            value= Multistate_Value_MaxState(rpdata->object_instance);
                            for (i = 0; i < value; i++) 
                            {
                                characterstring_init_ansi(&char_string,    Multistate_Value_State_Text(rpdata->object_instance,
                                                i));
                                /* FIXME: this might go beyond MAX_APDU length! */
                                len =
                                        encode_application_character_string(&apdu[apdu_len],
                                        &char_string);
                                /* add it if we have room */
                                if ((apdu_len + len) < MAX_APDU) 
                                {
                                        apdu_len += len;
                                } 
                                else 
                                {
                                        rpdata->error_class = ERROR_CLASS_SERVICES;
                                        rpdata->error_code = ERROR_CODE_NO_SPACE_FOR_OBJECT;
                                        apdu_len = BACNET_STATUS_ERROR;
                                        break;
                                }
                            }
                        } 
                        else 
                        {
                // object_index = 
                Multistate_Value_Instance_To_Index(rpdata->object_instance);
                                value= Multistate_Value_MaxState(rpdata->object_instance);
                if (rpdata->array_index < value) 
                                {
                    characterstring_init_ansi(&char_string,
                        Multistate_Value_State_Text(rpdata->object_instance,
                            rpdata->array_index));
                    apdu_len =
                        encode_application_character_string(&apdu[0],
                        &char_string);
                } 
                                else 
                                {
                    rpdata->error_class = ERROR_CLASS_PROPERTY;
                    rpdata->error_code = ERROR_CODE_INVALID_ARRAY_INDEX;
                    apdu_len = BACNET_STATUS_ERROR;
                }
            }
            break;
        default:
            rpdata->error_class = ERROR_CLASS_PROPERTY;
            rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
            apdu_len = BACNET_STATUS_ERROR;
            break;
    }
    /*  only array properties can have array options */
    if ((apdu_len >= 0) && (rpdata->object_property != PROP_STATE_TEXT) &&
        (rpdata->object_property != PROP_PRIORITY_ARRAY) &&
        (rpdata->array_index != BACNET_ARRAY_ALL)) {
        rpdata->error_class = ERROR_CLASS_PROPERTY;
        rpdata->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
        apdu_len = BACNET_STATUS_ERROR;
    }

    return apdu_len;
}

/* returns true if successful */
bool Multistate_Value_Write_Property(
    BACNET_WRITE_PROPERTY_DATA * wp_data)
{
    bool status = false;        /* return value */
    int len = 0;
    BACNET_APPLICATION_DATA_VALUE value;

    /* decode the some of the request */
    len =
            bacapp_decode_application_data(wp_data->application_data,
            wp_data->application_data_len, &value);
    /* FIXME: len < application_data_len: more data? */
    if (len < 0) 
    {
        /* error while decoding - a value larger than we can handle */
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
        return false;
    }

    if ((wp_data->object_property != PROP_STATE_TEXT) &&
        (wp_data->object_property != PROP_PRIORITY_ARRAY) &&
        (wp_data->array_index != BACNET_ARRAY_ALL)) 
    {
        /*  only array properties can have array options */
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
        return false;
    }
    
    switch (wp_data->object_property) 
    {
    case PROP_PRESENT_VALUE:
        if(value.tag == BACNET_APPLICATION_TAG_NULL)
            {
                status = Multistate_Value_Present_Value_Set
                    (wp_data->object_instance, MSV_MULTISTATE_NULL);
            }
            else
            {
                status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_UNSIGNED_INT,
                                                            &wp_data->error_class, &wp_data->error_code);
                if (status) 
                {
                    status = Multistate_Value_Present_Value_Set
                        (wp_data->object_instance, value.type.Unsigned_Int);
                    if (!status) 
                    {
                        wp_data->error_class = ERROR_CLASS_PROPERTY;
                        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                    }
                }
            }
            break;
    case PROP_OUT_OF_SERVICE:
            status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_BOOLEAN,
                    &wp_data->error_class, &wp_data->error_code);
            if (status) 
            {
                    Multistate_Value_Out_Of_Service_Set(wp_data->object_instance,
                            value.type.Boolean);
            }
            break;
    case PROP_OBJECT_IDENTIFIER:
    case PROP_OBJECT_NAME:
    case PROP_OBJECT_TYPE:
    case PROP_STATUS_FLAGS:
    case PROP_EVENT_STATE:
    case PROP_NUMBER_OF_STATES:
    case PROP_DESCRIPTION:
    case PROP_STATE_TEXT:
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




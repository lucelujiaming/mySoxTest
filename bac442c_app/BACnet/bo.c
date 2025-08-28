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

/* Binary Output Objects - customize for your use */

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
#include "bo.h"
#include "handlers.h"

#include "variables.h"
#include "mb_var_def.h"
#include "modbus.h"

#define BBO_MAX_QTY        2

#ifndef BBO_MAX_QTY
    #define BBO_MAX_QTY 0
#endif

const BAC_VAR_SET Var_BBO_Table[BBO_MAX_QTY]=
{
    {HR_HTG_STAGE_CMD,     0, "htg_stage_cmd",         "Heating Stage Command"}, 
    {HR_AUX_FAN_CMD,         1, "fan_cmd",                     "Fan Command"}, 
};

/* When all the priorities are level null, the present value returns */
/* the Relinquish Default value */
#define RELINQUISH_DEFAULT BINARY_INACTIVE
/* Here is our Priority Array.*/
//static BACNET_BINARY_PV BO_Present_Value[BBO_MAX_QTY][BACNET_MAX_PRIORITY];
/* Writable out-of-service allows others to play with our Present Value */
/* without changing the physical output */
static bool BO_Out_Of_Service[BBO_MAX_QTY];
/* polarity - normal or inverse */
//static uint8_t BO_Polarity[BBO_MAX_QTY];

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int Binary_Output_Properties_Required[] = {
    PROP_OBJECT_IDENTIFIER,
    PROP_OBJECT_NAME,
    PROP_OBJECT_TYPE,
    PROP_PRESENT_VALUE,
    PROP_STATUS_FLAGS,
    PROP_EVENT_STATE,
    PROP_OUT_OF_SERVICE,
    PROP_POLARITY,
    PROP_PRIORITY_ARRAY,
    PROP_RELINQUISH_DEFAULT,
    -1
};

static const int Binary_Output_Properties_Optional[] = {
    PROP_DESCRIPTION,
    PROP_DEVICE_TYPE,
    PROP_ACTIVE_TEXT,
    PROP_INACTIVE_TEXT,
    PROP_MINIMUM_ON_TIME,
    PROP_MINIMUM_OFF_TIME,
    -1
};

static const int Binary_Output_Properties_Proprietary[] = {
    -1
};

void Binary_Output_Property_Lists(
    const int **pRequired,
    const int **pOptional,
    const int **pProprietary)
{
    if (pRequired)
        *pRequired = Binary_Output_Properties_Required;
    if (pOptional)
        *pOptional = Binary_Output_Properties_Optional;
    if (pProprietary)
        *pProprietary = Binary_Output_Properties_Proprietary;

    return;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need validate that the */
/* given instance exists */
bool Binary_Output_Valid_Instance(
    uint32_t object_instance)
{
    if (object_instance < BBO_MAX_QTY)
        return true;

    return false;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then count how many you have */
unsigned Binary_Output_Count(
    void)
{
    return BBO_MAX_QTY;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */
uint32_t Binary_Output_Index_To_Instance(
    unsigned index)
{
    return index;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the index */
/* that correlates to the correct instance number */
unsigned Binary_Output_Instance_To_Index(
    uint32_t object_instance)
{
    unsigned index = BBO_MAX_QTY;

    if (object_instance < BBO_MAX_QTY)
        index = object_instance;

    return index;
}

BACNET_BINARY_PV Binary_Output_Present_Value(
    uint32_t object_instance)
{
    BACNET_BINARY_PV value = RELINQUISH_DEFAULT;
    unsigned index = 0;
    // unsigned i = 0;

    index = Binary_Output_Instance_To_Index(object_instance);
    if (index < BBO_MAX_QTY) 
    {
        value= VAR_GetRealValue(Var_BBO_Table[index].addr);
    }

    return value;
}

bool Binary_Output_Present_Value_Set(
    uint32_t instance,
    BACNET_BINARY_PV binary_value,
    unsigned priority)
{       /* 0..15 */
    bool status = false;
    unsigned index = 0;
    // uint8_t i;
    // bool out_value= false;

    index = Binary_Output_Instance_To_Index(instance);
    
    if (index < BBO_MAX_QTY) 
    {
        if(priority <= BACNET_MAX_PRIORITY) 
        {
//            BO_Present_Value[index][priority-1] = binary_value;
//                    //seeprom_bytes_write(NV_SEEPROM_BINARY_OUTPUT(instance,
//                                 // NV_SEEPROM_BO_PRIORITY_ARRAY_1 + priority),
//                            //&BO_Present_Value[instance][priority], 1);
//            status = true;

//            for (i = 0; i < BACNET_MAX_PRIORITY; i++) 
//            {
//                if (BO_Present_Value[index][i] != BINARY_NULL) 
//                {
//                    out_value= false;
//                    
//                    if(BO_Present_Value[index][i] == BINARY_ACTIVE)
//                    {
//                        out_value= true;
//                    }
//                //                    SetDigitalOutValue(index, out_value);
//                    break;
//                }
//            }
        }
    }

    return status;
}

void Binary_Output_Polarity_Set(
    uint32_t instance,
    BACNET_POLARITY polarity)
{
    if (instance < BBO_MAX_QTY) 
    {
//        if (polarity < MAX_POLARITY) 
//        {
//                BO_Polarity[instance] = polarity;
//        //            Param_WriteWord(EEP_BO_POLARITY_0 + instance, polarity);
//        }
    }
}

BACNET_POLARITY Binary_Output_Polarity(
    uint32_t instance)
{
//    unsigned index;

//    index= Binary_Output_Instance_To_Index(instance);

//    if (index < BBO_MAX_QTY) 
//    {
//        if (BO_Polarity[index] < MAX_POLARITY) 
//        {
//            return BO_Polarity[index];
//        }
//    }

    return POLARITY_NORMAL;
}

void Binary_Output_Out_Of_Service_Set(
    uint32_t instance,
    bool flag)
{
    if (instance < BBO_MAX_QTY) 
    {
        BO_Out_Of_Service[instance] = flag;
    }
}

/* note: the object name must be unique within this device */
char *Binary_Output_Name(
    uint32_t object_instance)
{
  char* text_string;   /* okay for single thread */
    unsigned int index;

    index = Binary_Output_Instance_To_Index(object_instance);
    if (index < BBO_MAX_QTY) 
    {
        text_string= (char*)Var_BBO_Table[index].name;
        
        return text_string;
    }

  return NULL;
}

char *Binary_Output_Description(
    uint32_t object_instance)
{
  char* text_string;   /* okay for single thread */
    unsigned int index;

    index = Binary_Output_Instance_To_Index(object_instance);
    if (index < BBO_MAX_QTY) 
    {
        text_string= (char*)Var_BBO_Table[index].description;
        
        return text_string;
    }

    return NULL;
}

uint32_t Binary_Output_MinOnTime(uint32_t object_instance)
{
    uint32_t value= 0;
    unsigned int index;

    index = Binary_Output_Instance_To_Index(object_instance);
    if (index < BBO_MAX_QTY) 
    {
        if(index== 0)
        {
            value= HoldingReg[HR_HTG_STAGE_ON_MINUTES];
        }
        else
        {
            value= HoldingReg[HR_AUX_FAN_ON_MINUTES];
        }
    }
    
    return value;
}

bool Binary_Output_MinOnTime_Set(uint32_t object_instance, uint16_t value)
{
    bool status= false;
    unsigned int index;

    index = Binary_Output_Instance_To_Index(object_instance);
    if (index < BBO_MAX_QTY) 
    {
        status= true;
        if(index== 0)
        {
            VAR_SetIntValue(HR_HTG_STAGE_ON_MINUTES, value, 0);
        }
        else
        {
            VAR_SetIntValue(HR_AUX_FAN_ON_MINUTES, value, 0);
        }
    }
    
    return status;
}

uint32_t Binary_Output_MinOffTime(uint32_t object_instance)
{
    uint32_t value= 0;
    unsigned int index;

    index = Binary_Output_Instance_To_Index(object_instance);
    if (index < BBO_MAX_QTY) 
    {
        if(index== 0)
        {
            value= HoldingReg[HR_HTG_STAGE_OFF_MINUTES];
        }
        else
        {
            value= HoldingReg[HR_AUX_FAN_OFF_MINUTES];
        }
    }
    
    return value;
}

bool Binary_Output_MinOffTime_Set(uint32_t object_instance, uint16_t value)
{
    bool status= false;
    unsigned int index;

    index = Binary_Output_Instance_To_Index(object_instance);
    if (index < BBO_MAX_QTY) 
    {
        status= true;
        if(index== 0)
        {
            VAR_SetIntValue(HR_HTG_STAGE_OFF_MINUTES, value, 0);
        }
        else
        {
            VAR_SetIntValue(HR_AUX_FAN_OFF_MINUTES, value, 0);
        }
    }
    
    return status;
}

//uint32_t Binary_Output_MinOnTime_Set(uint32_t object_instance)

//uint32_t Binary_Output_MinOffTime_Set(uint32_t object_instance)

/* return apdu len, or BACNET_STATUS_ERROR on error */
int Binary_Output_Read_Property(
    BACNET_READ_PROPERTY_DATA * rpdata)
{
    // int len = 0;
    int apdu_len = 0;   /* return value */
    BACNET_BIT_STRING bit_string;
    BACNET_CHARACTER_STRING char_string;
    BACNET_BINARY_PV present_value = BINARY_INACTIVE;
    BACNET_POLARITY polarity = POLARITY_NORMAL;
    unsigned object_index = 0;
    // unsigned i = 0;
    bool state = false;
    uint8_t *apdu = NULL;
        uint32_t value;

    if ((rpdata == NULL) || (rpdata->application_data == NULL) ||
        (rpdata->application_data_len == 0)) {
        return 0;
    }
    apdu = rpdata->application_data;
    switch (rpdata->object_property) {
        case PROP_OBJECT_IDENTIFIER:
            apdu_len =
                encode_application_object_id(&apdu[0], OBJECT_BINARY_OUTPUT,
                rpdata->object_instance);
            break;
            /* note: Name and Description don't have to be the same.
               You could make Description writable and different */
        case PROP_OBJECT_NAME:
            characterstring_init_ansi(&char_string,
                Binary_Output_Name(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_DESCRIPTION:
            characterstring_init_ansi(&char_string,
                Binary_Output_Description(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_TYPE:
            apdu_len =
                encode_application_enumerated(&apdu[0], OBJECT_BINARY_OUTPUT);
            break;
        case PROP_PRESENT_VALUE:
            present_value =
                Binary_Output_Present_Value(rpdata->object_instance);
            apdu_len = encode_application_enumerated(&apdu[0], present_value);
            break;
        case PROP_STATUS_FLAGS:
            /* note: see the details in the standard on how to use these */
            bitstring_init(&bit_string);
            bitstring_set_bit(&bit_string, STATUS_FLAG_IN_ALARM, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_FAULT, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OVERRIDDEN, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OUT_OF_SERVICE, false);
            apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
            break;
        case PROP_EVENT_STATE:
            /* note: see the details in the standard on how to use this */
            apdu_len =
                encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
            break;
        case PROP_OUT_OF_SERVICE:
            object_index = 
                Binary_Output_Instance_To_Index(rpdata->object_instance);
            state = BO_Out_Of_Service[object_index];
            apdu_len = encode_application_boolean(&apdu[0], state);
            break;
        case PROP_POLARITY:
                        polarity = Binary_Output_Polarity(rpdata->object_instance);
            apdu_len = encode_application_enumerated(&apdu[0], polarity);
            break;
//        case PROP_PRIORITY_ARRAY:
//            /* Array element zero is the number of elements in the array */
//            if (rpdata->array_index == 0)
//                apdu_len =
//                    encode_application_unsigned(&apdu[0], BACNET_MAX_PRIORITY);
//            /* if no index was specified, then try to encode the entire list */
//            /* into one packet. */
//            else if (rpdata->array_index == BACNET_ARRAY_ALL) {
//                object_index =
//                    Binary_Output_Instance_To_Index(rpdata->object_instance);
//                for (i = 0; i < BACNET_MAX_PRIORITY; i++) {
//                    /* FIXME: check if we have room before adding it to APDU */
//                    if (BO_Present_Value[object_index][i] == BINARY_NULL)
//                        len = encode_application_null(&apdu[apdu_len]);
//                    else {
//                        present_value = BO_Present_Value[object_index][i];
//                        len =
//                            encode_application_enumerated(&apdu[apdu_len],
//                            present_value);
//                    }
//                    /* add it if we have room */
//                    if ((apdu_len + len) < MAX_APDU)
//                        apdu_len += len;
//                    else {
//                        rpdata->error_class = ERROR_CLASS_SERVICES;
//                        rpdata->error_code = ERROR_CODE_NO_SPACE_FOR_OBJECT;
//                        apdu_len = BACNET_STATUS_ERROR;
//                        break;
//                    }
//                }
//            } else {
//                object_index =
//                    Binary_Output_Instance_To_Index(rpdata->object_instance);
//                if (rpdata->array_index <= BACNET_MAX_PRIORITY) {
//                    if (BO_Present_Value[object_index][rpdata->array_index -
//                            1] == BINARY_NULL)
//                        apdu_len = encode_application_null(&apdu[apdu_len]);
//                    else {
//                        present_value =
//                            BO_Present_Value[object_index]
//                            [rpdata->array_index - 1];
//                        apdu_len =
//                            encode_application_enumerated(&apdu[apdu_len],
//                            present_value);
//                    }
//                } else {
//                    rpdata->error_class = ERROR_CLASS_PROPERTY;
//                    rpdata->error_code = ERROR_CODE_INVALID_ARRAY_INDEX;
//                    apdu_len = BACNET_STATUS_ERROR;
//                }
//            }

//            break;
    case PROP_RELINQUISH_DEFAULT:
            present_value = RELINQUISH_DEFAULT;
            apdu_len = encode_application_enumerated(&apdu[0], present_value);
            break;
        case PROP_DEVICE_TYPE:
            characterstring_init_ansi(&char_string, "relay output");
            apdu_len =
                    encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_ACTIVE_TEXT:
            characterstring_init_ansi(&char_string, "on");
            apdu_len =
                    encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_INACTIVE_TEXT:
            characterstring_init_ansi(&char_string, "off");
            apdu_len =
                    encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_MINIMUM_ON_TIME:
            value= Binary_Output_MinOnTime(rpdata->object_instance);
            apdu_len = encode_application_unsigned(&apdu[0], value);
            break;
        case PROP_MINIMUM_OFF_TIME:
            value= Binary_Output_MinOffTime(rpdata->object_instance);
            apdu_len = encode_application_unsigned(&apdu[0], value);
            break;
        default:
                rpdata->error_class = ERROR_CLASS_PROPERTY;
                rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
                apdu_len = BACNET_STATUS_ERROR;
                break;
    } 
    /*  only array properties can have array options */
    if ((apdu_len >= 0) && (rpdata->object_property != PROP_PRIORITY_ARRAY) &&
        (rpdata->array_index != BACNET_ARRAY_ALL)) {
        rpdata->error_class = ERROR_CLASS_PROPERTY;
        rpdata->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
        apdu_len = BACNET_STATUS_ERROR;
    }

    return apdu_len;
}

/* returns true if successful */
bool Binary_Output_Write_Property(
    BACNET_WRITE_PROPERTY_DATA * wp_data)
{
    bool status = false;        /* return value */
    unsigned int object_index = 0;
    unsigned int priority = 0;
    BACNET_BINARY_PV level = BINARY_NULL;
    // int len = 0;
    BACNET_APPLICATION_DATA_VALUE value;

    /* decode the some of the request */
    // len =
    bacapp_decode_application_data(wp_data->application_data,
            wp_data->application_data_len, &value);
    /* FIXME: len < application_data_len: more data? */
    /* FIXME: len == 0: unable to decode? */
    switch (wp_data->object_property) 
    {
    case PROP_PRESENT_VALUE:
        if (value.tag == BACNET_APPLICATION_TAG_ENUMERATED) 
        {
            priority = wp_data->priority;
            /* Command priority 6 is reserved for use by Minimum On/Off
            algorithm and may not be used for other purposes in any
            object. */
            if (priority && (priority <= BACNET_MAX_PRIORITY) &&
                (priority != 6 /* reserved */ ) &&
                (value.type.Enumerated <= MAX_BINARY_PV)) 
            {
                level = (BACNET_BINARY_PV) value.type.Enumerated;
                object_index = Binary_Output_Instance_To_Index(wp_data->object_instance);
                //                    priority--;
                Binary_Output_Present_Value_Set(wp_data->object_instance,level, priority);
                /* Note: you could set the physical output here if we
                     are the highest priority.
                     However, if Out of Service is TRUE, then don't set the
                     physical output.  This comment may apply to the
                     main loop (i.e. check out of service before changing output) */
                status = true;
            } 
            else 
            {
                if (priority == 6) 
                {
                    /* Command priority 6 is reserved for use by Minimum On/Off
                    algorithm and may not be used for other purposes in any
                    object. */
                    wp_data->error_class = ERROR_CLASS_PROPERTY;
                    wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
                } 
                else 
                {
                    wp_data->error_class = ERROR_CLASS_PROPERTY;
                    wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                }
            }
        } 
        else 
        {
            status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_NULL,
                                                            &wp_data->error_class, &wp_data->error_code);
            if (status) 
            {
                level = BINARY_NULL;
                object_index = Binary_Output_Instance_To_Index(wp_data->object_instance);
                priority = wp_data->priority;
                if (priority && (priority <= BACNET_MAX_PRIORITY)) 
                {
//                    priority--;
//                    BO_Present_Value[object_index][priority] = level;
//                    /* Note: you could set the physical output here to the next
//                         highest priority, or to the relinquish default if no
//                         priorities are set.
//                         However, if Out of Service is TRUE, then don't set the
//                         physical output.  This comment may apply to the
//                         main loop (i.e. check out of service before changing output) */
                } 
                else 
                {
                    status = false;
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
            object_index = Binary_Output_Instance_To_Index(wp_data->object_instance);
            BO_Out_Of_Service[object_index] = value.type.Boolean;
        }
            break;
    case PROP_POLARITY:
        status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_ENUMERATED, 
                                        &wp_data->error_class, &wp_data->error_code);
        if (status) 
        {
            if (value.type.Enumerated < MAX_POLARITY) 
            {
                Binary_Output_Polarity_Set(wp_data->object_instance, value.type.Enumerated);
            } 
            else 
            {
                status = false;
                wp_data->error_class = ERROR_CLASS_PROPERTY;
                wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
            }
        }
        break;
    case PROP_MINIMUM_ON_TIME:
        status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_UNSIGNED_INT,
                                                            &wp_data->error_class, &wp_data->error_code);
        if(status)
        {
            status= Binary_Output_MinOnTime_Set(wp_data->object_instance, value.type.Unsigned_Int);
        }
        break;
    case PROP_MINIMUM_OFF_TIME:
        status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_UNSIGNED_INT,
                                                            &wp_data->error_class, &wp_data->error_code);
        if(status)
        {
            status= Binary_Output_MinOffTime_Set(wp_data->object_instance, value.type.Unsigned_Int);
        }
        break;
    default:
            wp_data->error_class = ERROR_CLASS_PROPERTY;
            wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
            break;
    }

  return status;
}

void Binary_Output_Init(
    void)
{
    // unsigned i, j;

  return;
}

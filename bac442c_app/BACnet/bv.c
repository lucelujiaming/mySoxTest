/**************************************************************************
*
* Copyright (C) 2006 Steve Karg <skarg@users.sourceforge.net>
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
#include "wp.h"
#include "rp.h"
#include "bv.h"
#include "handlers.h"

#include "mb_var_def.h"
#include "variables.h"

#ifndef MAX_BV
#define MAX_BV 2
#endif

const BAC_VAR_SET Var_BV_Table[MAX_BV]=
{
    {HR_UNOCC_FLW_ZERO,                 0, "unocc_flw_zero",                 "Unocc Set Flw To Zero",     UNITS_NO_UNITS}, 
    {HR_ACT_ADAPT,                             1, "actuator_adaption",         "Acutuator Adaption",     UNITS_NO_UNITS}, 
};

/* When all the priorities are level null, the present value returns */
/* the Relinquish Default value */
#define RELINQUISH_DEFAULT BINARY_INACTIVE
/* Here is our Priority Array.*/
static BACNET_BINARY_PV BV_Present_Value[MAX_BV];
/* Writable out-of-service allows others to play with our Present Value */
/* without changing the physical output */
static bool BV_Out_Of_Service[MAX_BV];

/* Change of Value flag */
static bool BV_Change_Of_Value_Table[MAX_BV];

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int Binary_Value_Properties_Required[] = 
{
    PROP_OBJECT_IDENTIFIER,
    PROP_OBJECT_NAME,
    PROP_OBJECT_TYPE,
    PROP_PRESENT_VALUE,
    PROP_STATUS_FLAGS,
    PROP_EVENT_STATE,
    PROP_OUT_OF_SERVICE,
    -1
};

static const int Binary_Value_Properties_Optional[] = 
{
    PROP_DESCRIPTION,
        PROP_ACTIVE_TEXT,
    PROP_INACTIVE_TEXT,
    -1
};

static const int Binary_Value_Properties_Proprietary[] = 
{
    -1
};

void Binary_Value_Property_Lists(
    const int **pRequired,
    const int **pOptional,
    const int **pProprietary)
{
    if (pRequired)
        *pRequired = Binary_Value_Properties_Required;
    if (pOptional)
        *pOptional = Binary_Value_Properties_Optional;
    if (pProprietary)
        *pProprietary = Binary_Value_Properties_Proprietary;

    return;
}

void Binary_Value_Init(void)
{
//    unsigned i;
//    static bool initialized = false;

//    if (!initialized) 
//    {
//        initialized = true;

//        /* initialize all the analog output priority arrays to NULL */
//        for (i = 0; i < MAX_BV; i++) 
//        {
//            BV_Present_Value[i] = BINARY_NULL;
//        }
//    }

    return;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need validate that the */
/* given instance exists */
bool Binary_Value_Valid_Instance(uint32_t object_instance)
{
     if(object_instance < MAX_BV)
    {
        return true;
    }

    return false;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then count how many you have */
unsigned Binary_Value_Count(void)
{
    return MAX_BV;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */
uint32_t Binary_Value_Index_To_Instance(
    unsigned index)
{
    return index;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the index */
/* that correlates to the correct instance number */
unsigned Binary_Value_Instance_To_Index(uint32_t object_instance)
{
    unsigned index = MAX_BV;
    
    if(Binary_Value_Valid_Instance(object_instance))
    {
        index= object_instance;
    }

    return index;
}

BACNET_BINARY_PV Binary_Value_Present_Value(uint32_t object_instance)
{
    BACNET_BINARY_PV value = RELINQUISH_DEFAULT;
    unsigned index = 0;

    index = Binary_Value_Instance_To_Index(object_instance);
    if (index < MAX_BV) 
    {
            value = VAR_GetRealValue(Var_BV_Table[index].addr);;
    }

    return value;
}

bool Binary_Value_Present_Value_Set(
    uint32_t object_instance,
    BACNET_BINARY_PV value,
    uint8_t priority)
{
    unsigned index = 0;
    bool status = false;

    index = Binary_Value_Instance_To_Index(object_instance);
    if (index < MAX_BV) 
    {
            VAR_SetIntValue(Var_BV_Table[index].addr, value, 0);
      status = true;
    }
    return status;
}

bool Binary_Value_Change_Of_Value(
    uint32_t object_instance)
{
    bool status = false;
    unsigned index;

    index = Binary_Value_Instance_To_Index(object_instance);
    if (index < MAX_BV) 
    {
        status = BV_Change_Of_Value_Table[index];
    }

    return status;
}

void Binary_Value_Change_Of_Value_Clear(
    uint32_t object_instance)
{
    unsigned index;

    index = Binary_Value_Instance_To_Index(object_instance);
    if (index < MAX_BV) 
    {
        BV_Change_Of_Value_Table[index] = false;
    }

    return;
}

/* note: the object name must be unique within this device */
char *Binary_Value_Name(uint32_t object_instance)
{
  char* text_string;   /* okay for single thread */
    unsigned int index;

    index = Binary_Value_Instance_To_Index(object_instance);
    if (index < MAX_BV) 
    {
    text_string= (char*)Var_BV_Table[index].name;    
        return text_string;
    }
    return NULL;
}

/* note: the object name must be unique within this device */
char *Binary_Value_Description(uint32_t object_instance)
{
  char* text_string;   /* okay for single thread */
    unsigned int index;

    index = Binary_Value_Instance_To_Index(object_instance);
    if (index < MAX_BV) 
    {
    text_string= (char*)Var_BV_Table[index].description;    
        return text_string;
    }
    return NULL;
}

/* return apdu len, or BACNET_STATUS_ERROR on error */
int Binary_Value_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata)
{
    int apdu_len = 0;   /* return value */
    BACNET_BIT_STRING bit_string;
    BACNET_CHARACTER_STRING char_string;
    BACNET_BINARY_PV present_value = BINARY_INACTIVE;
    unsigned object_index = 0;
    bool state = false;
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
        apdu_len = encode_application_object_id(&apdu[0], OBJECT_BINARY_VALUE, rpdata->object_instance);
        break;
        /* note: Name and Description don't have to be the same.
           You could make Description writable and different */
    case PROP_OBJECT_NAME:
        characterstring_init_ansi(&char_string, Binary_Value_Name(rpdata->object_instance));
        apdu_len = encode_application_character_string(&apdu[0], &char_string);
        break;
    case PROP_DESCRIPTION:
        characterstring_init_ansi(&char_string, Binary_Value_Description(rpdata->object_instance));
        apdu_len = encode_application_character_string(&apdu[0], &char_string);
        break;
    case PROP_OBJECT_TYPE:
        apdu_len = encode_application_enumerated(&apdu[0], OBJECT_BINARY_VALUE);
        break;
    case PROP_PRESENT_VALUE:
        present_value = Binary_Value_Present_Value(rpdata->object_instance);
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
        apdu_len = encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
        break;
    case PROP_OUT_OF_SERVICE:
        object_index = Binary_Value_Instance_To_Index(rpdata->object_instance);
        state = BV_Out_Of_Service[object_index];
        apdu_len = encode_application_boolean(&apdu[0], state);
        break;
    case PROP_RELINQUISH_DEFAULT:
        present_value = RELINQUISH_DEFAULT;
        apdu_len = encode_application_enumerated(&apdu[0], present_value);
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
    default:
        rpdata->error_class = ERROR_CLASS_PROPERTY;
        rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
        apdu_len = BACNET_STATUS_ERROR;
        break;
    }
    
    return apdu_len;
}

/* returns true if successful */
bool Binary_Value_Write_Property(BACNET_WRITE_PROPERTY_DATA * wp_data)
{
    bool status = false;        /* return value */
    unsigned int object_index = 0;
    BACNET_BINARY_PV level = BINARY_NULL;
    // int len = 0;
    unsigned int priority = 0;
    BACNET_APPLICATION_DATA_VALUE value;

    /* decode the some of the request */
    // len = 
    bacapp_decode_application_data(wp_data->application_data, wp_data->application_data_len, &value);
    
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
                object_index = Binary_Value_Instance_To_Index(wp_data->object_instance);
                priority--;
                Binary_Value_Present_Value_Set(wp_data->object_instance,level, priority);
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
                object_index = Binary_Value_Instance_To_Index(wp_data->object_instance);
                priority = wp_data->priority;
                if (priority && (priority <= BACNET_MAX_PRIORITY)) 
                {
                    priority--;
                    BV_Present_Value[object_index] = level;
                    /* Note: you could set the physical output here to the next
                       highest priority, or to the relinquish default if no
                       priorities are set.
                       However, if Out of Service is TRUE, then don't set the
                       physical output.  This comment may apply to the
                       main loop (i.e. check out of service before changing output) */
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
        status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_BOOLEAN, &wp_data->error_class, &wp_data->error_code);
        if (status) 
        {
            object_index = Binary_Value_Instance_To_Index(wp_data->object_instance);
            if(object_index < MAX_BV)
            {
                BV_Out_Of_Service[object_index] = value.type.Boolean;
            }
            else
            {
                status = false;
                wp_data->error_class = ERROR_CLASS_OBJECT;
                wp_data->error_code = ERROR_CODE_UNKNOWN_OBJECT;
            }
        }
        break;
    default:
        wp_data->error_class = ERROR_CLASS_PROPERTY;
        wp_data->error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
        break;
    }

    return status;
}



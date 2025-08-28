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

/* Analog Value Objects - customize for your use */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "bacdef.h"
#include "bacdcode.h"
#include "bacenum.h"
#include "bacapp.h"
#include "config.h"     /* the custom stuff */

#include "av.h"
#include "handlers.h"
#include "AddrList.h"

static uint16_t MAX_ANALOG_VALUES= 0;


/* we choose to have a NULL level in our system represented by */
/* a particular value.  When the priorities are not in use, they */
/* will be relinquished (i.e. set to the NULL level). */
#define ANALOG_LEVEL_NULL 0
/* When all the priorities are level null, the present value returns */
/* the Relinquish Default value */
#define ANALOG_RELINQUISH_DEFAULT 0
/* Here is our Priority Array.  They are supposed to be Real, but */
/* we don't have that kind of memory, so we will use a single byte */
/* and load a Real for returning the value when asked. */
//static float Analog_Value_Level[MAX_ANALOG_VALUES];
//static uint16_t* Analog_Value_Level= (uint16_t*)MB_REG_BASE;
/* Writable out-of-service allows others to play with our Present Value */
/* without changing the physical output */
//static bool Analog_Value_Out_Of_Service[MAX_ANALOG_VALUES];

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int Analog_Value_Properties_Required[] = 
{
    PROP_OBJECT_IDENTIFIER, 
    PROP_OBJECT_NAME,
    PROP_OBJECT_TYPE,
    PROP_PRESENT_VALUE,
    PROP_STATUS_FLAGS,
    PROP_EVENT_STATE,
    PROP_OUT_OF_SERVICE,
    PROP_UNITS,
    -1
};

static const int Analog_Value_Properties_Optional[] = 
{
    PROP_DESCRIPTION,
    PROP_RELINQUISH_DEFAULT,
    -1
};

static const int Analog_Value_Properties_Proprietary[] = 
{
    -1
};


void Analog_Value_Property_Lists(
    const int **pRequired,
    const int **pOptional,
    const int **pProprietary)
{
    if (pRequired)
        *pRequired = Analog_Value_Properties_Required;
    if (pOptional)
        *pOptional = Analog_Value_Properties_Optional;
    if (pProprietary)
        *pProprietary = Analog_Value_Properties_Proprietary;

    return;
}


void Analog_Value_Init(
    void)
{
//    unsigned i;
    
    MAX_ANALOG_VALUES= BACnet_AvCount();
    /* initialize all the analog output priority arrays to NULL */
    return;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need validate that the */
/* given instance exists */

bool Analog_Value_Valid_Instance(
    uint32_t object_instance)
{
    if(object_instance < MAX_ANALOG_VALUES)
    {
        return true;
    }
   
    return false;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then count how many you have */

unsigned Analog_Value_Count(
    void)
{
    return MAX_ANALOG_VALUES;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */

uint32_t Analog_Value_Index_To_Instance(
    unsigned index)
{
    if(index < MAX_ANALOG_VALUES)
    {
        return index;
    }
    
    return MAX_ANALOG_VALUES;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the index */
/* that correlates to the correct instance number */

unsigned Analog_Value_Instance_To_Index(
    uint32_t object_instance)
{
    if(object_instance <MAX_ANALOG_VALUES)
    {
        return object_instance;
    }

    return MAX_ANALOG_VALUES;
}


bool Analog_Value_Present_Value_Set(
    uint32_t object_instance,
    float value,
    uint8_t priority)
{
    bool status = false;
    
    printf("[%s:%s:%d] object_instance = %d  \n", 
              __FILE__, __FUNCTION__, __LINE__, object_instance);

    printf("[%s:%s:%d] MAX_ANALOG_VALUES = %d  \n", 
              __FILE__, __FUNCTION__, __LINE__, MAX_ANALOG_VALUES);

    if (object_instance < MAX_ANALOG_VALUES) 
    {
        status = BACnet_AvPresentValueSet(object_instance, value, priority);
    }
    return status;
}


float Analog_Value_Present_Value(
    uint32_t object_instance)
{
    float value = ANALOG_RELINQUISH_DEFAULT;

    if (object_instance < MAX_ANALOG_VALUES) 
    {
            value= BACnet_AvPresentValue(object_instance);
    }
    
    return value;
}

/* note: the object name must be unique within this device */

char *Analog_Value_Name(
    uint32_t object_instance)
{
    if (object_instance < MAX_ANALOG_VALUES) 
    {
            return BACnetAnalogValueName(object_instance);
    }

    return NULL;
}

 
char *Analog_Value_Description(uint32_t object_instance)
{
    if (object_instance < MAX_ANALOG_VALUES) 
    {
        return BACnetAnalogValueDescription(object_instance);
    }

    return NULL;
}

/* return apdu len, or BACNET_STATUS_ERROR on error */

int Analog_Value_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata)
{
    int apdu_len = 0;   /* return value */
    BACNET_BIT_STRING bit_string;
    BACNET_CHARACTER_STRING char_string;
    float real_value = (float) 1.414;
    // unsigned object_index = 0;
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
        apdu_len = encode_application_object_id(&apdu[0], OBJECT_ANALOG_VALUE, rpdata->object_instance);
        break;
    case PROP_OBJECT_NAME:
        characterstring_init_ansi(&char_string, Analog_Value_Name(rpdata->object_instance));
        apdu_len = encode_application_character_string(&apdu[0], &char_string);
        break;
    case PROP_DESCRIPTION:
        characterstring_init_ansi(&char_string, Analog_Value_Description(rpdata->object_instance));
        apdu_len = encode_application_character_string(&apdu[0], &char_string);
        break;
    case PROP_OBJECT_TYPE:
        apdu_len = encode_application_enumerated(&apdu[0], OBJECT_ANALOG_VALUE);
        break;
    case PROP_PRESENT_VALUE:
        real_value = Analog_Value_Present_Value(rpdata->object_instance);
        apdu_len = encode_application_real(&apdu[0], real_value);
        break;
    case PROP_STATUS_FLAGS:
        bitstring_init(&bit_string);
        bitstring_set_bit(&bit_string, STATUS_FLAG_IN_ALARM, false);
        bitstring_set_bit(&bit_string, STATUS_FLAG_FAULT, false);
        bitstring_set_bit(&bit_string, STATUS_FLAG_OVERRIDDEN, false);
        bitstring_set_bit(&bit_string, STATUS_FLAG_OUT_OF_SERVICE, false);
        apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
        break;
    case PROP_EVENT_STATE:
        apdu_len = encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
        break;
    case PROP_OUT_OF_SERVICE:
        Analog_Value_Instance_To_Index(rpdata->object_instance);
        state= false;
//        state = Analog_Value_Out_Of_Service[object_index];
        apdu_len = encode_application_boolean(&apdu[0], state);
        break;
    case PROP_UNITS:
        apdu_len = encode_application_enumerated(&apdu[0], UNITS_NO_UNITS);
        break;
    case PROP_RELINQUISH_DEFAULT:
        real_value = ANALOG_RELINQUISH_DEFAULT;
        apdu_len = encode_application_real(&apdu[0], real_value);
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

bool Analog_Value_Write_Property(BACNET_WRITE_PROPERTY_DATA * wp_data)
{
    bool status = false;        /* return value */
    unsigned int object_index = 0;
        uint16_t level = ANALOG_LEVEL_NULL;
    // int len = 0;
    BACNET_APPLICATION_DATA_VALUE value;

    /* decode the some of the request */
    bacapp_decode_application_data(wp_data->application_data, wp_data->application_data_len, &value);
    
        printf("[%s:%s:%d] wp_data->object_property = %d  \n", 
                  __FILE__, __FUNCTION__, __LINE__, wp_data->object_property);
    /* FIXME: len < application_data_len: more data? */
    /* FIXME: len == 0: unable to decode? */
    switch (wp_data->object_property) 
    {
    case PROP_PRESENT_VALUE:
        if (value.tag == BACNET_APPLICATION_TAG_REAL) 
        {
                    /* Command priority 6 is reserved for use by Minimum On/Off
                         algorithm and may not be used for other purposes in any
                         object. */
                    if (Analog_Value_Present_Value_Set(wp_data->object_instance, value.type.Real, wp_data->priority)) 
                    {
                            status = true;
                    } 
                    else 
                    {
                        wp_data->error_class = ERROR_CLASS_PROPERTY;
                        wp_data->error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                    }
        } 
        else 
        {
            status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_NULL, &wp_data->error_class, &wp_data->error_code);
            if (status) 
            {
                level = ANALOG_LEVEL_NULL;
                object_index = Analog_Value_Instance_To_Index(wp_data->object_instance);

                if (object_index < MAX_ANALOG_VALUES) 
                {
                                    Analog_Value_Present_Value_Set(wp_data->object_instance, level, wp_data->priority);
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
                    wp_data->error_class = ERROR_CLASS_OBJECT;
                    wp_data->error_code = ERROR_CODE_UNKNOWN_OBJECT;
                }
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



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

/* Analog Input Objects customize for your use */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "bacdef.h"
#include "bacdcode.h"
#include "bacenum.h"
#include "config.h"     /* the custom stuff */
#include "handlers.h"
#include "ai.h"

#include "variables.h"
#include "mb_var_def.h"

#define BAI_MAX_QTY		4

#ifndef BAI_MAX_QTY
	#define BAI_MAX_QTY 0
#endif

const BAC_VAR_SET Var_BAI_Table[BAI_MAX_QTY]=
{
	{HR_DUCT_DP, 					0, "vav_duct_dp", 				"Valve Delta Pressure", UNITS_PASCALS}, 
	{HR_DAMPER_POSITION, 	1, "damper_position", 		"Damper Position", UNITS_PERCENT}, 
	{HR_LOCAL_ZONE_TEMP, 	2, "local_zone_temp", 		"Local Zone Temperature", UNITS_DEGREES_CELSIUS},  
	{HR_VAV_DAT, 				 	3, "vav_dat", 						"Discharge Air Temperature", UNITS_DEGREES_CELSIUS}, 
};

bool AI_Out_Of_Service[BAI_MAX_QTY];

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int Properties_Required[] = {
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

static const int Properties_Optional[] = {
    PROP_DESCRIPTION,
	PROP_DEVICE_TYPE,
    -1
};

static const int Properties_Proprietary[] = {
    -1
};

void Analog_Input_Property_Lists(
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

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need validate that the */
/* given instance exists */
bool Analog_Input_Valid_Instance(
    uint32_t object_instance)
{
    unsigned int index;

    index = Analog_Input_Instance_To_Index(object_instance);
    if (index < BAI_MAX_QTY)
        return true;

    return false;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then count how many you have */
unsigned Analog_Input_Count(void)
{
	return BAI_MAX_QTY;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */
uint32_t Analog_Input_Index_To_Instance(unsigned index)
{
	return index;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the index */
/* that correlates to the correct instance number */
unsigned Analog_Input_Instance_To_Index(uint32_t object_instance)
{
	return object_instance;
}

float Analog_Input_Present_Value(
    uint32_t object_instance)
{
	float value = 0.0;
	unsigned int index;

	index = Analog_Input_Instance_To_Index(object_instance);
	if (index < BAI_MAX_QTY) 
	{
		value= VAR_GetRealValue(Var_BAI_Table[index].addr); 
	}

  return value; 
}

bool Analog_Input_Present_Value_Set(
    uint32_t object_instance,
    float value)
{
//	unsigned int index;

//	index = Analog_Input_Instance_To_Index(object_instance);
//	if (index < BAI_MAX_QTY) 
//	{
//		if(AI_Out_Of_Service[index])
//		{
//			AI_Present_Value[index] = value;
//		}
//		return true;
//	}
	return false;
}

char *Analog_Input_Name(
    uint32_t object_instance)
{
	char* text_string;   /* okay for single thread */
	unsigned int index;

	index = Analog_Input_Instance_To_Index(object_instance);
	if (index < BAI_MAX_QTY) 
	{
		text_string= (char*)Var_BAI_Table[index].name;
		
		return text_string;
	}

	return NULL;
}

char *Analog_Input_Description(
    uint32_t object_instance)
{
	char* text_string;   /* okay for single thread */
	unsigned int index;

	index = Analog_Input_Instance_To_Index(object_instance);
	if (index < BAI_MAX_QTY) 
	{
    text_string= (char*)Var_BAI_Table[index].description;    
		return text_string;
	}

   return NULL;
}

BACNET_ENGINEERING_UNITS Analog_Input_Unit(uint32_t object_instance)
{
	unsigned int index;

	index = Analog_Input_Instance_To_Index(object_instance);
	
	if(index < BAI_MAX_QTY)
	{
		return Var_BAI_Table[index].unit; 
	}
	return UNITS_NO_UNITS;
}

/* return apdu length, or BACNET_STATUS_ERROR on error */
/* assumption - object has already exists */
int Analog_Input_Read_Property(
    BACNET_READ_PROPERTY_DATA * rpdata)
{
    int apdu_len = 0;   /* return value */
    BACNET_BIT_STRING bit_string;
    BACNET_CHARACTER_STRING char_string;
    uint8_t *apdu = NULL;

    if ((rpdata == NULL) || (rpdata->application_data == NULL) ||
        (rpdata->application_data_len == 0)) {
        return 0;
    }
    apdu = rpdata->application_data;
    switch (rpdata->object_property) 
		{
        case PROP_OBJECT_IDENTIFIER:
            apdu_len =
                encode_application_object_id(&apdu[0], OBJECT_ANALOG_INPUT,
                rpdata->object_instance);
            break;
        case PROP_OBJECT_NAME:
			characterstring_init_ansi(&char_string,
                Analog_Input_Name(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
			break;
        case PROP_DESCRIPTION:
            characterstring_init_ansi(&char_string,
                Analog_Input_Description(rpdata->object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_TYPE:
            apdu_len =
                encode_application_enumerated(&apdu[0], OBJECT_ANALOG_INPUT);
            break;
        case PROP_PRESENT_VALUE:
            apdu_len =
                encode_application_real(&apdu[0],
                Analog_Input_Present_Value(rpdata->object_instance));
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
            apdu_len =
                encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
            break;
        case PROP_OUT_OF_SERVICE:
            apdu_len = encode_application_boolean(&apdu[0], false);
            break;
        case PROP_UNITS:
            apdu_len = encode_application_enumerated(&apdu[0], Analog_Input_Unit(rpdata->object_instance));
            break;
		case PROP_DEVICE_TYPE:
					characterstring_init_ansi(&char_string, "analog inputs");
          apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        default:
            rpdata->error_class = ERROR_CLASS_PROPERTY;
            rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
            apdu_len = BACNET_STATUS_ERROR;
            break;
    }
    /*  only array properties can have array options */
    if ((apdu_len >= 0) && (rpdata->array_index != BACNET_ARRAY_ALL)) {
        rpdata->error_class = ERROR_CLASS_PROPERTY;
        rpdata->error_code = ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY;
        apdu_len = BACNET_STATUS_ERROR;
    }

    return apdu_len;
}


/* returns true if successful */
bool Analog_Input_Write_Property(
    BACNET_WRITE_PROPERTY_DATA * wp_data)
{
	bool status = false;        /* return value */
	// int len = 0;
	unsigned int object_index = 0;
	BACNET_APPLICATION_DATA_VALUE value;

    /* decode the some of the request */
	// len = 
	bacapp_decode_application_data(wp_data->application_data, wp_data->application_data_len, &value);
    /* FIXME: len < application_data_len: more data? */
    /* FIXME: len == 0: unable to decode? */
    switch (wp_data->object_property) {
        case PROP_PRESENT_VALUE:
					if (value.tag == BACNET_APPLICATION_TAG_REAL) 
					{
						/* Command priority 6 is reserved for use by Minimum On/Off
							 algorithm and may not be used for other purposes in any
							 object. */
						if (Analog_Input_Present_Value_Set(wp_data->object_instance, value.type.Real)) 
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
                object_index = Analog_Input_Instance_To_Index(wp_data->object_instance);

                if (object_index < BAI_MAX_QTY) 
                {
                    status = true;
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
    case PROP_OUT_OF_SERVICE:
        status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_BOOLEAN, &wp_data->error_class, &wp_data->error_code);
        if (status) 
        {
            object_index = Analog_Input_Instance_To_Index(wp_data->object_instance);
            if (object_index < BAI_MAX_QTY) 
            {
				AI_Out_Of_Service[object_index] = value.type.Boolean;
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

void Analog_Input_Init(
    void)
{
	static bool initialized = false;

  if (!initialized) 
	{
		initialized = true;

    /* initialize all the values */
	}
  return;
}



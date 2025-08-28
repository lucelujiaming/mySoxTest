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

#include "variables.h"
#include "mb_var_def.h"

#define BAV_MAX_QTY        57

#ifndef BAV_MAX_QTY
    #define BAV_MAX_QTY 0
#endif

const BAC_VAR_SET Var_BAV_Table[BAV_MAX_QTY]=
{
    {HR_UNIT_FAULT,             0, "fault_bitmap",                 "VAV Fault",     UNITS_NO_UNITS}, 
    {HR_UNIT_ALARM,             1, "alarm_bitmap",                 "VAV Alarm",     UNITS_NO_UNITS}, 
    {HR_SAT_RST_REQ,             2, "sat_reset",                     "Supply Air Temp Reset", UNITS_NO_UNITS},  
    {HR_SAP_RST_REQ,             3, "sap_reset",                        "Supply Air Press Reset", UNITS_NO_UNITS}, 
    {HR_HWT_RST_REQ,             4, "hwt_reset",                        "Hot Water Temp Reset", UNITS_NO_UNITS}, 
    
    {HR_AHU_SAT,                     5, "ahu_sat",                            "Supply Air Temp", UNITS_DEGREES_CELSIUS}, 
    {HR_CLG_SETPT,                 6, "clg_setpt",                        "Cooling Temp Setpoint", UNITS_DEGREES_CELSIUS}, 
    {HR_HTG_SETPT,                 7, "htg_setpt",                        "Heating Temp Setpoint", UNITS_DEGREES_CELSIUS}, 
    {HR_ZONE_IAQ_SETPT,     8, "iaq_setpt",                        "Indoor Air Quality Setpoint", UNITS_PARTS_PER_MILLION}, 
    {HR_STAT_ADJUST,             9, "stat_temp_set",                "Stat Temp Setpoint Set", UNITS_DEGREES_CELSIUS},
    {HR_STAT_ADJUST_MAX,     10, "stat_set_range",            "Stat Temp Setpoint Set Range", UNITS_DEGREES_CELSIUS}, 
    {HR_STAT_KEYPAD_LOCK, 11, "stat_keypad_lock",        "Stat Keypad Lock", UNITS_NO_UNITS},
    {HR_EXT_MIN_SET,             12, "stat_ext_time",            "Stat Extend Time Set", UNITS_MINUTES},

    {HR_ZONE_TEMP,                 13, "zone_temp",                    "Zone Temperature", UNITS_DEGREES_CELSIUS}, 
    {HR_ZONE_IAQ,                 14, "zone_iaq",                        "Zone Indoor Air Quality", UNITS_PARTS_PER_MILLION}, 
    
    {HR_CLG_OUTPUT,             15, "clg_req",                        "Cooling Require", UNITS_PERCENT}, 
    {HR_HTG_OUTPUT,             16, "htg_req",                        "Heating Require", UNITS_PERCENT}, 
    {HR_IAQ_OUTPUT,             17, "iaq_offset",                    "Flow Offset for IAQ", UNITS_PERCENT}, 
    {HR_FLOW_REQUIRE,         18, "flow_req",                        "Flow Require", UNITS_CUBIC_METERS_PER_HOUR}, 
    {HR_DUCT_AIR_FLOW,         19, "duct_flow",                    "Actual Flow Volume", UNITS_CUBIC_METERS_PER_HOUR}, 
    
    {HR_OCC_CLG_SETPT,         20, "occ_clg_setpt",            "Occ Cooling Setpt", UNITS_DEGREES_CELSIUS}, 
    {HR_OCC_HTG_SETPT,         21, "occ_htg_setpt",            "Occ Heating Setpt", UNITS_DEGREES_CELSIUS}, 
    {HR_UNOCC_CLG_SETPT,     22, "unocc_clg_setpt",        "Unocc Cooling Setpt", UNITS_DEGREES_CELSIUS}, 
    {HR_UNOCC_HTG_SETPT,     23, "unocc_htg_setpt",        "Unocc Heating Setpt", UNITS_DEGREES_CELSIUS}, 
    
    {HR_CLG_FLOW_MAX,         24, "clg_flow_max",                "Cooling Flow Max", UNITS_CUBIC_METERS_PER_HOUR}, 
    {HR_FLOW_MIN,                 25, "clg_flow_min",                "Cooling Flow Min", UNITS_CUBIC_METERS_PER_HOUR}, 
    {HR_HTG_FLOW_MAX,         26, "htg_flow_max",                "Heating Flow Max", UNITS_CUBIC_METERS_PER_HOUR}, 
    {HR_RHTG_FLOW_MIN,         27, "rhtg_flow_min",            "Reheating Flow Min", UNITS_CUBIC_METERS_PER_HOUR}, 
    
    {HR_SAT_CLG_LOCK,         28, "sat_clg_lock",                "SAT Clg Mode Hi Limit", UNITS_DEGREES_CELSIUS},
    {HR_SAT_HTG_LOCK,         29, "sat_htg_lock",                "SAT Htg Mode Lo Limit", UNITS_DEGREES_CELSIUS},
    
    {HR_FORCE_DMP_VALUE,     30, "force_dmp_value",         "Force Damper Value", UNITS_PERCENT}, 
    {HR_FORCE_FLW_VALUE,     31, "force_flw_value",        "Force Flow Value", UNITS_CUBIC_METERS_PER_HOUR}, 
    {HR_AO_MAN_VALUE,         32, "force_ao_value",            "Force Analog Output Value", UNITS_PERCENT}, 
    
    {HR_STAT_TEMP_OFFSET, 33, "stat_temp_offset",     "Stat Temp Error Correction", UNITS_DEGREES_CELSIUS}, 
    {HR_EXT_MIN_SET,             34, "overtime_set",                "Overtime Set", UNITS_MINUTES}, 
    {HR_IAQ_OUTPUT    ,         35, "iaq_req",                        "Indoor Air Quality Require", UNITS_PERCENT}, 
    
    {HR_LOCAL_TEMP_OFFSET,    36, "local_temp_offset",         "Local Temp Error Correction", UNITS_DEGREES_CELSIUS}, 
    {HR_VAV_DAT_HI,                 37, "dat_hi_limit",                "Discharge Air Temp High Limit", UNITS_DEGREES_CELSIUS}, 
    {HR_HTG_STAGE_ON_MINUTES, 38, "htg_stage_min_on",        "Heating Stage Min Running Time", UNITS_MINUTES}, 
    
    {HR_HTG_STAGE_OFF_MINUTES, 39, "htg_stage_min_off", "Heating Stage Min Stop Time", UNITS_MINUTES}, 
    {HR_AUX_FAN_ON_MINUTES,     40, "aux_fan_min_on",        "Aux Fan Min Running Time", UNITS_MINUTES}, 
    {HR_AUX_FAN_OFF_MINUTES,     41, "aux_fan_min_off",    "Aux Fan Min Stop Time", UNITS_MINUTES}, 
    
    {HR_CLG_DB,                     42, "clg_pid_db",                 "Cooling PID Dead Band", UNITS_DEGREES_CELSIUS}, 
    {HR_CLG_PB,                     43, "clg_pid_pb",                    "Cooling PID Porp. Band", UNITS_DEGREES_CELSIUS}, 
    {HR_CLG_I,                         44, "clg_pid_i",                    "Cooling PID Integration", UNITS_NO_UNITS},  
    
    {HR_HTG_DB,                     45, "htg_pid_db",                 "Heating PID Dead Band", UNITS_DEGREES_CELSIUS}, 
    {HR_HTG_PB,                     46, "htg_pid_pb",                    "Heating PID Porp. Band", UNITS_DEGREES_CELSIUS}, 
    {HR_HTG_I,                         47, "htg_pid_i",                    "Heating PID Integration", UNITS_NO_UNITS}, 
    
    {HR_DAT_DB,                     48, "dat_pid_db",         "DAT PID Dead Band", UNITS_DEGREES_CELSIUS}, 
    {HR_DAT_PB,                     49, "dat_pid_pb",            "DAT PID Porp. Band", UNITS_DEGREES_CELSIUS}, 
    {HR_DAT_I,                         50, "dat_pid_i",            "DAT PID Integration", UNITS_NO_UNITS}, 
    
    {HR_FLOW_DB,                     51, "flow_pid_db",         "Flow PID Dead Band", UNITS_CUBIC_METERS_PER_HOUR}, 
    {HR_FLOW_PB,                     52, "flow_pid_pb",        "Flow PID Prop. Band", UNITS_CUBIC_METERS_PER_HOUR}, 
    {HR_FLOW_I,                     53, "flow_pid_i",            "DAT PID Integration", UNITS_NO_UNITS}, 
    {HR_FLOW_D,                     54, "flow_pid_d",         "DAT PID Differential", UNITS_NO_UNITS}, 
    
    {HR_STDBY_TEMP_OFFSET,         55, "standby_temp_offset",         "Standby Mode Temp Offset", UNITS_DEGREES_CELSIUS}, 
    
    {DBG_ADPTION,                 56, "dbg_adption",         "dbg_adption", UNITS_NO_UNITS}, 
};

/* we choose to have a NULL level in our system represented by */
/* a particular value.  When the priorities are not in use, they */
/* will be relinquished (i.e. set to the NULL level). */
#define AV_LEVEL_NULL 32767
/* When all the priorities are level null, the present value returns */
/* the Relinquish Default value */
#define AV_RELINQUISH_DEFAULT 0
/* Here is our Priority Array.  They are supposed to be Real, but */
/* we don't have that kind of memory, so we will use a single byte */
/* and load a Real for returning the value when asked. */
//static float AV_Present_Value[BAV_MAX_QTY];
/* Writable out-of-service allows others to play with our Present Value */
/* without changing the physical output */
static bool AV_Out_Of_Service[BAV_MAX_QTY];

/* Change of Value flag */
// static bool AV_Change_Of_Value_Table[BAV_MAX_QTY];

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
        PROP_PRIORITY_ARRAY,
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
    static bool initialized = false;

    if (!initialized) 
    {
        initialized = true;
    }

    return;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need validate that the */
/* given instance exists */
bool Analog_Value_Valid_Instance(
    uint32_t object_instance)
{
    if(object_instance < BAV_MAX_QTY)
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
    return BAV_MAX_QTY;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */
uint32_t Analog_Value_Index_To_Instance(
    unsigned index)
{
    return index;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the index */
/* that correlates to the correct instance number */
unsigned Analog_Value_Instance_To_Index(
    uint32_t object_instance)
{
    unsigned index = BAV_MAX_QTY;

    if(Analog_Value_Valid_Instance(object_instance))
    {
        index= object_instance;
    }

    return index;
}


bool Analog_Value_Present_Value_Set(
    uint32_t object_instance,
    float value,
    uint8_t priority)
{
    unsigned index = 0;
    bool status = false;

    index = Analog_Value_Instance_To_Index(object_instance);
    if (index < BAV_MAX_QTY) 
    {
            if (priority && (priority <= BACNET_MAX_PRIORITY) &&
                (priority != 6 /* reserved */ )) 
            {
                    VAR_SetRealValue(Var_BAV_Table[index].addr, value, 0);

                /* Note: you could set the physical output here to the next
                     highest priority, or to the relinquish default if no
                     priorities are set.
                     However, if Out of Service is TRUE, then don't set the
                     physical output.  This comment may apply to the
                     main loop (i.e. check out of service before changing output) */
                status = true;
            }
    }
    return status;
}


float Analog_Value_Present_Value(
    uint32_t object_instance)
{
    float value = AV_RELINQUISH_DEFAULT;
    unsigned index = 0;

    index = Analog_Value_Instance_To_Index(object_instance);
    if (index < BAV_MAX_QTY) 
    {
        value= VAR_GetRealValue(Var_BAV_Table[index].addr); 
    }

    return value;
}

char *Analog_Value_Description(
    uint32_t object_instance)
{
    char* text_string;   /* okay for single thread */
    unsigned int index;

    index = 
        Analog_Value_Instance_To_Index(object_instance);
    if (index < BAV_MAX_QTY) 
    {
        text_string= (char*)Var_BAV_Table[index].description;    
        return text_string;
    }

   return NULL;
}

BACNET_ENGINEERING_UNITS Analog_Value_Unit(uint32_t object_instance)
{
    BACNET_ENGINEERING_UNITS units;
    unsigned int index;

    index = Analog_Value_Instance_To_Index(object_instance);
    if (index < BAV_MAX_QTY) 
    {
        units= Var_BAV_Table[index].unit;    
        return units;
    }

   return UNITS_NO_UNITS;
}

bool Analog_Value_Change_Of_Value(
    uint32_t object_instance)
{
    bool status = false;
    unsigned index;

    index = Analog_Value_Instance_To_Index(object_instance);
        if (index < BAV_MAX_QTY) 
        {
            status = true;//AV_Change_Of_Value_Table[index];
        }

    return status;
}

void Analog_Value_Change_Of_Value_Clear(
    uint32_t object_instance)
{
    // unsigned index;

//    index = Analog_Value_Instance_To_Index(object_instance);
//    if (index < BAV_MAX_QTY) 
//    {
//        AV_Change_Of_Value_Table[index] = false;
//    }

    return;
}

/* note: the object name must be unique within this device */

char *Analog_Value_Name(
    uint32_t object_instance)
{
    char* text_string;   /* okay for single thread */
    unsigned int index;

    index = Analog_Value_Instance_To_Index(object_instance);
    if (index < BAV_MAX_QTY) 
    {
        text_string= (char*)Var_BAV_Table[index].name;
        
        return text_string;
    }

    return NULL;
}

/* return apdu len, or BACNET_STATUS_ERROR on error */

int Analog_Value_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata)
{
    int apdu_len = 0;   /* return value */
    BACNET_BIT_STRING bit_string;
    BACNET_ENGINEERING_UNITS units;
    BACNET_CHARACTER_STRING char_string;
    float real_value = 0.0;
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
        object_index = Analog_Value_Instance_To_Index(rpdata->object_instance);
        state = AV_Out_Of_Service[object_index];
        apdu_len = encode_application_boolean(&apdu[0], state);
        break;
    case PROP_UNITS:
                units= Analog_Value_Unit(rpdata->object_instance);
        apdu_len = encode_application_enumerated(&apdu[0], units);
        break;
    case PROP_RELINQUISH_DEFAULT:
        real_value = AV_RELINQUISH_DEFAULT;
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
    // uint16_t level = AV_LEVEL_NULL;
    // int len = 0;
    BACNET_APPLICATION_DATA_VALUE value;
    // uint8_t i;

    /* decode the some of the request */
    // len = 
    bacapp_decode_application_data(wp_data->application_data, wp_data->application_data_len, &value);
    
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
                // level = AV_LEVEL_NULL;
                object_index = Analog_Value_Instance_To_Index(wp_data->object_instance);

                if (object_index < BAV_MAX_QTY) 
                {
//                                AV_Present_Value[object_index] = level;
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
    case PROP_OUT_OF_SERVICE:
        status = WPValidateArgType(&value, BACNET_APPLICATION_TAG_BOOLEAN, &wp_data->error_class, &wp_data->error_code);
        if (status) 
        {
            object_index = Analog_Value_Instance_To_Index(wp_data->object_instance);
            if (object_index < BAV_MAX_QTY) 
            {
                AV_Out_Of_Service[object_index] = value.type.Boolean;
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



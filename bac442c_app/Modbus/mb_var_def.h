
#ifndef __MB_VAR_DEF_H__
#define __MB_VAR_DEF_H__

#include "hardware_def.h"

#define FIRMWARE_VERSION	170

#define HR_UNIT_FAULT			0		
#define HR_UNIT_ALARM			1

#define HR_COMMISSION_OVRD		2		//Commission Overide
#define HR_SAT_RST_REQ				3		//Supply Air Temp Reset Request
#define HR_SAP_RST_REQ				4		//Static Press Reset Request
#define HR_HWT_RST_REQ				5

#define HR_EFFECT_OCC					6		//
#define HR_EFFECT_HVAC_MODE		7
#define HR_CLG_SETPT			8
#define HR_HTG_SETPT			9

#define HR_AHU_OCC_REQ				10	
#define HR_AHU_OCC_ST					11
#define HR_SCHEDUULE_OCC			12	//
#define HR_HVAC_MODE_OVRD			13

#define HR_LOCAL_ZONE_TEMP		14
#define HR_VAV_DAT						15
#define HR_DUCT_DP						16
#define HR_AHU_SAT						17

#define HR_DAMPER_POSITION		18

#define HR_HTG_VLV_OUTPUT			19

#define HR_HTG_STAGE_CMD	20
#define HR_AUX_FAN_CMD		21

#define HR_STAT_OCC				22		//0:UNOCC, 1:OCC
#define HR_STAT_ADJUST		23
#define HR_STAT_TEMP			24
#define HR_STAT_ADJUST_MAX	25
#define HR_STAT_MODE_SET		26
#define HR_STAT_TEMP_OFFSET	27
#define HR_STAT_IAQ_OFFSET	28
#define HR_STAT_KEYPAD_LOCK	29
#define HR_STAT_EXT_MIN			30

#define HR_ZONE_TEMP			31
#define HR_ZONE_IAQ				32

#define HR_CLG_OUTPUT		33
#define HR_HTG_OUTPUT		34
#define HR_IAQ_OUTPUT		35

#define HR_FLOW_REQUIRE			36
#define HR_DUCT_AIR_FLOW		37

#define HR_EXT_MIN_SET			38

#define HR_OCC_CLG_SETPT					39
#define HR_OCC_HTG_SETPT					40
#define HR_UNOCC_CLG_SETPT				41
#define HR_UNOCC_HTG_SETPT				42

#define HR_LOCAL_TEMP_TYPE				43
#define HR_LOCAL_TEMP_OFFSET			44

#define HR_LOCAL_TEMP_FUNCTION		45
#define HR_VAV_DAT_HI		46

#define HR_HTG_TYPE			47
#define HR_HTG_STAGE_ON_MINUTES			48
#define HR_HTG_STAGE_OFF_MINUTES		49

#define HR_HTG_VAVE_TYPE					50	//0:0-10V, 1:2-10V

#define HR_AUX_FAN_TYPE						51
#define HR_AUX_FAN_ON_MINUTES			52
#define HR_AUX_FAN_OFF_MINUTES		53

#define HR_CLG_FLOW_MAX				54
#define HR_FLOW_MIN						55
#define HR_HTG_FLOW_MAX				56
#define HR_RHTG_FLOW_MIN			57

#define HR_ZONE_IAQ_SETPT			58

#define HR_SAT_CLG_LOCK				59
#define HR_SAT_HTG_LOCK				60

#define HR_CLG_DB		61
#define HR_CLG_PB		62
#define HR_CLG_I		63

#define HR_HTG_DB		64
#define HR_HTG_PB		65
#define HR_HTG_I		66

#define HR_DAT_DB		67
#define HR_DAT_PB		68
#define HR_DAT_I		69

#define HR_FLOW_DB	70
#define HR_FLOW_PB	71
#define HR_FLOW_I		72
#define HR_FLOW_D		73

#define HR_FORCE_DMP_VALUE	74
#define HR_FORCE_FLW_VALUE	75

#define HR_UNOCC_FLW_ZERO		76 
#define HR_AO_MAN_VALUE			77

#if HW_VERSION < 23 
	#define HR_ACT_10_NM	78
#endif
	
#define HR_STDBY_TEMP_OFFSET	79

#define DBG_ACT_POS_SETPT		80
//#define DBG_OPEN_RST				81
//#define DBG_CLOSE_RST			 	82
//#define DBG_TIMER_RST				83
#define DBG_PRESS_SPL_CNT		84
#define DBG_PRESS_EXP_CNT		85
#define DBG_SAT_STB_DELAY		86
#define DBG_TMR_SETBACK_UP_DLY		87
#define DBG_TMR_CLG_DIS_DLY		88
#define DBG_TMR_HTG_DIS_DLY		89
#define DBG_TMR_FAN_OFF_DLY	90
#define DBG_TMR_FAN_ON_MINS	91
#define DBG_TMR_FAN_OFF_MINS	92
#define DBG_TMR_HTG_STG_ON_MINS	93
#define DBG_TMR_HTG_STG_OFF_MINS	94
#define DBG_TMR_HTG_FLOW_READY	95
#define DBG_ADPTION					96

#define HR_ACT_ADAPT	 	97
#define HR_SW_VERSION		98
#define HR_UNIT_RESET		99
// Device
#define HR_UNIT_MAC				100
#define HR_USART_BAUD			101
#define HR_USART_PARITY		102
#define HR_USART_TYPE			103		//0:Modbus, 1:MSTP Master, 2:MSTP Slave

#define HR_DUCT_DP_COEFF	104
#define HR_DUCT_DP_OFFSET	105
#define HR_DUCT_AREA			106

#if	HW_VERSION >= 23	 
#define HR_ACT_CONFIG			109		//Nm, Direction
#endif

#define HR_INSTANCE_L			110
#define HR_INSTANCE_H			111
#define HR_MAX_INFO_FRAME	112
#define HR_MAX_MASTER			113
#define HR_LOCATION				114			//16 Register

#define HR_UPD_STATUS			130		//0: None, 1: Success, 2: Busy, 3: Error
#define HR_UPD_CMD				131		//0: None, 1: Complete, 2: Formate Flash, 3: Earase BACnet EEPROM, 4:Earase Config EEPROM, 5: Earase Update EEPROM
#define HR_UPD_FILE_SIZE_L	132
#define HR_UPD_FILE_SIZE_H	133
#define HR_UPD_FILE_CRC			134
#define HR_UPD_TIME_STATMP_L	135
#define HR_UPD_TIME_STATMP_H	136

//////////////////////////////////////////////////
//Keypad Lock
#define BIT_KEY_LCK_ONOFF		0
#define BIT_KEY_LCK_TEMP		1
#define BIT_KEY_LCK_MODE		2

#define BIT_FLT_TEMP_SENSOR 0
#define BIT_FLT_FLOW_SENSOR	1
#define BIT_FLT_LEAK_DMP		2
#define BIT_FLT_LEAK_VLV		3
//#define BIT_FLT_FAN_FAIL		4

#define BIT_FLT_MOTOR_LO		8
#define BIT_FLT_MOTOR_HI		9
#define BIT_FLT_POT_LO			10
#define BIT_FLT_POT_HI			11

#define BIT_ALM_LOW_SAF		0	
#define BIT_ALM_LOW_DAT		1
#define BIT_ALM_HIGH_DAT	2		//0:OFF, 1:ON

//Commission Override
typedef enum
{
	OVRD_NONE=	0,
	OVRD_FLW_SET_TO_ZERO=	1,
	OVRD_FLW_SET_TO_VCOOL_MAX= 2,
	OVRD_FLW_SET_TO_VCOOL_MIN= 3,
	OVRD_DMP_TO_CLOSE= 4,
	OVRD_DMP_TO_OPEN= 5,
	OVRD_HTG_TO_OFF= 6,
	OVRD_HTG_TO_ON= 7,
	OVRD_FAN_TO_OFF= 8,
	OVRD_FAN_TO_ON= 9,
	OVRD_FORCE_DAMPER= 10,
	OVRD_FORCE_FLOW= 11
}COMMISSION_TSET;

//Unit Alarm
typedef enum
{
	ALM_NORMAL= 0,
	ALM_LOW_SA_FLOW= 1,
	ALM_LOW_DA_TEMP= 2,
	ALM_FAN_FAIL_START= 3,
	ALM_FAN_FAIL_STOP= 4,
	ALM_FLOW_SENSOR= 5,
	ALM_LEAKING_DMP= 6,
	ALM_LEAKING_VLV= 7
}UNIT_ALARM;

typedef enum{
	MODE_NONE= 0,
	MODE_HEAT = 1,
	MODE_COOL = 2,
	MODE_WARMUP = 3,
	MODE_COOLDOWN= 4,
	MODE_SETBACK= 5,
	MODE_SETUP= 6,
	MODE_PURGE = 7,
} HVAC_MODE;

typedef enum{
	FAN_NONE= 0,
	FAN_SERIAL = 1,
	FAN_PAALLEL = 2,
} FAN_TYPE;

typedef enum{
	HTG_NONE= 0,
	HTG_STAGE = 1,
	HTG_MODULATE = 2,
} HEAT_TYPE;

typedef enum{
	TFN_NONE= 0,
	TFN_ZONE = 1,
	TFN_SAT = 2,
	TFN_DAT = 3
} TEMP_FUNCTION;

typedef enum{
	OCC_MODE_NONE= 0,
	OCC_MODE_UNOCC = 1,
	OCC_MODE_OCC = 2,
} OCC_MODE;

typedef enum
{
	UPD_CMD_NONE= 0,
	UPD_CMD_COMPLETE= 1,
	UPD_CMD_FORMAT_FLASH= 2,
	UPD_CMD_EARASE_CONFIG= 3,
	UPD_CMD_EARASE_DEVICE= 4,
	UPD_CMD_EARASE_BACNET= 5,
	UPD_CMD_EARASE_UPDATE= 6,
}UPDATE_CMD;

typedef enum
{
	UPD_ST_NONE= 0,
	UPD_ST_BUSY= 1,
	UPD_ST_SUCCESS= 2,
	UPD_ST_ERROR= 3,
}UPDATE_ST;

typedef enum{
	AT_NONE= 0,
	AT_0_10V = 1,
	AT_2_10V = 2,
} ANALOG_TYPE;

//
//HR_ACT_CONFIG
#define BIT_ACT_DIRECT	0x01	// bit 0
#define BIT_ACT_10NM		0x02	// bit 1
#endif


#ifndef __MODBUS_DEF_H__
#define __MODBUS_DEF_H__

#define MODBUS_BROADCAST_ADDR	0
#define MODBUS_RTU_MAX_FRAME	255

//define exception code
typedef enum
{
	ERR_NONE= 0,
	ERR_FUNCTION	= 1,
	ERR_DATA_ADDR = 2,
	ERR_DATA_VALUE = 3,
	ERR_SLV_FAILURE = 4,
	ACK	= 5,
	ERR_SLV_BUSY	= 6,
	ERR_SLV_NACK	= 7,
	MEMORY_PARITY_ERROR	= 8
}MB_ERROR;

//define function code
typedef enum
{
	SERVICE_READ_OUTPUT_COIL= 1,
	SERVICE_READ_INPUT_COIL	= 2,
	SERVICE_READ_HOLDING_REG= 3,
	SERVICE_READ_INPUT_REG= 4,
	SERVICE_FORCE_SINGLE_COIL	= 5,
	SERVICE_PRESET_SINGLE_REG	= 6,
	SERVICE_READ_EXCEPTION_STATUS	= 7,
	SERVICE_DIAGNOSTICS	= 8,

	SERVICE_FETCH_COMM_EVENT_COUNTER= 11,
	SERVICE_FETCH_COMM_EVENT_LOG= 12,
	SERVICE_PROGRAM_CONTROLLER= 13,
	SERVICE_POLL_CONTROLLER	= 14,
	SERVICE_FORCE_MULTIPLE_COILS= 15,
	SERVICE_PRESET_MULTIPLE_REG= 16,
	SERVICE_REPORT_SLAVE_ID	= 17,
	SERVICE_MAX= 17
}MB_SERVICE;

///////////////////////////////////////////////////
#define OCOIL_BASE			(uint16_t)1
#define OCOIL_TOP				(uint16_t)9999
#define OCOIL_EXT_BASE	(uint32_t)1
#define OCOIL_EXT_TOP		(uint32_t)65535

#define ICOIL_BASE			(uint16_t)10001
#define ICOIL_TOP				(uint16_t)19999
#define ICOIL_EXT_BASE	(uint32_t)100001
#define ICOIL_EXT_TOP		(uint32_t)165535

#define IREG_BASE				(uint16_t)30001
#define IREG_TOP				(uint16_t)39999
#define IREG_EXT_BASE		(uint32_t)300001
#define IREG_EXT_TOP		(uint32_t)365535

#define HREG_BASE				(uint16_t)40001
#define HREG_TOP				(uint16_t)49999
#define HREG_EXT_BASE		(uint32_t)400001
#define HREG_EXT_TOP		(uint32_t)465535

#define INVALID_ADDR	(uint32_t)0xFFFFFFFF

#define COIL_POINTS_MAX		2000
#define REG_POINTS_MAX	125

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
	MB_RTU= 0,
	MB_TCP= 1
}MB_TYPE;

#endif

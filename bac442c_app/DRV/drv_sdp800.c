
#include <stdint.h>
#include <stdbool.h>
#include "drv_sdp800.h"
#include "common.h"

#include "hal_i2c1.h"

static uint16_t retry_times;
volatile bool sdp_valid= true;

// Sensor Commands
typedef enum {
  /// Undefined dummy command.
  COMMAND_UNDEFINED                       = 0x0000,
  /// Start continous measurement                     \n
  /// Temperature compensation: Mass flow             \n
  /// Averaging: Average till read
  COMMAND_START_MEASURMENT_MF_AVERAGE     = 0x3603,
  /// Start continous measurement                     \n
  /// Temperature compensation: Mass flow             \n
  /// Averaging: None - Update rate 1ms
  COMMAND_START_MEASURMENT_MF_NONE        = 0x3608,
  /// Start continous measurement                     \n
  /// Temperature compensation: Differential pressure \n
  /// Averaging: Average till read
  COMMAND_START_MEASURMENT_DP_AVERAGE     = 0x3615,
  /// Start continous measurement                     \n
  /// Temperature compensation: Differential pressure \n
  /// Averaging: None - Update rate 1ms
  COMMAND_START_MEASURMENT_DP_NONE        = 0x361E,
  // Stop continuous measurement.
  COMMAND_STOP_CONTINOUS_MEASUREMENT      = 0x3FF9
} Command;

static Error execute_command(Command cmd);
static Error read_raw_results(int16_t* diffPressureTicks,
                                       int16_t* temperatureTicks,
                                       uint16_t* scaleFactor);
static Error crc_check(const uint8_t data[], uint8_t size, uint8_t checksum);

static const float scaleFactorTemperature = 200;
static uint8_t _i2cAddress;

void DRV_SDP800_Init(uint8_t i2cAddress){
  _i2cAddress = i2cAddress;
}

Error DRV_SDP800_StartMeasurement(Sdp800TempComp  tempComp,
                                       Sdp800Averaging averaging)
{
  Error error= ERROR_NONE;
  return error;
}

Error DRV_SDP800_StopMeasurement(void)
{
  return execute_command(COMMAND_STOP_CONTINOUS_MEASUREMENT);
}

Error DRV_SDP800_ReadResults(float* diffPressure, float* temperature)
{
  Error error= ERROR_NONE;
  int16_t  diffPressureTicks;
  int16_t  temperatureTicks;
  uint16_t scaleFactorDiffPressure;
  
  error = read_raw_results(&diffPressureTicks, &temperatureTicks,
                                    &scaleFactorDiffPressure);
  
  if(ERROR_NONE == error) {
    *diffPressure = (float)diffPressureTicks / (float)scaleFactorDiffPressure;
    *temperature  = (float)temperatureTicks / scaleFactorTemperature;
  }
  
  return error;
}

Error DRV_SDP800_SoftReset(void)
{
  Error error= ERROR_NONE;
  return error;
}

static Error read_raw_results(int16_t*  diffPressureTicks,
                                       int16_t*  temperatureTicks,
                                       uint16_t* scaleFactor)
{
  Error error= ERROR_NONE;
  return error;
}

Error execute_command(Command cmd)
{
  Error error= ERROR_NONE;
  return error;
}

static Error crc_check(const uint8_t data[], uint8_t size, uint8_t checksum)
{
  uint8_t crc = 0xFF;
	int i;
	uint8_t bit;
  
  // calculates 8-Bit checksum with given polynomial 0x31 (x^8 + x^5 + x^4 + 1)
  for(i = 0; i < size; i++) 
	{
    crc ^= (data[i]);
    for(bit = 8; bit > 0; --bit) 
		{
      if(crc & 0x80) 
				crc = (crc << 1) ^ 0x31;
      else           
				crc = (crc << 1);
    }
  }
  
  // verify checksum
  return (crc == checksum) ? ERROR_NONE : ERROR_CHECKSUM;
}

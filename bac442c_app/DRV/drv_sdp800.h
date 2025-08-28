#ifndef __DRV_SDP800_H__
#define __DRV_SDP800_H__

#include <stdint.h>

typedef enum {
  ERROR_NONE              = 0x00, // no error
  ERROR_ACK               = 0x01, // no acknowledgment error
  ERROR_CHECKSUM          = 0x02, // checksum mismatch error
  ERROR_IVALID_PARAMETER  = 0xFF, // invalid parameter
} Error;

// Enumeration to configure the temperature compensation for measurement.
typedef enum {
  SDP800_TEMPCOMP_MASS_FLOW,
  SDP800_TEMPCOMP_DIFFERNTIAL_PRESSURE
} Sdp800TempComp;

// Enumeration to configure the averaging for measurement.
typedef enum {
  SDP800_AVERAGING_NONE,
  SDP800_AVERAGING_TILL_READ
} Sdp800Averaging;

/// \brief  Initializes the I2C bus for communication with the sensor.
/// \param  i2cAddress  Sensors I2C address.
void DRV_SDP800_Init(uint8_t i2cAddress);

/// \brief  Starts the continous mesurement with the specified settings.
/// \param  tempComp  Temperature compensation: Mass flow or diff. pressure.
/// \param  averaging Averaging: None or average till read.
/// \return ERROR_NONE              = No error.
///         ERROR_ACK               = No acknowledgment from sensor.
///         ERROR_INVALID_PARAMETER = At least one of the specified parameter
///                                   are invalid.
Error DRV_SDP800_StartMeasurement(Sdp800TempComp  tempComp,
                                       Sdp800Averaging averaging);

/// \brief  Stops the continous mesurement.
/// \return ERROR_NONE              = No error.
///         ERROR_ACK               = No acknowledgment from sensor.
Error DRV_SDP800_StopMeasurement(void);

/// \brief  Reads the measurment result from the continous measurment.
/// \param  diffPressure Pointer to return the measured diverential pressur.
///         temperature  Pointer to return the measured temperature.
/// \return ERROR_NONE              = No error.
///         ERROR_ACK               = No acknowledgment from sensor.
///         ERROR_CHECKSUM          = Checksum does not match.
Error DRV_SDP800_ReadResults(float* diffPressure, float* temperature);

/// \brief  Calls the soft reset mechanism that forces the sensor into a
///         well-defined state without removing the power supply.
/// \return ERROR_NONE              = No error.
///         ERROR_ACK               = No acknowledgment from sensor.
Error DRV_SDP800_SoftReset(void);

#endif /* SDP800_H */

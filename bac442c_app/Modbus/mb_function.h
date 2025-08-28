#ifndef __MODBUS_FUNCTION_H__
#define __MODBUS_FUNCTION_H__

#include <stdint.h>
#include <stdbool.h>
#include "modbus_def.h"


int16_t mb_frame_npdu_decode(MB_TYPE bus, 
														uint8_t *pdu, 
														uint8_t *frame, 
														int16_t frm_len, 
														uint8_t *id,
														uint16_t *invokeID);

int16_t mb_frame_npdu_encode(MB_TYPE bus, 
														uint8_t *frame, 
														uint8_t *pdu, 
														int16_t pdu_len,
														uint8_t id,
														uint16_t invokeID);

#endif

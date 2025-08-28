
#ifndef __MB_HANDLE_H__
#define __MB_HANDLE_H__

#include <stdint.h>
#include "mb_function.h"

uint16_t mb_read_coil_handler(uint8_t * apdu, uint16_t apdu_len);
uint16_t mb_read_input_coil_handler(uint8_t * apdu, uint16_t apdu_len);
uint16_t mb_read_holding_reg_handler(uint8_t * apdu, uint16_t apdu_len);
uint16_t mb_read_input_reg_handler(uint8_t * apdu, uint16_t apdu_len);

uint16_t mb_force_single_coil_handler(uint8_t * apdu, uint16_t apdu_len);
uint16_t mb_force_multi_coil_handler(uint8_t * apdu, uint16_t apdu_len);
uint16_t mb_preset_single_reg_handler(uint8_t * apdu, uint16_t apdu_len);
uint16_t mb_preset_multi_reg_handler(uint8_t * apdu, uint16_t apdu_len);

uint16_t mb_exception_handler(uint8_t * apdu, MB_ERROR err_code);

#endif


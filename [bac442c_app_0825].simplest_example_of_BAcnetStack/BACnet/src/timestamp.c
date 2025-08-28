/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2008 John Minack

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to:
 The Free Software Foundation, Inc.
 59 Temple Place - Suite 330
 Boston, MA  02111-1307, USA.

 As a special exception, if other files instantiate templates or
 use macros or inline functions from this file, or you compile
 this file and link it with other works to produce a work based
 on this file, this file does not by itself cause the resulting
 work to be covered by the GNU General Public License. However
 the source code for this file must still be made available in
 accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work
 based on this file might be covered by the GNU General Public
 License.
 -------------------------------------------
####COPYRIGHTEND####*/

#include <assert.h>
#include "timestamp.h"

/** @file timestamp.c  Encode/Decode BACnet Timestamps  */


void bacapp_timestamp_copy(
    BACNET_TIMESTAMP * dest,
    BACNET_TIMESTAMP * src)
{
    if (dest && src) {
        dest->tag = src->tag;
        switch (src->tag) {
            case TIME_STAMP_TIME:
                datetime_copy_time(&dest->value.time, &src->value.time);
                break;
            case TIME_STAMP_SEQUENCE:
                dest->value.sequenceNum = src->value.sequenceNum;
                break;
            case TIME_STAMP_DATETIME:
                datetime_copy(&dest->value.dateTime, &src->value.dateTime);
                break;
            default:
                break;
        }
    }
}

int bacapp_encode_timestamp(
    uint8_t * apdu,
    BACNET_TIMESTAMP * value)
{
    int len = 0;        /* length of each encoding */

    if (value && apdu) {
        switch (value->tag) {
            case TIME_STAMP_TIME:
                len = encode_context_time(&apdu[0], 0, &value->value.time);
                break;

            case TIME_STAMP_SEQUENCE:
                len =
                    encode_context_unsigned(&apdu[0], 1,
                    value->value.sequenceNum);
                break;

            case TIME_STAMP_DATETIME:
                len =
                    bacapp_encode_context_datetime(&apdu[0], 2,
                    &value->value.dateTime);
                break;

            default:
                len = 0;
                //assert(0);
                break;
        }
    }

    return len;
}

int bacapp_encode_context_timestamp(
    uint8_t * apdu,
    uint8_t tag_number,
    BACNET_TIMESTAMP * value)
{
    int len = 0;        /* length of each encoding */
    int apdu_len = 0;

    if (value && apdu) {
        len = encode_opening_tag(&apdu[apdu_len], tag_number);
        apdu_len += len;
        len = bacapp_encode_timestamp(&apdu[apdu_len], value);
        apdu_len += len;
        len = encode_closing_tag(&apdu[apdu_len], tag_number);
        apdu_len += len;
    }
    return apdu_len;
}

int bacapp_decode_timestamp(
    uint8_t * apdu,
    BACNET_TIMESTAMP * value)
{
    int len = 0;
    int section_len;
    uint32_t len_value_type;
    uint32_t sequenceNum;

    if (apdu) {
        section_len =
            decode_tag_number_and_value(&apdu[len], &value->tag,
            &len_value_type);

        if (-1 == section_len) {
            return -1;
        }
        switch (value->tag) {
            case TIME_STAMP_TIME:
                if ((section_len =
                        decode_context_bacnet_time(&apdu[len], TIME_STAMP_TIME,
                            &value->value.time)) == -1) {
                    return -1;
                } else {
                    len += section_len;
                }
                break;

            case TIME_STAMP_SEQUENCE:
                if ((section_len =
                        decode_context_unsigned(&apdu[len],
                            TIME_STAMP_SEQUENCE, &sequenceNum)) == -1) {
                    return -1;
                } else {
                    if (sequenceNum <= 0xffff) {
                        len += section_len;
                        value->value.sequenceNum = (uint16_t) sequenceNum;
                    } else {
                        return -1;
                    }
                }
                break;

            case TIME_STAMP_DATETIME:
                if ((section_len =
                        bacapp_decode_context_datetime(&apdu[len],
                            TIME_STAMP_DATETIME,
                            &value->value.dateTime)) == -1) {
                    return -1;
                } else {
                    len += section_len;
                }
                break;

            default:
                return -1;
        }
    }

    return len;
}

int bacapp_decode_context_timestamp(
    uint8_t * apdu,
    uint8_t tag_number,
    BACNET_TIMESTAMP * value)
{
    int len = 0;
    int section_len;


    if (decode_is_opening_tag_number(&apdu[len], tag_number)) {
        len++;
        section_len = bacapp_decode_timestamp(&apdu[len], value);
        if (section_len > 0) {
            len += section_len;
            if (decode_is_closing_tag_number(&apdu[len], tag_number)) {
                len++;
            } else {
                return -1;
            }
        }
    }
    return len;
}

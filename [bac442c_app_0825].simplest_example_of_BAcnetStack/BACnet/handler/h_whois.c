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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "config.h"
#include "bacdef.h"
#include "bacdcode.h"
#include "iam.h"
#include "device.h"
#include "client.h"
#include "whois.h"
#include "txbuf.h"

/** @file h_whois.c  Handles Who-Is requests. */

/** Handler for Who-Is requests, with broadcast I-Am response.
 * @ingroup DMDDB
 * @param service_request [in] The received message to be handled.
 * @param service_len [in] Length of the service_request message.
 * @param src [in] The BACNET_ADDRESS of the message's source.
 */

void handler_who_is(uint8_t * service_request,uint16_t service_len,BACNET_ADDRESS * src)
{
    int len = 0;
    int32_t low_limit = 0;
    int32_t high_limit = 0;

    (void) src;
    len = whois_decode_service_request(service_request, service_len, &low_limit, &high_limit);
    printf("[%s:%s:%d] service_len = %d \r\n", 
                    __FILE__, __FUNCTION__, __LINE__, len);
    if (len == 0)
    {
        printf("[%s:%s:%d] Send_I_Am = %d \r\n", 
                        __FILE__, __FUNCTION__, __LINE__, service_len);
       Send_I_Am((uint8_t *)&Handler_Transmit_Buffer[0]);
    }
    else 
    {
        if (len != -1) 
        {
            printf("[%s:%s:%d] Device_Object_Instance_Number = %d \r\n", 
                        __FILE__, __FUNCTION__, __LINE__, Device_Object_Instance_Number());
            /* is my device id within the limits? */
            if (((Device_Object_Instance_Number() >= (uint32_t) low_limit) &&
                  (Device_Object_Instance_Number() <= (uint32_t) high_limit))   ||
            /* BACnet wildcard is the max instance number - everyone responds */
            ((BACNET_MAX_INSTANCE >= (uint32_t) low_limit) && (BACNET_MAX_INSTANCE <= (uint32_t) high_limit)))
            {
                printf("[%s:%s:%d] Send_I_Am = %d \r\n", 
                        __FILE__, __FUNCTION__, __LINE__, service_len);
                Send_I_Am((uint8_t *)&Handler_Transmit_Buffer[0]);
            }
        }
    }

    return;
}

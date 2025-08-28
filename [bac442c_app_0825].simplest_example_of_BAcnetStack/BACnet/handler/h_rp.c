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
#include "bacerror.h"
#include "bacdevobjpropref.h"
#include "apdu.h"
#include "npdu.h"
#include "abort.h"
#include "reject.h"
#include "device.h"
#include "txbuf.h"
#include "rp.h"
#include "abort.h"
#include "reject.h"
#include "bacerror.h"
#include "bip.h"
/* device object has custom handler for all objects */

/** @file h_rp.c  Handles Read Property requests. */

/** Handler for a ReadProperty Service request.
 * @ingroup DSRP
 * This handler will be invoked by apdu_handler() if it has been enabled
 * by a call to apdu_set_confirmed_handler().
 * This handler builds a response packet, which is
 * - an Abort if
 *   - the message is segmented
 *   - if decoding fails
 *   - if the response would be too large
 * - the result from Device_Read_Property(), if it succeeds
 * - an Error if Device_Read_Property() fails 
 *   or there isn't enough room in the APDU to fit the data.
 * 
 * @param service_request [in] The contents of the service request.
 * @param service_len [in] The length of the service_request.
 * @param src [in] BACNET_ADDRESS of the source of the message
 * @param service_data [in] The BACNET_CONFIRMED_SERVICE_DATA information 
 *                          decoded from the APDU header of this message. 
 */

void handler_read_property(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_DATA * service_data)
{
    BACNET_READ_PROPERTY_DATA rpdata;
    int len = 0;
    int pdu_len = 0;
    int apdu_len = -1;
    int npdu_len = -1;
    BACNET_NPDU_DATA npdu_data;
    bool error = true;  /* assume that there is an error */
    int bytes_sent = 0;
    BACNET_ADDRESS my_address;

    /* encode the NPDU portion of the packet */
    bip_get_my_address(&my_address);
    npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
    npdu_len = npdu_encode_pdu((uint8_t*)&Handler_Transmit_Buffer[0], src, &my_address, &npdu_data);
    
    //        printf("[%s:%s:%d] delay_cnt = %d  \n", 
    //            __FILE__, __FUNCTION__, __LINE__, npdu_len);
            
    if (service_data->segmented_message) 
    {      /* we don't support segmentation - send an abort */
        rpdata.error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
        len = BACNET_STATUS_ABORT;
        goto RP_FAILURE;
    }
    len = rp_decode_service_request(service_request, service_len, &rpdata);

    if (len < 0) 
    {
        /* bad decoding - skip to error/reject/abort handling */
        error = true;
        goto RP_FAILURE;
    }
    apdu_len = rp_ack_encode_apdu_init((uint8_t*)&Handler_Transmit_Buffer[npdu_len], service_data->invoke_id, &rpdata);
    /* configure our storage */
    rpdata.application_data = (uint8_t*)&Handler_Transmit_Buffer[npdu_len + apdu_len];
    rpdata.application_data_len = MAX_APDU - (npdu_len + apdu_len);

    len = Device_Read_Property(&rpdata);
    //        printf("[%s:%s:%d] len = %d  \n", 
    //            __FILE__, __FUNCTION__, __LINE__, len);
    if (len >= 0) 
    {
        apdu_len += len;
        len = rp_ack_encode_apdu_object_property_end((uint8_t*)&Handler_Transmit_Buffer[npdu_len + apdu_len]);
        apdu_len += len;
        //    printf("[%s:%s:%d] len = %d  \n", 
        //        __FILE__, __FUNCTION__, __LINE__, len);
        if (apdu_len > service_data->max_resp) 
        {
            /* too big for the sender - send an abort */
            rpdata.error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
            len = BACNET_STATUS_ABORT;
        } 
        else 
        {
            error = false;
            // printf("[%s:%s:%d] error = false  \n", __FILE__, __FUNCTION__, __LINE__);
        }
    }

  RP_FAILURE:
    if (error) 
    {
            printf("[%s:%s:%d] error = true  \n", __FILE__, __FUNCTION__, __LINE__);
            if (len == BACNET_STATUS_ABORT) 
            {
                    /* Kludge alert! At the moment we assume any abort is due to 
                     * to space issues due to segmentation or lack thereof. I wanted to show the proper
                     * handling via the abort_convert_error_code() so I put the error code
                     * in here, if you are sure all aborts properly set up the error_code then
                     * remove this next line
                     */
                    rpdata.error_code = ERROR_CODE_ABORT_SEGMENTATION_NOT_SUPPORTED;
                    apdu_len = abort_encode_apdu((uint8_t*)&Handler_Transmit_Buffer[npdu_len],
                            service_data->invoke_id,
                            abort_convert_error_code(rpdata.error_code), true);
            } 
            else 
            {
                if (len == BACNET_STATUS_ERROR) 
                {
                    apdu_len = bacerror_encode_apdu((uint8_t*)&Handler_Transmit_Buffer[npdu_len],
                        service_data->invoke_id, SERVICE_CONFIRMED_READ_PROPERTY,
                        rpdata.error_class, rpdata.error_code);
                } 
                else 
                {
                    if (len == BACNET_STATUS_REJECT) 
                    {
                        apdu_len =
                            reject_encode_apdu((uint8_t*)&Handler_Transmit_Buffer[npdu_len],
                            service_data->invoke_id,
                            reject_convert_error_code(rpdata.error_code));
                    }
                }
      }
    }

    pdu_len = npdu_len + apdu_len;
    // printf("[%s:%s:%d] pdu_len = %d  \n", 
    //            __FILE__, __FUNCTION__, __LINE__, pdu_len);
    bytes_sent = 
        bip_send_pdu(src, &npdu_data, &Handler_Transmit_Buffer[0], pdu_len);
    
    if (bytes_sent <= 0)
    {
#if PRINT_ENABLED
        fprintf(stderr,
            "Failed to bip_send_pdu Request (%s)!\n",
            strerror(errno));
#endif
    }
    return;
}


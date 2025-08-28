#ifndef __BIP_INIT_H__
#define __BIP_INIT_H__

#include <stdint.h>     /* for standard integer types uint8_t etc. */
#include <stdbool.h>    /* for the standard bool type. */
#include <stddef.h>
#include <string.h>
#include "bacdcode.h"
#include "bip.h"

#include "bacdef.h"
#include "crc.h"
#include "npdu.h"
#include <sys/socket.h>
#include "net.h"

#define MAX_HEADER_BIP (1+1+2)
#define MAX_APDU_BIP 1476
#define MAX_PDU_BIP (MAX_NPDU+MAX_APDU_BIP)
#define MAX_MPDU_BIP (MAX_HEADER_BIP + MAX_PDU_BIP)


#endif

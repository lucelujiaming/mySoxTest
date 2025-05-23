#include "stdafx.h"
#include "DASP.h"
#include "time.h"


////////////////////////////////////////////////////////////////
// Message Types
////////////////////////////////////////////////////////////////
int CDASP::MESSAGE_TYPES_DISCOVER = 0;
int CDASP::MESSAGE_TYPES_HELLO = 1;
int CDASP::MESSAGE_TYPES_CHALLENGE = 2;
int CDASP::MESSAGE_TYPES_AUTHENTICATE = 3;
int CDASP::MESSAGE_TYPES_WELCOME = 4;
int CDASP::MESSAGE_TYPES_KEEPALIVE = 5;
int CDASP::MESSAGE_TYPES_DATAGRAM = 6;
int CDASP::MESSAGE_TYPES_CLOSE = 7;

////////////////////////////////////////////////////////////////
// Header Values
////////////////////////////////////////////////////////////////
int CDASP::Header_Values_NIL = 0;
int CDASP::Header_Values_U2 = 1;
int CDASP::Header_Values_STR = 2;
int CDASP::Header_Values_BYTES = 3;

////////////////////////////////////////////////////////////////
// Header Ids (shifted down 2 bits)
////////////////////////////////////////////////////////////////
int CDASP::HEADER_IDS_VERSION = 0x05;  // 0x05 (1,1)
int CDASP::HEADER_IDS_REMOTE_ID = 0x09;  // 0x09 (2,1)
int CDASP::HEADER_IDS_DIGEST_ALGORITHM = 0x0e;  // 0x0e (3,2)
int CDASP::HEADER_IDS_NONCE = 0x13;  // 0x13 (4,3)
int CDASP::HEADER_IDS_USERNAME = 0x16;  // 0x16 (5,2)
int CDASP::HEADER_IDS_DIGEST = 0x1b;  // 0x1b (6,3)
int CDASP::HEADER_IDS_IDEAL_MAX = 0x1d;  // 0x1d (7,1)
int CDASP::HEADER_IDS_ABS_MAX = 0x21;  // 0x21 (8,1)
int CDASP::HEADER_IDS_ACK = 0x25;  // 0x25 (9,1)
int CDASP::HEADER_IDS_ACK_MORE = 0x2b;  // 0x2b (a,3)
int CDASP::HEADER_IDS_RECEIVE_MAX = 0x2d;  // 0x2d (b,1)  
int CDASP::HEADER_IDS_RECEIVE_TIMEOUT = 0x31;  // 0x31 (c,1)
int CDASP::HEADER_IDS_ERROR_CODE = 0x35;  // 0x35 (d,1)
int CDASP::HEADER_IDS_PLATFORM_ID = 0x3a;  // 0x3a (e,2)

extern "C" void sedona_sha1(unsigned char* input, 
							unsigned int inputOff, unsigned int len, 
							unsigned char* output, unsigned int outputOff);

#pragma pack(1)
typedef struct _DASP_MESSAGE
{
	unsigned char       sessionId[2];
	unsigned char       seqNum[2];
	unsigned char       typeAndfieldsNum; //  high 4 - bits msgType, low 4 - bits numFields
//	field[] headerFields
//	u1[]    payload
} DASP_MESSAGE, *PDASP_MESSAGE;

CDASP::CDASP()
{
	memset(m_sendBuf, 0x00, MAX_BUFFER_LEN);
	memset(m_receiveBuf, 0x00, MAX_BUFFER_LEN);

	memset(m_cNonce, 0x00, NAME_BUFFER_LEN);
	memset(m_csha1Output, 0x00, SHA1OUTPUT_BUFFER_LEN);

	memset(m_cPlateFormID, 0x00, NAME_BUFFER_LEN);
	m_ClientSessionID = 0x1234;
	m_ServerSessionID = 0x0000;
	iReceiveLeftBufLen = 0;
}


CDASP::~CDASP()
{
}

int CDASP::start()
{
	srand((unsigned)time(NULL));
	m_ClientSeqNum = 0x00;
	m_ServerSeqNum = 0x00;
	return initializeSocket();
}

void CDASP::close()
{
	closesocket(m_clientSocket);
	WSACleanup();
}

void CDASP::handShake()
{
	char cRet = 0 ;
	sendHelloRequest();
	while (1)
	{
		cRet = recvResponse();
		// Error
		if (cRet == CDASP::MESSAGE_TYPES_CLOSE)
		{
			// sendHelloRequest();
			break;
		}
		else if (cRet == CDASP::MESSAGE_TYPES_DISCOVER)
		{
			sendHelloRequest();
		}
		else if (cRet == CDASP::MESSAGE_TYPES_CHALLENGE)
		{
			sendAuthenticateRequest("admin", "");
		}
		else if (cRet == CDASP::MESSAGE_TYPES_WELCOME)
		{
			break;
		}
	}
}


int CDASP::initializeSocket()
{
	WSADATA wsadata;
	if (0 == WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		printf("客户端嵌套字已打开\r\n");
	}
	else
	{
		printf("客户端嵌套字打开失败\r\n");
	}
	m_clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN client_in;
	client_in.sin_addr.S_un.S_addr = inet_addr("192.168.168.2");//将网络地址字符串转换成二进制形式
	client_in.sin_family = AF_INET;
	client_in.sin_port = htons(1876);

	int iRet = connect(m_clientSocket, (SOCKADDR*)&client_in, sizeof(SOCKADDR));
	struct timeval tv;
	tv.tv_sec = 60000;
	tv.tv_usec = 0;
	int optlen = sizeof(struct timeval);

	if (setsockopt(m_clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, optlen) ==
		SOCKET_ERROR)
	{
		close();
		return 0;
	}

	return 1;
}

void CDASP::sendDiscoveryRequest()
{
	char * sendBufPtr = m_sendBuf;
	memset(m_sendBuf, 0x00, MAX_BUFFER_LEN);
	DASP_MESSAGE daspHeader;

	daspHeader.sessionId[0] = (short)0xFF;
	daspHeader.sessionId[1] = (short)0xFF;

	m_ClientSeqNum = rand() % 0x2000;
	// Convert local byte order to network byte order
	daspHeader.seqNum[0] = m_ClientSeqNum / 0x100;
	daspHeader.seqNum[1] = m_ClientSeqNum % 0x100;
	// m_ClientSeqNum++ ;

	//  The client sends a discover message with no header fields.
	daspHeader.typeAndfieldsNum = ((char)(CDASP::MESSAGE_TYPES_DISCOVER) << 4) | 0x00 ;
	memcpy(sendBufPtr, &daspHeader, sizeof(DASP_MESSAGE));
	sendBufPtr += sizeof(DASP_MESSAGE);
	
	send(m_clientSocket, m_sendBuf, sizeof(DASP_MESSAGE), 0);
}

void CDASP::sendHelloRequest()
{
	char * sendBufPtr = m_sendBuf;
	memset(m_sendBuf, 0x00, MAX_BUFFER_LEN);
	DASP_MESSAGE daspHeader;
	daspHeader.sessionId[0] = (short)0xFF;
	daspHeader.sessionId[1] = (short)0xFF;

	m_ClientSeqNum = rand() % 0x2000;
	// Convert local byte order to network byte order
	daspHeader.seqNum[0] = m_ClientSeqNum / 0x100;
	daspHeader.seqNum[1] = m_ClientSeqNum % 0x100;
	// m_ClientSeqNum++ ;

	daspHeader.typeAndfieldsNum = ((char)(CDASP::MESSAGE_TYPES_HELLO) << 4) | 0x02 ;
	memcpy(sendBufPtr, &daspHeader, sizeof(DASP_MESSAGE));
	sendBufPtr += sizeof(DASP_MESSAGE);
	
	sendBufPtr[0] = (char)(CDASP::HEADER_IDS_VERSION) ; 
	sendBufPtr[1] = 0x01;
	sendBufPtr[2] = 0x00;
	
	sendBufPtr[3] = (char)(CDASP::HEADER_IDS_REMOTE_ID) ; 
	sendBufPtr[4] = m_ClientSessionID / 0x100; // 0x34;
	sendBufPtr[5] = m_ClientSessionID % 0x100; // 0x12;
	send(m_clientSocket, m_sendBuf, sizeof(DASP_MESSAGE) + 6, 0);
}

void CDASP::sendKeepAliveRequest(unsigned char * seqNum)
{
	char * sendBufPtr = m_sendBuf;
	memset(m_sendBuf, 0x00, MAX_BUFFER_LEN);
	DASP_MESSAGE daspHeader;
	// Convert local byte order to network byte order
	daspHeader.sessionId[0] = m_ServerSessionID / 0x100;
	daspHeader.sessionId[1] = m_ServerSessionID % 0x100;

	// keepAlives的seqNum应该是0xffff。
	daspHeader.seqNum[0] = 0xFF ;
	daspHeader.seqNum[1] = 0xFF ;
	daspHeader.typeAndfieldsNum = ((char)(CDASP::MESSAGE_TYPES_KEEPALIVE) << 4) | 0x01 ;
	memcpy(sendBufPtr, &daspHeader, sizeof(DASP_MESSAGE));
	sendBufPtr += sizeof(DASP_MESSAGE);
	//for (int iSeq = m_shortAckCode; iSeq <= m_ClientSeqNum; iSeq++)
	//{
	sendBufPtr[0] = ((char)(CDASP::HEADER_IDS_ACK)) ; 
	//	sendBufPtr[1] = iSeq / 0x100 ; 
	sendBufPtr[1] = seqNum[0] ; 
	//	sendBufPtr[2] = iSeq % 0x100 ; 
	sendBufPtr[2] = seqNum[1] ; 
	sendBufPtr += 3;
	//}
	send(m_clientSocket, m_sendBuf, sendBufPtr - m_sendBuf, 0);
}

void CDASP::sendDatagramRequest(unsigned char *SoxBuf, int iSoxBufLen)
{
	char * sendBufPtr = m_sendBuf;
	memset(m_sendBuf, 0x00, MAX_BUFFER_LEN);
	DASP_MESSAGE daspHeader;
	// Convert local byte order to network byte order
	daspHeader.sessionId[0] = m_ServerSessionID / 0x100;
	daspHeader.sessionId[1] = m_ServerSessionID % 0x100;
	// Convert local byte order to network byte order
	daspHeader.seqNum[0] = m_ClientSeqNum / 0x100;
	daspHeader.seqNum[1] = m_ClientSeqNum % 0x100;
	m_ClientSeqNum++ ;
	daspHeader.typeAndfieldsNum = ((char)(CDASP::MESSAGE_TYPES_DATAGRAM) << 4) | 0x01 ;
	memcpy(sendBufPtr, &daspHeader, sizeof(DASP_MESSAGE));
	sendBufPtr += sizeof(DASP_MESSAGE);

	sendBufPtr[0] = ((char)(CDASP::HEADER_IDS_ACK)) ; 
	sendBufPtr[1] = m_shortAckCode / 0x100 ; 
	sendBufPtr[2] = m_shortAckCode % 0x100 ; 
	sendBufPtr += 3;

	memcpy(sendBufPtr, SoxBuf, iSoxBufLen);
	sendBufPtr += iSoxBufLen;

	send(m_clientSocket, m_sendBuf, sendBufPtr - m_sendBuf, 0);
}

int CDASP::generateDigestAlgorithm(unsigned char * strInputBuf, int iLen)
{
	int i = 0;
	memset(m_csha1Output, 0x00, SHA1OUTPUT_BUFFER_LEN);
	printf("strInputBuf = ");
	for (i = 0; i < iLen; i++)
	{
		printf("%02X ", strInputBuf[i]);
	}
	printf("\r\n");
	sedona_sha1(strInputBuf, 0, iLen, m_csha1Output, 0);
	printf("m_csha1Output = ");
	for (i = 0; i < strlen((char *)m_csha1Output); i++)
	{
		printf("%02X ", m_csha1Output[i]);
	}
	printf("\r\n");
	return strlen((char *)m_csha1Output);
}

void CDASP::sendAuthenticateRequest(char * cUserName, char * cPassword)
{
	int iOutLen = 0;
	unsigned char cDigest[SHA1OUTPUT_BUFFER_LEN];
	DASP_MESSAGE daspHeader;
	char * sendBufPtr = m_sendBuf;
	memset(m_sendBuf, 0x00, MAX_BUFFER_LEN);
	// Convert local byte order to network byte order
	daspHeader.sessionId[0] = m_ServerSessionID / 0x100;
	daspHeader.sessionId[1] = m_ServerSessionID % 0x100;

	// The seqNum should be the same as that used by the hello message.
	// Convert local byte order to network byte order
	daspHeader.seqNum[0] = m_ClientSeqNum / 0x100;
	daspHeader.seqNum[1] = m_ClientSeqNum % 0x100;
	// m_ClientSeqNum++ ;
	daspHeader.typeAndfieldsNum = ((char)(CDASP::MESSAGE_TYPES_AUTHENTICATE) << 4) | 0x02 ;
	memcpy(sendBufPtr, &daspHeader, sizeof(DASP_MESSAGE));
	sendBufPtr += sizeof(DASP_MESSAGE);
	// Write username not the  credentials
	sendBufPtr[0] =  ((char)(CDASP::HEADER_IDS_USERNAME)); 
	sendBufPtr++;
	sprintf(sendBufPtr, "%s", cUserName);
	sendBufPtr += strlen(sendBufPtr) + 1;

	// Write digest
	// generate user.cred : credentials = digestAlgorithm(username + ":" + password)
	memset(cDigest, 0x00, SHA1OUTPUT_BUFFER_LEN);
	memset(m_csha1Output, 0x00, SHA1OUTPUT_BUFFER_LEN);
	sprintf((char *)cDigest, "%s:%s", cUserName, cPassword);
	iOutLen = generateDigestAlgorithm(cDigest,  strlen((char *)cDigest));
	memset(cDigest, 0x00, SHA1OUTPUT_BUFFER_LEN);
	strncpy((char *)cDigest, (char *)m_csha1Output, iOutLen);
	// generate digest : digest = digestAlgorithm(credentials + nonce)
	// Append Nonce
	memcpy(cDigest + iOutLen, m_cNonce, m_cNonceLen);
	memset(m_csha1Output, 0x00, SHA1OUTPUT_BUFFER_LEN);
	iOutLen = generateDigestAlgorithm(cDigest,  iOutLen + m_cNonceLen);

	sendBufPtr[0] = (char)(CDASP::HEADER_IDS_DIGEST) ; 
	sendBufPtr++;
	sendBufPtr[0] = iOutLen;
	sendBufPtr++;
	memcpy(sendBufPtr, m_csha1Output, iOutLen);
	sendBufPtr += iOutLen;

	send(m_clientSocket, m_sendBuf, sendBufPtr - m_sendBuf, 0);

}

int CDASP::dealCloseResponse(int numFields, char *recvBufPtr)
{
	int iSize = 0;
	for (int i = 0 ; i < (int)numFields; i++)
	{
		if(recvBufPtr[0] == CDASP::HEADER_IDS_ERROR_CODE)
		{
			// Convert network byte order to local byte order
			m_shortErrorCode  = (unsigned char)recvBufPtr[1];
			m_shortErrorCode *= 0x100;
			m_shortErrorCode += (unsigned char)recvBufPtr[2];
			printf("ERROR_CODE = %04X\r\n", m_shortErrorCode);
			recvBufPtr += 3;
			iSize      += 3;
			// Send KeepAlive Request
			// sendKeepAliveRequest();
		}
	}
	return iSize;
}

int CDASP::dealKeepAliveResponse(int numFields, char *recvBufPtr)
{
	int iSize = 0;
	for (int i = 0 ; i < (int)numFields; i++)
	{
		if(recvBufPtr[0] == CDASP::HEADER_IDS_ACK)
		{
			// Convert network byte order to local byte order
			m_shortAckCode  = (unsigned char)recvBufPtr[1];
			m_shortAckCode *= 0x100;
			m_shortAckCode += (unsigned char)recvBufPtr[2];
			if(m_ServerSeqNum >= m_shortAckCode)
				printf("ACK ERROR\r\n");
			// m_shortAckCode++;
			m_ServerSeqNum = m_shortAckCode;
			printf("shortAckCode = %04X\r\n", m_shortAckCode);
			recvBufPtr += 3;
			iSize      += 3;
		}
	}
	return iSize;
}

int CDASP::dealDiscoverResponse(int numFields, char *recvBufPtr)
{
	int iSize = 0;
	for (int i = 0 ; i < (int)numFields; i++)
	{
		if(recvBufPtr[0] == CDASP::HEADER_IDS_PLATFORM_ID)
		{
			strcpy(m_cPlateFormID, recvBufPtr + 1);
			printf("m_cPlateFormID = %s\r\n", m_cPlateFormID);
			iSize      += strlen(m_cPlateFormID) + 1;
		}
	}
	return iSize;
}

int CDASP::dealChallengeResponse(int numFields, char *recvBufPtr)
{
	int iSize = 0;
	for (int i = 0 ; i < (int)numFields; i++)
	{
		if(recvBufPtr[0] == CDASP::HEADER_IDS_REMOTE_ID)
		{
			// Convert network byte order to local byte order
			m_ServerSessionID  = (unsigned char)recvBufPtr[1];
			m_ServerSessionID *= 0x100;
			m_ServerSessionID += (unsigned char)recvBufPtr[2];
			printf("ServerSessionID = %04X\r\n", m_ServerSessionID);
			recvBufPtr += 3;
			iSize      += 3;
		}
		else if(recvBufPtr[0] == CDASP::HEADER_IDS_NONCE)
		{
			m_cNonceLen = recvBufPtr[1];
			memset(m_cNonce, 0x00, NAME_BUFFER_LEN);
			memcpy(m_cNonce, recvBufPtr + 2, m_cNonceLen);
			recvBufPtr += m_cNonceLen + 2;
			iSize      += m_cNonceLen + 2;
		}
	}
	return iSize;
}

int CDASP::dealWelcomeResponse(int numFields, char *recvBufPtr)
{
	int iSize = 0;
	for (int i = 0 ; i < (int)numFields; i++)
	{
		if(recvBufPtr[0] == CDASP::HEADER_IDS_IDEAL_MAX)
		{
			// Convert network byte order to local byte order
			m_idealMax  = (unsigned char)recvBufPtr[2];
			m_idealMax *= 0x100;
			m_idealMax += (unsigned char)recvBufPtr[1];
			printf("m_idealMax = %04X\r\n", m_idealMax);
			recvBufPtr += 3;
			iSize      += 3;
		}
		else if(recvBufPtr[0] == CDASP::HEADER_IDS_RECEIVE_MAX)
		{
			// Convert network byte order to local byte order
			m_receiveMax  = (unsigned char)recvBufPtr[2];
			m_receiveMax *= 0x100;
			m_receiveMax += (unsigned char)recvBufPtr[1];
			printf("m_receiveMax = %04X\r\n", m_receiveMax);
			recvBufPtr += 3;
			iSize      += 3;
		}
		else if(recvBufPtr[0] == CDASP::HEADER_IDS_RECEIVE_TIMEOUT)
		{
			// Convert network byte order to local byte order
			m_receiveTimeout  = (unsigned char)recvBufPtr[2];
			m_receiveTimeout *= 0x100;
			m_receiveTimeout += (unsigned char)recvBufPtr[1];
			printf("m_receiveTimeout = %04X\r\n", m_receiveTimeout);
			recvBufPtr += 3;
			iSize      += 3;
		}
	}
	return iSize;
}

int CDASP::dealDataGramResponse(int numFields, char *recvBufPtr)
{
	int iSize = 0;
	for (int i = 0 ; i < (int)numFields; i++)
	{
		if(recvBufPtr[0] == CDASP::HEADER_IDS_ACK)
		{
			// Convert network byte order to local byte order
			m_shortAckCode  = (unsigned char)recvBufPtr[1];
			m_shortAckCode *= 0x100;
			m_shortAckCode += (unsigned char)recvBufPtr[2];
			if(m_ServerSeqNum >= m_shortAckCode)
				printf("ACK ERROR\r\n");
			// m_shortAckCode++;
			m_ServerSeqNum = m_shortAckCode;
			printf("shortAckCode = %04X\r\n", m_shortAckCode);
			recvBufPtr += 3;
			iSize      += 3;
			// Send KeepAlive Request
			// sendKeepAliveRequest();
		}
		else if(recvBufPtr[0] == CDASP::HEADER_IDS_ACK_MORE)
		{
			char cLen = recvBufPtr[1];
			char cTemp[NAME_BUFFER_LEN] ;
			memset(cTemp, 0x00, NAME_BUFFER_LEN);
			memcpy(cTemp, recvBufPtr + 2, cLen);
			recvBufPtr += cLen + 2;
			iSize      += cLen + 2;
		}
	}
	return iSize;
}

char CDASP::recvResponse()
{
	int iDealLen = 0 ;
	iReceiveLeftBufLen = 0;
	char * recvBufPtr = m_receiveBuf;
	DASP_MESSAGE * daspHeaderPtr = (DASP_MESSAGE *)m_receiveBuf;
	memset(m_receiveBuf, 0x00, MAX_BUFFER_LEN);

	int iLen = recv(m_clientSocket, m_receiveBuf, 1024, 0);
	// Get Header
	char cType = daspHeaderPtr->typeAndfieldsNum >> 4;
	char numFields = daspHeaderPtr->typeAndfieldsNum & 0x000F;
	recvBufPtr += sizeof(DASP_MESSAGE);
	// Deal command
	if(cType == (char)CDASP::MESSAGE_TYPES_CLOSE)
	{
		iDealLen = dealCloseResponse(numFields, recvBufPtr);
	}
	else if(cType == (char)CDASP::MESSAGE_TYPES_KEEPALIVE)
	{
		iDealLen = dealKeepAliveResponse(numFields, recvBufPtr);
	}
	else if(cType == (char)CDASP::MESSAGE_TYPES_DISCOVER)
	{
		iDealLen = dealDiscoverResponse(numFields, recvBufPtr);
	}
	else if(cType == (char)CDASP::MESSAGE_TYPES_CHALLENGE)
	{
		m_ServerSeqNum = daspHeaderPtr->seqNum[0] * 0x100 + daspHeaderPtr->seqNum[1];
		iDealLen = dealChallengeResponse(numFields, recvBufPtr);
	}
	else if(cType == (char)CDASP::MESSAGE_TYPES_WELCOME)
	{
		iDealLen = dealWelcomeResponse(numFields, recvBufPtr);
	}
	else if(cType == (char)CDASP::MESSAGE_TYPES_DATAGRAM)
	{
		iDealLen = dealDataGramResponse(numFields, recvBufPtr);
	}
	sendKeepAliveRequest(daspHeaderPtr->seqNum);
	recvBufPtr += iDealLen;

	if(recvBufPtr < m_receiveBuf + iLen)
	{
		memset(m_receiveLeftBuf, 0x00, MAX_BUFFER_LEN);
		memcpy(m_receiveLeftBuf, recvBufPtr, m_receiveBuf + iLen - recvBufPtr);
		iReceiveLeftBufLen = m_receiveBuf + iLen - recvBufPtr;
	}
	return cType;
}


#pragma once
/*	All DASP messages are formatted as follows: */

#include <iostream>
#include <winsock2.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

#define MAX_BUFFER_LEN 1024
#define NAME_BUFFER_LEN 128
#define SHA1OUTPUT_BUFFER_LEN 64

class CDASP
{
public:
	CDASP();
	~CDASP();
	
public:
	////////////////////////////////////////////////////////////////
	// Message Types
	////////////////////////////////////////////////////////////////
	static int MESSAGE_TYPES_DISCOVER;
	static int MESSAGE_TYPES_HELLO;
	static int MESSAGE_TYPES_CHALLENGE;
	static int MESSAGE_TYPES_AUTHENTICATE;
	static int MESSAGE_TYPES_WELCOME;
	static int MESSAGE_TYPES_KEEPALIVE;
	static int MESSAGE_TYPES_DATAGRAM;
	static int MESSAGE_TYPES_CLOSE;

public:
	////////////////////////////////////////////////////////////////
	// Header Values
	////////////////////////////////////////////////////////////////
	static int Header_Values_NIL;
	static int Header_Values_U2;
	static int Header_Values_STR;
	static int Header_Values_BYTES;

public:
	////////////////////////////////////////////////////////////////
	// Header Ids (shifted down 2 bits)
	////////////////////////////////////////////////////////////////
	static int HEADER_IDS_VERSION;  // 0x05 (1,1)
	static int HEADER_IDS_REMOTE_ID;  // 0x09 (2,1)
	static int HEADER_IDS_DIGEST_ALGORITHM;  // 0x0e (3,2)
	static int HEADER_IDS_NONCE;  // 0x13 (4,3)
	static int HEADER_IDS_USERNAME;  // 0x16 (5,2)
	static int HEADER_IDS_DIGEST;  // 0x1b (6,3)
	static int HEADER_IDS_IDEAL_MAX;  // 0x1d (7,1)
	static int HEADER_IDS_ABS_MAX;  // 0x21 (8,1)
	static int HEADER_IDS_ACK;  // 0x25 (9,1)
	static int HEADER_IDS_ACK_MORE;  // 0x2b (a,3)
	static int HEADER_IDS_RECEIVE_MAX;  // 0x2d (b,1)  
	static int HEADER_IDS_RECEIVE_TIMEOUT;  // 0x31 (c,1)
	static int HEADER_IDS_ERROR_CODE;  // 0x35 (d,1)
	static int HEADER_IDS_PLATFORM_ID;  // 0x3a (e,2)
	
private:
	// unsigned short m_shortRemotID;
	unsigned short m_ClientSessionID;
	unsigned short m_ServerSessionID;
	
	unsigned short m_ClientSeqNum;
	unsigned short m_ServerSeqNum;

	unsigned short m_idealMax;
	unsigned short m_receiveMax;
	unsigned short m_receiveTimeout;
public:
	int initializeSocket();
	int start();
	void close();
	void handShake();

public:
	void sendDatagramRequest(unsigned char *SoxBuf, int iSoxBufLen);
	char recvResponse();
	unsigned char * getReceiveLeftBuf() {  return m_receiveLeftBuf; }
	int    getReceiveLeftBufLen() {  return iReceiveLeftBufLen; }

private:

	void sendDiscoveryRequest();
	void sendHelloRequest();
	void sendKeepAliveRequest(unsigned char * seqNum);
	void sendAuthenticateRequest(char * cUserName, char * cPassword);

	int generateDigestAlgorithm(unsigned char * strInputBuf, int iLen);

private:
	int dealCloseResponse(int numFields, char *recvBufPtr);
	int dealKeepAliveResponse(int numFields, char *recvBufPtr);
	int dealDiscoverResponse(int numFields, char *recvBufPtr);
	int dealChallengeResponse(int numFields, char *recvBufPtr);
	int dealWelcomeResponse(int numFields, char *recvBufPtr);
	int dealDataGramResponse(int numFields, char *recvBufPtr);

private:
	SOCKET m_clientSocket;

	char m_sendBuf[MAX_BUFFER_LEN];
	char m_receiveBuf[MAX_BUFFER_LEN];
	
	unsigned char m_receiveLeftBuf[MAX_BUFFER_LEN];
	int  iReceiveLeftBufLen ;


	unsigned short m_shortErrorCode;
	unsigned short m_shortAckCode;	

	char  m_cNonceLen;
	unsigned char  m_cNonce[NAME_BUFFER_LEN];
	
	unsigned char m_csha1Output[SHA1OUTPUT_BUFFER_LEN];
	
public:
	char m_cPlateFormID[NAME_BUFFER_LEN];
};






















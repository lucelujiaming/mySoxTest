#include "stdafx.h"
#include <process.h>
#include "SoxMsg.h"
#include <windows.h>

#pragma pack(1)
typedef struct _SOX_MESSAGE
{
	char      cmd;
	char      seqNum;
//	u1[]    payload
} SOX_MESSAGE, *PSOX_MESSAGE;

typedef struct _SOX_FILEOPEN_MESSAGE
{
	SOX_MESSAGE     header;
	char            seqNum;
	char            method;
	char *          uri_ptr;
	unsigned int    fileSize;
	short			suggestedChunkSize;
	char *          headers_name_ptr;
	char *          value_ptr;
} SOX_FILEOPEN_MESSAGE, *PSOX_FILEOPEN_MESSAGE;


CSoxMsg::CSoxMsg()
{
	m_bExitRecvThread = TRUE;
	resetFileOpenInfo();
}


CSoxMsg::~CSoxMsg()
{
}

int CSoxMsg::start()
{
	return m_objDASPMsg.start();
}

void CSoxMsg::handShake()
{
	m_objDASPMsg.handShake();
}

void CSoxMsg::close()
{
	m_objDASPMsg.close();
}

char CSoxMsg::recvResponse()
{
	return m_objDASPMsg.recvResponse();
}

void recv_thread_func(void * arg)
{
	CSoxMsg * objSoxMsg = (CSoxMsg *)arg;

	while (1)
	{
		char cRet = objSoxMsg->recvResponse();
		// Error
		if (cRet == CDASP::MESSAGE_TYPES_CLOSE)
		{
			// sendHelloRequest();
			break;
		}
		else if (cRet == CDASP::MESSAGE_TYPES_DATAGRAM)
		{
			if(objSoxMsg->m_objDASPMsg.getReceiveLeftBufLen() > 0)
			{
				char * cLeftDataPtr = objSoxMsg->m_objDASPMsg.getReceiveLeftBuf();

				objSoxMsg->dealSoxPacket(cLeftDataPtr, 
					objSoxMsg->m_objDASPMsg.getReceiveLeftBufLen());
			}
			printf("MESSAGE_TYPES_DATAGRAM \r\n");
			continue;
		}
		Sleep(1);
		if (objSoxMsg->getRecvThreadStatus() == TRUE)
		{
			break;
		}
	}
}

void CSoxMsg::dealSoxPacket(char * cDataBuf, int iDataBufLen)
{
	char cReplyNum = 0x00;
	switch(cDataBuf[0]) {
	//	a	add	Add a new component to the application
	case 'a':
		dealAddRequest(cDataBuf, iDataBufLen);
		break;
	case 'A':
		dealAddResponse(cDataBuf, iDataBufLen);
		break;
	//	b	fileRename	Rename or move a file
	case 'b':
		dealFileRenameRequest(cDataBuf, iDataBufLen);
		break;
	case 'B':
		dealFileRenameResponse(cDataBuf, iDataBufLen);
		break;
	//	c	readComp	Read a component in the application
	case 'c':
		dealReadCompRequest(cDataBuf, iDataBufLen);
		break;
	case 'C':
		dealReadCompResponse(cDataBuf, iDataBufLen);
		break;
	//	d	delete	Delete an component and its children from the application
	case 'd':
		dealReleteRequest(cDataBuf, iDataBufLen);
		break;
	case 'D':
		dealReleteResponse(cDataBuf, iDataBufLen);
		break;
	//	e	event	COV event for a subscribed component
	case 'e':
		dealEventRequest(cDataBuf, iDataBufLen);
		break;
	case 'E':
		dealEventResponse(cDataBuf, iDataBufLen);
		break;
	//	f	fileOpen	Begin a get or put file transfer operation
	case 'f':
		dealFileOpenRequest(cDataBuf, iDataBufLen);
		break;
	case 'F':
		dealFileOpenResponse(cDataBuf, iDataBufLen);
		break;
	//	i	invoke	Invoke a component action
	case 'i':
		dealInvokeRequest(cDataBuf, iDataBufLen);
		break;
	case 'I':
		dealInvokeResponse(cDataBuf, iDataBufLen);
		break;
	//	k	fileChunk	Receive or send a chunk during a file transfer
	case 'k':
		dealFileChunkRequest(cDataBuf, iDataBufLen);
		break;
	case 'K':
		dealFileChunkResponse(cDataBuf, iDataBufLen);
		break;
	//	l	link	Add or delete a link
	case 'l':
		dealLinkRequest(cDataBuf, iDataBufLen);
		break;
	case 'L':
		dealLinkResponse(cDataBuf, iDataBufLen);
		break;
	//	n	rename	Rename a component
	case 'n':
		dealRenameRequest(cDataBuf, iDataBufLen);
		break;
	case 'N':
		dealRenameResponse(cDataBuf, iDataBufLen);
		break;
	//	o	reorder	Reorder a component's children
	case 'o':
		dealReorderRequest(cDataBuf, iDataBufLen);
		break;
	case 'O':
		dealReorderResponse(cDataBuf, iDataBufLen);
		break;
	//	q	query	Query installed services
	case 'q':
		dealQueryRequest(cDataBuf, iDataBufLen);
		break;
	case 'Q':
		dealQueryResponse(cDataBuf, iDataBufLen);
		break;
	//	r	readProp	Read a single property from a component
	case 'r':
		dealReadPropRequest(cDataBuf, iDataBufLen);
		break;
	case 'R':
		dealReadPropResponse(cDataBuf, iDataBufLen);
		break;
	//	s	subscribe	Subscribe to a component for COV events
	case 's':
		dealSubscribeRequest(cDataBuf, iDataBufLen);
		break;
	case 'S':
		dealSubscribeResponse(cDataBuf, iDataBufLen);
		break;
	//	u	unsubscribe	Unsubscribe from a component for COV events
	case 'u':
		dealUnsubscribeRequest(cDataBuf, iDataBufLen);
		break;
	case 'U':
		dealUnsubscribeResponse(cDataBuf, iDataBufLen);
		break;
	//	v	version	Query for the kits installed
	case 'v':
		dealVersionRequest(cDataBuf, iDataBufLen);
		break;
	case 'V':
		dealVersionResponse(cDataBuf, iDataBufLen);
		break;
	//	w	write	Write the value of a single component property
	case 'w':
		dealWriteRequest(cDataBuf, iDataBufLen);
		break;
	case 'W':
		dealWriteResponse(cDataBuf, iDataBufLen);
		break;
	//	y	versionMore	Query for additional version meta-data
	case 'y':
		dealVersionMoreRequest(cDataBuf, iDataBufLen);
		break;
	case 'Y':
		dealVersionMoreResponse(cDataBuf, iDataBufLen);
		break;
	//	z	fileClose	Close a file transfer operation
	case 'z':
		dealFileCloseRequest(cDataBuf, iDataBufLen);
		break;
	case 'Z':
		dealFileCloseResponse(cDataBuf, iDataBufLen);
		break;
	//	!	error	Response id for a command that could not be processed
	case '!':
		dealErrorResponse(cDataBuf, iDataBufLen);
		break;
	default:
		break;
	}
}

BOOL CSoxMsg::getRecvThreadStatus()
{
	return m_bExitRecvThread;
}

void CSoxMsg::startRecvThread()
{
	m_bExitRecvThread = FALSE;
	_beginthread(recv_thread_func, 0, this);
}

//	A	add	Add a new component to the application
void CSoxMsg::dealAddResponse(char * cDataBuf, int iDataBufLen)
{

}
//	B	fileRename	Rename or move a file
void CSoxMsg::dealFileRenameResponse(char * cDataBuf, int iDataBufLen)
{
	
}
//	C	readComp	Read a component in the application
void CSoxMsg::dealReadCompResponse(char * cDataBuf, int iDataBufLen)
{

}
//	D	delete	Delete an component and its children from the application
void CSoxMsg::dealReleteResponse(char * cDataBuf, int iDataBufLen)
{

}
//	E	event	COV event for a subscribed component
void CSoxMsg::dealEventResponse(char * cDataBuf, int iDataBufLen)
{

}
//	F	fileOpen	Begin a get or put file transfer operation
void CSoxMsg::dealFileOpenResponse(char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	// 
	m_fileSize        = (unsigned char)cDataBuf[2];
	m_fileSize       *= 0x100;
	m_fileSize       += (unsigned char)cDataBuf[3];
	m_fileSize       *= 0x100;
	m_fileSize       += (unsigned char)cDataBuf[4];
	m_fileSize       *= 0x100;
	m_fileSize       += (unsigned char)cDataBuf[5];
	// 
	m_actualChunkSize  = (unsigned char)cDataBuf[6];
	m_actualChunkSize *= 0x100;
	m_actualChunkSize += (unsigned char)cDataBuf[7];
    m_numChunks = m_fileSize/m_actualChunkSize - 1;
    if (m_fileSize % m_actualChunkSize != 0) m_numChunks++;
}
//	I	invoke	Invoke a component action
void CSoxMsg::dealInvokeResponse(char * cDataBuf, int iDataBufLen)
{

}
//	k	fileChunk	Receive or send a chunk during a file transfer
void CSoxMsg::dealFileChunkResponse(char * cDataBuf, int iDataBufLen)
{
}

//	l	link	Add or delete a link
void CSoxMsg::dealLinkResponse(char * cDataBuf, int iDataBufLen)
{

}
//	n	rename	Rename a component
void CSoxMsg::dealRenameResponse(char * cDataBuf, int iDataBufLen)
{

}
//	o	reorder	Reorder a component's children
void CSoxMsg::dealReorderResponse(char * cDataBuf, int iDataBufLen)
{

}
//	q	query	Query installed services
void CSoxMsg::dealQueryResponse(char * cDataBuf, int iDataBufLen)
{

}
//	r	readProp	Read a single property from a component
void CSoxMsg::dealReadPropResponse(char * cDataBuf, int iDataBufLen)
{

}
//	s	subscribe	Subscribe to a component for COV events
void CSoxMsg::dealSubscribeResponse(char * cDataBuf, int iDataBufLen)
{

}
//	u	unsubscribe	Unsubscribe from a component for COV events
void CSoxMsg::dealUnsubscribeResponse(char * cDataBuf, int iDataBufLen)
{

}
//	v	version	Query for the kits installed
void CSoxMsg::dealVersionResponse(char * cDataBuf, int iDataBufLen)
{

}
//	w	write	Write the value of a single component property
void CSoxMsg::dealWriteResponse(char * cDataBuf, int iDataBufLen)
{

}
//	y	versionMore	Query for additional version meta-data
void CSoxMsg::dealVersionMoreResponse(char * cDataBuf, int iDataBufLen)
{

}
//	z	fileClose	Close a file transfer operation
void CSoxMsg::dealFileCloseResponse(char * cDataBuf, int iDataBufLen)
{

}
//	!	error	Response id for a Response that could not be processed
void CSoxMsg::dealErrorResponse(char * cDataBuf, int iDataBufLen)
{
	char cReplyNum = cDataBuf[1];
	printf("ERROR: %s\r\n", cDataBuf + 2);
}

//	a	add	Add a new component to the application
void CSoxMsg::dealAddRequest(char * cDataBuf, int iDataBufLen)
{

}
//	b	fileRename	Rename or move a file
void CSoxMsg::dealFileRenameRequest(char * cDataBuf, int iDataBufLen)
{
	
}
//	c	readComp	Read a component in the application
void CSoxMsg::dealReadCompRequest(char * cDataBuf, int iDataBufLen)
{

}
//	d	delete	Delete an component and its children from the application
void CSoxMsg::dealReleteRequest(char * cDataBuf, int iDataBufLen)
{

}
//	e	event	COV event for a subscribed component
void CSoxMsg::dealEventRequest(char * cDataBuf, int iDataBufLen)
{

}
//	f	fileOpen	Begin a get or put file transfer operation
void CSoxMsg::dealFileOpenRequest(char * cDataBuf, int iDataBufLen)
{
}

//	i	invoke	Invoke a component action
void CSoxMsg::dealInvokeRequest(char * cDataBuf, int iDataBufLen)
{

}
//	k	fileChunk	Receive or send a chunk during a file transfer
void CSoxMsg::dealFileChunkRequest(char * cDataBuf, int iDataBufLen)
{
	char cFileChunkName[NAME_BUFFER_LEN];
	
	char replyNum  = cDataBuf[1];
	unsigned short chunkNum  = (unsigned char)cDataBuf[2];
	chunkNum                *= 0x100;
	chunkNum                += (unsigned char)cDataBuf[3];
	unsigned short chunkSize = (unsigned char)cDataBuf[4];
	chunkSize               *= 0x100; 
	chunkSize               += (unsigned char)cDataBuf[5];
	// Create FileChunk Name
	memset(cFileChunkName, 0x00, NAME_BUFFER_LEN);
	sprintf(cFileChunkName, "%s.%d", m_currentURI, chunkNum);
	// Save FileContent
	char * cFileContent = (char *)malloc(chunkSize + 2);
	memset(cFileContent, 0x00, chunkSize + 2);
	memcpy(cFileContent, cDataBuf + 6, chunkSize);
	FILE * fp = fopen(cFileChunkName, "wb");
	fwrite(cFileContent, 1, chunkSize, fp);
	fclose(fp);
	free(cFileContent);

	if(chunkNum == m_numChunks)
	{
		mergeFileChunkToFile();
		if(strcmp(strstr(m_currentURI, "app.scode"), "app.scode") == 0)
		{
			m_objSCodeReader.readSCodeFile(m_currentURI);
		}
		resetFileOpenInfo();
	}
}

void CSoxMsg::resetFileOpenInfo()
{
	memset(m_currentURI, 0x00, MAX_BUFFER_LEN);
	m_fileSize = m_actualChunkSize = m_numChunks = 0;
}

void CSoxMsg::mergeFileChunkToFile()
{
	char cFileName[NAME_BUFFER_LEN];
	char cFileChunkName[NAME_BUFFER_LEN];
	char * cFileContent = (char *)malloc(MAX_BUFFER_LEN);
	
	memset(cFileName, 0x00, NAME_BUFFER_LEN);
	sprintf(cFileName, "%s", m_currentURI);

	FILE * fpFile = fopen(cFileName, "wb");
	for (int i =0; i <= m_numChunks; i++)
	{
		// Create FileChunk Name
		memset(cFileChunkName, 0x00, NAME_BUFFER_LEN);
		sprintf(cFileChunkName, "%s.%d", m_currentURI, i);

		// Merge FileChunk to File
		memset(cFileContent, 0x00, MAX_BUFFER_LEN);
		FILE * fpChunk = fopen(cFileChunkName, "rb");
		int iLen = fread(cFileContent, 1, MAX_BUFFER_LEN, fpChunk);
		fclose(fpChunk);
		remove(cFileChunkName);
		fwrite(cFileContent, 1, iLen, fpFile);
	}
	fclose(fpFile);
	free(cFileContent);
}

//	l	link	Add or delete a link
void CSoxMsg::dealLinkRequest(char * cDataBuf, int iDataBufLen)
{

}
//	n	rename	Rename a component
void CSoxMsg::dealRenameRequest(char * cDataBuf, int iDataBufLen)
{

}
//	o	reorder	Reorder a component's children
void CSoxMsg::dealReorderRequest(char * cDataBuf, int iDataBufLen)
{

}
//	q	query	Query installed services
void CSoxMsg::dealQueryRequest(char * cDataBuf, int iDataBufLen)
{

}
//	r	readProp	Read a single property from a component
void CSoxMsg::dealReadPropRequest(char * cDataBuf, int iDataBufLen)
{

}
//	s	subscribe	Subscribe to a component for COV events
void CSoxMsg::dealSubscribeRequest(char * cDataBuf, int iDataBufLen)
{

}
//	u	unsubscribe	Unsubscribe from a component for COV events
void CSoxMsg::dealUnsubscribeRequest(char * cDataBuf, int iDataBufLen)
{

}
//	v	version	Query for the kits installed
void CSoxMsg::dealVersionRequest(char * cDataBuf, int iDataBufLen)
{

}
//	w	write	Write the value of a single component property
void CSoxMsg::dealWriteRequest(char * cDataBuf, int iDataBufLen)
{

}
//	y	versionMore	Query for additional version meta-data
void CSoxMsg::dealVersionMoreRequest(char * cDataBuf, int iDataBufLen)
{

}
//	z	fileClose	Close a file transfer operation
void CSoxMsg::dealFileCloseRequest(char * cDataBuf, int iDataBufLen)
{

}

//	a	add	Add a new component to the application
void CSoxMsg::sendAddRequest()
{

}
//	b	fileRename	Rename or move a file
void CSoxMsg::sendFileRenameRequest()
{
	
}
//	c	readComp	Read a component in the application
void CSoxMsg::sendReadCompRequest()
{

}
//	d	delete	Delete an component and its children from the application
void CSoxMsg::sendReleteRequest()
{

}
//	e	event	COV event for a subscribed component
void CSoxMsg::sendEventRequest()
{

}
//	f	fileOpen	Begin a get or put file transfer operation
void CSoxMsg::sendFileOpenRequest(char method, char * uri, 
								  unsigned int fileSize, 
								  unsigned short suggestedChunkSize)
							//	  , char * name, char * value)
{
	int iDataLen = sizeof(SOX_FILEOPEN_MESSAGE) 
		+ strlen(uri) + 16; // + strlen(name) + strlen(value) 
	char* msg_buffer = (char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	char * sendBufPtr = msg_buffer;
	// u1   'f'
	sendBufPtr[0] = 'f';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	// str  method ("g" for get, "p" for put)
	sendBufPtr[2] =  method;
	sendBufPtr[3] =  0x00;
	sendBufPtr += 4;
	// str  uri
	memcpy(sendBufPtr, uri, strlen(uri));
	sendBufPtr += strlen(uri) + 1;
	// Save URI
	memset(m_currentURI, 0x00, MAX_BUFFER_LEN);
	strcpy(m_currentURI, uri);
	// u4   fileSize
	sendBufPtr[0] = fileSize >> 24;
	sendBufPtr[1] = (fileSize >> 16) & 0xFF;
	sendBufPtr[2] = (fileSize >> 8) & 0xFF;
	sendBufPtr[3] =  fileSize & 0xFF;
	sendBufPtr += 4;
	// u2   suggestedChunkSize  (suggested by client)
	sendBufPtr[0] = (suggestedChunkSize >> 8) & 0xFF;
	sendBufPtr[1] =  suggestedChunkSize & 0xFF;
	sendBufPtr += 2;
/*
	memcpy(sendBufPtr, name, strlen(name));
	sendBufPtr += strlen(name) + 1;

	memcpy(sendBufPtr, value, strlen(value));
	sendBufPtr += strlen(value) + 1;
 */
	// u1 end of headers '\0'
	sendBufPtr++;
	// 
	resetFileOpenInfo();
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
}
//	i	invoke	Invoke a component action
void CSoxMsg::sendInvokeRequest()
{

}
//	k	fileChunk	Receive or send a chunk during a file transfer
void CSoxMsg::sendFileChunkRequest()
{

}
//	l	link	Add or delete a link
void CSoxMsg::sendLinkRequest()
{

}
//	n	rename	Rename a component
void CSoxMsg::sendRenameRequest()
{

}
//	o	reorder	Reorder a component's children
void CSoxMsg::sendReorderRequest()
{

}
//	q	query	Query installed services
void CSoxMsg::sendQueryRequest()
{

}
//	r	readProp	Read a single property from a component
void CSoxMsg::sendReadPropRequest()
{

}
//	s	subscribe	Subscribe to a component for COV events
void CSoxMsg::sendSubscribeRequest()
{

}
//	u	unsubscribe	Unsubscribe from a component for COV events
void CSoxMsg::sendUnsubscribeRequest()
{

}
//	v	version	Query for the kits installed
void CSoxMsg::sendVersionRequest()
{

}
//	w	write	Write the value of a single component property
void CSoxMsg::sendWriteRequest()
{

}
//	y	versionMore	Query for additional version meta-data
void CSoxMsg::sendVersionMoreRequest()
{

}
//	z	fileClose	Close a file transfer operation
void CSoxMsg::sendFileCloseRequest()
{

}
//	!	error	Response id for a command that could not be processed
void CSoxMsg::sendErrorRequest()
{

}

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
			cout << "MESSAGE_TYPES_DATAGRAM " << endl;
			continue;
		}
		Sleep(1);
		if (objSoxMsg->getRecvThreadStatus() == TRUE)
		{
			break;
		}
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

//	a	add	Add a new component to the application
void CSoxMsg::sendAddCommand()
{

}
//	b	fileRename	Rename or move a file
void CSoxMsg::sendFileRenameCommand()
{
	
}
//	c	readComp	Read a component in the application
void CSoxMsg::sendReadCompCommand()
{

}
//	d	delete	Delete an component and its children from the application
void CSoxMsg::sendReleteCommand()
{

}
//	e	event	COV event for a subscribed component
void CSoxMsg::sendEventCommand()
{

}
//	f	fileOpen	Begin a get or put file transfer operation
void CSoxMsg::sendFileOpenCommand(char method, char * uri, 
								  unsigned int fileSize, 
								  unsigned short suggestedChunkSize, 
								  char * name, char * value)
{
	int iDataLen = sizeof(SOX_FILEOPEN_MESSAGE) 
		+ strlen(uri) + strlen(name) + strlen(value) + 16;
	char* msg_buffer = (char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	char * sendBufPtr = msg_buffer;
	// soxFileOpenHeader.header.cmd
	sendBufPtr[0] = 'f';
	// soxFileOpenHeader.header.seqNum
	sendBufPtr[1] = rand() % 0xFF;
	// soxFileOpenHeader.method
	sendBufPtr[2] =  method;
	sendBufPtr[3] =  0x00;
	sendBufPtr += 4;
	memcpy(sendBufPtr, uri, strlen(uri));
	sendBufPtr += strlen(uri) + 1;
	memcpy(sendBufPtr, &fileSize, sizeof(int));
	sendBufPtr += sizeof(int);
	memcpy(sendBufPtr, &suggestedChunkSize, sizeof(short));
	sendBufPtr += sizeof(short);
	
	memcpy(sendBufPtr, name, strlen(name));
	sendBufPtr += strlen(name) + 1;

	memcpy(sendBufPtr, value, strlen(value));
	sendBufPtr += strlen(value) + 1;
	// 
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);

}
//	i	invoke	Invoke a component action
void CSoxMsg::sendInvokeCommand()
{

}
//	k	fileChunk	Receive or send a chunk during a file transfer
void CSoxMsg::sendFileChunkCommand()
{

}
//	l	link	Add or delete a link
void CSoxMsg::sendLinkCommand()
{

}
//	n	rename	Rename a component
void CSoxMsg::sendRenameCommand()
{

}
//	o	reorder	Reorder a component's children
void CSoxMsg::sendReorderCommand()
{

}
//	q	query	Query installed services
void CSoxMsg::sendQueryCommand()
{

}
//	r	readProp	Read a single property from a component
void CSoxMsg::sendReadPropCommand()
{

}
//	s	subscribe	Subscribe to a component for COV events
void CSoxMsg::sendSubscribeCommand()
{

}
//	u	unsubscribe	Unsubscribe from a component for COV events
void CSoxMsg::sendUnsubscribeCommand()
{

}
//	v	version	Query for the kits installed
void CSoxMsg::sendVersionCommand()
{

}
//	w	write	Write the value of a single component property
void CSoxMsg::sendWriteCommand()
{

}
//	y	versionMore	Query for additional version meta-data
void CSoxMsg::sendVersionMoreCommand()
{

}
//	z	fileClose	Close a file transfer operation
void CSoxMsg::sendFileCloseCommand()
{

}
//	!	error	Response id for a command that could not be processed
void CSoxMsg::sendErrorCommand()
{

}

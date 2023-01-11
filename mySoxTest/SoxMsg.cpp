#include "stdafx.h"
#include <process.h>
#include "SoxMsg.h"
#include <windows.h>

CSoxMsg::CSoxMsg()
{
	setBigEndian();
	m_bExitRecvThread = TRUE;
	resetFileOpenInfo();
	m_componentState = 0xFF;
	memset(&m_objSabReadCompRsp, 0x00, sizeof(SOX_READCOMP_RSP));
}

CSoxMsg::~CSoxMsg()
{
}

void CSoxMsg::setAndSkipUnsignedShortValueToBuf(unsigned char ** cBuffer, unsigned short uValue)
{
	if (m_bBigEndian)
	{
		(*cBuffer)[0] = (uValue >> 8) & 0xFF;
		(*cBuffer)[1] =  uValue & 0xFF;
	}
	else
	{
		(*cBuffer)[1] = (uValue >> 8) & 0xFF;
		(*cBuffer)[0] =  uValue & 0xFF;
	}
	*cBuffer += 2;
}

void CSoxMsg::setAndSkipUnsignedIntValueToBuf(unsigned char ** cBuffer, unsigned int uValue)
{
	if (m_bBigEndian)
	{
		(*cBuffer)[0] = (uValue >> 24) & 0xFF;
		(*cBuffer)[1] = (uValue >> 16) & 0xFF;
		(*cBuffer)[2] = (uValue >> 8)  & 0xFF;
		(*cBuffer)[3] =  uValue & 0xFF;
	}
	else
	{
		(*cBuffer)[3] = (uValue >> 24) & 0xFF;
		(*cBuffer)[2] = (uValue >> 16) & 0xFF;
		(*cBuffer)[1] = (uValue >> 8)  & 0xFF;
		(*cBuffer)[0] =  uValue & 0xFF;
	}
	*cBuffer += 4;
}

void CSoxMsg::setAndSkipUnsignedLongValueToBuf(unsigned char ** cBuffer, unsigned long uValue)
{
	if (m_bBigEndian)
	{
		(*cBuffer)[0] = (uValue >> 56) & 0xFF;
		(*cBuffer)[1] = (uValue >> 48) & 0xFF;
		(*cBuffer)[2] = (uValue >> 40) & 0xFF;
		(*cBuffer)[3] = (uValue >> 32) & 0xFF;
		(*cBuffer)[4] = (uValue >> 24) & 0xFF;
		(*cBuffer)[5] = (uValue >> 16) & 0xFF;
		(*cBuffer)[6] = (uValue >> 8)  & 0xFF;
		(*cBuffer)[7] =  uValue & 0xFF;
	}
	else
	{
		(*cBuffer)[7] = (uValue >> 56) & 0xFF;
		(*cBuffer)[6] = (uValue >> 48) & 0xFF;
		(*cBuffer)[5] = (uValue >> 40) & 0xFF;
		(*cBuffer)[4] = (uValue >> 32) & 0xFF;
		(*cBuffer)[3] = (uValue >> 24) & 0xFF;
		(*cBuffer)[2] = (uValue >> 16) & 0xFF;
		(*cBuffer)[1] = (uValue >> 8)  & 0xFF;
		(*cBuffer)[0] =  uValue & 0xFF;
	}
	*cBuffer += 8;
}

unsigned short CSoxMsg::calcAndSkipUnsignedShortValue(unsigned char ** cBuffer)
{
	unsigned short uValue = 0x00;
	if (m_bBigEndian)
	{
		uValue        = (unsigned char)(*cBuffer)[0];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuffer)[1];
	}
	else
	{
		uValue       += (unsigned char)(*cBuffer)[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuffer)[0];
	}
	(*cBuffer) += 2;
	return uValue;
}

unsigned int CSoxMsg::calcAndSkipUnsignedIntValue(unsigned char ** cBuffer)
{
	unsigned int uValue = 0x00;
	if (m_bBigEndian)
	{
		uValue        = (unsigned char)(*cBuffer)[0];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuffer)[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuffer)[2];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuffer)[3];
	}
	else
	{
		uValue        = (unsigned char)(*cBuffer)[3];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuffer)[2];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuffer)[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuffer)[0];
	}
	(*cBuffer) += 4;
	return uValue;
}

unsigned long  CSoxMsg::calcAndSkipUnsignedLongValue(unsigned char ** cBuffer)
{
	unsigned long uValue = 0x00;
	unsigned int uValueOne = 0x00;
	uValueOne = calcAndSkipUnsignedIntValue(cBuffer);
	unsigned int uValueTwo = 0x00;
	uValueTwo = calcAndSkipUnsignedIntValue(cBuffer);

	if (m_bBigEndian)
	{
		uValue = uValueOne * 0x100000000 + uValueTwo;
	}
	else
	{
		uValue = uValueTwo * 0x100000000 + uValueOne;
	}
	return uValue;
}

bool CSoxMsg::loadSabFile(char * cFileName)
{
	char * cReplacePtr;
	char cScodePath[URI_SUGGESTED_LEN];
	memset(cScodePath, 0x00, URI_SUGGESTED_LEN);
	strncpy(cScodePath, cFileName, URI_SUGGESTED_LEN);
	// Replace *.sab to *.scode
	cReplacePtr = strstr(cScodePath, ".sab");
	if (cReplacePtr == cScodePath + strlen(cScodePath) - strlen(".sab"))
	{
		strcpy(cReplacePtr, ".scode");
	}
	else
	{
		return false;
	}

	bool bRet = m_objSabReader.loadSCodeFile(cScodePath);
	if (bRet)
	{
		int iRet = m_objSabReader.readSabFile(cFileName);
		return (iRet == SAB_OK);
	}
	return bRet;
}

void CSoxMsg::unloadSabFile()
{
	m_objSabReader.unloadSCodeFile();
	m_objSabReader.releaseSabFileBuffer();
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
				unsigned char * cLeftDataPtr = objSoxMsg->m_objDASPMsg.getReceiveLeftBuf();

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

void CSoxMsg::dealSoxPacket(unsigned char * cDataBuf, int iDataBufLen)
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
		dealReadCompResponse(cDataBuf, iDataBufLen, m_objSabReadCompRsp);
		break;
	//	d	delete	Delete an component and its children from the application
	case 'd':
		dealDeleteRequest(cDataBuf, iDataBufLen);
		break;
	case 'D':
		dealDeleteResponse(cDataBuf, iDataBufLen);
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

//	a	add	Add a new component to the application
void CSoxMsg::dealAddRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	A	add	Add a new component to the application
void CSoxMsg::dealAddResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	char compID    = cDataBuf[2];
	printf("replyNum = %d, compID = %d.\r\n", (int)replyNum, (int)compID);
}
//	a	add	Add a new component to the application
bool CSoxMsg::sendAddRequest(unsigned short	  parentID,
							 unsigned char    kitId,
							 unsigned char    typeId,
							 char *           cName,
							 SAB_PROP_VALUE * configProps, int configPropsLen)
{
	unsigned char cPropsBuf[PROP_VALUE_LEN];
	SCODE_KIT_TYPE * objSCodeKitType = NULL;
	if (m_objSabReader.isSCodeKitsGet() == false)
	{
		printf("We need the scode file.");
		return false;
	}
	if (strlen(cName) >= PROP_PROP_NAME)
	{
		printf(" Name should be less than eight characters.");
		return false;
	}

	objSCodeKitType = m_objSabReader.getScodeKitTypeByKitIDAndTypeID(kitId, typeId);
	if (objSCodeKitType)
	{
		memset(cPropsBuf, 0x00, PROP_VALUE_LEN);
		int iPropsLen = m_objSabReader.encodePropsToBuf(cPropsBuf,
			           objSCodeKitType, configProps, configPropsLen);
		int iDataLen = sizeof(SOX_ADD_REQ)
				 + strlen(cName) + iPropsLen + 1;
		unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
		memset(msg_buffer, 0x00, iDataLen);
		unsigned char * sendBufPtr = msg_buffer;
		// u1   'a'
		sendBufPtr[0] = 'a';
		// u1   replyNum
		sendBufPtr[1] = rand() % 0xFF;
		sendBufPtr += 2;
		// u2    parentId
		setAndSkipUnsignedShortValueToBuf(&sendBufPtr, parentID);
		// u1    kitId
		sendBufPtr[0] = kitId;
		// u1    typeId
		sendBufPtr[1] = typeId;
		sendBufPtr += 2;
		// str  uri
		memcpy(sendBufPtr, cName, strlen(cName));
		sendBufPtr += strlen(cName) + 1;
		// val[] configProps
		memcpy(sendBufPtr, cPropsBuf, iPropsLen);

		m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
		free(msg_buffer);
		return true;
	}
	// SAB_PROP
	return false;
}

//	b	fileRename	Rename or move a file
void CSoxMsg::dealFileRenameRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	B	fileRename	Rename or move a file
void CSoxMsg::dealFileRenameResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	b	fileRename	Rename or move a file
bool CSoxMsg::sendFileRenameRequest(unsigned char * cFromName, char * cToName)
{
	int iDataLen = strlen((char *)cFromName) + strlen((char *)cFromName) + 4;
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'b'
	sendBufPtr[0] = 'b';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	// str  cFromName
	memcpy(sendBufPtr, cFromName, strlen((char *)cFromName));
	sendBufPtr += strlen((char *)cFromName) + 1;
	// str  cToName
	memcpy(sendBufPtr, cToName, strlen((char *)cToName));
	sendBufPtr += strlen((char *)cToName) + 1;

	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	c	readComp	Read a component in the application
void CSoxMsg::dealReadCompRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	C	readComp	Read a component in the application
void CSoxMsg::dealReadCompResponse(unsigned char * cDataBuf, int iDataBufLen, SOX_READCOMP_RSP& objSabReadCompRsp)
{
	memset(&objSabReadCompRsp, 0x00, sizeof(SOX_READCOMP_RSP));
	unsigned char * sendBufPtr = cDataBuf;
	unsigned char replyNum  = sendBufPtr[1];
	sendBufPtr += 2;
	printf("replyNum = %d.\r\n", (unsigned int)replyNum);

	objSabReadCompRsp.compID      = calcAndSkipUnsignedShortValue(&sendBufPtr);
	objSabReadCompRsp.componentState  = sendBufPtr[0];
	sendBufPtr ++;
	if (objSabReadCompRsp.componentState == RTFLAGS_FILTER_TREE)
	{
		objSabReadCompRsp.objSabCompTree.kitID   = sendBufPtr[0];
		objSabReadCompRsp.objSabCompTree.typeID  = sendBufPtr[1];
		sendBufPtr += 2;
		strcpy((char *)objSabReadCompRsp.objSabCompTree.cName, (char *)sendBufPtr);
		sendBufPtr += strlen((char *)objSabReadCompRsp.objSabCompTree.cName) + 1;
		objSabReadCompRsp.objSabCompTree.parentID      = calcAndSkipUnsignedShortValue(&sendBufPtr);
		objSabReadCompRsp.objSabCompTree.permissions   = sendBufPtr[0];
		objSabReadCompRsp.objSabCompTree.numKids       = sendBufPtr[1];
		sendBufPtr += 2;
		objSabReadCompRsp.objSabCompTree.kidIds = (unsigned short *)malloc(
			sizeof(unsigned short) * objSabReadCompRsp.objSabCompTree.numKids);
		for (int i = 0; i < objSabReadCompRsp.objSabCompTree.numKids; i++)
		{
			objSabReadCompRsp.objSabCompTree.kidIds[i] = calcAndSkipUnsignedShortValue(&sendBufPtr);
		}
	}
	else if ((objSabReadCompRsp.componentState == RTFLAGS_FILTER_CONFIG) ||
		     (objSabReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_CONFIG)||
		     (objSabReadCompRsp.componentState == RTFLAGS_FILTER_RUNTIME)||
		     (objSabReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_RUNTIME))
	{
		SCODE_KIT_TYPE * objSCodeKitType = m_objSabReader.getScodeKitTypeByCompID(objSabReadCompRsp.compID);
		if (objSCodeKitType)
		{
			m_objSabReader.getSabNameAndTypeNameByCompID(objSabReadCompRsp.compID, 
				objSabReadCompRsp.cName, objSabReadCompRsp.cTypeName);
			objSabReadCompRsp.objSabCompProps.propValuesCount = 
				m_objSabReader.decodeAndSkipPropsFromBuf(&sendBufPtr, objSCodeKitType, 
					&objSabReadCompRsp.objSabCompProps.cValueList);
		}
		
	}
	else if (objSabReadCompRsp.componentState == RTFLAGS_FILTER_LINKS)
	{
	}
	printSoxReadcompRsp(objSabReadCompRsp);
}

void CSoxMsg::printSoxReadcompRsp(SOX_READCOMP_RSP& objSabReadCompRsp)
{
	printf("objSabReadCompRsp(compID, cName, cTypeName, componentState) = (%02d, %s, %s, %c) and ",
		objSabReadCompRsp.compID, objSabReadCompRsp.cName, 
		objSabReadCompRsp.cTypeName,objSabReadCompRsp.componentState);
	if (objSabReadCompRsp.componentState == RTFLAGS_FILTER_TREE)
	{
		printf(" objSabCompTree is:  \r\n");
		
	}
	else if ((objSabReadCompRsp.componentState == RTFLAGS_FILTER_CONFIG) ||
		     (objSabReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_CONFIG)||
		     (objSabReadCompRsp.componentState == RTFLAGS_FILTER_RUNTIME)||
		     (objSabReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_RUNTIME))
	{
		printf(" objSabCompProps is:  \r\n");
		for (int j=0; j < objSabReadCompRsp.objSabCompProps.propValuesCount; j++)
		{
			printf("\t objSabCompProps.cValueList[%d](cPropName, cValue) "
				"  = (%s, '", j, objSabReadCompRsp.objSabCompProps.cValueList[j].cPropName);
			m_objSabReader.printSingleSabProp(
				objSabReadCompRsp.objSabCompProps.cValueList[j].cValue,
				objSabReadCompRsp.objSabCompProps.cValueList[j].cPropName);
			printf("' ) \r\n");
		}
	}
	else if (objSabReadCompRsp.componentState == RTFLAGS_FILTER_LINKS)
	{
		printf(" objSabCompLinks is:  \r\n");
	}
}

//	c	readComp	Read a component in the application
bool CSoxMsg::sendReadCompRequest(unsigned short	componentID,
							      unsigned char   componentState)
{
	if ((componentState != RTFLAGS_FILTER_TREE) &&
		(componentState != RTFLAGS_FILTER_CONFIG) &&
		(componentState != RTFLAGS_FILTER_RUNTIME) &&
		(componentState != RTFLAGS_FILTER_LINKS))
	{ 
		return false;
	}
	// Clear recv object.
	// memset(&m_objSabReadCompRsp, 0x00, sizeof(SAB_READCOMP_RSP))

	int iDataLen = 4 + 1;
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'c'
	sendBufPtr[0] = 'c';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, componentID);
	sendBufPtr[0] = componentState;
	sendBufPtr ++;
	m_componentState = componentState;

	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;

}
//	d	delete	Delete an component and its children from the application
void CSoxMsg::dealDeleteRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	D	delete	Delete an component and its children from the application
void CSoxMsg::dealDeleteResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	d	delete	Delete an component and its children from the application
bool CSoxMsg::sendDeleteRequest()
{
	return true;

}

//	e	event	COV event for a subscribed component
void CSoxMsg::dealEventRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	E	event	COV event for a subscribed component
void CSoxMsg::dealEventResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	e	event	COV event for a subscribed component
bool CSoxMsg::sendEventRequest()
{

	return true;
}

//	f	fileOpen	Begin a get or put file transfer operation
void CSoxMsg::dealFileOpenRequest(unsigned char * cDataBuf, int iDataBufLen)
{
}

//	F	fileOpen	Begin a get or put file transfer operation
void CSoxMsg::dealFileOpenResponse(unsigned char * cDataBuf, int iDataBufLen)
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

//	f	fileOpen	Begin a get or put file transfer operation
bool CSoxMsg::sendFileOpenRequest(unsigned char method, char * uri,
								  unsigned int fileSize,
								  unsigned short suggestedChunkSize)
							//	  , char * name, char * value)
{
	int iDataLen = sizeof(SOX_FILEOPEN_REQ)
		+ strlen(uri) + 16; // + strlen(name) + strlen(value)
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
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
	setAndSkipUnsignedIntValueToBuf(&sendBufPtr, fileSize);
	// u2   suggestedChunkSize  (suggested by client)
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, suggestedChunkSize);
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
	free(msg_buffer);
	return true;
}

//	i	invoke	Invoke a component action
void CSoxMsg::dealInvokeRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	I	invoke	Invoke a component action
void CSoxMsg::dealInvokeResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	i	invoke	Invoke a component action
bool CSoxMsg::sendInvokeRequest()
{
	return true;

}
//	k	fileChunk	Receive or send a chunk during a file transfer
void CSoxMsg::dealFileChunkRequest(unsigned char * cDataBuf, int iDataBufLen)
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
		// If we get new app.scode, we would read scode information from app.scode
		// and empty all information of app.sab. Otherwise, we would get reference error.
		if(strcmp(strstr(m_currentURI, "app.scode"), "app.scode") == 0)
		{
			if (m_objSabReader.isSCodeKitsGet())
			{
				m_objSabReader.unloadSCodeFile();
				// Empty all information of app.sab. Otherwise, we would get reference error.
				m_objSabReader.releaseSabFileBuffer();
				m_objSabReader.loadSCodeFile(m_currentURI);
				// Now, youcan use m_objSabReader.getSCodeKits get info of app.scode
			}
		}
		// If we get new app.sab, we would update app.scode and app.sab at the same time.
		else if(strcmp(strstr(m_currentURI, "app.sab"), "app.sab") == 0)
		{
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

//	K	fileChunk	Receive or send a chunk during a file transfer
void CSoxMsg::dealFileChunkResponse(unsigned char * cDataBuf, int iDataBufLen)
{
}
//	k	fileChunk	Receive or send a chunk during a file transfer
bool CSoxMsg::sendFileChunkRequest()
{
	return true;

}

//	l	link	Add or delete a link
void CSoxMsg::dealLinkRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	L	link	Add or delete a link
void CSoxMsg::dealLinkResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	l	link	Add or delete a link
bool CSoxMsg::sendLinkRequest()
{
	return true;

}
//	n	rename	Rename a component
void CSoxMsg::dealRenameRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	N	rename	Rename a component
void CSoxMsg::dealRenameResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	n	rename	Rename a component
bool CSoxMsg::sendRenameRequest()
{
	return true;

}

//	o	reorder	Reorder a component's children
void CSoxMsg::dealReorderRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	O	reorder	Reorder a component's children
void CSoxMsg::dealReorderResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	o	reorder	Reorder a component's children
bool CSoxMsg::sendReorderRequest()
{
	return true;

}
//	q	query	Query installed services
void CSoxMsg::dealQueryRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	Q	query	Query installed services
void CSoxMsg::dealQueryResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	q	query	Query installed services
bool CSoxMsg::sendQueryRequest()
{
	return true;

}

//	r	readProp	Read a single property from a component
void CSoxMsg::dealReadPropRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	R	readProp	Read a single property from a component
void CSoxMsg::dealReadPropResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	r	readProp	Read a single property from a component
bool CSoxMsg::sendReadPropRequest()
{
	return true;

}
//	s	subscribe	Subscribe to a component for COV events
void CSoxMsg::dealSubscribeRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	S	subscribe	Subscribe to a component for COV events
void CSoxMsg::dealSubscribeResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	s	subscribe	Subscribe to a component for COV events
bool CSoxMsg::sendSubscribeRequest()
{
	return true;

}
//	u	unsubscribe	Unsubscribe from a component for COV events
void CSoxMsg::dealUnsubscribeRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	u	unsubscribe	Unsubscribe from a component for COV events
void CSoxMsg::dealUnsubscribeResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	u	unsubscribe	Unsubscribe from a component for COV events
bool CSoxMsg::sendUnsubscribeRequest()
{
	return true;

}
//	v	version	Query for the kits installed
void CSoxMsg::dealVersionRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	V	version	Query for the kits installed
void CSoxMsg::dealVersionResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	v	version	Query for the kits installed
bool CSoxMsg::sendVersionRequest()
{
	return true;

}
//	w	write	Write the value of a single component property
void CSoxMsg::dealWriteRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	w	write	Write the value of a single component property
void CSoxMsg::dealWriteResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	w	write	Write the value of a single component property
bool CSoxMsg::sendWriteRequest()
{
	return true;

}
//	y	versionMore	Query for additional version meta-data
void CSoxMsg::dealVersionMoreRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	y	versionMore	Query for additional version meta-data
void CSoxMsg::dealVersionMoreResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	y	versionMore	Query for additional version meta-data
bool CSoxMsg::sendVersionMoreRequest()
{
	return true;

}
//	z	fileClose	Close a file transfer operation
void CSoxMsg::dealFileCloseRequest(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	z	fileClose	Close a file transfer operation
void CSoxMsg::dealFileCloseResponse(unsigned char * cDataBuf, int iDataBufLen)
{

}
//	z	fileClose	Close a file transfer operation
bool CSoxMsg::sendFileCloseRequest()
{
	return true;

}
//	!	error	Response id for a Response that could not be processed
void CSoxMsg::dealErrorResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char cReplyNum = cDataBuf[1];
	printf("ERROR: %s\r\n", cDataBuf + 2);
}

//	!	error	Response id for a command that could not be processed
bool CSoxMsg::sendErrorRequest()
{
	return true;

}

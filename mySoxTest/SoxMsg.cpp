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
	memset(&m_objSoxReadCompRsp, 0x00, sizeof(SOX_READCOMP_RSP));
	m_objSchemaKitList = NULL;
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

bool CSoxMsg::loadSabFileAndSCodeFile(char * cSabFileName, char * cSCodeFileName)
{
	bool bRet = m_objSabReader.loadSCodeFile(cSCodeFileName);
	if (bRet)
	{
		int iRet = m_objSabReader.readSabFile(cSabFileName);
		return (iRet == SAB_OK);
	}
	return bRet;
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
		dealReadCompResponse(cDataBuf, iDataBufLen, m_objSoxReadCompRsp);
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
		dealEventRequest(cDataBuf, iDataBufLen, m_objSoxReadCompRsp);
		break;
	case 'E':
	//  Never happen in the SOX
	//	dealEventResponse(cDataBuf, iDataBufLen);
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
		dealQueryResponse(cDataBuf, iDataBufLen, m_queryRes);
		break;
	//	r	readProp	Read a single property from a component
	case 'r':
		dealReadPropRequest(cDataBuf, iDataBufLen);
		break;
	case 'R':
		dealReadPropResponse(cDataBuf, iDataBufLen, m_objSabProp);
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
		dealVersionResponse(cDataBuf, iDataBufLen, &m_objSchemaKitList);
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
		dealVersionMoreResponse(cDataBuf, iDataBufLen, m_objSoxVersionMore);
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
			           objSCodeKitType, RTFLAGS_FILTER_CONFIG, configProps, configPropsLen);
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
bool CSoxMsg::sendFileRenameRequest(unsigned char * cFromName, unsigned char * cToName)
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
void CSoxMsg::dealReadCompResponse(unsigned char * cDataBuf, int iDataBufLen, SOX_READCOMP_RSP& objSoxReadCompRsp)
{
	memset(&objSoxReadCompRsp, 0x00, sizeof(SOX_READCOMP_RSP));
	unsigned char * sendBufPtr = cDataBuf;
	unsigned char replyNum  = sendBufPtr[1];
	sendBufPtr += 2;
	printf("replyNum = %d.\r\n", (unsigned int)replyNum);

	objSoxReadCompRsp.compID      = calcAndSkipUnsignedShortValue(&sendBufPtr);
	objSoxReadCompRsp.componentState  = sendBufPtr[0];
	sendBufPtr++;
	if (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_TREE)
	{
		objSoxReadCompRsp.objSabCompTree.kitID   = sendBufPtr[0];
		objSoxReadCompRsp.objSabCompTree.typeID  = sendBufPtr[1];
		sendBufPtr += 2;
		strcpy((char *)objSoxReadCompRsp.objSabCompTree.cName, (char *)sendBufPtr);
		sendBufPtr += strlen((char *)objSoxReadCompRsp.objSabCompTree.cName) + 1;
		objSoxReadCompRsp.objSabCompTree.parentID      = calcAndSkipUnsignedShortValue(&sendBufPtr);
		objSoxReadCompRsp.objSabCompTree.permissions   = sendBufPtr[0];
		objSoxReadCompRsp.objSabCompTree.numKids       = sendBufPtr[1];
		sendBufPtr += 2;
		objSoxReadCompRsp.objSabCompTree.kidIds = (unsigned short *)malloc(
			sizeof(unsigned short) * objSoxReadCompRsp.objSabCompTree.numKids);
		memset(objSoxReadCompRsp.objSabCompTree.kidIds, 0x00, 
			sizeof(unsigned short) * objSoxReadCompRsp.objSabCompTree.numKids);
		for (int i = 0; i < objSoxReadCompRsp.objSabCompTree.numKids; i++)
		{
			objSoxReadCompRsp.objSabCompTree.kidIds[i] = calcAndSkipUnsignedShortValue(&sendBufPtr);
		}
	}
	else if ((objSoxReadCompRsp.componentState == RTFLAGS_FILTER_CONFIG) ||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_CONFIG)||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_RUNTIME)||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_RUNTIME))
	{
		SCODE_KIT_TYPE * objSCodeKitType = m_objSabReader.getScodeKitTypeByCompID(objSoxReadCompRsp.compID);
		if (objSCodeKitType)
		{
			m_objSabReader.getComponentNameAndTypeNameByCompID(objSoxReadCompRsp.compID, 
				objSoxReadCompRsp.cName, objSoxReadCompRsp.cTypeName);

			objSoxReadCompRsp.objSabCompProps.cValueList = (SAB_PROP_ELEMENT *)malloc(
				sizeof(SAB_PROP_ELEMENT) * objSCodeKitType->slotsLen);
			memset(objSoxReadCompRsp.objSabCompProps.cValueList, 0x00, 
				sizeof(SAB_PROP_ELEMENT) * objSCodeKitType->slotsLen);
			
			objSoxReadCompRsp.objSabCompProps.propValuesCount = 
				m_objSabReader.decodeAndSkipPropsFromBuf(&sendBufPtr, objSCodeKitType, 
					objSoxReadCompRsp.componentState, 
					objSoxReadCompRsp.objSabCompProps.cValueList);
		}
	}
	else if (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_LINKS)
	{
		std::vector<SAB_LINK>    objSabLinkList;
		SAB_LINK objSabLinkInfo;
		while (1)
		{
			memset(&objSabLinkInfo, 0x00, sizeof(SAB_LINK));
			objSabLinkInfo.fromComp  = calcAndSkipUnsignedShortValue(&sendBufPtr);
			if ((objSabLinkInfo.fromComp == 0xFFFF) || (objSabLinkInfo.fromComp < 0))
				break;
			objSabLinkInfo.fromSlot  = sendBufPtr[0];
			sendBufPtr++;
			m_objSabReader.getLinkNameByCompIDAndSlotID(
				objSabLinkInfo.fromComp, objSabLinkInfo.fromSlot, objSabLinkInfo.cFromLinkName);
			objSabLinkInfo.toComp    = calcAndSkipUnsignedShortValue(&sendBufPtr);
			objSabLinkInfo.toSlot    = sendBufPtr[0];
			sendBufPtr++;
			m_objSabReader.getLinkNameByCompIDAndSlotID(
				objSabLinkInfo.toComp, objSabLinkInfo.toSlot, objSabLinkInfo.cToLinkName);
			objSabLinkList.push_back(objSabLinkInfo);
		}
		objSoxReadCompRsp.objSabCompLinks.compLinksCount = objSabLinkList.size();
		objSoxReadCompRsp.objSabCompLinks.cLinksList = (SAB_LINK *)malloc(sizeof(SAB_LINK)
										* objSoxReadCompRsp.objSabCompLinks.compLinksCount);
		memset(objSoxReadCompRsp.objSabCompLinks.cLinksList, 0x00, 
			sizeof(SAB_LINK) * objSoxReadCompRsp.objSabCompLinks.compLinksCount);
		int iLinkSeq = 0;
		std::vector<SAB_LINK>::iterator iter;
		for (iter = objSabLinkList.begin(); iter != objSabLinkList.end(); iter++)
		{
			objSoxReadCompRsp.objSabCompLinks.cLinksList[iLinkSeq] = *iter;
			iLinkSeq++;
		}
	}
	printSoxReadcompRsp(objSoxReadCompRsp);
	releaseSoxReadcompRsp(m_objSoxReadCompRsp);
}

void CSoxMsg::printSoxReadcompRsp(SOX_READCOMP_RSP& objSoxReadCompRsp)
{
	printf("objSoxReadCompRsp(compID, cName, cTypeName, componentState) = (%02d, %s, %s, %c) and ",
		objSoxReadCompRsp.compID, objSoxReadCompRsp.cName, 
		objSoxReadCompRsp.cTypeName, objSoxReadCompRsp.componentState);
	if (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_TREE)
	{
		printf(" objSabCompTree is:  \r\n");
		printf("\t objSabCompTree(kitID, typeID, cName, parentID, permissions) = "
			       "(%02d, %02d, %s, %02d, %02d) and kids =",
				objSoxReadCompRsp.objSabCompTree.kitID, objSoxReadCompRsp.objSabCompTree.typeID, 
				objSoxReadCompRsp.objSabCompTree.cName, objSoxReadCompRsp.objSabCompTree.parentID, 
				objSoxReadCompRsp.objSabCompTree.permissions);
		for (int j=0; j < objSoxReadCompRsp.objSabCompTree.numKids; j++)
		{
			printf(" (%d)", objSoxReadCompRsp.objSabCompTree.kidIds[j]);
			
		}
		printf(". \r\n");
	}
	else if ((objSoxReadCompRsp.componentState == RTFLAGS_FILTER_CONFIG) ||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_CONFIG)||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_RUNTIME)||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_RUNTIME))
	{
		printf(" objSabCompProps is:  \r\n");
		for (int j=0; j < objSoxReadCompRsp.objSabCompProps.propValuesCount; j++)
		{
			printf("\t objSabCompProps.cValueList[%d](cPropName, cValue) "
				"  = (%s, '", j, objSoxReadCompRsp.objSabCompProps.cValueList[j].cPropName);
			m_objSabReader.printSingleSabProp(
				objSoxReadCompRsp.objSabCompProps.cValueList[j].cValue,
				objSoxReadCompRsp.objSabCompProps.cValueList[j].cPropName);
			printf("' ) \r\n");
		}
	}
	else if (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_LINKS)
	{
		printf(" objSabCompLinks is:  \r\n");
		for (int j=0; j < objSoxReadCompRsp.objSabCompLinks.compLinksCount; j++)
		{
			printf("\t ");
			m_objSabReader.printSingleSabLink(objSoxReadCompRsp.objSabCompLinks.cLinksList[j]);
			printf(". \r\n");
		}
	}
}

void CSoxMsg::releaseSoxReadcompRsp(SOX_READCOMP_RSP& objSoxReadCompRsp)
{
	if (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_TREE)
	{
		free(objSoxReadCompRsp.objSabCompTree.kidIds);
	}
	else if ((objSoxReadCompRsp.componentState == RTFLAGS_FILTER_CONFIG) ||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_CONFIG)||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_RUNTIME)||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_RUNTIME))
	{
		free(objSoxReadCompRsp.objSabCompProps.cValueList);
	}
	else if (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_LINKS)
	{
		free(objSoxReadCompRsp.objSabCompLinks.cLinksList);
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
	// memset(&m_objSoxReadCompRsp, 0x00, sizeof(SAB_READCOMP_RSP))

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
	// Not implement
}
//	D	delete	Delete an component and its children from the application
void CSoxMsg::dealDeleteResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	d	delete	Delete an component and its children from the application
bool CSoxMsg::sendDeleteRequest(unsigned short	componentID)
{
	int iDataLen = 4 + 1;
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'd'
	sendBufPtr[0] = 'd';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, componentID);
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}

//	e	event	COV event for a subscribed component
void CSoxMsg::dealEventRequest(unsigned char * cDataBuf, int iDataBufLen
							   , SOX_READCOMP_RSP& objSoxReadCompRsp)
{
	unsigned char * sendBufPtr = cDataBuf;
	unsigned char replyNum  = sendBufPtr[1];
	sendBufPtr += 2;
	printf("replyNum = %d.\r\n", (unsigned int)replyNum);
	
	objSoxReadCompRsp.compID      = calcAndSkipUnsignedShortValue(&sendBufPtr);
	objSoxReadCompRsp.componentState  = sendBufPtr[0];
	sendBufPtr++;
	if (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_TREE)
	{
		objSoxReadCompRsp.objSabCompTree.kitID   = sendBufPtr[0];
		objSoxReadCompRsp.objSabCompTree.typeID  = sendBufPtr[1];
		sendBufPtr += 2;
		strcpy((char *)objSoxReadCompRsp.objSabCompTree.cName, (char *)sendBufPtr);
		sendBufPtr += strlen((char *)objSoxReadCompRsp.objSabCompTree.cName) + 1;
		objSoxReadCompRsp.objSabCompTree.parentID      = calcAndSkipUnsignedShortValue(&sendBufPtr);
		objSoxReadCompRsp.objSabCompTree.permissions   = sendBufPtr[0];
		objSoxReadCompRsp.objSabCompTree.numKids       = sendBufPtr[1];
		sendBufPtr += 2;
		objSoxReadCompRsp.objSabCompTree.kidIds = (unsigned short *)malloc(
			sizeof(unsigned short) * objSoxReadCompRsp.objSabCompTree.numKids);
		memset(objSoxReadCompRsp.objSabCompTree.kidIds, 0x00, 
			sizeof(unsigned short) * objSoxReadCompRsp.objSabCompTree.numKids);
		for (int i = 0; i < objSoxReadCompRsp.objSabCompTree.numKids; i++)
		{
			objSoxReadCompRsp.objSabCompTree.kidIds[i] = calcAndSkipUnsignedShortValue(&sendBufPtr);
		}
	}
	else if ((objSoxReadCompRsp.componentState == RTFLAGS_FILTER_CONFIG) ||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_CONFIG)||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_RUNTIME)||
		     (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_OPERATOR_RUNTIME))
	{
		SCODE_KIT_TYPE * objSCodeKitType = m_objSabReader.getScodeKitTypeByCompID(objSoxReadCompRsp.compID);
		if (objSCodeKitType)
		{
			m_objSabReader.getComponentNameAndTypeNameByCompID(objSoxReadCompRsp.compID, 
				objSoxReadCompRsp.cName, objSoxReadCompRsp.cTypeName);

			objSoxReadCompRsp.objSabCompProps.cValueList = (SAB_PROP_ELEMENT *)malloc(
				sizeof(SAB_PROP_ELEMENT) * objSCodeKitType->slotsLen);
			memset(objSoxReadCompRsp.objSabCompProps.cValueList, 0x00, 
				sizeof(SAB_PROP_ELEMENT) * objSCodeKitType->slotsLen);
			
			objSoxReadCompRsp.objSabCompProps.propValuesCount = 
				m_objSabReader.decodeAndSkipPropsFromBuf(&sendBufPtr, objSCodeKitType, 
					objSoxReadCompRsp.componentState, 
					objSoxReadCompRsp.objSabCompProps.cValueList);
		}
	}
	else if (objSoxReadCompRsp.componentState == RTFLAGS_FILTER_LINKS)
	{
		std::vector<SAB_LINK>    objSabLinkList;
		SAB_LINK objSabLinkInfo;
		while (1)
		{
			memset(&objSabLinkInfo, 0x00, sizeof(SAB_LINK));
			objSabLinkInfo.fromComp  = calcAndSkipUnsignedShortValue(&sendBufPtr);
			if ((objSabLinkInfo.fromComp == 0xFFFF) || (objSabLinkInfo.fromComp < 0))
				break;
			objSabLinkInfo.fromSlot  = sendBufPtr[0];
			sendBufPtr++;
			m_objSabReader.getLinkNameByCompIDAndSlotID(
				objSabLinkInfo.fromComp, objSabLinkInfo.fromSlot, objSabLinkInfo.cFromLinkName);
			objSabLinkInfo.toComp    = calcAndSkipUnsignedShortValue(&sendBufPtr);
			objSabLinkInfo.toSlot    = sendBufPtr[0];
			sendBufPtr++;
			m_objSabReader.getLinkNameByCompIDAndSlotID(
				objSabLinkInfo.toComp, objSabLinkInfo.toSlot, objSabLinkInfo.cToLinkName);
			objSabLinkList.push_back(objSabLinkInfo);
		}
		objSoxReadCompRsp.objSabCompLinks.compLinksCount = objSabLinkList.size();
		objSoxReadCompRsp.objSabCompLinks.cLinksList = (SAB_LINK *)malloc(sizeof(SAB_LINK)
										* objSoxReadCompRsp.objSabCompLinks.compLinksCount);
		memset(objSoxReadCompRsp.objSabCompLinks.cLinksList, 0x00, 
			sizeof(SAB_LINK) * objSoxReadCompRsp.objSabCompLinks.compLinksCount);
		int iLinkSeq = 0;
		std::vector<SAB_LINK>::iterator iter;
		for (iter = objSabLinkList.begin(); iter != objSabLinkList.end(); iter++)
		{
			objSoxReadCompRsp.objSabCompLinks.cLinksList[iLinkSeq] = *iter;
			iLinkSeq++;
		}
	}
	printSoxReadcompRsp(objSoxReadCompRsp);
}
//	E	event	COV event for a subscribed component
// void CSoxMsg::dealEventResponse(unsigned char * cDataBuf, int iDataBufLen)
// {
//
// }
//	e	event	COV event for a subscribed component
bool CSoxMsg::sendEventRequest()
{
	// Not implement
	return true;
}

//	f	fileOpen	Begin a get or put file transfer operation
void CSoxMsg::dealFileOpenRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
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
	// Not implement
}
//	I	invoke	Invoke a component action
void CSoxMsg::dealInvokeResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	i	invoke	Invoke a component action
bool CSoxMsg::sendInvokeRequest(unsigned short	componentID,
								unsigned char 	slotID,
								SAB_PROP_VALUE& objArgument)
{
	SCODE_KIT_TYPE * objSCodeKitType = m_objSabReader.getScodeKitTypeByCompID(componentID);
	if ((!objSCodeKitType) ||(slotID >= objSCodeKitType->slotsLen))
	{
		return false;
	}
	int iArgumentLen = m_objSabReader.calcPropSize(objSCodeKitType->kit_type_slots_list[slotID].fpBix, objArgument);
	int iDataLen = 5 + iArgumentLen + 1;

	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'i'
	sendBufPtr[0] = 'i';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, componentID);
	sendBufPtr[0] = slotID;
	sendBufPtr ++;
	if (iArgumentLen > 0)
	{
		m_objSabReader.encodeOnePropToBuf(&sendBufPtr, objArgument, 
				// objArgument.propType, 
				objSCodeKitType->kit_type_slots_list[slotID].fpBix, 
				m_objSabReader.isRtFlagsAsStr(objSCodeKitType->kit_type_slots_list[slotID].rtFlags));
	}
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
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
	// Not implement
	return true;
}

//	l	link	Add or delete a link
void CSoxMsg::dealLinkRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	L	link	Add or delete a link
void CSoxMsg::dealLinkResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	l	link	Add or delete a link
bool CSoxMsg::sendLinkRequest(unsigned char opcode, SAB_LINK& objSabLink)
{
	int iDataLen = 3 + 6 + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'f'
	sendBufPtr[0] = 'l';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	// u1   'a' | 'd' (add/delete)
	sendBufPtr[2] =  opcode;
	sendBufPtr += 3;
	//   u2   fromCompId
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, objSabLink.fromComp);
	//  u1   fromSlotId
	sendBufPtr[0] = objSabLink.fromSlot;
	sendBufPtr++;
	//  u2   toCompId
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, objSabLink.toComp);
	//  u1   toSlotId
	sendBufPtr[0] = objSabLink.toSlot;
	sendBufPtr++;
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	n	rename	Rename a component
void CSoxMsg::dealRenameRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	N	rename	Rename a component
void CSoxMsg::dealRenameResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	n	rename	Rename a component
bool CSoxMsg::sendRenameRequest(unsigned short	componentID,
								unsigned char* 	newName)
{
	int iDataLen = 4 + strlen((char *)newName) + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'f'
	sendBufPtr[0] = 'n';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	// u2   CompId
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, componentID);
	// str   newName
	memcpy(sendBufPtr, newName, strlen((char *)newName));
	sendBufPtr += strlen((char *)newName) + 1;
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}

//	o	reorder	Reorder a component's children
void CSoxMsg::dealReorderRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	O	reorder	Reorder a component's children
void CSoxMsg::dealReorderResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	o	reorder	Reorder a component's children
bool CSoxMsg::sendReorderRequest(unsigned short	    componentID,
								unsigned short* 	childrenIds, 
								unsigned char       numChildren)
{
	int iDataLen = 5 + numChildren * sizeof(unsigned short) + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'o'
	sendBufPtr[0] = 'o';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	// u2   CompId
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, componentID);
	// u1   numChildren
	sendBufPtr[0] = numChildren;
	sendBufPtr++;
	// u2[]  childrenIds
	for (int i = 0; i < numChildren; i++)
	{
		setAndSkipUnsignedShortValueToBuf(&sendBufPtr, childrenIds[i]);
	}
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	q	query	Query installed services
void CSoxMsg::dealQueryRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	Q	query	Query installed services
void CSoxMsg::dealQueryResponse(unsigned char * cDataBuf, int iDataBufLen, SOX_QUERYRES& queryRes)
{
	unsigned char * sendBufPtr = cDataBuf;
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
	sendBufPtr += 2;
	memset(&queryRes, 0x00, sizeof(SOX_QUERYRES));
	std::vector<unsigned short>    objQueryResList;
	while (1)
	{
		unsigned short uQueryRes = calcAndSkipUnsignedShortValue(&sendBufPtr);
		if (uQueryRes == 0xFFFF) 
			break;
		objQueryResList.push_back(uQueryRes);
	}
	if (objQueryResList.size() > 0)
	{
		int iLinkSeq = 0;
		queryRes.numberOfQueryRes = objQueryResList.size();
		queryRes.queryResults = (unsigned short *)malloc(
			sizeof(unsigned short) * queryRes.numberOfQueryRes);
		std::vector<unsigned short>::iterator iter;
		for (iter = objQueryResList.begin(); iter != objQueryResList.end(); iter++)
		{
			queryRes.queryResults[iLinkSeq] = *iter;
			iLinkSeq++;
		}
	}
	printQueryResult(queryRes);
	releaseQueryResult(queryRes);
}

void CSoxMsg::printQueryResult(SOX_QUERYRES& objQueryRes)
{
	if (objQueryRes.numberOfQueryRes > 0)
	{
		int i = 0;
		printf("objQueryRes(numberOfQueryRes) = (%02d) and queryResults is: \r\n", 
			objQueryRes.numberOfQueryRes);
		for (i = 0; i< objQueryRes.numberOfQueryRes; i++)
		{
			printf("\t queryResults[%d] = (%d). \r\n", i, objQueryRes.queryResults[i]);
		}
	}
	else 
	{
		printf("objQueryRes(numberOfQueryRes) = (%02d) \r\n", 
			objQueryRes.numberOfQueryRes);
	}
}
void CSoxMsg::releaseQueryResult(SOX_QUERYRES& objQueryRes)
{
	if (objQueryRes.queryResults)
	{
		free(objQueryRes.queryResults);
	}
}

//	q	query	Query installed services
bool CSoxMsg::sendQueryRequest(unsigned char	queryType,
							   unsigned char 	kitID,
							   unsigned char 	typeID)
{
	if (queryType != 's')
	{
		return false;
	}
	int iDataLen = 5 + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'r'
	sendBufPtr[0] = 'q';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	// u1   queryType
	sendBufPtr[0] = queryType;
	sendBufPtr++;
	// u1   queryType
	sendBufPtr[0] = kitID;
	sendBufPtr[1] = typeID;
	sendBufPtr += 2;
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}

//	r	readProp	Read a single property from a component
void CSoxMsg::dealReadPropRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	R	readProp	Read a single property from a component
void CSoxMsg::dealReadPropResponse(unsigned char * cDataBuf, int iDataBufLen, SAB_PROP& objSabProp)
{
	unsigned char * sendBufPtr = cDataBuf;
	char replyNum  = cDataBuf[1];
	sendBufPtr += 2;
	printf("replyNum = %d.\r\n", (int)replyNum);
	
	unsigned short    componentID = calcAndSkipUnsignedShortValue(&sendBufPtr);
	unsigned char     propID      = sendBufPtr[0];
	unsigned char     typeID      = sendBufPtr[1];
	sendBufPtr += 2;
     
	SCODE_KIT_TYPE * objSCodeKitType = m_objSabReader.getScodeKitTypeByCompID(componentID);
	if ((!objSCodeKitType) ||(propID >= objSCodeKitType->slotsLen))
	{
		return ;
	}
	printf("cPropName = %s.\r\n", objSCodeKitType->kit_type_slots_list[propID].cPropName);
	if (m_objSabReader.getMagicSlotTypebySlotTypeID(typeID) != objSCodeKitType->kit_type_slots_list[propID].fpBix)
	{
		return ;
	}
	m_objSabReader.getSabPropByCompIDAndSlotID(componentID, propID, objSabProp);
	m_objSabReader.decodeAndSkipOnePropFromBuf(&sendBufPtr, objSabProp.objSoxProp,
				objSCodeKitType->kit_type_slots_list[propID].fpBix);
}
//	r	readProp	Read a single property from a component
bool CSoxMsg::sendReadPropRequest(unsigned short	componentID,
								unsigned char 	propID)
{
	int iDataLen = 4 + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'r'
	sendBufPtr[0] = 'r';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	// u2   CompId
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, componentID);
	// m_componentID = componentID;
	// u1   numChildren
	sendBufPtr[0] = propID;
	sendBufPtr++;
	// m_propID = propID;
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	s	subscribe	Subscribe to a component for COV events
void CSoxMsg::dealSubscribeRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	S	subscribe	Subscribe to a component for COV events
void CSoxMsg::dealSubscribeResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
	printf("numSubscribed = %d.\r\n", (int)cDataBuf[2]);

}
//	s	subscribe	Subscribe to a component for COV events
bool CSoxMsg::sendSubscribeRequest(unsigned char	cMask,
								unsigned short* 	compIds, 
								unsigned char       numComIds)
{
	int iDataLen = 5 + numComIds * sizeof(unsigned short) + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   's'
	sendBufPtr[0] = 's';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	// u1   whatMask: tree=0x1 config=0x2 rt=0x4 links=0x8 0xff all tree
	sendBufPtr[0] = cMask;
	sendBufPtr++;
	// u1   numComIds
	sendBufPtr[0] = numComIds;
	sendBufPtr++;
	// u2[]  childrenIds
	for (int i = 0; i < numComIds; i++)
	{
		setAndSkipUnsignedShortValueToBuf(&sendBufPtr, compIds[i]);
	}
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	u	unsubscribe	Unsubscribe from a component for COV events
void CSoxMsg::dealUnsubscribeRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	u	unsubscribe	Unsubscribe from a component for COV events
void CSoxMsg::dealUnsubscribeResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	u	unsubscribe	Unsubscribe from a component for COV events
bool CSoxMsg::sendUnsubscribeRequest(unsigned char	cMask,
								unsigned short* 	compIds, 
								unsigned char       numComIds)
{
	int iDataLen = 5 + numComIds * sizeof(unsigned short) + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'u'
	sendBufPtr[0] = 'u';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	// u1   whatMask: tree=0x1 config=0x2 rt=0x4 links=0x8 0xff all tree
	sendBufPtr[0] = cMask;
	sendBufPtr++;
	// u1   numComIds
	sendBufPtr[0] = numComIds;
	sendBufPtr++;
	// u2[]  childrenIds
	for (int i = 0; i < numComIds; i++)
	{
		setAndSkipUnsignedShortValueToBuf(&sendBufPtr, compIds[i]);
	}
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	v	version	Query for the kits installed
void CSoxMsg::dealVersionRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	V	version	Query for the kits installed
void CSoxMsg::dealVersionResponse(unsigned char * cDataBuf, int iDataBufLen,
								  SCHEMA_KIT ** objSchemaKitList)
{
	unsigned char * sendBufPtr = cDataBuf;
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
	unsigned char kitCount  = cDataBuf[2];
	sendBufPtr += 3;

	(*objSchemaKitList) = (SCHEMA_KIT *)malloc(sizeof(SCHEMA_KIT) * kitCount);
	for (int i = 0; i < kitCount; i++)
	{
		memset(&((*objSchemaKitList)[i]), 0x00, sizeof(SCHEMA_KIT));
		strcpy((char *)(*objSchemaKitList)[i].cName, (char *)sendBufPtr);
		sendBufPtr += strlen((char *)(*objSchemaKitList)[i].cName) + 1;
		(*objSchemaKitList)[i].checksum = calcAndSkipUnsignedIntValue(&sendBufPtr);
	}
}
//	v	version	Query for the kits installed
bool CSoxMsg::sendVersionRequest()
{
	int iDataLen = 2 + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'u'
	sendBufPtr[0] = 'v';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	w	write	Write the value of a single component property
void CSoxMsg::dealWriteRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	w	write	Write the value of a single component property
void CSoxMsg::dealWriteResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	w	write	Write the value of a single component property
bool CSoxMsg::sendWriteRequest(unsigned short	componentID,
								unsigned char 	slotID,
								SAB_PROP_VALUE& objValue)
{
	SCODE_KIT_TYPE * objSCodeKitType = m_objSabReader.getScodeKitTypeByCompID(componentID);
	if ((!objSCodeKitType) ||(slotID >= objSCodeKitType->slotsLen))
	{
		return false;
	}
	int iValueLen = m_objSabReader.calcPropSize(objSCodeKitType->kit_type_slots_list[slotID].fpBix, objValue);
	int iDataLen = 5 + iValueLen + 1;

	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'w'
	sendBufPtr[0] = 'w';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	setAndSkipUnsignedShortValueToBuf(&sendBufPtr, componentID);
	sendBufPtr[0] = slotID;
	sendBufPtr ++;
	if (iValueLen > 0)
	{
		m_objSabReader.encodeOnePropToBuf(&sendBufPtr, objValue, 
				// objArgument.propType, 
				objSCodeKitType->kit_type_slots_list[slotID].fpBix, 
				m_objSabReader.isRtFlagsAsStr(objSCodeKitType->kit_type_slots_list[slotID].rtFlags));
	}
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	y	versionMore	Query for additional version meta-data
void CSoxMsg::dealVersionMoreRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}

//	y	versionMore	Query for additional version meta-data
void CSoxMsg::dealVersionMoreResponse(unsigned char * cDataBuf, int iDataBufLen, SOX_VERSIONMORE& objSoxVersionMore)
{
	int i = 0;
	memset(&objSoxVersionMore, 0x00, sizeof(SOX_VERSIONMORE));
	unsigned char * sendBufPtr = cDataBuf;
	char replyNum  = cDataBuf[1];
	sendBufPtr += 2;
	printf("replyNum = %d.\r\n", (int)replyNum);
	strcpy((char *)objSoxVersionMore.strPlatformID, (char *)sendBufPtr);
	sendBufPtr += strlen((char *)objSoxVersionMore.strPlatformID) + 1;
	objSoxVersionMore.scodeFlags = sendBufPtr[0];
	sendBufPtr ++;

	objSoxVersionMore.numberOfKits = m_objSabReader.getNumberOfKits();
	objSoxVersionMore.kitVersions = (SOX_VERSIONMORE_KITVERSION *)malloc(
		sizeof(SOX_VERSIONMORE_KITVERSION) * objSoxVersionMore.numberOfKits);
	memset(objSoxVersionMore.kitVersions, 0x00, 
		sizeof(SOX_VERSIONMORE_KITVERSION) * objSoxVersionMore.numberOfKits);
	for (i = 0; i< objSoxVersionMore.numberOfKits; i++)
	{
		strcpy((char *)objSoxVersionMore.kitVersions[i].strkitName, 
			(char *)m_objSabReader.getSCodeKitName(i));
		strcpy((char *)objSoxVersionMore.kitVersions[i].strkitVersion, (char *)sendBufPtr);
		sendBufPtr += strlen((char *)objSoxVersionMore.kitVersions[i].strkitVersion) + 1;
	}
	objSoxVersionMore.uPairs = sendBufPtr[0];
	sendBufPtr ++;
	objSoxVersionMore.structPairKeyValList = (SOX_VERSIONMORE_PAIR *)malloc(
		sizeof(SOX_VERSIONMORE_PAIR) * objSoxVersionMore.uPairs);
	memset(objSoxVersionMore.structPairKeyValList, 
		0x00, sizeof(SOX_VERSIONMORE_PAIR) * objSoxVersionMore.uPairs);
	for (i = 0; i< objSoxVersionMore.uPairs; i++)
	{
		strcpy((char *)objSoxVersionMore.structPairKeyValList[i].strPairKey, (char *)sendBufPtr);
		sendBufPtr += strlen((char *)objSoxVersionMore.structPairKeyValList[i].strPairKey) + 1;
		strcpy((char *)objSoxVersionMore.structPairKeyValList[i].strPairVal, (char *)sendBufPtr);
		sendBufPtr += strlen((char *)objSoxVersionMore.structPairKeyValList[i].strPairVal) + 1;
	}
	printVersionMore(objSoxVersionMore);
	releaseVersionMore(m_objSoxVersionMore);
}

void CSoxMsg::printVersionMore(SOX_VERSIONMORE& objSoxVersionMore)
{
	int i = 0;
	printf("objSoxReadCompRsp(strPlatformID, scodeFlags, numberOfKits) = "
		   "(%s, %02d, %02d) and kitVersions is: \r\n",
		objSoxVersionMore.strPlatformID, objSoxVersionMore.scodeFlags, objSoxVersionMore.numberOfKits);
	for (i = 0; i< objSoxVersionMore.numberOfKits; i++)
	{
		printf("\t (strkitName, strkitVersion) = (%s, %s). \r\n", 
			objSoxVersionMore.kitVersions[i].strkitName, objSoxVersionMore.kitVersions[i].strkitVersion);
	}
	printf("objSoxReadCompRsp(uPairs) = (%02d) and structPairKeyValList is: \r\n", objSoxVersionMore.uPairs);
	for (i = 0; i< objSoxVersionMore.uPairs; i++)
	{
		printf("\t (strPairKey, strPairVal) = (%s, %s). \r\n", 
			objSoxVersionMore.structPairKeyValList[i].strPairKey, 
			objSoxVersionMore.structPairKeyValList[i].strPairVal);
	}
}

void CSoxMsg::releaseVersionMore(SOX_VERSIONMORE& objSoxVersionMore)
{
	free(objSoxVersionMore.kitVersions);
	free(objSoxVersionMore.structPairKeyValList);
	memset(&objSoxVersionMore, 0x00, sizeof(SOX_VERSIONMORE));
}
//	y	versionMore	Query for additional version meta-data
bool CSoxMsg::sendVersionMoreRequest()
{
	int iDataLen = 2 + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'y'
	sendBufPtr[0] = 'y';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	z	fileClose	Close a file transfer operation
void CSoxMsg::dealFileCloseRequest(unsigned char * cDataBuf, int iDataBufLen)
{
	// Not implement
}
//	z	fileClose	Close a file transfer operation
void CSoxMsg::dealFileCloseResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	char replyNum  = cDataBuf[1];
	printf("replyNum = %d.\r\n", (int)replyNum);
}
//	z	fileClose	Close a file transfer operation
bool CSoxMsg::sendFileCloseRequest()
{
	int iDataLen = 2 + 1; 
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   'z'
	sendBufPtr[0] = 'z';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	
	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}
//	!	error	Response id for a Response that could not be processed
void CSoxMsg::dealErrorResponse(unsigned char * cDataBuf, int iDataBufLen)
{
	unsigned char cCause[100];
	int i = 0;
	unsigned char * sendBufPtr = cDataBuf;
	char replyNum  = cDataBuf[1];
	sendBufPtr += 2;
	printf("replyNum = %d.\r\n", (int)replyNum);

	memset(cCause, 0x00, 100);
	strcpy((char *)cCause, (char *)sendBufPtr);
	sendBufPtr += strlen((char *)cCause) + 1;
	printf("ERROR: ErrorCause = %s.\r\n", cCause);
}

//	!	error	Response id for a command that could not be processed
bool CSoxMsg::sendErrorRequest(unsigned char * cCause)
{
	int iDataLen = 2 + strlen((char *)cCause) + 1;
	unsigned char* msg_buffer = (unsigned char *)malloc(iDataLen);
	memset(msg_buffer, 0x00, iDataLen);

	unsigned char * sendBufPtr = msg_buffer;
	// u1   '!'
	sendBufPtr[0] = '!';
	// u1   replyNum
	sendBufPtr[1] = rand() % 0xFF;
	sendBufPtr += 2;
	// str  cFromName
	memcpy(sendBufPtr, cCause, strlen((char *)cCause));
	sendBufPtr += strlen((char *)cCause) + 1;

	m_objDASPMsg.sendDatagramRequest(msg_buffer, sendBufPtr - msg_buffer);
	free(msg_buffer);
	return true;
}

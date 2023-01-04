#pragma once
#include "DASP.h"
#include "SCodeReader.h"

#define URI_SUGGESTED_LEN                 1024          // 
#define SCODE_FILE_SUGGESTED_CHUNKSIZE    (100 * 1024)  // Normal size is 92K
#define APP_FILE_SUGGESTED_CHUNKSIZE      (1 * 1024)    // Normal size is 1-2K

class CSoxMsg
{
public:
	CSoxMsg();
	~CSoxMsg();

public:
	int initializeSocket();
	int start();
	void close();
	char recvResponse();

	void startRecvThread();
	void handShake();
	BOOL getRecvThreadStatus();

public:
	void dealSoxPacket(char * cDataBuf, int iDataBufLen);

public:
	//	a	add	Add a new component to the application
	void sendAddRequest();
	//	b	fileRename	Rename or move a file
	void sendFileRenameRequest();
	//	c	readComp	Read a component in the application
	void sendReadCompRequest();
	//	d	delete	Delete an component and its children from the application
	void sendReleteRequest();
	//	e	event	COV event for a subscribed component
	void sendEventRequest();
	//	f	fileOpen	Begin a get or put file transfer operation
	void sendFileOpenRequest(char method, char * uri, 
								  unsigned int fileSize, 
								  unsigned short suggestedChunkSize);
							//	  ,char * name, char * value);
	//	i	invoke	Invoke a component action
	void sendInvokeRequest();
	//	k	fileChunk	Receive or send a chunk during a file transfer
	void sendFileChunkRequest();
	//	l	link	Add or delete a link
	void sendLinkRequest();
	//	n	rename	Rename a component
	void sendRenameRequest();
	//	o	reorder	Reorder a component's children
	void sendReorderRequest();
	//	q	query	Query installed services
	void sendQueryRequest();
	//	r	readProp	Read a single property from a component
	void sendReadPropRequest();
	//	s	subscribe	Subscribe to a component for COV events
	void sendSubscribeRequest();
	//	u	unsubscribe	Unsubscribe from a component for COV events
	void sendUnsubscribeRequest();
	//	v	version	Query for the kits installed
	void sendVersionRequest();
	//	w	write	Write the value of a single component property
	void sendWriteRequest();
	//	y	versionMore	Query for additional version meta-data
	void sendVersionMoreRequest();
	//	z	fileClose	Close a file transfer operation
	void sendFileCloseRequest();
	//	!	error	Response id for a command that could not be processed
	void sendErrorRequest();
	//

public:
	//	a	add	Add a new component to the application
	void dealAddResponse(char * cDataBuf, int iDataBufLen);
	//	b	fileRename	Rename or move a file
	void dealFileRenameResponse(char * cDataBuf, int iDataBufLen);
	//	c	readComp	Read a component in the application
	void dealReadCompResponse(char * cDataBuf, int iDataBufLen);
	//	d	delete	Delete an component and its children from the application
	void dealReleteResponse(char * cDataBuf, int iDataBufLen);
	//	e	event	COV event for a subscribed component
	void dealEventResponse(char * cDataBuf, int iDataBufLen);
	//	f	fileOpen	Begin a get or put file transfer operation
	void dealFileOpenResponse(char * cDataBuf, int iDataBufLen);
	//	i	invoke	Invoke a component action
	void dealInvokeResponse(char * cDataBuf, int iDataBufLen);
	//	k	fileChunk	Receive or send a chunk during a file transfer
	void dealFileChunkResponse(char * cDataBuf, int iDataBufLen);
	//	l	link	Add or delete a link
	void dealLinkResponse(char * cDataBuf, int iDataBufLen);
	//	n	rename	Rename a component
	void dealRenameResponse(char * cDataBuf, int iDataBufLen);
	//	o	reorder	Reorder a component's children
	void dealReorderResponse(char * cDataBuf, int iDataBufLen);
	//	q	query	Query installed services
	void dealQueryResponse(char * cDataBuf, int iDataBufLen);
	//	r	readProp	Read a single property from a component
	void dealReadPropResponse(char * cDataBuf, int iDataBufLen);
	//	s	subscribe	Subscribe to a component for COV events
	void dealSubscribeResponse(char * cDataBuf, int iDataBufLen);
	//	u	unsubscribe	Unsubscribe from a component for COV events
	void dealUnsubscribeResponse(char * cDataBuf, int iDataBufLen);
	//	v	version	Query for the kits installed
	void dealVersionResponse(char * cDataBuf, int iDataBufLen);
	//	w	write	Write the value of a single component property
	void dealWriteResponse(char * cDataBuf, int iDataBufLen);
	//	y	versionMore	Query for additional version meta-data
	void dealVersionMoreResponse(char * cDataBuf, int iDataBufLen);
	//	z	fileClose	Close a file transfer operation
	void dealFileCloseResponse(char * cDataBuf, int iDataBufLen);
	//	!	error	Response id for a Response that could not be processed
	void dealErrorResponse(char * cDataBuf, int iDataBufLen);

public:
	//	a	add	Add a new component to the application
	void dealAddRequest(char * cDataBuf, int iDataBufLen);
	//	b	fileRename	Rename or move a file
	void dealFileRenameRequest(char * cDataBuf, int iDataBufLen);
	//	c	readComp	Read a component in the application
	void dealReadCompRequest(char * cDataBuf, int iDataBufLen);
	//	d	delete	Delete an component and its children from the application
	void dealReleteRequest(char * cDataBuf, int iDataBufLen);
	//	e	event	COV event for a subscribed component
	void dealEventRequest(char * cDataBuf, int iDataBufLen);
	//	f	fileOpen	Begin a get or put file transfer operation
	void dealFileOpenRequest(char * cDataBuf, int iDataBufLen);
	//	i	invoke	Invoke a component action
	void dealInvokeRequest(char * cDataBuf, int iDataBufLen);
	//	k	fileChunk	Receive or send a chunk during a file transfer
	void dealFileChunkRequest(char * cDataBuf, int iDataBufLen);
	//	l	link	Add or delete a link
	void dealLinkRequest(char * cDataBuf, int iDataBufLen);
	//	n	rename	Rename a component
	void dealRenameRequest(char * cDataBuf, int iDataBufLen);
	//	o	reorder	Reorder a component's children
	void dealReorderRequest(char * cDataBuf, int iDataBufLen);
	//	q	query	Query installed services
	void dealQueryRequest(char * cDataBuf, int iDataBufLen);
	//	r	readProp	Read a single property from a component
	void dealReadPropRequest(char * cDataBuf, int iDataBufLen);
	//	s	subscribe	Subscribe to a component for COV events
	void dealSubscribeRequest(char * cDataBuf, int iDataBufLen);
	//	u	unsubscribe	Unsubscribe from a component for COV events
	void dealUnsubscribeRequest(char * cDataBuf, int iDataBufLen);
	//	v	version	Query for the kits installed
	void dealVersionRequest(char * cDataBuf, int iDataBufLen);
	//	w	write	Write the value of a single component property
	void dealWriteRequest(char * cDataBuf, int iDataBufLen);
	//	y	versionMore	Query for additional version meta-data
	void dealVersionMoreRequest(char * cDataBuf, int iDataBufLen);
	//	z	fileClose	Close a file transfer operation
	void dealFileCloseRequest(char * cDataBuf, int iDataBufLen);
	//	!	error	Request id for a Request that could not be processed
	void dealErrorRequest(char * cDataBuf, int iDataBufLen);

public:
	void mergeFileChunkToFile();
	void resetFileOpenInfo();

public:
	CDASP m_objDASPMsg;
	SCodeReader m_objSCodeReader;
	
private:
	BOOL m_bExitRecvThread;
	
private:
	//	f	fileOpen	Begin a get or put file transfer operation
	char m_currentURI[MAX_BUFFER_LEN];
	int  m_fileSize;
	int  m_actualChunkSize;
	int  m_numChunks;
};


#pragma once
#include "DASP.h"
#include "SabReader.h"

#define URI_SUGGESTED_LEN                 1024          // 
#define SCODE_FILE_SUGGESTED_CHUNKSIZE    (100 * 1024)  // Normal size is 92K
#define APP_FILE_SUGGESTED_CHUNKSIZE      (1 * 1024)    // Normal size is 1-2K
#define PROP_VALUE_LEN                    1024  
#define PROP_PROP_NAME                    8  

#pragma pack(1)
typedef struct _SOX_MESSAGE
{
	unsigned char      cmd;
	unsigned char      seqNum;
//	u1[]    payload
} SOX_MESSAGE, *PSOX_MESSAGE;

typedef struct _SOX_ADD_REQ
{
	SOX_MESSAGE     header;
	unsigned short	parentID;
	unsigned char   kitId;
	unsigned char   typeId;
	char *          name;
	SOX_PROP *      configProps;
} SOX_ADD_REQ, *PSOX_ADD_REQ;

typedef struct _SOX_FILEOPEN_REQ
{
	SOX_MESSAGE     header;
	char            method;
	char *          uri_ptr;
	unsigned int    fileSize;
	short			suggestedChunkSize;
	char *          headers_name_ptr;
	char *          value_ptr;
} SOX_FILEOPEN_REQ, *PSOX_FILEOPEN_REQ;
#pragma pack()

class CSoxMsg
{
public:
	bool loadSabFile(char * cFileName);
	void unloadSabFile();
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
	bool sendAddRequest(unsigned short	parentID,
							 unsigned char   kitId,
							 unsigned char   typeId,
							 char *          cName,
							 SOX_PROP *      configProps, int configPropsLen);
	//	b	fileRename	Rename or move a file
	bool sendFileRenameRequest();
	//	c	readComp	Read a component in the application
	bool sendReadCompRequest();
	//	d	delete	Delete an component and its children from the application
	bool sendReleteRequest();
	//	e	event	COV event for a subscribed component
	bool sendEventRequest();
	//	f	fileOpen	Begin a get or put file transfer operation
	bool sendFileOpenRequest(char method, char * uri, 
								  unsigned int fileSize, 
								  unsigned short suggestedChunkSize);
							//	  ,char * name, char * value);
	//	i	invoke	Invoke a component action
	bool sendInvokeRequest();
	//	k	fileChunk	Receive or send a chunk during a file transfer
	bool sendFileChunkRequest();
	//	l	link	Add or delete a link
	bool sendLinkRequest();
	//	n	rename	Rename a component
	bool sendRenameRequest();
	//	o	reorder	Reorder a component's children
	bool sendReorderRequest();
	//	q	query	Query installed services
	bool sendQueryRequest();
	//	r	readProp	Read a single property from a component
	bool sendReadPropRequest();
	//	s	subscribe	Subscribe to a component for COV events
	bool sendSubscribeRequest();
	//	u	unsubscribe	Unsubscribe from a component for COV events
	bool sendUnsubscribeRequest();
	//	v	version	Query for the kits installed
	bool sendVersionRequest();
	//	w	write	Write the value of a single component property
	bool sendWriteRequest();
	//	y	versionMore	Query for additional version meta-data
	bool sendVersionMoreRequest();
	//	z	fileClose	Close a file transfer operation
	bool sendFileCloseRequest();
	//	!	error	Response id for a command that could not be processed
	bool sendErrorRequest();
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
	CDASP       m_objDASPMsg;
	SabReader   m_objSabReader;
	
private:
	BOOL m_bExitRecvThread;
	
private:
	//	f	fileOpen	Begin a get or put file transfer operation
	char m_currentURI[MAX_BUFFER_LEN];
	int  m_fileSize;
	int  m_actualChunkSize;
	int  m_numChunks;
};


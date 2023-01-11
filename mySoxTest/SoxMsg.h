#pragma once
#include "DASP.h"
#include "SabReader.h"

#define URI_SUGGESTED_LEN                 1024          //
#define SCODE_FILE_SUGGESTED_CHUNKSIZE    (100 * 1024)  // Normal size is 92K
#define APP_FILE_SUGGESTED_CHUNKSIZE      (1 * 1024)    // Normal size is 1-2K
#define PROP_VALUE_LEN                    1024
#define PROP_PROP_NAME                    8
#define PROP_COMP_NAME                    128

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
	unsigned char   kitID;
	unsigned char   typeID;
	char *          name;
	SAB_PROP *      configProps;
} SOX_ADD_REQ, *PSOX_ADD_REQ;

typedef struct _SOX_FILEOPEN_REQ
{
	SOX_MESSAGE     header;
	char            method;
	char *          uri_ptr;
	unsigned int    fileSize;
	unsigned short	suggestedChunkSize;
	char *          headers_name_ptr;
	char *          value_ptr;
} SOX_FILEOPEN_REQ, *PSOX_FILEOPEN_REQ;

typedef struct _SOX_COMPTREE
{
	unsigned char    kitID;
	unsigned char    typeID;
	unsigned char    cName[PROP_COMP_NAME];
	unsigned short	 parentID;
	unsigned char    permissions;
	unsigned char    numKids;
	unsigned short * kidIds;
} SOX_COMPTREE, *PSOX_COMPTREE;

typedef struct _SOX_COMPPROPS
{
	unsigned int    propValuesCount;
	SAB_PROP_ELEMENT * cValueList;
} SOX_COMPPROPS, *PSOX_COMPPROPS;

typedef struct _SOX_COMPLINKS
{
	unsigned int    compLinksCount;
	SAB_LINK  *     cLinksList;
} SOX_COMPLINKS, *PSAB_COMPLINKS;

typedef struct _SOX_READCOMP_RSP
{
	unsigned short       compID;
	// For debug
	unsigned char        cName[PROP_COMP_NAME];
	unsigned char        cTypeName[PROP_COMP_NAME];
	// For debug end
	unsigned char        componentState ;
	union {
		SOX_COMPTREE     objSabCompTree;
		SOX_COMPPROPS    objSabCompProps;
		SOX_COMPLINKS    objSabCompLinks;
	};
} SOX_READCOMP_RSP, *PSOX_READCOMP_RSP;


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
	void dealSoxPacket(unsigned char * cDataBuf, int iDataBufLen);

public:
	//	a	add	Add a new component to the application
	bool sendAddRequest(unsigned short	        parentID,
							 unsigned char      kitID,
							 unsigned char      typeID,
							 char *             cName,
							 SAB_PROP_VALUE *   configProps, int configPropsLen);
	//	b	fileRename	Rename or move a file
	bool sendFileRenameRequest(unsigned char * cFromName, char * cToName);
	//	c	readComp	Read a component in the application
	bool sendReadCompRequest(unsigned short	componentID,
							 unsigned char   componentState);
	//	d	delete	Delete an component and its children from the application
	bool sendDeleteRequest();
	//	e	event	COV event for a subscribed component
	bool sendEventRequest();
	//	f	fileOpen	Begin a get or put file transfer operation
	bool sendFileOpenRequest(unsigned char method, char * uri,
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
	void dealAddResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	b	fileRename	Rename or move a file
	void dealFileRenameResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	c	readComp	Read a component in the application
	void dealReadCompResponse(unsigned char * cDataBuf, int iDataBufLen, SOX_READCOMP_RSP& objSabReadCompRsp);
	void printSoxReadcompRsp(SOX_READCOMP_RSP& objSabReadCompRsp);
	//	d	delete	Delete an component and its children from the application
	void dealDeleteResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	e	event	COV event for a subscribed component
	void dealEventResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	f	fileOpen	Begin a get or put file transfer operation
	void dealFileOpenResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	i	invoke	Invoke a component action
	void dealInvokeResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	k	fileChunk	Receive or send a chunk during a file transfer
	void dealFileChunkResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	l	link	Add or delete a link
	void dealLinkResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	n	rename	Rename a component
	void dealRenameResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	o	reorder	Reorder a component's children
	void dealReorderResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	q	query	Query installed services
	void dealQueryResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	r	readProp	Read a single property from a component
	void dealReadPropResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	s	subscribe	Subscribe to a component for COV events
	void dealSubscribeResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	u	unsubscribe	Unsubscribe from a component for COV events
	void dealUnsubscribeResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	v	version	Query for the kits installed
	void dealVersionResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	w	write	Write the value of a single component property
	void dealWriteResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	y	versionMore	Query for additional version meta-data
	void dealVersionMoreResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	z	fileClose	Close a file transfer operation
	void dealFileCloseResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	!	error	Response id for a Response that could not be processed
	void dealErrorResponse(unsigned char * cDataBuf, int iDataBufLen);

public:
	//	a	add	Add a new component to the application
	void dealAddRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	b	fileRename	Rename or move a file
	void dealFileRenameRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	c	readComp	Read a component in the application
	void dealReadCompRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	d	delete	Delete an component and its children from the application
	void dealDeleteRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	e	event	COV event for a subscribed component
	void dealEventRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	f	fileOpen	Begin a get or put file transfer operation
	void dealFileOpenRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	i	invoke	Invoke a component action
	void dealInvokeRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	k	fileChunk	Receive or send a chunk during a file transfer
	void dealFileChunkRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	l	link	Add or delete a link
	void dealLinkRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	n	rename	Rename a component
	void dealRenameRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	o	reorder	Reorder a component's children
	void dealReorderRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	q	query	Query installed services
	void dealQueryRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	r	readProp	Read a single property from a component
	void dealReadPropRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	s	subscribe	Subscribe to a component for COV events
	void dealSubscribeRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	u	unsubscribe	Unsubscribe from a component for COV events
	void dealUnsubscribeRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	v	version	Query for the kits installed
	void dealVersionRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	w	write	Write the value of a single component property
	void dealWriteRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	y	versionMore	Query for additional version meta-data
	void dealVersionMoreRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	z	fileClose	Close a file transfer operation
	void dealFileCloseRequest(unsigned char * cDataBuf, int iDataBufLen);
	//	!	error	Request id for a Request that could not be processed
	void dealErrorRequest(unsigned char * cDataBuf, int iDataBufLen);

public:
	void           setBigEndian()    { m_bBigEndian = true; }
	void           setLittleEndian() { m_bBigEndian = false; }
	bool           isBigEndian()     { return (m_bBigEndian == true);  }
	bool           isLittleEndian()  { return (m_bBigEndian == false); }

public:
	bool           m_bBigEndian;
	void setAndSkipUnsignedShortValueToBuf(unsigned char ** cBuffer, unsigned short uValue);
	void setAndSkipUnsignedIntValueToBuf(unsigned char ** cBuffer,   unsigned int uValue);
	void setAndSkipUnsignedLongValueToBuf(unsigned char ** cBuffer,  unsigned long uValue);

	unsigned short calcAndSkipUnsignedShortValue(unsigned char ** cBuffer);
	unsigned int   calcAndSkipUnsignedIntValue(unsigned char ** cBuffer);
	unsigned long  calcAndSkipUnsignedLongValue(unsigned char ** cBuffer);

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
	// readComp:	Read a component in the application
	unsigned char     m_componentState;
	SOX_READCOMP_RSP  m_objSabReadCompRsp;

};


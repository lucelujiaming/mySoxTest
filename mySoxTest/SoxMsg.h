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

typedef struct _SOX_VERSIONMORE_KITVERSION
{
	unsigned char        strkitName[SAB_TYPE_NAME_LEN];
	unsigned char        strkitVersion[SAB_TYPE_NAME_LEN];
} SOX_VERSIONMORE_KITVERSION, *PSOX_VERSIONMORE_KITVERSION;

typedef struct _SOX_VERSIONMORE_PAIR
{
	unsigned char        strPairKey[SAB_TYPE_NAME_LEN];
	unsigned char        strPairVal[SAB_TYPE_NAME_LEN];
} SOX_VERSIONMORE_PAIR, *PSOX_VERSIONMORE_PAIR;

typedef struct _SOX_VERSIONMORE
{
	unsigned char strPlatformID[SAB_TYPE_NAME_LEN];
	unsigned char scodeFlags;
	unsigned char numberOfKits;
	SOX_VERSIONMORE_KITVERSION* kitVersions ;
	unsigned char uPairs;
	SOX_VERSIONMORE_PAIR * structPairKeyValList ;
} SOX_VERSIONMORE, *PSOX_VERSIONMORE;


typedef struct _SOX_QUERYRES
{
	unsigned char numberOfQueryRes;
	unsigned short * queryResults ;
} SOX_QUERYRES, *PSOX_QUERYRES;

#pragma pack()

class CSoxMsg
{
public:
	bool loadSabFileAndSCodeFile(char * cSabFileName, char * cSCodeFileName);
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
	bool sendFileRenameRequest(unsigned char * cFromName,unsigned char * cToName);
	//	c	readComp	Read a component in the application
	bool sendReadCompRequest(unsigned short	componentID,
							 unsigned char   componentState);
	//	d	delete	Delete an component and its children from the application
	bool sendDeleteRequest(unsigned short	componentID);
	//	e	event	COV event for a subscribed component
	bool sendEventRequest();
	//	f	fileOpen	Begin a get or put file transfer operation
	bool sendFileOpenRequest(unsigned char method, char * uri,
								  unsigned int fileSize,
								  unsigned short suggestedChunkSize);
							//	  ,char * name, char * value);
	//	i	invoke	Invoke a component action
	bool sendInvokeRequest(unsigned short	componentID,
							unsigned char	slotID,
							SAB_PROP_VALUE& objArgument);
	//	k	fileChunk	Receive or send a chunk during a file transfer
	bool sendFileChunkRequest();
	//	l	link	Add or delete a link
	bool sendLinkRequest(unsigned char opcode, SAB_LINK& objSabLink);
	//	n	rename	Rename a component
	bool sendRenameRequest(unsigned short	componentID,
								unsigned char* 	newName);
	//	o	reorder	Reorder a component's children
	bool sendReorderRequest(unsigned short	    componentID,
								unsigned short* 	childrenIds, 
								unsigned char       numChildren);
	//	q	query	Query installed services
	bool sendQueryRequest(unsigned char	queryType,
							   unsigned char 	kitID,
							   unsigned char 	typeID);
	//	r	readProp	Read a single property from a component
	bool sendReadPropRequest(unsigned short	componentID,
								unsigned char 	propID);
	//	s	subscribe	Subscribe to a component for COV events
	bool sendSubscribeRequest(unsigned char	cMask,
								unsigned short* 	compIds, 
								unsigned char       numComIds);
	//	u	unsubscribe	Unsubscribe from a component for COV events
	bool sendUnsubscribeRequest(unsigned char	cMask,
								unsigned short* 	compIds, 
								unsigned char       numComIds);
	//	v	version	Query for the kits installed
	bool sendVersionRequest();
	//	w	write	Write the value of a single component property
	bool sendWriteRequest(unsigned short	componentID,
								unsigned char 	slotID,
								SAB_PROP_VALUE& objValue);
	//	y	versionMore	Query for additional version meta-data
	bool sendVersionMoreRequest();
	//	z	fileClose	Close a file transfer operation
	bool sendFileCloseRequest();
	//	!	error	Response id for a command that could not be processed
	bool sendErrorRequest(unsigned char * cCause);

public:
	//	a	add	Add a new component to the application
	void dealAddResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	b	fileRename	Rename or move a file
	void dealFileRenameResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	c	readComp	Read a component in the application
	void dealReadCompResponse(unsigned char * cDataBuf, int iDataBufLen, SOX_READCOMP_RSP& objSoxReadCompRsp);
	void printSoxReadcompRsp(SOX_READCOMP_RSP& objSoxReadCompRsp);
	void releaseSoxReadcompRsp(SOX_READCOMP_RSP& objSoxReadCompRsp);
	//	d	delete	Delete an component and its children from the application
	void dealDeleteResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	e	event	COV event for a subscribed component
	//  void dealEventResponse(unsigned char * cDataBuf, int iDataBufLen);
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
	void dealQueryResponse(
		unsigned char * cDataBuf, int iDataBufLen, SOX_QUERYRES& objQueryRes);
	void printQueryResult(SOX_QUERYRES& objQueryRes);
	void releaseQueryResult(SOX_QUERYRES& objQueryRes);
	//	r	readProp	Read a single property from a component
	void dealReadPropResponse(
		unsigned char * cDataBuf, int iDataBufLen, SAB_PROP& objSabProp);
	//	s	subscribe	Subscribe to a component for COV events
	void dealSubscribeResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	u	unsubscribe	Unsubscribe from a component for COV events
	void dealUnsubscribeResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	v	version	Query for the kits installed
	void dealVersionResponse(unsigned char * cDataBuf, int iDataBufLen,
								  SCHEMA_KIT ** objSchemaKitList);
	//	w	write	Write the value of a single component property
	void dealWriteResponse(unsigned char * cDataBuf, int iDataBufLen);
	//	y	versionMore	Query for additional version meta-data
	void dealVersionMoreResponse(
		unsigned char * cDataBuf, int iDataBufLen, SOX_VERSIONMORE& objSoxVersionMore);
	void printVersionMore(SOX_VERSIONMORE& objSoxVersionMore);
	void releaseVersionMore(SOX_VERSIONMORE& objSoxVersionMore);
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
	void dealEventRequest(unsigned char * cDataBuf, int iDataBufLen
				, SOX_READCOMP_RSP& objSoxReadCompRsp);
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
	void setAndSkipUnsignedLongValueToBuf(unsigned char ** cBuffer,  unsigned _int64 uValue);

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
	SOX_READCOMP_RSP  m_objSoxReadCompRsp;
	// readProp:	Read a single property value from a component.
	// unsigned short    m_componentID;
	// unsigned char     m_propID;
	SAB_PROP          m_objSabProp;

	SCHEMA_KIT * m_objSchemaKitList;

	SOX_VERSIONMORE m_objSoxVersionMore;
	SOX_QUERYRES    m_queryRes;

	int             m_kitCount;
};


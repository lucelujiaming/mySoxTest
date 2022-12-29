#pragma once
#include "DASP.h"

#define URI_SUGGESTED_LEN                 1024          // 
#define SCODE_FILE_SUGGESTED_CHUNKSIZE    (200 * 1024)  // Normal size is 92K
#define APP_FILE_SUGGESTED_CHUNKSIZE      (4 * 1024)    // Normal size is 1-2K

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
	void dealSoxResponse(char * cDataBuf);

public:
	//	a	add	Add a new component to the application
	void sendAddCommand();
	//	b	fileRename	Rename or move a file
	void sendFileRenameCommand();
	//	c	readComp	Read a component in the application
	void sendReadCompCommand();
	//	d	delete	Delete an component and its children from the application
	void sendReleteCommand();
	//	e	event	COV event for a subscribed component
	void sendEventCommand();
	//	f	fileOpen	Begin a get or put file transfer operation
	void sendFileOpenCommand(char method, char * uri, 
								  unsigned int fileSize, 
								  unsigned short suggestedChunkSize, 
								  char * name, char * value);
	//	i	invoke	Invoke a component action
	void sendInvokeCommand();
	//	k	fileChunk	Receive or send a chunk during a file transfer
	void sendFileChunkCommand();
	//	l	link	Add or delete a link
	void sendLinkCommand();
	//	n	rename	Rename a component
	void sendRenameCommand();
	//	o	reorder	Reorder a component's children
	void sendReorderCommand();
	//	q	query	Query installed services
	void sendQueryCommand();
	//	r	readProp	Read a single property from a component
	void sendReadPropCommand();
	//	s	subscribe	Subscribe to a component for COV events
	void sendSubscribeCommand();
	//	u	unsubscribe	Unsubscribe from a component for COV events
	void sendUnsubscribeCommand();
	//	v	version	Query for the kits installed
	void sendVersionCommand();
	//	w	write	Write the value of a single component property
	void sendWriteCommand();
	//	y	versionMore	Query for additional version meta-data
	void sendVersionMoreCommand();
	//	z	fileClose	Close a file transfer operation
	void sendFileCloseCommand();
	//	!	error	Response id for a command that could not be processed
	void sendErrorCommand();
	//

public:
	CDASP m_objDASPMsg;
	
private:
	BOOL m_bExitRecvThread;
};


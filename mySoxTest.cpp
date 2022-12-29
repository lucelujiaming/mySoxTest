// mySoxTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SoxMsg.h"

int main(int argc, char* argv[])
{
	char cRet = 0 ;
	CSoxMsg objCSoxMsg ;
	objCSoxMsg.start();
	
	objCSoxMsg.handShake();
	objCSoxMsg.startRecvThread();

	objCSoxMsg.sendFileOpenCommand('g', "app.scode", 
								  SCODE_FILE_SUGGESTED_CHUNKSIZE, 
								  1024, 
								   "app.scode.name",  "app.scode.value");
	
	while (objCSoxMsg.getRecvThreadStatus() == FALSE)
	{
		Sleep(1);
	}

	objCSoxMsg.close();

	printf("Hello World!\n");
	return 0;
}


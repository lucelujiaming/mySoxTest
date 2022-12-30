// mySoxTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SoxMsg.h"
#include "SCodeReader.h"

int main(int argc, char* argv[])
{
	SCodeReader objSCodeReader;
	char cCurrentURI[MAX_BUFFER_LEN];
	strcpy(cCurrentURI, "./app.scode");
	char * cPos = strstr(cCurrentURI, "app.scode");
	if (cPos)
	{
		if(strcmp(cPos, "app.scode") == 0)
		{
			objSCodeReader.readSCodeFile(cCurrentURI);
		}
	}
/*
	char cRet = 0 ;
	CSoxMsg objCSoxMsg ;
	objCSoxMsg.start();
	
	objCSoxMsg.handShake();
	objCSoxMsg.startRecvThread();

	objCSoxMsg.sendFileOpenRequest('g', "./app.scode", 
								  SCODE_FILE_SUGGESTED_CHUNKSIZE, 
								  1024);
						//	  ,  "app.scode.name",  "app.scode.value");
	
	while (objCSoxMsg.getRecvThreadStatus() == FALSE)
	{
		Sleep(1);
	}

	objCSoxMsg.close();
*/
	printf("Hello World!\n");
	return 0;
}


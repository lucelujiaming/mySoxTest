// mySoxTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SoxMsg.h"
#include "SCodeReader.h"
#include "SabReader.h"

void testSCodeReader()
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
}

void testSabReader()
{
	SabReader objSabReader;
	char cCurrentURI[MAX_BUFFER_LEN];
	strcpy(cCurrentURI, "./app.sab");
	char * cPos = strstr(cCurrentURI, "app.sab");
	if (cPos)
	{
		if(strcmp(cPos, "app.sab") == 0)
		{
			objSabReader.readSabFile(cCurrentURI);
		}
	}
}

void testSoxMsg()
{
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
}

int main(int argc, char* argv[])
{
// 	testSabReader();
	testSCodeReader();
	// testSoxMsg();
	printf("Hello World!\n");
	return 0;
}


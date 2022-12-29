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

	objCSoxMsg.sendFileOpenCommand('g', "app.scode", 
								  SCODE_FILE_SUGGESTED_CHUNKSIZE, 
								  1024, 
								   "app.scode.name",  "app.scode.value");

	objCSoxMsg.close();

	printf("Hello World!\n");
	return 0;
}


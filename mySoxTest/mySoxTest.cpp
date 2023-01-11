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
			objSCodeReader.releaseSCodeFileBuffer();
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
			bool bRet = objSabReader.loadSCodeFile("./app.scode");
			if (bRet)
			{
				objSabReader.readSabFile(cCurrentURI);
				// We can use Sab info now.
				// After we finish the operation, we should release memory of SCode and Sab File
				objSabReader.unloadSCodeFile();
				objSabReader.releaseSabFileBuffer();
				return ;
			}
		}
	}
}

void testSoxMsg()
{
	char cRet = 0 ;
	CSoxMsg objCSoxMsg ;
	objCSoxMsg.loadSabFile("./app.sab");
	objCSoxMsg.start();
	
	objCSoxMsg.handShake();
	objCSoxMsg.startRecvThread();

	SOX_PROP objSoxProp[2];
	objSoxProp[0].uRet    = 1;
	objSoxProp[1].uBufLen = 1;
	memset(objSoxProp[1].cBuf, 0x00, PROP_VALUE_LEN);
	objCSoxMsg.sendAddRequest(7, 11, 0, "AliasVa", objSoxProp, 2);

//	objCSoxMsg.sendFileOpenRequest('g', "./app.scode", 
//								  SCODE_FILE_SUGGESTED_CHUNKSIZE, 
//								  1024);
//						//	  ,  "app.scode.name",  "app.scode.value");
	
	while (objCSoxMsg.getRecvThreadStatus() == FALSE)
	{
		Sleep(1);
	}

	objCSoxMsg.close();
}

void test_copy_buf(unsigned char ** buf)
{
	strcpy((char *)*buf, "12345");
	*buf += 5;
}

void test_buf()
{
	unsigned short parentID = 7;
	unsigned char cTest[100];
	memset(cTest, 0x00, 100);
	unsigned char *cTestPtr = cTest;
	
	CSoxMsg objCSoxMsg ;
	objCSoxMsg.setAndSkipUnsignedShortValueToBuf(&cTestPtr, parentID);
	objCSoxMsg.setAndSkipUnsignedShortValueToBuf(&cTestPtr, parentID);
	objCSoxMsg.setAndSkipUnsignedShortValueToBuf(&cTestPtr, parentID);
	objCSoxMsg.setAndSkipUnsignedShortValueToBuf(&cTestPtr, parentID);
//	test_copy_buf(&cTestPtr);
//	test_copy_buf(&cTestPtr);
//	test_copy_buf(&cTestPtr);


}

int main(int argc, char* argv[])
{
	// test_buf();

	// // testSCodeReader();
 	// testSabReader();
	testSoxMsg();
	printf("Hello World!\n");
	return 0;
}


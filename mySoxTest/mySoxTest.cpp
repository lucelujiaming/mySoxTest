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

	SAB_PROP_VALUE objSoxProp[3];
	memset(objSoxProp, 0x00, sizeof(SAB_PROP_VALUE) * 3);
	objSoxProp[0].uRet    = 705438738;  // 0x12242219
	
	strcpy(objSoxProp[1].cBuf, "UUUUU");
	// NOTICE: This uBufLen cannot be less than cBuf, 
	//         otherwise we will crash the sab file
	objSoxProp[1].uBufLen = strlen(objSoxProp[1].cBuf) + 1;
	strcpy(objSoxProp[2].cBuf, "FFFFF");
	objSoxProp[2].uBufLen = strlen(objSoxProp[2].cBuf) + 1;

	// You have to change the name in the next time because of duplication.
	objCSoxMsg.sendAddRequest(7, 11, 10, "UU", objSoxProp, 3);
	objCSoxMsg.sendAddRequest(7, 11, 10, "RR", objSoxProp, 3);
	// objCSoxMsg.sendReadCompRequest(7, 't');
	// objCSoxMsg.sendReadCompRequest(11, 'r');
	// objCSoxMsg.sendReadCompRequest(11, 'l');
	// objCSoxMsg.sendReadPropRequest(11, 2);
	// objCSoxMsg.sendVersionRequest();
	// objCSoxMsg.sendVersionMoreRequest();

	// objCSoxMsg.sendQueryRequest('s', 0, 15);  // sys::User
	
//	SAB_PROP_VALUE objInvokeArgument;
//	objInvokeArgument.uBufLen = 0;
//	objCSoxMsg.sendInvokeRequest(0, 2, objInvokeArgument);

//	objCSoxMsg.sendFileOpenRequest('g', "./app.sab", 
//								  SCODE_FILE_SUGGESTED_CHUNKSIZE, 
//								  1024);

//						//	  ,  "app.scode.name",  "app.scode.value");
	int i = 0;
	while (objCSoxMsg.getRecvThreadStatus() == FALSE)
	{
		Sleep(1);
		i++;
		if(i == 1000)
		{
			objCSoxMsg.sendFileOpenRequest('g', "./app.sab", 
										  SCODE_FILE_SUGGESTED_CHUNKSIZE, 
										  1024);
			i = 0;
		}
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

	// testSCodeReader();
 	// //  testSabReader();
	testSoxMsg();
	printf("Hello World!\n");
	return 0;
}


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
	objCSoxMsg.loadSabFile("./app_fixlink.sab");
	objCSoxMsg.start();
	
	objCSoxMsg.handShake();
	objCSoxMsg.startRecvThread();

	SAB_PROP_VALUE objSoxProp[18];
	objSoxProp[0].propId = 0;
	objSoxProp[0].propType = 4;
	objSoxProp[0].uBufLen = 4;
	objSoxProp[0].uRet = 0x01250001;

	objSoxProp[1].propId = 0;
	objSoxProp[1].propType = 8;
	strcpy(objSoxProp[1].cBuf, "Url");
	objSoxProp[1].uBufLen = strlen(objSoxProp[1].cBuf) + 1;

	objSoxProp[2].propId = 0;
	objSoxProp[2].propType = 4;
	objSoxProp[2].uRet = 2;
	objSoxProp[2].uBufLen = 4;

	objSoxProp[3].propId = 0;
	objSoxProp[3].propType = 4;
	objSoxProp[3].uRet = 1000;
	objSoxProp[3].uBufLen = 4;

	for (int i = 4; i < 16; i++)
	{
		objSoxProp[i].propId = 0;
		objSoxProp[i].propType = 4;
		objSoxProp[i].uRet = i+1;
		objSoxProp[i].uBufLen = 4;
	}
	
	objSoxProp[16].propId = 0;
	objSoxProp[16].propType = 8;
	strcpy(objSoxProp[16].cBuf, "Alias");
	objSoxProp[16].uBufLen = strlen(objSoxProp[16].cBuf) + 1;
	
	objCSoxMsg.sendAddRequest(7, 11, 2, "AddT5", objSoxProp, 18);
	
	Sleep(500);
	SAB_PROP_VALUE objInvokeArgument;
	objInvokeArgument.uBufLen = 0;
	objCSoxMsg.sendInvokeRequest(0, 1, objInvokeArgument);

//			SAB_PROP_VALUE objSoxProp[3];
//			memset(objSoxProp, 0x00, sizeof(SAB_PROP_VALUE) * 3);
//			objSoxProp[0].uRet    = 705438738;  // 0x12242219
//			
//			strcpy(objSoxProp[1].cBuf, "UUUUU");
//			// NOTICE: This uBufLen cannot be less than cBuf, 
//			//         otherwise we will crash the sab file
//			objSoxProp[1].uBufLen = strlen(objSoxProp[1].cBuf) + 1;
//			strcpy(objSoxProp[2].cBuf, "FFFFF");
//			objSoxProp[2].uBufLen = strlen(objSoxProp[2].cBuf) + 1;
//
//			// You have to change the name in the next time because of duplication.
//			objCSoxMsg.sendAddRequest(7, 11, 10, "55", objSoxProp, 0);

	// objCSoxMsg.sendAddRequest(7, 11, 10, "RR", objSoxProp, 1);
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
	int k = 0;
	while (objCSoxMsg.getRecvThreadStatus() == FALSE)
	{
		Sleep(1);
		k++;
		if(k == 1000)
		{
			objCSoxMsg.sendFileOpenRequest('g', "./app.sab", 
										  SCODE_FILE_SUGGESTED_CHUNKSIZE, 
										  1024);
			k = 0;
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
	// testSoxMsg();
	CSoxMsg objCSoxMsg ;
	objCSoxMsg.loadSabFile("./app_fixlink.sab");
	printf("Hello World!\n");
	return 0;
}


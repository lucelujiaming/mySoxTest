// SCodeReader.cpp: implementation of the SCodeReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include "SCodeReader.h"
#include "SCode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define HEAD_MAGIC_BIG_ENDIAN_FIRST       0x5E
#define HEAD_MAGIC_BIG_ENDIAN_SECOND      0xD0
#define HEAD_MAGIC_BIG_ENDIAN_THIRD       0xBA
#define HEAD_MAGIC_BIG_ENDIAN_FOURTH      0x07

#define HEAD_MAGIC_LITTLE_ENDIAN_FIRST       0x07
#define HEAD_MAGIC_LITTLE_ENDIAN_SECOND      0xBA
#define HEAD_MAGIC_LITTLE_ENDIAN_THIRD       0xD0
#define HEAD_MAGIC_LITTLE_ENDIAN_FOURTH      0x5E

SCodeReader::SCodeReader()
{
	// svm use little endian
	setLittleEndian();
	m_fileBuf = NULL;
	m_scode_log_veclist.reserve(8);
	m_scode_kit_list = NULL;
	m_scode_test_table = NULL;
}

SCodeReader::~SCodeReader()
{
	if(m_fileBuf)
	{
		free(m_fileBuf);
		m_fileBuf = NULL;
	}
}

unsigned short SCodeReader::calcAndSkipUnsignedShortValue(unsigned char ** cBuf)
{
	unsigned short uValue = 0x00;
	if (m_bBigEndian)
	{
		uValue        = (unsigned char)(*cBuf)[0];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuf)[1];
	}
	else 
	{
		uValue       += (unsigned char)(*cBuf)[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuf)[0];
	}
	(*cBuf) += 2;
	return uValue;
}

unsigned int SCodeReader::calcAndSkipUnsignedIntValue(unsigned char ** cBuf)
{
	unsigned int uValue = 0x00;
	if (m_bBigEndian)
	{
		uValue        = (unsigned char)(*cBuf)[0];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuf)[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuf)[2];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuf)[3];
	}
	else 
	{
		uValue        = (unsigned char)(*cBuf)[3];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuf)[2];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuf)[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)(*cBuf)[0];
	}
	(*cBuf) += 4;
	return uValue;
}

unsigned long  SCodeReader::calcAndSkipUnsignedLongValue(unsigned char ** cBuf)
{
	unsigned long uValue = 0x00;
	unsigned int uValueOne = 0x00;
	uValueOne = calcAndSkipUnsignedIntValue(cBuf);
	unsigned int uValueTwo = 0x00;
	uValueTwo = calcAndSkipUnsignedIntValue(cBuf);
	
	if (m_bBigEndian)
	{
		uValue = uValueOne * 0x100000000 + uValueTwo;
	}
	else 
	{
		uValue = uValueTwo * 0x100000000 + uValueOne;
	}
	return uValue;
}

void SCodeReader::printSCodeHeader()
{
	printf("m_head_magic[0] = %02X \r\n", m_head_magic[0]);
	printf("m_head_magic[1] = %02X \r\n", m_head_magic[1]);
	printf("m_head_magic[2] = %02X \r\n", m_head_magic[2]);
	printf("m_head_magic[3] = %02X \r\n", m_head_magic[3]);
	printf("m_head_majorVer = %d \r\n", m_head_majorVer);
	printf("m_head_minorVer = %d \r\n", m_head_minorVer);
	
	printf("m_head_blockSize = %d \r\n", m_head_blockSize);
	printf("m_head_refSize = %d \r\n", m_head_refSize);
	printf("m_head_imageSize = %04X \r\n", m_head_imageSize);
	printf("m_head_dataSize = %04X \r\n", m_head_dataSize);

	printf("m_head_mainMethodBlockIndex = %02X \r\n", m_head_mainMethodBlockIndex);
	printf("m_head_testTable = %02X \r\n", m_head_testBlockIndex);
	printf("m_head_kitsArrayBlockIndex = %02X \r\n", m_head_kitsArrayBlockIndex);
	printf("m_head_numberOfKits = %02X \r\n", m_head_numberOfKits);
	printf("m_head_scodeFlags   = %02X \r\n", m_head_scodeFlags);
}


void SCodeReader::setSCodeHeader()
{
	memcpy(m_head_magic, m_cFileBufPtr, 4);
	m_cFileBufPtr += 4;
	if ((m_head_magic[0] == HEAD_MAGIC_BIG_ENDIAN_FIRST ) &&
		(m_head_magic[1] == HEAD_MAGIC_BIG_ENDIAN_SECOND) &&
		(m_head_magic[2] == HEAD_MAGIC_BIG_ENDIAN_THIRD ) &&
		(m_head_magic[3] == HEAD_MAGIC_BIG_ENDIAN_FOURTH))
	{
		setBigEndian();
	}
	else if ((m_head_magic[0] == HEAD_MAGIC_LITTLE_ENDIAN_FIRST ) &&
	    	 (m_head_magic[1] == HEAD_MAGIC_LITTLE_ENDIAN_SECOND) &&
	    	 (m_head_magic[2] == HEAD_MAGIC_LITTLE_ENDIAN_THIRD ) &&
	    	 (m_head_magic[3] == HEAD_MAGIC_LITTLE_ENDIAN_FOURTH))
	{
		setLittleEndian();
	}
	m_head_majorVer    =  m_cFileBufPtr[0];
	m_head_minorVer    =  m_cFileBufPtr[1];
	m_head_blockSize   =  m_cFileBufPtr[2];
	m_head_refSize     =  m_cFileBufPtr[3];
	m_cFileBufPtr += 4;
	
	m_head_imageSize        = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);
	m_head_dataSize         = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);

	m_head_mainMethodBlockIndex        = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
	m_head_testBlockIndex              = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
	m_head_kitsArrayBlockIndex         = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);

	m_head_numberOfKits =  (unsigned char)m_cFileBufPtr[0];
	m_head_scodeFlags   =  (unsigned char)m_cFileBufPtr[1];
	
	m_head_debugFlag    = (m_head_scodeFlags & SCODE_DEBUG_FLAG)!=0;
	m_head_testFlag     = (m_head_scodeFlags & SCODE_TEST_FLAG)!=0;
	m_cFileBufPtr += 2;
}

//	**
//	** Return if this slot is a property against the specified
//	** filter:
//	**    '*' = any property
//	**    'c' = only config properties
//	**    'r' = only runtime properties
//	**    'C' = only operator level config properties
//	**    'R' = only operator level runtime properties
//	**
bool SCodeReader::matchRtFlagsProp(unsigned char filter, unsigned int rtFlags)
{
	if (isRtFlagsProp(rtFlags))
	{
		if (filter == RTFLAGS_FILTER_ANY) 
			return true;
		if (filter == RTFLAGS_FILTER_CONFIG) 
			return (rtFlags & RTFLAGS_CONFIG) != 0;
		if (filter == RTFLAGS_FILTER_RUNTIME) 
			return (rtFlags & RTFLAGS_CONFIG) == 0;
		if (filter == RTFLAGS_FILTER_OPERATOR_CONFIG) 
			return (rtFlags & (RTFLAGS_CONFIG|RTFLAGS_OPERATOR)) == (RTFLAGS_CONFIG|RTFLAGS_OPERATOR);
		if (filter == RTFLAGS_FILTER_OPERATOR_RUNTIME) 
			return (rtFlags & (RTFLAGS_CONFIG|RTFLAGS_OPERATOR)) == RTFLAGS_OPERATOR;
	}
	return false;
}

void SCodeReader::generateRtFlagsInfo(char * cRtFlagsInfo, int rtFlags)
{
	if((rtFlags > RTFLAGS_MAX) |(rtFlags <= 0))
	{
		sprintf(cRtFlagsInfo, " ERROR ");
		return;
	}
	if(rtFlags & RTFLAGS_ACTION)
	{
		sprintf(cRtFlagsInfo, " ACTION |");
	}
	if(rtFlags & RTFLAGS_CONFIG)
	{
		sprintf(cRtFlagsInfo, "%s CONFIG |", cRtFlagsInfo);
	}
	if(rtFlags & RTFLAGS_AS_STR)
	{
		sprintf(cRtFlagsInfo, "%s AS_STR |", cRtFlagsInfo);
	}
	if(rtFlags & RTFLAGS_OPERATOR)
	{
		sprintf(cRtFlagsInfo, "%s OPERATOR |", cRtFlagsInfo);
	}
	// Remove last '|'
	if (strlen(cRtFlagsInfo) > 0)
	{
		cRtFlagsInfo[strlen(cRtFlagsInfo) - 1] = '\0';
	}

}

void SCodeReader::printSCodeKits()
{
	for (int i=0; i< m_head_numberOfKits; ++i) 
    {
		printf("SCode::kit[%d].(id, typesLen, cName, cVer, pad, checksum) "
			"  = (%02d, %02d, %s, %s, %d, %04X) \r\n", i, 
			m_scode_kit_list[i].id, m_scode_kit_list[i].typesLen,
			m_scode_kit_list[i].cName, m_scode_kit_list[i].cVer,
			m_scode_kit_list[i].pad, m_scode_kit_list[i].checksum);

		for (int j=0; j < m_scode_kit_list[i].typesLen; j++)
		{
			printf("\t SCode::kit[%d].type[%d].(id, slotsLen, nameBix, cName, kitBix, baseBix, size, initBix) "
				"  = (%02d, %02d, %02d, %s, %d, %d, %04X) \r\n", i, j, 
				m_scode_kit_list[i].kit_type_list[j].id, 
				m_scode_kit_list[i].kit_type_list[j].slotsLen,
				m_scode_kit_list[i].kit_type_list[j].nameBix,
				m_scode_kit_list[i].kit_type_list[j].cName, 
				m_scode_kit_list[i].kit_type_list[j].kitBix,
				m_scode_kit_list[i].kit_type_list[j].baseBix,
				m_scode_kit_list[i].kit_type_list[j].size,
				m_scode_kit_list[i].kit_type_list[j].initBix);
			
			for (int k=0; k < m_scode_kit_list[i].kit_type_list[j].slotsLen; k++)
			{
				char cRtFlagsInfo[SCODE_NAME_LEN];
				memset(cRtFlagsInfo, 0x00, SCODE_NAME_LEN);
				generateRtFlagsInfo(cRtFlagsInfo, m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].rtFlags);
				printf("\t\t SCode::kit[%d].type[%d].slot[%d].(id, rtFlags, nameBix, cPropName, fpBix, codeBix) "
					"  = (%02d, %02d(%s), %02d, %s, %02X, %02X) \r\n", i, j, k, 
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].id, 
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].rtFlags, cRtFlagsInfo,
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].nameBix,
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].cPropName, 
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].fpBix,
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].codeBix);
			}
		}
    }
}

void SCodeReader::printSCodeLogs()
{
	std::vector<SCODE_LOG>::iterator iter;
	for (iter = m_scode_log_veclist.begin(); iter != m_scode_log_veclist.end(); iter++)
	{
		printf("SCode::Log(id, nameBix, cName) = (%02d, %02d, %s) \r\n", 
			iter->id, iter->nameBix, iter->cName);
	}
}

void SCodeReader::printMethods()
{
	std::vector<SCODE_METHOD>::iterator iter;
	for (iter = m_scode_method_veclist.begin(); iter != m_scode_method_veclist.end(); iter++)
	{
		printMethod(*iter, true);
	}
}

void SCodeReader::printMethod(SCODE_METHOD objMethod, bool bPrintOpCodeInfo)
{
	printf("\tSCode::Method(numParams, numLocals) = (%02d, %02d) ", 
		objMethod.numParamsOpCode, objMethod.numLocalsOpCode);
	if (bPrintOpCodeInfo)
	{
		printf("and SCode::Method content is \r\n");
		std::vector<std::string>::iterator iter;
		for (iter = objMethod.opcodeInfo.begin(); iter != objMethod.opcodeInfo.end(); iter++)
		{

			printf("\t\t%s\r\n", iter->c_str());
		}
	}
	else
	{
		printf(". \r\n");

	}
}

void SCodeReader::printTests()
{
	for (int i=0; i< m_numTests; ++i) 
    {
		printf("SCode::Test(slotBix, codeBix) = (%02d, %02d) and (%s:%s.%s) \r\n", 
			m_scode_test_table[i].slotBix, m_scode_test_table[i].codeBix,
			m_scode_test_table[i].qnameSlot.typeName, 
			m_scode_test_table[i].qnameSlot.slotName, 
			m_scode_test_table[i].qnameSlot.methodName);
		printMethod(m_scode_test_table[i].method, true);
	}
}

bool SCodeReader::readSCodeFile(char * cFileName)
{
	m_sysKitBix = -1;
	m_endTypeBix = -1;
	m_endSlotBix = -1;
	m_endLogBix = -1;
	int iRet = m_objFileReader.readSCodeFileBuffer(&m_fileBuf, cFileName);
	if (iRet == 0)
	{
		return false;
	}
	m_cFileBufPtr = m_fileBuf;
	setSCodeHeader();
	printSCodeHeader();

	parseBootstrap();
	if (m_head_numberOfKits > 0)
	{
		m_scode_kit_list = (SCODE_KIT *)malloc(sizeof(SCODE_KIT) * m_head_numberOfKits);
		memset(m_scode_kit_list, 0x00, sizeof(SCODE_KIT) * m_head_numberOfKits);
		
		for (int i=0; i< m_head_numberOfKits; ++i) 
		{
			unsigned int iPos = m_head_kitsArrayBlockIndex *  m_head_blockSize + i * 2;
			m_cFileBufPtr = m_fileBuf + iPos;
			iPos = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
			parseKit(m_scode_kit_list[i], iPos);
		}
		printSCodeKits();
	}

	parseLogs(m_endSlotBix);
	printSCodeLogs();
	parseMethods(m_endLogBix);
	printMethods();
		
    if (m_head_testBlockIndex > 0)
    {
		// parseTests();
		// printTests();
	}
	m_objFileReader.freeSCodeFileBuffer(&m_fileBuf);
	return true;
}

void SCodeReader::releaseSCodeFileBuffer()
{
	if (m_scode_kit_list)
	{
		for (int i=0; i< m_head_numberOfKits; ++i) 
		{
			for (int j=0; j < m_scode_kit_list[i].typesLen; j++)
			{
				if (m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list)
				{
					free(m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list);
					// m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list = NULL;
				}
			}
			if (m_scode_kit_list[i].kit_type_list)
			{
				free(m_scode_kit_list[i].kit_type_list);
				// m_scode_kit_list[i].kit_type_list = NULL;
			}
		}
		free(m_scode_kit_list);
		m_scode_kit_list = NULL;
	}
	// Release Logs
	m_scode_log_veclist.clear();
	// Release Methods
	m_scode_method_veclist.clear();
	if (m_scode_test_table)
	{
		free(m_scode_test_table);
		m_scode_test_table = NULL;
	}
}

void SCodeReader::parseLogs(unsigned int logTabBix)
{
	SCODE_LOG objSCodeLog;
    int prev_id = -2;
    int id = -1;
    unsigned char * savePos = m_cFileBufPtr;

    // If logTabBix<0 then buf.pos already at correct position
    if (logTabBix>0) 
	{
		m_cFileBufPtr = m_fileBuf + logTabBix * m_head_blockSize;
	}

	// Stupid heuristic to find end of log table: when ids stop being consecutive
	// (assumes we start at 0)
	while (id == prev_id+1)
	{
		memset(&objSCodeLog, 0x00, sizeof(objSCodeLog));
		prev_id = id;

		objSCodeLog.id           = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		objSCodeLog.nameBix      = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		parseStr(objSCodeLog.cName, objSCodeLog.nameBix);

		if (id == prev_id+1)   // if consecutive, assume it's a valid log entry
		{
			//	System.out.print("\nLog " + id);
			//	System.out.println("\tName = " + name + "  NameBix = 0x" + tHS(nameBix));
		}
		m_scode_log_veclist.push_back(objSCodeLog);
    } 

    m_endLogBix = ((m_cFileBufPtr - m_fileBuf) + (m_head_blockSize-1)) / m_head_blockSize;

	m_cFileBufPtr = savePos;
}

int SCodeReader::parseMethod(SCODE_METHOD& objMethod, int codeBix, bool bRestorePos)
{
    int       opCode;
    char      opCodeInfo[SCODE_ARG_LEN];
	// char      argStr[SCODE_ARG_LEN];
	SCODE_CODE_ARG objSCodeCodeArg ;
    unsigned char * savePos = m_cFileBufPtr;
	// If codeBix<0 then buf.pos already at correct position
    if (codeBix > 0) 
	{
		m_cFileBufPtr = m_fileBuf + codeBix * m_head_blockSize;
	}
	else
	{
		codeBix = (m_cFileBufPtr - m_fileBuf) / m_head_blockSize;
	}

    // This represents the maximum dest addr jumped to within the method
    // (so we know which Return opcode is really the end)
    int maxjmp = (m_cFileBufPtr - m_fileBuf); 

    // Print num params & locals like opcodes
    // System.out.println("\nParsing method at bix = 0x" + tHS(codeBix));

    objMethod.numParamsOpCode = m_cFileBufPtr[0];
	m_cFileBufPtr ++;
    // System.out.println( opFormat(opPos, opCode, "Num params") + opCode );

    objMethod.numLocalsOpCode = m_cFileBufPtr[0];
	m_cFileBufPtr ++;
    // System.out.println( opFormat(opPos, opCode, "Num locals") + opCode );

	    // Stop at first Return type opcode, unless method includes a jump past that point
    bool bDone = false;
    //
    // Step through the opcodes and args until end of method
    //
    while (!bDone) 
    {
		int opPos = m_cFileBufPtr - m_fileBuf;
		memset(opCodeInfo, 0x00, SCODE_ARG_LEN);
		memset(&objSCodeCodeArg, 0x00, sizeof(SCODE_CODE_ARG));
		// Get next opcode
		opCode = m_cFileBufPtr[0];
		m_cFileBufPtr ++;
		sprintf(opCodeInfo, "%04X %02X %s ", (m_cFileBufPtr - m_fileBuf),
			opCode, get_scode_name(opCode));
		// Print opcode & name
		// System.out.print( opFormat(opPos, opCode, SCode.name(opCode)) );

		int iRet = getArgString(objSCodeCodeArg, opCode);
		if (iRet == -1)
		{
			strcat(opCodeInfo, objSCodeCodeArg.argPrintStr);
			objMethod.opcodeInfo.push_back(std::string(opCodeInfo));
			return -1;
		}
		strcat(opCodeInfo, objSCodeCodeArg.argPrintStr);

		objMethod.opcodeInfo.push_back(std::string(opCodeInfo));
		// printf("\tobjMethod.opcodeInfo.size() = %d and add %s . \r\n", 
		//  	objMethod.opcodeInfo.size(), opCodeInfo);

//		if(m_scode_method_veclist.size() >= 1273)
//		{
//			if(objMethod.opcodeInfo.size() == 64)
//			{
//				printf("AAA");
//			}
//		}
		// Quit when we reach the first return, unless there's a jump that
		// takes us past it (not sure this is sufficient test?)
		switch (opCode)
		{
			case g_scode_Call:   
				// calledMethods.add(argStr);
				break;
			case g_scode_Jump:
			case g_scode_JumpNonZero:
			case g_scode_JumpZero:      
			case g_scode_JumpFar:
			case g_scode_JumpFarNonZero:
			case g_scode_JumpFarZero:   
			case g_scode_JumpIntEq:
			case g_scode_JumpIntNotEq:
			case g_scode_JumpIntGt:
			case g_scode_JumpIntGtEq:
			case g_scode_JumpIntLt:
			case g_scode_JumpIntLtEq:
			case g_scode_JumpFarIntEq:
			case g_scode_JumpFarIntNotEq:
			case g_scode_JumpFarIntGt:
			case g_scode_JumpFarIntGtEq:
			case g_scode_JumpFarIntLt:
			case g_scode_JumpFarIntLtEq:  // update maxjmp as needed
				if (atoi(objSCodeCodeArg.argIntStr) > 500)
				{
					if(strstr(opCodeInfo, "JumpFar") == NULL)
					{
					    printf("We add maxjmp with %d .", atoi(objSCodeCodeArg.argIntStr));
					}
				}
				if (opPos + atoi(objSCodeCodeArg.argIntStr) > maxjmp)
					maxjmp = opPos + atoi(objSCodeCodeArg.argIntStr);
				break;

			case g_scode_ReturnVoid:
			case g_scode_ReturnPop:
			case g_scode_ReturnPopWide:  
				if ((m_cFileBufPtr - m_fileBuf) > maxjmp) 
					bDone = true;  
				break;
			default: 
				break;
		}

	}  // while (!bDone)
	
	if (bRestorePos)
	{
		m_cFileBufPtr = savePos;
	}
	return 0;
}

int SCodeReader::getArgString(SCODE_CODE_ARG& objSCodeCodeArg, int op)
{     
	unsigned short uShortVal = 0;
	unsigned int   uIntVal   = 0;
	unsigned long  uLongVal  = 0;
    // Find width of opcode's arg and read it in.
    // Return string showing arg value (incl 0x prefix for hex),
    // or null if no arg.

    // For now, keep it simple - just get hex value of arg bytes
    switch (scode_argType(op))
    {
      // signed 1-byte arg
      case g_scode_jmpArg    : 
		  // argStr = String.valueOf(buf.s1());
		  sprintf(objSCodeCodeArg.argIntStr, "%d", m_cFileBufPtr[0]);
		  sprintf(objSCodeCodeArg.argPrintStr, "%d", m_cFileBufPtr[0]);
		  m_cFileBufPtr++;
          break;
      // unsigned 1-byte arg
      case g_scode_u1Arg     : 
		  // argStr = "0x" + tHS(buf.u1());
		  sprintf(objSCodeCodeArg.argIntStr, "%d", m_cFileBufPtr[0]);
		  sprintf(objSCodeCodeArg.argPrintStr, "0x%02X", m_cFileBufPtr[0]);
          break;
      // signed 2-byte arg 
      case g_scode_jmpfarArg : 
		  // argStr = String.valueOf(buf.s2());
		  uShortVal = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		  sprintf(objSCodeCodeArg.argIntStr, "%d", uShortVal);
		  sprintf(objSCodeCodeArg.argPrintStr, "0x%02X", uShortVal);
          break;
      // unsigned 2-byte arg 
      case g_scode_strArg    :
      case g_scode_bufArg    :
      case g_scode_typeArg   :
      case g_scode_slotArg   :
      case g_scode_u2Arg     : 
		  // argStr = "0x" + tHS(u2aligned(buf));
		  uShortVal = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		  sprintf(objSCodeCodeArg.argIntStr, "%d", uShortVal);
		  sprintf(objSCodeCodeArg.argPrintStr, "0x%02X", uShortVal);
          break;
      // unsigned 2-byte arg, not necessarily aligned
      case g_scode_methodArg : 
		  // argStr = "0x" + tHS(buf.u2());
		  uShortVal = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		  sprintf(objSCodeCodeArg.argIntStr, "%d", uShortVal);
		  sprintf(objSCodeCodeArg.argPrintStr, "0x%02X", uShortVal);
          break;
      // signed 4-byte arg
      case g_scode_intArg    : 
		  // argStr = String.valueOf(i4aligned(buf));
		  uIntVal = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);
		  sprintf(objSCodeCodeArg.argIntStr, "%d", uIntVal);
		  sprintf(objSCodeCodeArg.argPrintStr, "%04d", uIntVal);
          break;
      case g_scode_floatArg  : 
		  // argStr = String.valueOf(f4aligned(buf));
		  uIntVal = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);
		  // ??????
		  sprintf(objSCodeCodeArg.argIntStr, "%f", intBitsToFloat(uIntVal));
		  sprintf(objSCodeCodeArg.argPrintStr, "%f", intBitsToFloat(uIntVal));
          break;
      // unsigned 4-byte arg
      case g_scode_s4Arg     : 
		  // argStr = "0x" + tHS(i4aligned(buf));
		  uIntVal = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);
		  sprintf(objSCodeCodeArg.argIntStr, "%d", uIntVal);
		  sprintf(objSCodeCodeArg.argPrintStr, "%04X", uIntVal);
          break;
      // unsigned 8-byte arg
      case g_scode_longArg   : 
		  // argStr = "0x" + java.lang.Long.toHexString(i8aligned(buf));
		  uLongVal = calcAndSkipUnsignedLongValue(&m_cFileBufPtr);
		  sprintf(objSCodeCodeArg.argIntStr, "%lld", uLongVal);
		  sprintf(objSCodeCodeArg.argPrintStr, "%08lX", uLongVal);
          break;
      // signed 8-byte arg
      case g_scode_doubleArg : 
		  // argStr = String.valueOf(f8aligned(buf)); 
		  uLongVal = calcAndSkipUnsignedLongValue(&m_cFileBufPtr);
		  sprintf(objSCodeCodeArg.argIntStr, "%f", longBitsToDouble(uLongVal));
		  sprintf(objSCodeCodeArg.argPrintStr, "%f", longBitsToDouble(uLongVal));
          break;

      // switch has multiple args, first is #args to follow
      case g_scode_switchArg : 
		  {
			  int nArgs = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
			  if(nArgs > 128)
			  {
				  printf("ERROR: g_scode_switchArg::nArgs = %d \r\n", nArgs);
				  return -1;
			  }
			  sprintf(objSCodeCodeArg.argIntStr, "%d", nArgs);
			  sprintf(objSCodeCodeArg.argPrintStr, "(%d) ", nArgs);
			  for (int k = 0; k < nArgs; k++) 
			  {
				  if (k > 0) 
					  sprintf(objSCodeCodeArg.argPrintStr + strlen(objSCodeCodeArg.argPrintStr), ", ");
				  // argStr += "0x" + tHS(u2aligned(buf));
				  sprintf(objSCodeCodeArg.argPrintStr + strlen(objSCodeCodeArg.argPrintStr), 
					  "0x%X", calcAndSkipUnsignedShortValue(&m_cFileBufPtr));
			  }
		  }
		  break;
      // field arg (varies with opcode, usually unsigned)
      case g_scode_fieldArg  : 
		  {
			  const char * retName = get_scode_name(op);
			  unsigned char endChar = retName[strlen(retName) - 1];
			  if (endChar == '1')
			  {
				  // argStr = "0x" + tHS(buf.u1());
				  sprintf(objSCodeCodeArg.argIntStr, "%d", m_cFileBufPtr[0]);
				  sprintf(objSCodeCodeArg.argPrintStr, "0x%X", m_cFileBufPtr[0]);
				  m_cFileBufPtr++;
			  }
			  else if (endChar == '2')
			  {
				  // argStr = "0x" + tHS(u2aligned(buf));
				  uShortVal = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
				  sprintf(objSCodeCodeArg.argIntStr, "%d", uShortVal);
				  sprintf(objSCodeCodeArg.argPrintStr, "0x%02X", uShortVal);
			  }
			  else if (endChar == '4')
			  {
				  // argStr = "0x" + tHS(i4aligned(buf));
				  uIntVal = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);
				  sprintf(objSCodeCodeArg.argIntStr, "%d", uIntVal);
				  sprintf(objSCodeCodeArg.argPrintStr, "%04d", uIntVal);
			  }
			  else 
			  {
				  // argStr = "0x" + tHS(u2aligned(buf));  // width of "Const Static" arg?
				  uShortVal = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
				  sprintf(objSCodeCodeArg.argIntStr, "%d", uShortVal);
				  sprintf(objSCodeCodeArg.argPrintStr, "0x%02X", uShortVal);
			  }
			  break;
		  }
      // no arg
      case g_scode_noArg     : 
      default              : 
		  return 0;
    }

    return 1;
}

void SCodeReader::parseMethods(unsigned int methodTabBix)
{
	SCODE_METHOD  objMethod;
	unsigned char * cImageEnd = NULL;
	// If methodTabBix<0 then buf.pos already at correct position
    if (methodTabBix > 0) 
	{
		m_cFileBufPtr = m_fileBuf + methodTabBix * m_head_blockSize;
	}
	cImageEnd = m_fileBuf + m_head_imageSize;
	m_scode_method_veclist.reserve(102400);
    // Not sure how to tell we're done???
    while (m_cFileBufPtr < cImageEnd)
    {
		objMethod.opcodeInfo.reserve(102400);
		objMethod.opcodeInfo.clear();
//		if(m_scode_method_veclist.size() == 1273)
//		{
//			printf("AAA");
//		}
		int iRet = parseMethod(objMethod, -1, false);
		if (iRet == -1)
		{
			break;
		}
		// printf("\tm_scode_method_veclist.size() = %d at %d (%08X, %08X). \r\n", 
		//	m_scode_method_veclist.size(), cImageEnd - m_cFileBufPtr, cImageEnd, m_cFileBufPtr);
		m_scode_method_veclist.push_back(objMethod);
    }
	printf("m_scode_method_veclist.size() = %d at %d (%08X, %08X). \r\n", 
		m_scode_method_veclist.size(), cImageEnd - m_cFileBufPtr, cImageEnd, m_cFileBufPtr);
}

void SCodeReader::parseTests()
{
	m_cFileBufPtr = m_fileBuf + m_head_testBlockIndex * m_head_blockSize;
	m_numTests = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);

    if (m_numTests > 0)
	{
		m_scode_test_table = (SCODE_TEST *)malloc(sizeof(SCODE_TEST) * m_numTests);
		memset(m_scode_test_table, 0x00, sizeof(SCODE_TEST) * m_numTests);

		// traverse the pointers for each test in table
		for (int i=0; i<m_numTests; ++i) 
		{
			m_scode_test_table[i].slotBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
			m_scode_test_table[i].codeBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		//	System.out.print("\nTest " + i + ": qname bix = 0x" + tHS(slotBix));
		//	System.out.println(", code bix = 0x" + tHS(codeBix));
			parseQnameSlot(m_scode_test_table[i].qnameSlot, m_scode_test_table[i].slotBix);
		//	System.out.println("  Name = " + parseQnameSlot(slotBix));
		//	System.out.println();

			parseMethod(m_scode_test_table[i].method, m_scode_test_table[i].codeBix, true);
		}
	}
}

void SCodeReader::parseQnameSlot(SCODE_TEST_QNAMESLOT& objQnameSlot, int bi)
{                               
    unsigned char * savePos = m_cFileBufPtr;

	m_cFileBufPtr = m_fileBuf + bi * m_head_blockSize;
    objQnameSlot.qnameBix  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    objQnameSlot.methodBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);

	m_cFileBufPtr = m_fileBuf + objQnameSlot.qnameBix * m_head_blockSize;
    objQnameSlot.typeNameBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    objQnameSlot.slotNameBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    parseStr(objQnameSlot.typeName, objQnameSlot.typeNameBix);
    parseStr(objQnameSlot.slotName, objQnameSlot.slotNameBix);
	parseStr(objQnameSlot.methodName, objQnameSlot.methodBix);

	m_cFileBufPtr = savePos;
    // return (typeName + ":" + slotName + "." + methodName);
} 

void SCodeReader::parseBootstrap()
{
	m_cFileBufPtr = m_fileBuf + m_head_mainMethodBlockIndex * m_head_blockSize;
    int numParams = m_cFileBufPtr[0];
    int numLocals = m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
	
    int callBix;
    int nextByte = m_cFileBufPtr[2];
	m_cFileBufPtr++;
	
    // If debug is true, next bytes are MetaSlot and qname
    if (m_head_debugFlag)
    {
		while (nextByte==0)  
		{
			nextByte = m_cFileBufPtr[0];   // skip pad byte(s) if any
			m_cFileBufPtr++;
		}

		if (nextByte != g_scode_MetaSlot) // SCode.MetaSlot)
		{
			// printf("Found " + nextByte + " not MetaSlot at offset " + tHS(buf.pos));
		}

		callBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		// System.out.println("Main bix = 0x" + tHS(callBix));

		nextByte = m_cFileBufPtr[0];
		m_cFileBufPtr++;
	}

    while (nextByte==0) 
	{
		nextByte = m_cFileBufPtr[0];   // skip pad byte(s) if any
		m_cFileBufPtr++;
	}

	// If there are static inits they come next, as list of 
    // 1-byte Call opcode followed by 2-byte bix
    while (nextByte== g_scode_Call)
    {
		callBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		// System.out.println("Static init call to bix = 0x" + tHS(callBix));
		nextByte = m_cFileBufPtr[0];
		m_cFileBufPtr++;
		while (nextByte==0)
		{
			nextByte = m_cFileBufPtr[0];    // skip pad byte(s) if any
			m_cFileBufPtr++;
		}
    }

    // Past loop, next opcode is LoadParam0
//    if (nextByte!=SCode.LoadParam0)
//      throw new IOException("Found " + nextByte + " not LoadParam0 at offset " + tHS(buf.pos));

    nextByte = m_cFileBufPtr[0];
	m_cFileBufPtr++;
//    if (nextByte!=SCode.LoadParam1)
//      throw new IOException("Found " + nextByte + " not LoadParam1 at offset " + tHS(buf.pos));

    nextByte = m_cFileBufPtr[0];
	m_cFileBufPtr++;
    if (nextByte==0) 
	{
		nextByte = m_cFileBufPtr[0];    // skip pad byte(s) if any
		m_cFileBufPtr++;
	}

//    if (nextByte!=SCode.Call)
//      throw new IOException("Found " + nextByte + " not Call at offset " + tHS(buf.pos));

    callBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    nextByte = m_cFileBufPtr[0];
	m_cFileBufPtr++;
//    if (nextByte!=SCode.ReturnPop)
//      throw new IOException("Found " + nextByte + " not ReturnPop at offset " + tHS(buf.pos));
}

void SCodeReader::parseKit(SCODE_KIT& objScodeKit, unsigned int bi)
{
    unsigned char * savePos = m_cFileBufPtr;
	m_cFileBufPtr = m_fileBuf + bi * m_head_blockSize;
	
    objScodeKit.id       = m_cFileBufPtr[0];
    objScodeKit.typesLen = m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
	short sNameLen  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    parseStr(objScodeKit.cName, sNameLen);
	short sVerLen  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    parseStr(objScodeKit.cVer, sVerLen);
    objScodeKit.pad      = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    objScodeKit.checksum        = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);
	
	if (strcmp((char *)objScodeKit.cName, "sys") == 0)
	{
		m_sysKitBix = bi;
	}
	
	if (objScodeKit.typesLen > 0)
	{
		objScodeKit.kit_type_list = 
			(SCODE_KIT_TYPE *)malloc(
				sizeof(SCODE_KIT_TYPE) * objScodeKit.typesLen);
		memset(objScodeKit.kit_type_list, 
			0x00, sizeof(SCODE_KIT_TYPE) * objScodeKit.typesLen);
	
		for (int j=0; j< objScodeKit.typesLen; j++)
		{
			int tbix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
			parseType(objScodeKit.kit_type_list[j], tbix);
		}
	}
	m_cFileBufPtr = savePos;
}

void SCodeReader::parseType(SCODE_KIT_TYPE& objScodeKitType, unsigned int typeBix)
{
    unsigned char * savePos = m_cFileBufPtr;
	if (typeBix > 0)
	{
		m_cFileBufPtr = m_fileBuf + typeBix * m_head_blockSize;
	}
	else
	{
		typeBix = (m_cFileBufPtr - m_fileBuf) / m_head_blockSize;
	}

	objScodeKitType.id       = (unsigned char)m_cFileBufPtr[0];
    objScodeKitType.slotsLen = (unsigned char)m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
    objScodeKitType.nameBix  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    objScodeKitType.kitBix   = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
	parseStr(objScodeKitType.cName, objScodeKitType.nameBix);
	if(objScodeKitType.nameBix == m_sysKitBix && islower(objScodeKitType.cName[0]))
	{
		m_cFileBufPtr = savePos;
		return;
	}
	objScodeKitType.baseBix  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr); // base class typebix, 0 if not comp
    objScodeKitType.size     = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
    objScodeKitType.initBix  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);     // bix for init code

	if (objScodeKitType.slotsLen > 0)
	{
		objScodeKitType.kit_type_slots_list = 
			(SCODE_KIT_TYPE_SLOT *)malloc(
				sizeof(SCODE_KIT_TYPE_SLOT) * objScodeKitType.slotsLen);
		memset(objScodeKitType.kit_type_slots_list, 
			0x00, sizeof(SCODE_KIT_TYPE_SLOT) * objScodeKitType.slotsLen);
		
		for (int j = 0; j < objScodeKitType.slotsLen; j++)
		{
			int slotBix = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
			parseSlot(objScodeKitType.kit_type_slots_list[j], slotBix);
		}
	}
	m_cFileBufPtr = savePos;
}

void SCodeReader::parseSlot(SCODE_KIT_TYPE_SLOT& objScodeKitTypeSlot, unsigned int slotBix)
{
    unsigned char * savePos = m_cFileBufPtr;
	m_cFileBufPtr = m_fileBuf + slotBix * m_head_blockSize;

    objScodeKitTypeSlot.id         = (unsigned char)m_cFileBufPtr[0];
    objScodeKitTypeSlot.rtFlags    = (unsigned char)m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
    objScodeKitTypeSlot.nameBix    =  calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
	parseStr(objScodeKitTypeSlot.cPropName, objScodeKitTypeSlot.nameBix);
    objScodeKitTypeSlot.fpBix    = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);   // bix for field type OR method param
    objScodeKitTypeSlot.codeBix  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);  // bix for code ONLY if method!!

    if ((int)slotBix > m_endSlotBix)
	{
		m_endSlotBix = ((m_cFileBufPtr - m_fileBuf) +
							(m_head_blockSize-1)) / m_head_blockSize;   // round up to next bix
	}
	m_cFileBufPtr = savePos;
}

void SCodeReader::parseStr(unsigned char * strBuf, unsigned int bi)
{
    int off = bi * m_head_blockSize;
    for (int i=0; m_fileBuf[off+i] != 0; ++i)
		strBuf[i] = m_fileBuf[off+i];
}
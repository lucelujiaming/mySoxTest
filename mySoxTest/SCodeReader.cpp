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


SCodeReader::SCodeReader()
{
	// svm use little endian
	setLittleEndian();
	m_fileBuf = NULL;
	m_scode_log_veclist.reserve(8);
}

SCodeReader::~SCodeReader()
{
	if(m_fileBuf)
	{
		free(m_fileBuf);
		m_fileBuf = NULL;
	}
}

unsigned short SCodeReader::calcAndSkipUnsignedShortValue()
{
	unsigned short uValue = 0x00;
	if (m_bBigEndian)
	{
		uValue        = (unsigned char)m_cFileBufPtr[0];
		uValue       *= 0x100;
		uValue       += (unsigned char)m_cFileBufPtr[1];
	}
	else 
	{
		uValue       += (unsigned char)m_cFileBufPtr[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)m_cFileBufPtr[0];
	}
	m_cFileBufPtr += 2;
	return uValue;
}

unsigned int SCodeReader::calcAndSkipUnsignedIntValue()
{
	unsigned int uValue = 0x00;
	if (m_bBigEndian)
	{
		uValue        = (unsigned char)m_cFileBufPtr[0];
		uValue       *= 0x100;
		uValue       += (unsigned char)m_cFileBufPtr[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)m_cFileBufPtr[2];
		uValue       *= 0x100;
		uValue       += (unsigned char)m_cFileBufPtr[3];
	}
	else 
	{
		uValue        = (unsigned char)m_cFileBufPtr[3];
		uValue       *= 0x100;
		uValue       += (unsigned char)m_cFileBufPtr[2];
		uValue       *= 0x100;
		uValue       += (unsigned char)m_cFileBufPtr[1];
		uValue       *= 0x100;
		uValue       += (unsigned char)m_cFileBufPtr[0];
	}
	m_cFileBufPtr += 4;
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
	m_head_majorVer    =  m_cFileBufPtr[0];
	m_head_minorVer    =  m_cFileBufPtr[1];
	m_head_blockSize   =  m_cFileBufPtr[2];
	m_head_refSize     =  m_cFileBufPtr[3];
	m_cFileBufPtr += 4;
	
	m_head_imageSize        = calcAndSkipUnsignedIntValue();
	m_head_dataSize         = calcAndSkipUnsignedIntValue();

	m_head_mainMethodBlockIndex        = calcAndSkipUnsignedShortValue();
	m_head_testBlockIndex              = calcAndSkipUnsignedShortValue();
	m_head_kitsArrayBlockIndex         = calcAndSkipUnsignedShortValue();

	m_head_numberOfKits =  (unsigned char)m_cFileBufPtr[0];
	m_head_scodeFlags   =  (unsigned char)m_cFileBufPtr[1];
	
	m_head_debugFlag    = (m_head_scodeFlags & SCODE_DEBUG_FLAG)!=0;
	m_head_testFlag     = (m_head_scodeFlags & SCODE_TEST_FLAG)!=0;
	m_cFileBufPtr += 2;
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
				printf("\t\t SCode::kit[%d].type[%d].slot[%d].(id, rtFlags, nameBix, cName, fpBix, codeBix) "
					"  = (%02d, %02d, %02d, %s, %d, %d) \r\n", i, j, k, 
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].id, 
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].rtFlags,
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].nameBix,
					m_scode_kit_list[i].kit_type_list[j].kit_type_slots_list[k].cName, 
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
		printMethod(*iter);
	}
}

void SCodeReader::printMethod(SCODE_METHOD objMethod)
{
}

void SCodeReader::printTests()
{
	for (int i=0; i< m_numTests; ++i) 
    {
		printf("SCode::Test(slotBix, codeBix) = (%02d, %02d) \r\n", 
			m_scode_test_table[i].slotBix, m_scode_test_table[i].codeBix);
		printMethod(m_scode_test_table[i].method);
	}
}

void SCodeReader::readSCodeFile(char * cFileName)
{
	m_sysKitBix = -1;
	m_endTypeBix = -1;
	m_endSlotBix = -1;
	m_endLogBix = -1;
	m_objFileReader.readSCodeFileBuffer(&m_fileBuf, cFileName);
	m_cFileBufPtr = m_fileBuf;
	setSCodeHeader();
	printSCodeHeader();

	if (m_head_numberOfKits > 0)
	{
		m_scode_kit_list = (SCODE_KIT *)malloc(sizeof(SCODE_KIT) * m_head_numberOfKits);
		memset(m_scode_kit_list, 0x00, sizeof(SCODE_KIT) * m_head_numberOfKits);
		
		for (int i=0; i< m_head_numberOfKits; ++i) 
		{
			unsigned int iPos = m_head_kitsArrayBlockIndex *  m_head_blockSize + i * 2;
			m_cFileBufPtr = m_fileBuf + iPos;
			iPos = calcAndSkipUnsignedShortValue();
			parseKit(m_scode_kit_list[i], iPos);
		}
		// printSCodeKits();
	}

	parseLogs(m_endSlotBix);
	printSCodeLogs();
	// parseMethods(m_endLogBix);
		
    if (m_head_testBlockIndex > 0)
    {
		parseTests();
		printTests();
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

		objSCodeLog.id           = calcAndSkipUnsignedShortValue();
		objSCodeLog.nameBix      = calcAndSkipUnsignedShortValue();
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

void SCodeReader::parseMethod(SCODE_METHOD& objMethod, int codeBix, bool bRestorePos)
{
    int       opCode;
    char      argStr[SCODE_ARG_LEN];
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
		// Get next opcode
		opCode = m_cFileBufPtr[0];
		m_cFileBufPtr ++;

		// Print opcode & name
		// System.out.print( opFormat(opPos, opCode, SCode.name(opCode)) );

		// argStr = getArgString(opCode);

		// Print arg if any
		// if (argStr!=null) System.out.println(argStr);
		// else System.out.println();


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
				if (m_cFileBufPtr - m_fileBuf + atoi(argStr) > maxjmp)
					maxjmp = m_cFileBufPtr - m_fileBuf + atoi(argStr);
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
	
	m_cFileBufPtr = savePos;
}

void SCodeReader::parseMethods(unsigned int methodTabBix)
{
	SCODE_METHOD  objMethod;
	unsigned char * cMethodEnd = NULL;
	// If methodTabBix<0 then buf.pos already at correct position
    if (methodTabBix > 0) 
	{
		m_cFileBufPtr = m_fileBuf + methodTabBix * m_head_blockSize;
	}
	cMethodEnd = m_cFileBufPtr + m_head_imageSize;

    // Not sure how to tell we're done???
    while (m_cFileBufPtr < cMethodEnd)
    {
		parseMethod(objMethod, -1, false);
		m_scode_method_veclist.push_back(objMethod);
    }
}

void SCodeReader::parseTests()
{
	m_cFileBufPtr = m_fileBuf + m_head_testBlockIndex * m_head_blockSize;
	m_numTests = calcAndSkipUnsignedShortValue();

    if (m_numTests > 0)
	{
		m_scode_test_table = (SCODE_TEST *)malloc(sizeof(SCODE_TEST) * m_numTests);
		memset(m_scode_test_table, 0x00, sizeof(SCODE_TEST) * m_numTests);

		// traverse the pointers for each test in table
		for (int i=0; i<m_numTests; ++i) 
		{
			m_scode_test_table[i].slotBix = calcAndSkipUnsignedShortValue();
			m_scode_test_table[i].codeBix = calcAndSkipUnsignedShortValue();
		//	System.out.print("\nTest " + i + ": qname bix = 0x" + tHS(slotBix));
		//	System.out.println(", code bix = 0x" + tHS(codeBix));
		//	System.out.println("  Name = " + parseQnameSlot(slotBix));
		//	System.out.println();

			parseMethod(m_scode_test_table[i].method, m_scode_test_table[i].codeBix, true);
		}
	}
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

		callBix = calcAndSkipUnsignedShortValue();
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
		callBix = calcAndSkipUnsignedShortValue();
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

    callBix = calcAndSkipUnsignedShortValue();
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
	short sNameLen  = calcAndSkipUnsignedShortValue();
    parseStr(objScodeKit.cName, sNameLen);
	short sVerLen  = calcAndSkipUnsignedShortValue();
    parseStr(objScodeKit.cVer, sVerLen);
    objScodeKit.pad      = calcAndSkipUnsignedShortValue();
    objScodeKit.checksum        = calcAndSkipUnsignedIntValue();
	
	if (strcmp(objScodeKit.cName, "sys") == 0)
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
			int tbix = calcAndSkipUnsignedShortValue();
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
    objScodeKitType.nameBix  = calcAndSkipUnsignedShortValue();
    objScodeKitType.kitBix   = calcAndSkipUnsignedShortValue();
	parseStr(objScodeKitType.cName, objScodeKitType.nameBix);
	if(objScodeKitType.nameBix == m_sysKitBix && islower(objScodeKitType.cName[0]))
	{
		m_cFileBufPtr = savePos;
		return;
	}
	objScodeKitType.baseBix  = calcAndSkipUnsignedShortValue(); // base class typebix, 0 if not comp
    objScodeKitType.size     = calcAndSkipUnsignedShortValue();
    objScodeKitType.initBix  = calcAndSkipUnsignedShortValue();     // bix for init code

	if (objScodeKitType.slotsLen > 0)
	{
		objScodeKitType.kit_type_slots_list = 
			(SCODE_KIT_TYPE_SLOT *)malloc(
				sizeof(SCODE_KIT_TYPE_SLOT) * objScodeKitType.slotsLen);
		memset(objScodeKitType.kit_type_slots_list, 
			0x00, sizeof(SCODE_KIT_TYPE_SLOT) * objScodeKitType.slotsLen);
		
		for (int j = 0; j < objScodeKitType.slotsLen; j++)
		{
			int slotBix = calcAndSkipUnsignedShortValue();
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
    objScodeKitTypeSlot.nameBix    =  calcAndSkipUnsignedShortValue();
	parseStr(objScodeKitTypeSlot.cName, objScodeKitTypeSlot.nameBix);
    objScodeKitTypeSlot.fpBix    = calcAndSkipUnsignedShortValue();   // bix for field type OR method param
    objScodeKitTypeSlot.codeBix  = calcAndSkipUnsignedShortValue();  // bix for code ONLY if method!!

    if ((int)slotBix > m_endSlotBix)
	{
		m_endSlotBix = ((m_cFileBufPtr - m_fileBuf) +
							(m_head_blockSize-1)) / m_head_blockSize;   // round up to next bix
	}
	m_cFileBufPtr = savePos;
}

void SCodeReader::parseStr(char * strBuf, unsigned int bi)
{
    int off = bi * m_head_blockSize;
    for (int i=0; m_fileBuf[off+i] != 0; ++i)
		strBuf[i] = m_fileBuf[off+i];
}
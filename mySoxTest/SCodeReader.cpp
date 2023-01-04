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
	m_fileBuf = NULL;
}

SCodeReader::~SCodeReader()
{
	if(m_fileBuf)
	{
		free(m_fileBuf);
		m_fileBuf = NULL;
	}
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
	
	m_head_imageSize        = (unsigned char)m_cFileBufPtr[3];
	m_head_imageSize       *= 0x100;
	m_head_imageSize       += (unsigned char)m_cFileBufPtr[2];
	m_head_imageSize       *= 0x100;
	m_head_imageSize       += (unsigned char)m_cFileBufPtr[1];
	m_head_imageSize       *= 0x100;
	m_head_imageSize       += (unsigned char)m_cFileBufPtr[0];
	m_cFileBufPtr += 4;

	m_head_dataSize        = (unsigned char)m_cFileBufPtr[3];
	m_head_dataSize       *= 0x100;
	m_head_dataSize       += (unsigned char)m_cFileBufPtr[2];
	m_head_dataSize       *= 0x100;
	m_head_dataSize       += (unsigned char)m_cFileBufPtr[1];
	m_head_dataSize       *= 0x100;
	m_head_dataSize       += (unsigned char)m_cFileBufPtr[0];
	m_cFileBufPtr += 4;

	m_head_mainMethodBlockIndex        = (unsigned char)m_cFileBufPtr[1];
	m_head_mainMethodBlockIndex       *= 0x100;
	m_head_mainMethodBlockIndex       += (unsigned char)m_cFileBufPtr[0];
	m_cFileBufPtr += 2;

	m_head_testBlockIndex        = (unsigned char)m_cFileBufPtr[1];
	m_head_testBlockIndex       *= 0x100;
	m_head_testBlockIndex       += (unsigned char)m_cFileBufPtr[0];
	m_cFileBufPtr += 2;

	m_head_kitsArrayBlockIndex        = (unsigned char)m_cFileBufPtr[1];
	m_head_kitsArrayBlockIndex       *= 0x100;
	m_head_kitsArrayBlockIndex       += (unsigned char)m_cFileBufPtr[0];
	m_cFileBufPtr += 2;

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
		printf("m_scode_kit_list[%d].id = %02X \r\n", i, m_scode_kit_list[i].id);
		printf("m_scode_kit_list[%d].typesLen = %02X \r\n", i, m_scode_kit_list[i].typesLen);
		printf("m_scode_kit_list[%d].cName = %s \r\n", i, m_scode_kit_list[i].cName);
		printf("m_scode_kit_list[%d].cVer = %s \r\n", i, m_scode_kit_list[i].cVer);
		printf("m_scode_kit_list[%d].pad = %d \r\n", i, m_scode_kit_list[i].pad);
		printf("m_scode_kit_list[%d].checksum = %04X \r\n", i, m_scode_kit_list[i].checksum);
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

	m_scode_kit_list = (SCODE_KIT *)malloc(sizeof(SCODE_KIT) * m_head_numberOfKits);
	memset(m_scode_kit_list, 0x00, sizeof(SCODE_KIT) * m_head_numberOfKits);

	for (int i=0; i< m_head_numberOfKits; ++i) 
    {
		unsigned int iPos = m_head_kitsArrayBlockIndex *  m_head_blockSize + i * 2;
		m_cFileBufPtr = m_fileBuf + iPos;
		iPos = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
		m_cFileBufPtr += 2;
		parseKit(i, iPos);
    }
	printSCodeKits();
	parseLogs(m_endLogBix);
	parseMethods(m_endLogBix);
		
    if (m_head_testBlockIndex > 0)
    {
		parseTests();
	}
}

void SCodeReader::parseLogs(unsigned int endSlotBix)
{
}

void SCodeReader::parseMethods(unsigned int endLogBix)
{
}

void SCodeReader::parseTests()
{
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

		callBix = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
		m_cFileBufPtr += 2;
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
		callBix = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
		m_cFileBufPtr += 2;
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

    callBix = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
	m_cFileBufPtr += 2;
    nextByte = m_cFileBufPtr[0];
	m_cFileBufPtr++;
//    if (nextByte!=SCode.ReturnPop)
//      throw new IOException("Found " + nextByte + " not ReturnPop at offset " + tHS(buf.pos));
}

void SCodeReader::parseKit(int iSeq, unsigned int bi)
{
    unsigned char * savePos = m_cFileBufPtr;
	m_cFileBufPtr = m_fileBuf + bi * m_head_blockSize;
	
    m_scode_kit_list[iSeq].id       = m_cFileBufPtr[0];
    m_scode_kit_list[iSeq].typesLen = m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
	short sNameLen  = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
	m_cFileBufPtr += 2;
    parseStr(m_scode_kit_list[iSeq].cName, sNameLen);
	short sVerLen  = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
	m_cFileBufPtr += 2;
    parseStr(m_scode_kit_list[iSeq].cVer, sVerLen);
    m_scode_kit_list[iSeq].pad      = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
	m_cFileBufPtr += 2;
    m_scode_kit_list[iSeq].checksum        = (unsigned char)m_cFileBufPtr[3];
	m_scode_kit_list[iSeq].checksum       *= 0x100;
	m_scode_kit_list[iSeq].checksum       += (unsigned char)m_cFileBufPtr[2];
	m_scode_kit_list[iSeq].checksum       *= 0x100;
	m_scode_kit_list[iSeq].checksum       += (unsigned char)m_cFileBufPtr[1];
	m_scode_kit_list[iSeq].checksum       *= 0x100;
	m_scode_kit_list[iSeq].checksum       += (unsigned char)m_cFileBufPtr[0];
	m_cFileBufPtr += 4;
	
	if (strcmp(m_scode_kit_list[iSeq].cName, "sys") == 0)
	{
		m_sysKitBix = bi;
	}
    for (int j=0; j< m_scode_kit_list[iSeq].typesLen; j++)
    {
		int tbix = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];;
		parseType(iSeq, tbix);
    }

}

void SCodeReader::parseType(int iSeq, unsigned int typeBix)
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

	int id       = (unsigned char)m_cFileBufPtr[0];
    int slotsLen = (unsigned char)m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
    int nameBix  = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
	m_cFileBufPtr += 2;
    int kitBix   = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
	m_cFileBufPtr += 2;
    char name[100];
	parseStr(name, nameBix);
	if(nameBix == m_sysKitBix && islower(name[0]))
	{
		m_cFileBufPtr = savePos;
		return;
	}
	int baseBix  = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0]; // base class typebix, 0 if not comp
	m_cFileBufPtr += 2;
    int size     = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
	m_cFileBufPtr += 2;
    int initBix  = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];      // bix for init code
	m_cFileBufPtr += 2;

    for (int j=0; j<slotsLen; j++)
    {
		int slotBix = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
		parseSlot(iSeq, slotBix);
    }
	m_cFileBufPtr = savePos;
}

void SCodeReader::parseSlot(int iSeq, unsigned int slotBix)
{
    unsigned char * savePos = m_cFileBufPtr;
	m_cFileBufPtr = m_fileBuf + slotBix * m_head_blockSize;

    int id         = (unsigned char)m_cFileBufPtr[0];
    int rtFlags    = (unsigned char)m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
    int nameBix    =  m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];
	m_cFileBufPtr += 2;
    char name[100];
	parseStr(name, nameBix);
    int fpBix    = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];   // bix for field type OR method param
	m_cFileBufPtr += 2;
    int codeBix  = m_cFileBufPtr[1] * 0x100 + m_cFileBufPtr[0];  // bix for code ONLY if method!!
	m_cFileBufPtr += 2;

    if ((int)slotBix > m_endSlotBix)
      m_endSlotBix = ((m_cFileBufPtr - m_cFileBufPtr) +
							(m_head_blockSize-1)) / m_head_blockSize;   // round up to next bix

	m_cFileBufPtr = savePos;
}

void SCodeReader::parseStr(char * strBuf, unsigned int bi)
{
    int off = bi * m_head_blockSize;
    for (int i=0; m_fileBuf[off+i] != 0; ++i)
		strBuf[i] = m_fileBuf[off+i];
}
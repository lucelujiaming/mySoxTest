// SabReader.cpp: implementation of the SabReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "SabReader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SabReader::SabReader()
{
	// svm use little endian
	setLittleEndian();
	m_fileBuf = NULL;
}

SabReader::~SabReader()
{

}
unsigned short SabReader::calcAndSkipUnsignedShortValue()
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

unsigned int SabReader::calcAndSkipUnsignedIntValue()
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

#pragma warning(disable : 4244)
unsigned long  SabReader::calcAndSkipUnsignedLongValue()
{
	unsigned long uValue = 0x00;
	unsigned int uValueOne = 0x00;
	uValueOne = calcAndSkipUnsignedIntValue();
	unsigned int uValueTwo = 0x00;
	uValueTwo = calcAndSkipUnsignedIntValue();
	
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

// 这里补充一下sab文件的串行化二进制结构。
//   app
//   {
//     u4           magic 0x73617070 "sapp"
//     u4           version 0x0001 0.3
//     Schema       schema   // Schema is the list of kit names and checksums:
//                                             //   schema
//                                             //   {
//                                             //     u1  count
//                                             //     kits[count]
//                                             //     {
//                                             //       str  name
//                                             //       u4   checksum
//                                             //     }
//                                             //   }
//     u2           maxId                      // components个数。
//     Component[]  comps                      // Component列表
//     u2           0xffff end of comps marker // Component列表末端标记
//     Link[]       links                      // Link列表
//     u2           0xffff end of links marker // Link列表末端标记
//     u1           '.' end of app marker      // App末端标记
//   }
void SabReader::readSabFile(char * cFileName)
{
	m_objFileReader.readSCodeFileBuffer(&m_fileBuf, cFileName);
	m_cFileBufPtr = m_fileBuf;
	memcpy(m_head_magic, m_cFileBufPtr, 4);
	m_cFileBufPtr += 4;
	memcpy(m_head_Version, m_cFileBufPtr, 4);
	m_cFileBufPtr += 4;
	loadSchema();
	printSchema();
	m_maxid = calcAndSkipUnsignedShortValue();
	loadComponents();
}

#define SYS_KITSLEN     1
void SabReader::loadSchema()
{
	m_numkits = m_cFileBufPtr[0];
	m_cFileBufPtr ++;
//	if (m_numkits > SYS_KITSLEN)
//	{
//		printf("m_numkits = %d.\r\n", m_numkits);
//		return;
//	}
	m_kitIdMap = (int *)malloc(sizeof(int) * m_numkits);
    int n = m_numkits;

	// loadSchema
	m_schema_kit_list = (SCHEMA_KIT *)malloc(m_numkits * sizeof(SCHEMA_KIT *));
	memset(m_schema_kit_list, 0x00, m_numkits * sizeof(SCHEMA_KIT *));
	for (int i = 0 ; i < m_numkits; i++)
	{
		memset(m_schema_kit_list[i].cName, 0x00, SAB_NAME_LEN);
		strcpy(m_schema_kit_list[i].cName, (char *)m_cFileBufPtr);
		m_cFileBufPtr += strlen((char *)m_cFileBufPtr) + 1;
		m_schema_kit_list[i].checksum        = calcAndSkipUnsignedIntValue();

		// m_kitIdMap[ m_numkits-n-1 ] = kit.id;
	}
}

void SabReader::printSchema()
{
	for (int i=0; i< m_numkits; ++i) 
    {
		printf("Sab::Schema[%d].(cName, checksum) = (%s, %04X) \r\n", i, 
			m_schema_kit_list[i].cName, m_schema_kit_list[i].checksum);
	}	
}

// 这里补充一下sab文件的串行化二进制结构。
//   appComp
//   {
//     u2      id												
//     u1      kitId
//     u1      typeId
//     str     name
//     u2      parent
//     u2      children
//     u2      nextSibling
//     val[]   configProps
//     u1      ';' end marker
//   }
// components
void SabReader::loadComponents()
{
	while (1)
	{
		int compId = calcAndSkipUnsignedShortValue();
		if ((compId == 0xFFFF) | (compId < 0)) 
			break;
		unsigned char kit_id  = m_cFileBufPtr[0];
		unsigned char type_id = m_cFileBufPtr[1];
		m_cFileBufPtr += 2;
		loadAppComp();
	}
}

int SabReader::loadAppComp()
{
	char name[SAB_NAME_LEN];
	strcpy(name, (char *)m_cFileBufPtr);
	m_cFileBufPtr += strlen((char *)m_cFileBufPtr) + 1;
	
	unsigned short parent      = calcAndSkipUnsignedShortValue();
	unsigned short children    = calcAndSkipUnsignedShortValue();
	unsigned short nextSibling = calcAndSkipUnsignedShortValue();
	loadProps('c');
	if (m_cFileBufPtr[0] == ';')
	{
		m_cFileBufPtr++;
		return 0;
	}
	else
	{
		m_cFileBufPtr++;
		return 1;
	}
}

//  **
//  ** Load the property values to the output stream.
//  ** Filter:
//  **   0   = all
//  **   'c' = config only
//  **   'r' = runtime only
void SabReader::loadProps(char filter)
{
	
}


// SabReader.cpp: implementation of the SabReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "SabReader.h"

#define SAB_NAME_LEN   16 // (8 * 2)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SabReader::SabReader()
{
	m_fileBuf = NULL;
}

SabReader::~SabReader()
{

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
	m_schema_count = m_cFileBufPtr[0];
	m_cFileBufPtr ++;
	loadSchema();
	m_maxid = m_cFileBufPtr[0] * 0x100 + m_cFileBufPtr[1];
	loadComponents();
}

void SabReader::loadSchema()
{
	// loadSchema
	m_schema_kit_list = (SCHEMA_KIT *)malloc(m_schema_count * sizeof(SCHEMA_KIT *));
	for (int i = 0 ; i < m_schema_count; i++)
	{
		m_schema_kit_list[i].name = (char *)malloc(SAB_NAME_LEN);
		memset(m_schema_kit_list[i].name, 0x00, SAB_NAME_LEN);
		strcpy(m_schema_kit_list[i].name, (char *)m_cFileBufPtr);
		m_cFileBufPtr += strlen((char *)m_cFileBufPtr);
		m_schema_kit_list[i].checksum        = (unsigned char)m_cFileBufPtr[0];
		m_schema_kit_list[i].checksum       *= 0x100;
		m_schema_kit_list[i].checksum       += (unsigned char)m_cFileBufPtr[1];
		m_schema_kit_list[i].checksum       *= 0x100;
		m_schema_kit_list[i].checksum       += (unsigned char)m_cFileBufPtr[2];
		m_schema_kit_list[i].checksum       *= 0x100;
		m_schema_kit_list[i].checksum       += (unsigned char)m_cFileBufPtr[3];
		m_cFileBufPtr += 4;
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
		int compId = 0;
		compId = m_cFileBufPtr[0] * 0x100 + m_cFileBufPtr[1];
		m_cFileBufPtr += 2;
		if (compId == 0xFFFF) 
			break;
		unsigned char kit_id  = m_cFileBufPtr[0];
		unsigned char type_id = m_cFileBufPtr[1];
		m_cFileBufPtr += 2;

		loadAppComp();
	}
}

void SabReader::loadAppComp()
{
	char name[SAB_NAME_LEN];
	strcpy(name, (char *)m_cFileBufPtr);
	m_cFileBufPtr += strlen((char *)m_cFileBufPtr);
	
	unsigned short parent   = m_cFileBufPtr[0] * 0x100 + m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
	unsigned short children = m_cFileBufPtr[0] * 0x100 + m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
	unsigned short nextSibling = m_cFileBufPtr[0] * 0x100 + m_cFileBufPtr[1];
	m_cFileBufPtr += 2;
	loadProps();
}

void SabReader::loadProps()
{
	
}


// SabReader.cpp: implementation of the SabReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "SabReader.h"
#include "SCode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SabReader::SabReader()
{
	// sab use big endian
	setBigEndian();
	m_fileBuf = NULL;
	m_objSabPropList.reserve(10);
}

SabReader::~SabReader()
{

}


bool SabReader::loadSCodeFile(char * cFileName)
{
	m_objSCodeReader.readSCodeFile(cFileName);
	return true;
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
	if (m_objSCodeReader.getSCodeKits() == NULL)
	{
		printf("We need the scode file.");
		return;
	}
	int iFileLen = m_objFileReader.readSCodeFileBuffer(&m_fileBuf, cFileName);
	m_cFileBufPtr = m_fileBuf;
	memcpy(m_head_magic, m_cFileBufPtr, 4);
	m_cFileBufPtr += 4;
	memcpy(m_head_Version, m_cFileBufPtr, 4);
	m_cFileBufPtr += 4;
	loadSchema();
	createSchemaAssociationTable();
	printSchema();
	m_maxid = calcAndSkipUnsignedShortValue();
	// components
	loadComponents();
	if ((m_fileBuf + iFileLen - m_cFileBufPtr) < 4)
	{
		printf("Left space is %d and stop analyze \r\n", 
			(m_fileBuf + iFileLen - m_cFileBufPtr));
		return;
	}
	// links
	loadLinks();
	printLinks();
	if ((m_fileBuf + iFileLen - m_cFileBufPtr) < 4)
	{
		printf("Left space is %d and stop analyze \r\n", 
			(m_fileBuf + iFileLen - m_cFileBufPtr));
		return;
	}

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
	}
}

void SabReader::printSchema()
{
	for (int i=0; i< m_numkits; ++i) 
    {
		printf("Sab::Schema[%d].(cName, checksum) = (%s, %04X) and relate to %d \r\n", i, 
			m_schema_kit_list[i].cName, m_schema_kit_list[i].checksum, 
			m_objSchemaAssociationTable[i]);
	}

	SCODE_KIT *  objKitsList = m_objSCodeReader.getSCodeKits();
    int         numberOfKits = m_objSCodeReader.getNumberOfKits();

	printf("==================================== start ==================================== \r\n");
	for (int j=0; j< numberOfKits; ++j) 
    {
		printf("objSCodeKitType[%d] is %s \r\n", j , objKitsList[j].cName);
	}


}

void SabReader::createSchemaAssociationTable()
{
	memset(m_objSchemaAssociationTable, 0x00, 256);

	SCODE_KIT *  objKitsList = m_objSCodeReader.getSCodeKits();
    int         numberOfKits = m_objSCodeReader.getNumberOfKits();

	for (int i=0; i< m_numkits; ++i) 
    {	
		for (int j=0; j< numberOfKits; ++j) 
		{
			if (stricmp(m_schema_kit_list[i].cName, objKitsList[j].cName) == 0)
			{
				m_objSchemaAssociationTable[i] = j;
				break;
			}
		}
	}
	m_objSchemaAssociationTable[m_numkits] = 0xFF;
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
	SAB_COMP_INFO objSabCompInfo;
	while (1)
	{
		memset(&objSabCompInfo, 0x00, sizeof(SAB_COMP_INFO));
		objSabCompInfo.compID = calcAndSkipUnsignedShortValue();
		if ((objSabCompInfo.compID == 0xFFFF) || (objSabCompInfo.compID < 0)) 
			break;
		objSabCompInfo.kitID  = m_cFileBufPtr[0];
		objSabCompInfo.typeID = m_cFileBufPtr[1];
		m_cFileBufPtr += 2;
		loadAppComp(objSabCompInfo);
	}
}

int SabReader::loadAppComp(SAB_COMP_INFO& objSabCompInfo)
{
	SCODE_KIT_TYPE * objSCodeKitType = NULL;
	strcpy(objSabCompInfo.cName, (char *)m_cFileBufPtr);
	m_cFileBufPtr += strlen((char *)m_cFileBufPtr) + 1;
	
	unsigned short parent      = calcAndSkipUnsignedShortValue();
	unsigned short children    = calcAndSkipUnsignedShortValue();
	unsigned short nextSibling = calcAndSkipUnsignedShortValue();

	SCODE_KIT *  objKitsList = m_objSCodeReader.getSCodeKits();
    int         numberOfKits = m_objSCodeReader.getNumberOfKits();

	// if (stricmp(objKitsList[objSabCompInfo.kitID].kit_type_list[objSabCompInfo.typeID].cName, objSabCompInfo.cName) == 0)
	// 
	objSCodeKitType = NULL;

	if(objSabCompInfo.kitID >= m_numkits)
	{
		return 1;
	}
	int iAssociationIdx = m_objSchemaAssociationTable[objSabCompInfo.kitID];

	objSCodeKitType = &objKitsList[iAssociationIdx].kit_type_list[objSabCompInfo.typeID];
	if (objSCodeKitType)
	{
		printf("objSCodeKitType is %s::%s \r\n", objKitsList[iAssociationIdx].cName, 
			objKitsList[iAssociationIdx].kit_type_list[objSabCompInfo.typeID].cName);
		loadProps(objSabCompInfo, objSCodeKitType, 'c');
		printProps();
		if (m_cFileBufPtr[0] == ';')
		{
			m_cFileBufPtr++;
			return 0;
		}
	}
	return 1;
}

void SabReader::printProps()
{
	std::vector<SAB_PROP>::iterator iter;
	printf("==================================== start ==================================== \r\n");
	for (iter = m_objSabPropList.begin(); iter != m_objSabPropList.end(); iter++)
	{
		printf("objSabCompInfo(compID, nameBix, cName) = (%02d, %02d, %02d, %s), (propType) = (%03X) ", 
			iter->objSabCompInfo.compID, iter->objSabCompInfo.kitID, 
			iter->objSabCompInfo.typeID, iter->objSabCompInfo.cName,
			iter->propType);
		switch (iter->propType)
		{
		case MAGIC_SLOT_TYPE_VOID	:
			break;
		case MAGIC_SLOT_TYPE_BOOL	:
			printf(", (val) = (%s) \r\n", iter->bRet?"true":"false");
			break;
		case MAGIC_SLOT_TYPE_INT	:
			printf(", (val) = (%d) \r\n", iter->uRet);
			break;
		case MAGIC_SLOT_TYPE_LONG	:
			printf(", (val) = (%l) \r\n", iter->ulRet);
			break;
		case MAGIC_SLOT_TYPE_FLOAT	:
			printf(", (val) = (%f) \r\n", iter->ulRet);
			break;
		case MAGIC_SLOT_TYPE_DOUBLE	:
			printf(", (val) = (%f) \r\n", iter->udRet);
			break;
		case MAGIC_SLOT_TYPE_BUF	:
			printf(", (val) = (%d : %s) \r\n", iter->uBufLen, iter->cBuf);
			break;
		case MAGIC_SLOT_TYPE_BYTE	:
			printf(", (val) = (%02X) \r\n",iter->cByte);
			break;
		case MAGIC_SLOT_TYPE_SHORT	:
			printf(", (val) = (%0d) \r\n", iter->shortRet);
			break;
		}		
	}
	printf("====================================  end  ==================================== \r\n");
}

//  **
//  ** Load the property values to the output stream.
//  ** Filter:
//  **   0   = all
//  **   'c' = config only
//  **   'r' = runtime only
void SabReader::loadProps(SAB_COMP_INFO& objSabCompInfo, SCODE_KIT_TYPE * objSCodeKitType, char filter)
{
	SAB_PROP         objSabProp;
	if (!objSCodeKitType)
	{
		return;
	}
	for (int k=0; k < objSCodeKitType->slotsLen; k++)
	{
		if (m_objSCodeReader.matchRtFlagsProp(filter, objSCodeKitType->kit_type_slots_list[k].rtFlags))
		{
			memset(&objSabProp, 0x00, sizeof(SAB_PROP));
			objSabProp.objSabCompInfo = objSabCompInfo;
			loadProp(objSabProp, objSCodeKitType->kit_type_slots_list[k].fpBix, 
				m_objSCodeReader.isRtFlagsAsStr(objSCodeKitType->kit_type_slots_list[k].rtFlags));
			m_objSabPropList.push_back(objSabProp);
		}
	}
	
}

void SabReader::loadProp(SAB_PROP& objSabProp, int iFpBix, bool isStr)
{
	objSabProp.propType = iFpBix;
    switch (iFpBix)
    {
	case MAGIC_SLOT_TYPE_VOID	:
		break;
	case MAGIC_SLOT_TYPE_BOOL	:
		objSabProp.bRet = (m_cFileBufPtr[0]!= 0);
		m_cFileBufPtr ++;
		break;
	case MAGIC_SLOT_TYPE_INT	:
		objSabProp.uRet = calcAndSkipUnsignedIntValue();
		break;
	case MAGIC_SLOT_TYPE_LONG	:
		objSabProp.ulRet = calcAndSkipUnsignedLongValue();
		break;
	case MAGIC_SLOT_TYPE_FLOAT	:
		objSabProp.ulRet = intBitsToFloat(calcAndSkipUnsignedIntValue());
		break;
	case MAGIC_SLOT_TYPE_DOUBLE	:
		objSabProp.udRet = longBitsToDouble(calcAndSkipUnsignedLongValue());
		break;
	case MAGIC_SLOT_TYPE_BUF	:
		objSabProp.uBufLen  = calcAndSkipUnsignedShortValue();
		if (objSabProp.uBufLen > (m_cFileBufPtr - m_fileBuf))
		{
			objSabProp.uBufLen = m_cFileBufPtr - m_fileBuf;
		}
		memset(objSabProp.cBuf, 0x00, 1024);
		memcpy(objSabProp.cBuf, m_cFileBufPtr, objSabProp.uBufLen);
		m_cFileBufPtr += objSabProp.uBufLen;
		if(isStr)
		{
			objSabProp.cBuf[objSabProp.uBufLen - 1] = '\0';
		}
		break;
	case MAGIC_SLOT_TYPE_BYTE	:
		objSabProp.cByte = m_cFileBufPtr[0];
		m_cFileBufPtr ++;
		break;
	case MAGIC_SLOT_TYPE_SHORT	:
		objSabProp.shortRet = calcAndSkipUnsignedShortValue();
		break;
    }
}

void SabReader::loadLinks()
{
	SAB_LINK objSabLinkInfo;
	while (1)
	{
		memset(&objSabLinkInfo, 0x00, sizeof(SAB_LINK));
		objSabLinkInfo.fromComp  = calcAndSkipUnsignedShortValue();
		if ((objSabLinkInfo.fromComp == 0xFFFF) || (objSabLinkInfo.fromComp < 0)) 
			break;
		objSabLinkInfo.fromSlot  = m_cFileBufPtr[0];
		m_cFileBufPtr++;
		objSabLinkInfo.toComp    = calcAndSkipUnsignedShortValue();
		objSabLinkInfo.toSlot    = m_cFileBufPtr[0];
		m_cFileBufPtr++;
		m_objSabLinkList.push_back(objSabLinkInfo);
	}
}

void SabReader::printLinks()
{
	std::vector<SAB_LINK>::iterator iter;
	printf("==================================== start ==================================== \r\n");
	for (iter = m_objSabLinkList.begin(); iter != m_objSabLinkList.end(); iter++)
	{
		printf("SAB_LINK(fromComp, fromSlot, toComp, toSlot) = (%02d, %02d, %02d, %02d) \r\n", 
			iter->fromComp, iter->fromSlot, iter->toComp, iter->toSlot);
	}
	printf("====================================  end  ==================================== \r\n");
}
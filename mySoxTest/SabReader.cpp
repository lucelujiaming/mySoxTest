// SabReader.cpp: implementation of the SabReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "SabReader.h"
#include "SCode.h"
#include "base64_convert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SabReader::SabReader()
{
	// sab use big endian
	setBigEndian();
	m_fileBuf = NULL;
	m_objSabPropList.reserve(10);

	// Set m_magic_slot_type_table
//	m_magic_slot_type_table[SLOT_TYPEID_VOIDID]   = MAGIC_SLOT_TYPE_VOID  ;
//	m_magic_slot_type_table[SLOT_TYPEID_BOOLID]   = MAGIC_SLOT_TYPE_BOOL  ;
//    m_magic_slot_type_table[SLOT_TYPEID_BYTEID]   = MAGIC_SLOT_TYPE_BYTE  ;
//    m_magic_slot_type_table[SLOT_TYPEID_SHORTID]  = MAGIC_SLOT_TYPE_SHORT ;
//    m_magic_slot_type_table[SLOT_TYPEID_INTID]    = MAGIC_SLOT_TYPE_INT   ;
//    m_magic_slot_type_table[SLOT_TYPEID_LONGID]   = MAGIC_SLOT_TYPE_LONG  ;
//    m_magic_slot_type_table[SLOT_TYPEID_FLOATID]  = MAGIC_SLOT_TYPE_FLOAT ;
//    m_magic_slot_type_table[SLOT_TYPEID_DOUBLEID] = MAGIC_SLOT_TYPE_DOUBLE;
//    m_magic_slot_type_table[SLOT_TYPEID_BUFID]    = MAGIC_SLOT_TYPE_BUF   ;
//	memset(&m_magic_slot_type_table, 0x00, sizeof(unsigned int) * (SLOT_TYPEID_MAXID + 1));
}

SabReader::~SabReader()
{

}

int  SabReader::getMagicSlotTypebySlotTypeID(unsigned int  slotTypeID)
{
	return m_objSCodeReader.getMagicSlotTypebySlotTypeID(slotTypeID);
}


int  SabReader::getSlotTypeIDbyMagicSlotType(unsigned int  magicSlotType)
{
	return m_objSCodeReader.getSlotTypeIDbyMagicSlotType(magicSlotType);
}

bool SabReader::loadSCodeFile(char * cFileName)
{
    bool bRet = m_objSCodeReader.readSCodeFile(cFileName);
    return bRet;
}

void SabReader::unloadSCodeFile()
{
	m_objSCodeReader.releaseSCodeFileBuffer();
}

void SabReader::setAndSkipUnsignedShortValueToBuf(unsigned char ** cBuffer, unsigned short uValue)
{
	if (m_bBigEndian)
	{
		(*cBuffer)[0] = (uValue >> 8) & 0xFF;
		(*cBuffer)[1] =  uValue & 0xFF;
	}
	else
	{
		(*cBuffer)[1] = (uValue >> 8) & 0xFF;
		(*cBuffer)[0] =  uValue & 0xFF;
	}
	*cBuffer += 2;
}

void SabReader::setAndSkipUnsignedIntValueToBuf(unsigned char ** cBuffer, unsigned int uValue)
{
	if (m_bBigEndian)
	{
		(*cBuffer)[0] = (uValue >> 24) & 0xFF;
		(*cBuffer)[1] = (uValue >> 16) & 0xFF;
		(*cBuffer)[2] = (uValue >> 8)  & 0xFF;
		(*cBuffer)[3] =  uValue & 0xFF;
	}
	else
	{
		(*cBuffer)[3] = (uValue >> 24) & 0xFF;
		(*cBuffer)[2] = (uValue >> 16) & 0xFF;
		(*cBuffer)[1] = (uValue >> 8)  & 0xFF;
		(*cBuffer)[0] =  uValue & 0xFF;
	}
	*cBuffer += 4;
}

void SabReader::setAndSkipUnsignedLongValueToBuf(unsigned char ** cBuffer, unsigned _int64 uValue)
{
	if (m_bBigEndian)
	{
		(*cBuffer)[0] = (uValue >> 56) & 0xFF;
		(*cBuffer)[1] = (uValue >> 48) & 0xFF;
		(*cBuffer)[2] = (uValue >> 40) & 0xFF;
		(*cBuffer)[3] = (uValue >> 32) & 0xFF;
		(*cBuffer)[4] = (uValue >> 24) & 0xFF;
		(*cBuffer)[5] = (uValue >> 16) & 0xFF;
		(*cBuffer)[6] = (uValue >> 8)  & 0xFF;
		(*cBuffer)[7] =  uValue & 0xFF;
	}
	else
	{
		(*cBuffer)[7] = (uValue >> 56) & 0xFF;
		(*cBuffer)[6] = (uValue >> 48) & 0xFF;
		(*cBuffer)[5] = (uValue >> 40) & 0xFF;
		(*cBuffer)[4] = (uValue >> 32) & 0xFF;
		(*cBuffer)[3] = (uValue >> 24) & 0xFF;
		(*cBuffer)[2] = (uValue >> 16) & 0xFF;
		(*cBuffer)[1] = (uValue >> 8)  & 0xFF;
		(*cBuffer)[0] =  uValue & 0xFF;
	}
	*cBuffer += 8;
}

unsigned short SabReader::calcAndSkipUnsignedShortValue(unsigned char ** cBuf)
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

unsigned int SabReader::calcAndSkipUnsignedIntValue(unsigned char ** cBuf)
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

#pragma warning(disable : 4244)
unsigned long  SabReader::calcAndSkipUnsignedLongValue(unsigned char ** cBuf)
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
int SabReader::readSabFile(char * cFileName)
{
	int iRet = SAB_OK;
	if (m_objSCodeReader.getSCodeKits() == NULL)
	{
		printf("We need the scode file.");
		return SAB_MISS_SCODE;
	}
	int iFileLen = m_objFileReader.readSCodeFileBuffer(&m_fileBuf, cFileName);
	if (!m_fileBuf)
	{
		printf("We need the sab file.");
		return SAB_MISS_SAB;
	}
	m_cFileBufPtr = m_fileBuf;
	memcpy(m_head_magic, m_cFileBufPtr, 4);
	m_cFileBufPtr += 4;
	memcpy(m_head_Version, m_cFileBufPtr, 4);
	m_cFileBufPtr += 4;
	loadSchema();
	iRet = createSchemaAssociationTable();
	if (iRet != SAB_OK)
	{
		m_objFileReader.freeSCodeFileBuffer(&m_fileBuf);
		return iRet;
	}
	printSchema();
	m_maxid = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
	// components
	iRet = loadComponents();
	if (iRet != SAB_OK)
	{
		m_objFileReader.freeSCodeFileBuffer(&m_fileBuf);
		return iRet;
	}
	if ((m_fileBuf + iFileLen - m_cFileBufPtr) < 4)
	{
		printf("Left space is %d and stop analyze \r\n",
			(m_fileBuf + iFileLen - m_cFileBufPtr));
		m_objFileReader.freeSCodeFileBuffer(&m_fileBuf);
		return SAB_OK;
	}
	// links
	loadLinks();
	printLinks();
	if ((m_fileBuf + iFileLen - m_cFileBufPtr) < 4)
	{
		printf("Left space is %d and stop analyze \r\n",
			(m_fileBuf + iFileLen - m_cFileBufPtr));
		m_objFileReader.freeSCodeFileBuffer(&m_fileBuf);
		return SAB_OK;
	}
	m_objFileReader.freeSCodeFileBuffer(&m_fileBuf);
	return SAB_OK;
}

void SabReader::releaseSabFileBuffer()
{
	if (m_schema_kit_list)
	{
		free(m_schema_kit_list);
	}
	if (m_objSchemaAssociationTable)
	{
		free(m_objSchemaAssociationTable);
	}
	m_objSabPropList.clear();
	m_objSabLinkList.clear();
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
	m_schema_kit_list = (SCHEMA_KIT *)malloc(m_numkits * sizeof(SCHEMA_KIT));
	memset(m_schema_kit_list, 0x00, m_numkits * sizeof(SCHEMA_KIT));
	for (int i = 0 ; i < m_numkits; i++)
	{
		memset(m_schema_kit_list[i].cName, 0x00, SAB_NAME_LEN);
		strcpy((char *)m_schema_kit_list[i].cName, (char *)m_cFileBufPtr);
		m_cFileBufPtr += strlen((char *)m_cFileBufPtr) + 1;
		m_schema_kit_list[i].checksum        = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);
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

int SabReader::createSchemaAssociationTable()
{

	SCODE_KIT *  objKitsList = m_objSCodeReader.getSCodeKits();
    int         numberOfKits = m_objSCodeReader.getNumberOfKits();

	m_objSchemaAssociationTable = (int *)malloc((numberOfKits + 1) * sizeof(int));
	for (int i=0; i< m_numkits; ++i)
    {
		for (int j=0; j< numberOfKits; ++j)
		{
			if (stricmp((char *)m_schema_kit_list[i].cName, (char *)objKitsList[j].cName) == 0)
			{
				if (m_schema_kit_list[i].checksum != objKitsList[j].checksum)
				{
					return SAB_APPCOMP_NG;
				}
				m_objSchemaAssociationTable[i] = j;
				break;
			}
		}
	}
	return SAB_OK;
}

int SabReader::encodePropsToBuf(unsigned char * cPropsBuf, SCODE_KIT_TYPE * objSCodeKitType,
										unsigned char filter, 
						                SAB_PROP_VALUE *       configProps, int configPropsLen)
{
	unsigned char * cBuf = cPropsBuf;
	int iConfigPropsIdx = 0;
	for (int k=0; k < objSCodeKitType->slotsLen; k++)
	{
		// if (m_objSCodeReader.matchRtFlagsProp(RTFLAGS_FILTER_CONFIG, objSCodeKitType->kit_type_slots_list[k].rtFlags))
		if (m_objSCodeReader.matchRtFlagsProp(filter, objSCodeKitType->kit_type_slots_list[k].rtFlags))
		{
			if(iConfigPropsIdx > configPropsLen - 1)
				return cBuf - cPropsBuf;
			else
			{
				encodeOnePropToBuf(&cBuf, configProps[iConfigPropsIdx],
					objSCodeKitType->kit_type_slots_list[k].fpBix,
					m_objSCodeReader.isRtFlagsAsStr(objSCodeKitType->kit_type_slots_list[k].rtFlags));
				iConfigPropsIdx++;
			}
		}
	}

	return cBuf - cPropsBuf;
}

int SabReader::decodeAndSkipPropsFromBuf(unsigned char ** cPropsBuf, SCODE_KIT_TYPE * objSCodeKitType, 
										unsigned char filter, 
						                SAB_PROP_ELEMENT *       configProps)
{
	int iSeq = 0;
	unsigned char * cBuf = *cPropsBuf;
	for (int k=0; k < objSCodeKitType->slotsLen; k++)
	{
		// if (m_objSCodeReader.matchRtFlagsProp(RTFLAGS_FILTER_CONFIG, objSCodeKitType->kit_type_slots_list[k].rtFlags))
		if (m_objSCodeReader.matchRtFlagsProp(filter, objSCodeKitType->kit_type_slots_list[k].rtFlags))
		{
			strcpy((char *)configProps[iSeq].cPropName, (char *)objSCodeKitType->kit_type_slots_list[k].cPropName);
			configProps[iSeq].cValue.propType = objSCodeKitType->kit_type_slots_list[k].fpBix;
			decodeAndSkipOnePropFromBuf(&cBuf, configProps[iSeq].cValue,
				objSCodeKitType->kit_type_slots_list[k].fpBix);
				//, m_objSCodeReader.isRtFlagsAsStr(objSCodeKitType->kit_type_slots_list[k].rtFlags));
			iSeq++;
		}
	}
	return iSeq;
}

bool SabReader::decodeAndSkipOnePropFromBuf(unsigned char ** cBuf, SAB_PROP_VALUE& configProps,
						 int         iFpBix)// ,      bool isStr)
{
	int slotTypeID = getSlotTypeIDbyMagicSlotType(iFpBix);
    // switch (iFpBix)
	switch (slotTypeID)
    {
	// case MAGIC_SLOT_TYPE_VOID	:
	case SLOT_TYPEID_VOIDID	:
		break;
	// case MAGIC_SLOT_TYPE_BOOL	:
	case SLOT_TYPEID_BOOLID	:
		if ((*cBuf)[0] == '\0')
		{
			configProps.bRet = true;
		}
		else
		{
			configProps.bRet = false;
		}
		(*cBuf)++;
		break;
	// case MAGIC_SLOT_TYPE_INT	:
	case SLOT_TYPEID_INTID	:
		configProps.uRet = calcAndSkipUnsignedIntValue(cBuf);
		break;
	// case MAGIC_SLOT_TYPE_LONG	:
	case SLOT_TYPEID_LONGID	:
		configProps.ulRet = calcAndSkipUnsignedLongValue(cBuf);
		break;
	// case MAGIC_SLOT_TYPE_FLOAT	:
	case SLOT_TYPEID_FLOATID	:
		configProps.ufRet = intBitsToFloat(calcAndSkipUnsignedIntValue(cBuf));
		break;
	// case MAGIC_SLOT_TYPE_DOUBLE	:
	case SLOT_TYPEID_DOUBLEID	:
		configProps.udRet = longBitsToDouble(calcAndSkipUnsignedLongValue(cBuf));
		break;
	// case MAGIC_SLOT_TYPE_BUF	:
	case SLOT_TYPEID_BUFID	:
		configProps.uBufLen = calcAndSkipUnsignedShortValue(cBuf);
		memcpy(configProps.cBuf, *cBuf, configProps.uBufLen);
		*cBuf += configProps.uBufLen;
		break;
	// case MAGIC_SLOT_TYPE_BYTE	:
	case SLOT_TYPEID_BYTEID	:
		configProps.cByte = (*cBuf)[0];
		(*cBuf)++;
		break;
	// case MAGIC_SLOT_TYPE_SHORT	:
	case SLOT_TYPEID_SHORTID	:
		configProps.shortRet = calcAndSkipUnsignedShortValue(cBuf);
		break;
    }	
	return true;
}

bool SabReader::encodeOnePropToBuf(unsigned char ** cBuf, SAB_PROP_VALUE configProps,
						 int         iFpBix,      bool isStr)
{
	int uRetConvert = 0;
	unsigned _int64  ullRetConvert = 0;
	int slotTypeID = getSlotTypeIDbyMagicSlotType(iFpBix);
    // switch (iFpBix)
	switch (slotTypeID)
    {
	// case MAGIC_SLOT_TYPE_VOID	:
	case SLOT_TYPEID_VOIDID	:
		break;
	// case MAGIC_SLOT_TYPE_BOOL	:
	case SLOT_TYPEID_BOOLID	:
		if (configProps.bRet)
		{
			memcpy(*cBuf, "\0", 1);
		}
		else
		{
			memcpy(*cBuf, "\1", 1);
		}
		*cBuf++;
		break;
	// case MAGIC_SLOT_TYPE_INT	:
	case SLOT_TYPEID_INTID	:
		// memcpy(*cBuf, (char *)(&configProps.uRet), sizeof(int));
		setAndSkipUnsignedIntValueToBuf(cBuf, configProps.uRet);
		*cBuf += sizeof(int);
		break;
	// case MAGIC_SLOT_TYPE_LONG	:
	case SLOT_TYPEID_LONGID	:
		// memcpy(*cBuf, (char *)(&configProps.ulRet), sizeof(long));
		setAndSkipUnsignedIntValueToBuf(cBuf, configProps.ulRet);
		*cBuf += sizeof(long);
		break;
	// case MAGIC_SLOT_TYPE_FLOAT	:
	case SLOT_TYPEID_FLOATID	:
		memcpy(&uRetConvert, (char *)(&configProps.ufRet), sizeof(int));
		// memcpy(*cBuf, (char *)(&configProps.ufRet), sizeof(int));
		setAndSkipUnsignedIntValueToBuf(cBuf, uRetConvert);
		*cBuf += sizeof(int);
		break;
	// case MAGIC_SLOT_TYPE_DOUBLE	:
	case SLOT_TYPEID_DOUBLEID	:
		memcpy(&ullRetConvert, (char *)(&configProps.udRet), sizeof(ullRetConvert));
		// memcpy(*cBuf, (char *)(&configProps.udRet), sizeof(long));
		setAndSkipUnsignedIntValueToBuf(cBuf, ullRetConvert);
		*cBuf += sizeof(_int64);
		break;
	// case MAGIC_SLOT_TYPE_BUF	:
	case SLOT_TYPEID_BUFID	:
		setAndSkipUnsignedShortValueToBuf(cBuf, (unsigned short)configProps.uBufLen);
		memcpy(*cBuf, configProps.cBuf, configProps.uBufLen);
		*cBuf += configProps.uBufLen;
		break;
	// case MAGIC_SLOT_TYPE_BYTE	:
	case SLOT_TYPEID_BYTEID	:
		memcpy(*cBuf, (char *)(&configProps.cByte), 1);
		*cBuf++;
		break;
	// case MAGIC_SLOT_TYPE_SHORT	:
	case SLOT_TYPEID_SHORTID	:
		memcpy(*cBuf, (char *)(&configProps.shortRet), sizeof(short));
		*cBuf += sizeof(short);
		break;
    }

	return true;
}

typedef unsigned char       BYTE;
int  SabReader::calcPropSize(int iFpBix, SAB_PROP_VALUE& configProps)
{
	int slotTypeID = getSlotTypeIDbyMagicSlotType(iFpBix);
    // switch (iFpBix)
	switch (slotTypeID)
    {
	// case MAGIC_SLOT_TYPE_VOID	:
	case SLOT_TYPEID_VOIDID	:
		return 0;
	// case MAGIC_SLOT_TYPE_BOOL	:
	case SLOT_TYPEID_BOOLID	:
		return 1;  // bool is a char in sedona
	// case MAGIC_SLOT_TYPE_INT	:
	case SLOT_TYPEID_INTID	:
		return sizeof(int);
	// case MAGIC_SLOT_TYPE_LONG	:
	case SLOT_TYPEID_LONGID	:
		return sizeof(long);
	// case MAGIC_SLOT_TYPE_FLOAT	:
	case SLOT_TYPEID_FLOATID	:
		return sizeof(int);
	// case MAGIC_SLOT_TYPE_DOUBLE	:
	case SLOT_TYPEID_DOUBLEID	:
		return sizeof(long);
	// case MAGIC_SLOT_TYPE_BUF	:
	case SLOT_TYPEID_BUFID	:
		return  configProps.uBufLen + sizeof(short);
	// case MAGIC_SLOT_TYPE_BYTE	:
	case SLOT_TYPEID_BYTEID	:
		return sizeof(BYTE);
	// case MAGIC_SLOT_TYPE_SHORT	:
	case SLOT_TYPEID_SHORTID	:
		return sizeof(short);
    }
	return 0;
}

bool  SabReader::getParentNameByCompID(unsigned short   compID, unsigned char * cParentName)
{
	std::vector<SAB_PROP>::iterator iter;
	for (iter = m_objSabPropList.begin(); iter != m_objSabPropList.end(); iter++)
	{
		if(iter->objSabCompInfo.compID == compID)
		{
			return getComponentNameByCompID(iter->objSabCompInfo.parentID, cParentName);
		}
	}
	return false;

}
bool  SabReader::getComponentNameByCompID(unsigned short compID, unsigned char * cName)
{
	std::vector<SAB_PROP>::iterator iter;
	for (iter = m_objSabPropList.begin(); iter != m_objSabPropList.end(); iter++)
	{
		if(iter->objSabCompInfo.compID == compID)
		{
			strcpy((char *)cName, (char *)iter->objSabCompInfo.cName);
			return true;
		}
	}
	return false;
}

bool  SabReader::getSabPropByCompIDAndSlotID(unsigned short   compID, unsigned short slotID, SAB_PROP& objSabProp)
{
	std::vector<SAB_PROP>::iterator iter;
	for (iter = m_objSabPropList.begin(); iter != m_objSabPropList.end(); iter++)
	{
		if((iter->objSabCompInfo.compID == compID) && (iter->iSlotID == slotID)) //  
		{
			objSabProp = *iter;
			return true;
		}
	}
	return false;
}

bool  SabReader::getComponentNameAndTypeNameByCompID(unsigned short compID, unsigned char * cName, unsigned char * cTypeName)
{
	std::vector<SAB_PROP>::iterator iter;
	for (iter = m_objSabPropList.begin(); iter != m_objSabPropList.end(); iter++)
	{
		if(iter->objSabCompInfo.compID == compID)
		{
			strcpy((char *)cName, (char *)iter->objSabCompInfo.cName);
			strcpy((char *)cTypeName, (char *)iter->cTypeName);
			return true;
		}
	}
	return false;
}

SCODE_KIT_TYPE * SabReader::getScodeKitTypeByCompID(unsigned short   compID)
{
	std::vector<SAB_PROP>::iterator iter;
	for (iter = m_objSabPropList.begin(); iter != m_objSabPropList.end(); iter++)
	{
		if(iter->objSabCompInfo.compID == compID)
		{
			return getScodeKitTypeByKitIDAndTypeID(iter->objSabCompInfo.kitID,
							     iter->objSabCompInfo.typeID);
		}
	}
	return NULL;
}

bool SabReader::getSlotNameByCompIDAndSlotID(unsigned short   compID, unsigned short slotID, unsigned char * cSlotName)
{
	SCODE_KIT_TYPE * objScodeKitTypePtr = NULL;
	std::vector<SAB_PROP>::iterator iter;
	for (iter = m_objSabPropList.begin(); iter != m_objSabPropList.end(); iter++)
	{
		if(iter->objSabCompInfo.compID == compID)
		{
			objScodeKitTypePtr = getScodeKitTypeByKitIDAndTypeID(iter->objSabCompInfo.kitID,
																iter->objSabCompInfo.typeID);
			if(slotID < objScodeKitTypePtr->nameBix)
			{
				strcpy((char *)cSlotName, (char *)objScodeKitTypePtr->kit_type_slots_list[slotID].cPropName);
				return true;
			}
			return false;
		}
	}
	return false;
}

bool SabReader::getLinkNameByCompIDAndSlotID(unsigned short compID, unsigned short slotID, unsigned char * cLinkName)
{
	unsigned char cParentName[SCODE_NAME_LEN];
	unsigned char cComponentName[SCODE_NAME_LEN];
	unsigned char cSlotName[SCODE_NAME_LEN];

	memset(cParentName, 0x00, SCODE_NAME_LEN);
	memset(cComponentName, 0x00, SCODE_NAME_LEN);
	memset(cSlotName, 0x00, SCODE_NAME_LEN);

	getParentNameByCompID(compID, cParentName);
	getComponentNameByCompID(compID, cComponentName);
	getSlotNameByCompIDAndSlotID(compID, slotID, cSlotName);
	sprintf((char *)cLinkName, "/%s/%s.%s", cParentName, cComponentName, cSlotName);
	return true;
}

SCODE_KIT_TYPE * SabReader::getScodeKitTypeByKitIDAndTypeID(unsigned char   kitId,
							     unsigned char   typeId)
{
	SCODE_KIT * objKitsList = m_objSCodeReader.getSCodeKits();
    int         numberOfKits = m_objSCodeReader.getNumberOfKits();
	if(kitId >= numberOfKits)
	{
		return NULL;
	}
	if (m_objSchemaAssociationTable)
	{
		int iAssociationIdx = m_objSchemaAssociationTable[kitId];
		return &objKitsList[iAssociationIdx].kit_type_list[typeId];
	}
	return NULL;
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
int SabReader::loadComponents()
{
	int iRet = SAB_OK;
	SAB_COMP_INFO objSabCompInfo;
	while (1)
	{
		memset(&objSabCompInfo, 0x00, sizeof(SAB_COMP_INFO));
		objSabCompInfo.compID = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		if ((objSabCompInfo.compID == 0xFFFF) || (objSabCompInfo.compID < 0))
			break;
		objSabCompInfo.kitID  = m_cFileBufPtr[0];
		objSabCompInfo.typeID = m_cFileBufPtr[1];
		m_cFileBufPtr += 2;
		iRet = loadAppComp(objSabCompInfo);
		if (iRet == SAB_APPCOMP_NG)
		{
			break;
		}
	}
	return iRet;
}

int SabReader::loadAppComp(SAB_COMP_INFO& objSabCompInfo)
{
	char cType[SAB_TYPE_NAME_LEN];
	SCODE_KIT_TYPE * objSCodeKitType = NULL;
	strcpy((char *)objSabCompInfo.cName, (char *)m_cFileBufPtr);
	m_cFileBufPtr += strlen((char *)m_cFileBufPtr) + 1;

	objSabCompInfo.parentID      = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
	objSabCompInfo.numChildren   = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
	objSabCompInfo.nextSiblingID = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);

	SCODE_KIT *  objKitsList = m_objSCodeReader.getSCodeKits();
    int         numberOfKits = m_objSCodeReader.getNumberOfKits();

	objSCodeKitType = NULL;

	if(objSabCompInfo.kitID >= m_numkits)
	{
		return SAB_APPCOMP_NG;
	}
	int iAssociationIdx = m_objSchemaAssociationTable[objSabCompInfo.kitID];

	objSCodeKitType = &objKitsList[iAssociationIdx].kit_type_list[objSabCompInfo.typeID];
	if (objSCodeKitType)
	{
		memset(cType, 0x00, SAB_TYPE_NAME_LEN);
		sprintf(cType, "%s::%s", objKitsList[iAssociationIdx].cName,
			objKitsList[iAssociationIdx].kit_type_list[objSabCompInfo.typeID].cName);
		printf("objSCodeKitType is %s \r\n", cType);
		loadProps(objSabCompInfo, objSCodeKitType, cType, RTFLAGS_FILTER_CONFIG);
		printSabProps();
		if (m_cFileBufPtr[0] == ';')
		{
			m_cFileBufPtr++;
			return SAB_OK;
		}
		else
		{
			return SAB_APPCOMP_NG;
		}
	}
	return SAB_APPCOMP_NG;
}

void SabReader::printSingleSabProp(SAB_PROP_VALUE& objSabPropValue, unsigned char * cPropName)
{
	char base64[1024];
	int slotTypeID = getSlotTypeIDbyMagicSlotType(objSabPropValue.propType);
    // switch (objSabPropValue.propType)
	switch (slotTypeID)
	{
	// case MAGIC_SLOT_TYPE_VOID	:
	case SLOT_TYPEID_VOIDID	:
		break;
	// case MAGIC_SLOT_TYPE_BOOL	:
	case SLOT_TYPEID_BOOLID	:
		printf("%s", objSabPropValue.bRet?"true":"false");
		break;
	// case MAGIC_SLOT_TYPE_INT	:
	case SLOT_TYPEID_INTID	:
		printf("%d", objSabPropValue.uRet);
		break;
	// case MAGIC_SLOT_TYPE_LONG	:
	case SLOT_TYPEID_LONGID	:
		printf("%l", objSabPropValue.ulRet);
		break;
	// case MAGIC_SLOT_TYPE_FLOAT	:
	case SLOT_TYPEID_FLOATID	:
		printf("%f", objSabPropValue.ufRet);
		break;
	// case MAGIC_SLOT_TYPE_DOUBLE	:
	case SLOT_TYPEID_DOUBLEID	:
		printf("%f", objSabPropValue.udRet);
		break;
	// case MAGIC_SLOT_TYPE_BUF	:
	case SLOT_TYPEID_BUFID	:
		memset(base64, 0x00, 1024);
		if (stricmp((char *)cPropName, "cred") == 0)
		{
			printf("%d : '%s'", objSabPropValue.uBufLen,
				base64_encode((const unsigned char *)objSabPropValue.cBuf,
											base64, objSabPropValue.uBufLen));
		}
		else
		{
			printf("%d : '%s'", objSabPropValue.uBufLen, objSabPropValue.cBuf);
		}
		break;
	// case MAGIC_SLOT_TYPE_BYTE	:
	case SLOT_TYPEID_BYTEID	:
		printf("%02X",objSabPropValue.cByte);
		break;
	// case MAGIC_SLOT_TYPE_SHORT	:
	case SLOT_TYPEID_SHORTID	:
		printf("%0d", objSabPropValue.shortRet);
		break;
	}	
}

void SabReader::printSabProps()
{
	std::vector<SAB_PROP>::iterator iter;
	printf("==================================== start ==================================== \r\n");
	for (iter = m_objSabPropList.begin(); iter != m_objSabPropList.end(); iter++)
	{
		printf("objSabCompInfo(compID, nameBix, cName, cTypeName, cPropName) = "
			"(%02d, %02d, %02d, %s, %s, %s), (propType) = (%03X) , (val) = (",
			iter->objSabCompInfo.compID, iter->objSabCompInfo.kitID,
			iter->objSabCompInfo.typeID, iter->objSabCompInfo.cName,
			iter->cTypeName, iter->cPropName,
			iter->objSoxProp.propType);
		printSingleSabProp(iter->objSoxProp, iter->cPropName);
		printf(") \r\n");
	}
	printf("====================================  end  ==================================== \r\n");
}

//  **
//  ** Load the property values to the output stream.
//  ** Filter:
//  **   0   = all
//  **   'c' = config only
//  **   'r' = runtime only
void SabReader::loadProps(SAB_COMP_INFO& objSabCompInfo,
                  SCODE_KIT_TYPE * objSCodeKitType, char* cTypeStr, char filter)
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
			objSabProp.iSlotID = k;
 			sprintf((char *)objSabProp.cTypeName, "%s", cTypeStr);
			sprintf((char *)objSabProp.cPropName, "%s", objSCodeKitType->kit_type_slots_list[k].cPropName);
			loadProp(objSabProp, objSCodeKitType->kit_type_slots_list[k].fpBix,
				m_objSCodeReader.isRtFlagsAsStr(objSCodeKitType->kit_type_slots_list[k].rtFlags));
			m_objSabPropList.push_back(objSabProp);
		}
	}

}

void SabReader::loadProp(SAB_PROP& objSabProp, int iFpBix, bool isStr)
{
	int slotTypeID = getSlotTypeIDbyMagicSlotType(iFpBix);
	objSabProp.objSoxProp.propType = iFpBix;
    // switch (iFpBix)
	switch (slotTypeID)
    {
	// case MAGIC_SLOT_TYPE_VOID	:
	case SLOT_TYPEID_VOIDID	:
		break;
	// case MAGIC_SLOT_TYPE_BOOL	:
	case SLOT_TYPEID_BOOLID	:
		objSabProp.objSoxProp.bRet = (m_cFileBufPtr[0]!= 0);
		m_cFileBufPtr ++;
		break;
	// case MAGIC_SLOT_TYPE_INT	:
	case SLOT_TYPEID_INTID	:
		objSabProp.objSoxProp.uRet = calcAndSkipUnsignedIntValue(&m_cFileBufPtr);
		break;
	// case MAGIC_SLOT_TYPE_LONG	:
	case SLOT_TYPEID_LONGID	:
		objSabProp.objSoxProp.ulRet = calcAndSkipUnsignedLongValue(&m_cFileBufPtr);
		break;
	// case MAGIC_SLOT_TYPE_FLOAT	:
	case SLOT_TYPEID_FLOATID	:
		objSabProp.objSoxProp.ufRet = intBitsToFloat(calcAndSkipUnsignedIntValue(&m_cFileBufPtr));
		break;
	// case MAGIC_SLOT_TYPE_DOUBLE	:
	case SLOT_TYPEID_DOUBLEID	:
		objSabProp.objSoxProp.udRet = longBitsToDouble(calcAndSkipUnsignedLongValue(&m_cFileBufPtr));
		break;
	// case MAGIC_SLOT_TYPE_BUF	:
	case SLOT_TYPEID_BUFID	:
		objSabProp.objSoxProp.uBufLen  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		if (objSabProp.objSoxProp.uBufLen > (m_cFileBufPtr - m_fileBuf))
		{
			objSabProp.objSoxProp.uBufLen = m_cFileBufPtr - m_fileBuf;
		}
		memset(objSabProp.objSoxProp.cBuf, 0x00, 1024);
		memcpy(objSabProp.objSoxProp.cBuf, m_cFileBufPtr, objSabProp.objSoxProp.uBufLen);
		m_cFileBufPtr += objSabProp.objSoxProp.uBufLen;
		if(isStr)
		{
			objSabProp.objSoxProp.cBuf[objSabProp.objSoxProp.uBufLen - 1] = '\0';
		}
		break;
	// case MAGIC_SLOT_TYPE_BYTE	:
	case SLOT_TYPEID_BYTEID	:
		objSabProp.objSoxProp.cByte = m_cFileBufPtr[0];
		m_cFileBufPtr ++;
		break;
	// case MAGIC_SLOT_TYPE_SHORT	:
	case SLOT_TYPEID_SHORTID	:
		objSabProp.objSoxProp.shortRet = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		break;
    }
}

void SabReader::loadLinks()
{
	SAB_LINK objSabLinkInfo;
	while (1)
	{
		memset(&objSabLinkInfo, 0x00, sizeof(SAB_LINK));
		objSabLinkInfo.fromComp  = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		if ((objSabLinkInfo.fromComp == 0xFFFF) || (objSabLinkInfo.fromComp < 0))
			break;
		objSabLinkInfo.fromSlot  = m_cFileBufPtr[0];
		m_cFileBufPtr++;
		getLinkNameByCompIDAndSlotID(objSabLinkInfo.fromComp, objSabLinkInfo.fromSlot, objSabLinkInfo.cFromLinkName);
		objSabLinkInfo.toComp    = calcAndSkipUnsignedShortValue(&m_cFileBufPtr);
		objSabLinkInfo.toSlot    = m_cFileBufPtr[0];
		m_cFileBufPtr++;
		getLinkNameByCompIDAndSlotID(objSabLinkInfo.toComp, objSabLinkInfo.toSlot, objSabLinkInfo.cToLinkName);
		m_objSabLinkList.push_back(objSabLinkInfo);
	}
}

void SabReader::printLinks()
{
	std::vector<SAB_LINK>::iterator iter;
	printf("==================================== start ==================================== \r\n");
	for (iter = m_objSabLinkList.begin(); iter != m_objSabLinkList.end(); iter++)
	{
		printf("\t ");
		printSingleSabLink(*iter);
		printf(". \r\n");
	}
	printf("====================================  end  ==================================== \r\n");
}

void SabReader::printSingleSabLink(SAB_LINK& objSabLink)
{
	printf("SAB_LINK(fromComp, fromSlot, cFromLinkName, toComp, toSlot, cFromLinkName) = (%02d, %02d, %s, %02d, %02d, %s)",
			objSabLink.fromComp, objSabLink.fromSlot, objSabLink.cFromLinkName, 
			objSabLink.toComp,   objSabLink.toSlot,   objSabLink.cToLinkName);
}

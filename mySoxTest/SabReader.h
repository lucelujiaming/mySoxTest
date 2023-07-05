// SabReader.h: interface for the SabReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SABREADER_H__B4EE535F_3490_4897_A9A6_2888CD958741__INCLUDED_)
#define AFX_SABREADER_H__B4EE535F_3490_4897_A9A6_2888CD958741__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileReader.h"
#include "SCodeReader.h"


#define SAB_OK           0
#define SAB_APPCOMP_NG   1
#define SAB_MISS_SCODE   2
#define SAB_MISS_SAB     3

#define SAB_NAME_LEN          16 // (8 * 2)
#define SAB_TYPE_NAME_LEN     128 
#define SAB_PROP_VALUE_LEN    1024
#define SAB_KIT_VERSION_LEN   128 

typedef struct _SCHEMA_KIT
{
	unsigned char       cName[SAB_NAME_LEN];
	unsigned int        checksum;
} SCHEMA_KIT, *PSCHEMA_KIT;

typedef struct _SAB_LINK
{
	unsigned int     fromComp;
	unsigned int     fromSlot;
	unsigned char    cFromLinkName[SAB_TYPE_NAME_LEN];
	unsigned int     toComp;
	unsigned int     toSlot;
	unsigned char    cToLinkName[SAB_TYPE_NAME_LEN];
} SAB_LINK, *PSAB_LINK;

typedef struct _SAB_COMP_INFO
{
	unsigned short     compID;
	unsigned char      kitID;
	unsigned char      typeID;
	unsigned char      cName[SAB_NAME_LEN];
	unsigned short     parentID;
	unsigned short     numChildren;
	unsigned short     nextSiblingID;
} SAB_COMP_INFO, *PSAB_COMP_INFO;

typedef struct _SAB_PROP_VALUE
{
	unsigned int        propId;
	unsigned int        propType;
	unsigned int        uBufLen ;
	union {
		bool            bRet ;
		unsigned char   cByte ;
		unsigned short  shortRet ;
		unsigned int    uRet ;
		unsigned long   ulRet ;
		float           ufRet ;
		double          udRet ;
		char            cBuf[SAB_PROP_VALUE_LEN] ;
	};
} SAB_PROP_VALUE, *PSAB_PROP_VALUE;

typedef struct _SAB_PROP_ELEMENT
{
	unsigned char    cPropName[SAB_TYPE_NAME_LEN];
	SAB_PROP_VALUE   cValue;
} SAB_PROP_ELEMENT, *PSAB_PROP_ELEMENT;

typedef struct _SAB_PROP
{
	SAB_COMP_INFO        objSabCompInfo;
    unsigned int         iSlotID;
	unsigned char        cTypeName[SAB_TYPE_NAME_LEN];
	unsigned char        cPropName[SAB_TYPE_NAME_LEN];
	SAB_PROP_VALUE       objSoxProp;
} SAB_PROP, *PSAB_PROP;


// Based on App.sedona
class SabReader  
{
public:
	bool loadSCodeFile(char * cFileName);
	void unloadSCodeFile();
	
	int    getMagicSlotTypebySlotTypeID(unsigned int  slotTypeID);
	int    getSlotTypeIDbyMagicSlotType(unsigned int  magicSlotType);
	// SCODE_KIT * getSCodeKits()    { return m_objSCodeReader.getSCodeKits(); }
	unsigned char * getSCodeKitName(unsigned int iCompID) { return m_objSCodeReader.getSCodeKitName(iCompID); }
	int         getNumberOfKits() { return m_objSCodeReader.getNumberOfKits(); }
	bool           isSCodeKitsGet()  { return (m_objSCodeReader.getSCodeKits() != NULL); }
	SCODE_KIT_TYPE * getScodeKitTypeByKitIDAndTypeID(unsigned char   kitID, unsigned char   typeID);
	SCODE_KIT_TYPE * getScodeKitTypeByCompID(unsigned short   compID);
	//
	bool    getSabPropByCompIDAndSlotID(unsigned short   compID, unsigned short slotID, SAB_PROP& objSabProp);
	bool    getComponentNameAndTypeNameByCompID(unsigned short   compID, unsigned char * cName, unsigned char * cTypeName);
	bool    getComponentNameByCompID(unsigned short   compID, unsigned char * cName);
	bool    getParentNameByCompID(unsigned short   compID, unsigned unsigned char * cParentName);
	bool    getSlotNameByCompIDAndSlotID(unsigned short   compID, unsigned short slotID, unsigned char * cSlotName);
	bool    getLinkNameByCompIDAndSlotID(unsigned short   compID, unsigned short slotID, unsigned char * cLinkName);
	//
	int     encodePropsToBuf(unsigned char * cPropsBuf, SCODE_KIT_TYPE * objSCodeKitType, 
										unsigned char filter, 
						                SAB_PROP_VALUE *       configProps, int configPropsLen);
	int     decodeAndSkipPropsFromBuf(unsigned char ** cPropsBuf, SCODE_KIT_TYPE * objSCodeKitType, 
										unsigned char filter, 
						                SAB_PROP_ELEMENT *      configProps);
	int     calcPropSize(int iFpBix, SAB_PROP_VALUE& configProps);
	
	int  readSabFile(char * cFileName);
	void releaseSabFileBuffer();
	SabReader();
	virtual ~SabReader();

	void printSingleSabProp(SAB_PROP_VALUE& objSabPropValue, unsigned char * cPropName);
	void printSingleSabLink(SAB_LINK& objSabLink);
	
	bool encodeOnePropToBuf(unsigned char ** cBuf, SAB_PROP_VALUE configProps, 
						 int         iFpBix,      bool isStr);
	bool decodeAndSkipOnePropFromBuf(unsigned char ** cBuf, SAB_PROP_VALUE& configProps,
						 int         iFpBix);// ,      bool isStr);
public:
	bool matchRtFlagsProp(unsigned char filter, unsigned int rtFlags) 
					{ return m_objSCodeReader.matchRtFlagsProp(filter, rtFlags); }
	bool isRtFlagsProp(unsigned int rtFlags)     { return m_objSCodeReader.isRtFlagsProp(rtFlags); }
	bool isRtFlagsAction(unsigned int rtFlags)   { return m_objSCodeReader.isRtFlagsAction(rtFlags); }
	bool isRtFlagsConfig(unsigned int rtFlags)   { return m_objSCodeReader.isRtFlagsConfig(rtFlags); }
	bool isRtFlagsAsStr(unsigned int rtFlags)    { return m_objSCodeReader.isRtFlagsAsStr(rtFlags); }
	bool isRtFlagsOperator(unsigned int rtFlags) { return m_objSCodeReader.isRtFlagsOperator(rtFlags); }
	
private:
	void loadSchema();
	int  loadComponents();
	int  loadAppComp(SAB_COMP_INFO& objSabCompInfo);
	void loadProps(SAB_COMP_INFO& objSabCompInfo, 
	        SCODE_KIT_TYPE * objSCodeKitType, char* cTypeStr, char filter);
	void loadProp(SAB_PROP& objSabProp, int iFpBix, bool isStr);
	void printSabProps();
	void printSchema();
	int  createSchemaAssociationTable();
	
	void loadLinks();
	void printLinks();

public:
	void           setBigEndian()    { m_bBigEndian = true; }
	void           setLittleEndian() { m_bBigEndian = false; }
	bool           isBigEndian()     { return (m_bBigEndian == true);  }
	bool           isLittleEndian()  { return (m_bBigEndian == false); }

private:
	bool           m_bBigEndian;
	unsigned short calcAndSkipUnsignedShortValue(unsigned char ** cBuf);
	unsigned int   calcAndSkipUnsignedIntValue(unsigned char ** cBuf);
	unsigned long  calcAndSkipUnsignedLongValue(unsigned char ** cBuf);
	
	void setAndSkipUnsignedShortValueToBuf(unsigned char ** cBuffer, unsigned short uValue);
	void setAndSkipUnsignedIntValueToBuf(unsigned char ** cBuffer,   unsigned int uValue);
	void setAndSkipUnsignedLongValueToBuf(unsigned char ** cBuffer,  unsigned _int64 uValue);
	
private:
	FileReader         m_objFileReader;
	SCodeReader        m_objSCodeReader;
	
	unsigned char *    m_fileBuf;
	unsigned char *    m_cFileBufPtr;
	
	// head Data.
	unsigned char       m_head_magic[4]; 
	unsigned char       m_head_Version[4];
	unsigned char       m_numkits; 
	SCHEMA_KIT *        m_schema_kit_list;
	unsigned short      m_maxid; 
	
	int *               m_objSchemaAssociationTable;

	std::vector<SAB_PROP>    m_objSabPropList;
	std::vector<SAB_LINK>    m_objSabLinkList;

	// unsigned int             m_magic_slot_type_table[SLOT_TYPEID_MAXID + 1];
};

#endif // !defined(AFX_SABREADER_H__B4EE535F_3490_4897_A9A6_2888CD958741__INCLUDED_)

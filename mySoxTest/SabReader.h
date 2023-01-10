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

#define SAB_NAME_LEN		16 // (8 * 2)
#define SAB_TYPE_NAME_LEN   64 // (8 * 2)
#define SAB_PROP_VALUE_LEN  1024

// I got it by observation and I am not sure about it .
#define MAGIC_SLOT_TYPE_VOID			0x02DB
#define MAGIC_SLOT_TYPE_BOOL			0x02DD
#define MAGIC_SLOT_TYPE_INT			    0x02E3
#define MAGIC_SLOT_TYPE_LONG			0x02E5
#define MAGIC_SLOT_TYPE_FLOAT			0x02E7
#define MAGIC_SLOT_TYPE_DOUBLE			0x02E9
#define MAGIC_SLOT_TYPE_BUF			    0x055F
#define MAGIC_SLOT_TYPE_BYTE			0x02DF
#define MAGIC_SLOT_TYPE_SHORT			0x02E1

#define SLOT_TYPEID_VOIDID            0 
#define SLOT_TYPEID_BOOLID            1 
#define SLOT_TYPEID_BYTEID            2 
#define SLOT_TYPEID_SHORTID           3 
#define SLOT_TYPEID_INTID             4 
#define SLOT_TYPEID_LONGID            5 
#define SLOT_TYPEID_FLOATID           6 
#define SLOT_TYPEID_DOUBLEID          7 
#define SLOT_TYPEID_MAXPRIMITIVEID    7 
#define SLOT_TYPEID_BUFID             8 
#define SLOT_TYPEID_ERRORID           0xff 

typedef struct _SCHEMA_KIT
{
	char       cName[SAB_NAME_LEN];
	int        checksum;
} SCHEMA_KIT, *PSCHEMA_KIT;

typedef struct _SAB_LINK
{
	int      fromComp;
	int      fromSlot;
	int      toComp;
	int      toSlot;
} SAB_LINK, *PSAB_LINK;

typedef struct _SAB_COMP_INFO
{
	int      compID;
	int      kitID;
	int      typeID;
	char     cName[SAB_NAME_LEN];
} SAB_COMP_INFO, *PSAB_COMP_INFO;

typedef struct _SOX_PROP
{
	unsigned int         uBufLen ;
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
} SOX_PROP, *PSOX_PROP;

typedef struct _SAB_PROP
{
	SAB_COMP_INFO        objSabCompInfo;
	char                 cType[SAB_TYPE_NAME_LEN];
	char                 cPropName[SAB_TYPE_NAME_LEN];
	int                  propType;
	// SOX_PROP
	SOX_PROP             objSoxProp;
} SAB_PROP, *PSAB_PROP;


// Based on App.sedona
class SabReader  
{
public:
	bool loadSCodeFile(char * cFileName);
	void unloadSCodeFile();
	
	SCODE_KIT * getSCodeKits()    { return m_objSCodeReader.getSCodeKits(); }
	int         getNumberOfKits() { return m_objSCodeReader.getNumberOfKits(); }
	SCODE_KIT_TYPE * getScodeKitType(unsigned char   kitId, unsigned char   typeId);
	int  serializePropsBuf(unsigned char * cPropsBuf, SCODE_KIT_TYPE * objSCodeKitType, 
						                       SOX_PROP *       configProps, 
											   int configPropsLen);
	
	int  readSabFile(char * cFileName);
	void releaseSabFileBuffer();
	SabReader();
	virtual ~SabReader();


private:
	void loadSchema();
	int  loadComponents();
	int  loadAppComp(SAB_COMP_INFO& objSabCompInfo);
	void loadProps(SAB_COMP_INFO& objSabCompInfo, 
	        SCODE_KIT_TYPE * objSCodeKitType, char* cTypeStr, char filter);
	void loadProp(SAB_PROP& objSabProp, int iFpBix, bool isStr);
	void printProps();
	void printSchema();
	void createSchemaAssociationTable();
	
	void loadLinks();
	void printLinks();

	bool serializeProp(unsigned char ** cBuf, SOX_PROP configProps, 
						 int         iFpBix,      bool isStr);
public:
	void           setBigEndian() { m_bBigEndian = true; }
	void           setLittleEndian() { m_bBigEndian = false; }
	bool           isBigEndian()     { return m_bBigEndian;  }

private:
	bool           m_bBigEndian;
	unsigned short calcAndSkipUnsignedShortValue();
	unsigned int   calcAndSkipUnsignedIntValue();
	unsigned long  calcAndSkipUnsignedLongValue();
	
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
};

#endif // !defined(AFX_SABREADER_H__B4EE535F_3490_4897_A9A6_2888CD958741__INCLUDED_)

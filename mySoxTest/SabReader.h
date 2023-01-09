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

#define SAB_NAME_LEN   16 // (8 * 2)

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

typedef struct _SAB_COMP_INFO
{
	int      compID;
	int      kidID;
	int      typeID;
	char     cName[SAB_NAME_LEN];
} SAB_COMP_INFO, *PSAB_COMP_INFO;

typedef struct _SAB_PROP
{
	SAB_COMP_INFO        objSabCompInfo;
	int                  propType;
	unsigned int         uBufLen ;
	union {
		bool            bRet ;
		unsigned char   cByte ;
		unsigned short  shortRet ;
		unsigned int    uRet ;
		unsigned long   ulRet ;
		double          udRet ;
		char            cBuf[1024] ;
	};
} SAB_PROP, *PSAB_PROP;


// Based on App.sedona
class SabReader  
{
public:
	bool loadSCodeFile(char * cFileName);
	void readSabFile(char * cFileName);
	SabReader();
	virtual ~SabReader();

private:
	void loadSchema();
	void loadComponents();
	int  loadAppComp(SAB_COMP_INFO& objSabCompInfo);
	void loadProps(SAB_COMP_INFO& objSabCompInfo, SCODE_KIT_TYPE * objSCodeKitType, char filter);
	void loadProp(SAB_PROP& objSabProp, int iFpBix, bool isStr);
	void printProps();
	void printSchema();
	
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

	std::vector<SAB_PROP>    m_objSabPropList;
};

#endif // !defined(AFX_SABREADER_H__B4EE535F_3490_4897_A9A6_2888CD958741__INCLUDED_)

// SCodeReader.h: interface for the SCodeReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCODEREADER_H)
#define AFX_SCODEREADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)
#include "FileReader.h"
#include <vector>
#include <string>

#define SCODE_ARG_LEN          256 
#define SCODE_LOGNAME_LEN      256 
#define SCODE_NAME_LEN         256 

#define  SCODE_DEBUG_FLAG  0x01   // is debug compiled in
#define  SCODE_TEST_FLAG   0x02   // are unit tests compiled in

//  ** Flag to indicate property versus action
#define RTFLAGS_ACTION            0x01
//  ** Flag to indicate configuration property versus runtime property
#define RTFLAGS_CONFIG            0x02
//  ** Flag to indicate if a Buf should be treated as a Str
#define RTFLAGS_AS_STR            0x04
//  ** Flag to indicate if slot is operator level - if not 
//  ** operator then a slot is consider admin level.
#define RTFLAGS_OPERATOR          0x08
#define RTFLAGS_MAX               (RTFLAGS_ACTION + RTFLAGS_CONFIG + RTFLAGS_AS_STR + RTFLAGS_OPERATOR)

#define RTFLAGS_FILTER_ANY                    '*'
#define RTFLAGS_FILTER_CONFIG                 'c'
#define RTFLAGS_FILTER_RUNTIME                'r'
#define RTFLAGS_FILTER_OPERATOR_CONFIG        'C'
#define RTFLAGS_FILTER_OPERATOR_RUNTIME       'R'
// compTree
#define RTFLAGS_FILTER_TREE                   't'
#define RTFLAGS_FILTER_LINKS                  'l'

typedef struct _SCODE_KIT_TYPE_SLOT
{
    unsigned int  id;
    unsigned int  rtFlags;
    unsigned int  nameBix;
    unsigned char cPropName[SCODE_NAME_LEN];
    unsigned int  fpBix;
    unsigned int  codeBix;
} SCODE_KIT_TYPE_SLOT, *PSCODE_KIT_TYPE_SLOT;

typedef struct _SCODE_KIT_TYPE
{
    unsigned int id;
    unsigned int slotsLen;
    unsigned int nameBix;
    unsigned char cName[SCODE_NAME_LEN];
    unsigned int kitBix;
    unsigned int baseBix;
    unsigned int size;
    unsigned int initBix;
	SCODE_KIT_TYPE_SLOT * kit_type_slots_list;
} SCODE_KIT_TYPE, *PSCODE_KIT_TYPE;

typedef struct _SCODE_KIT
{
    unsigned int id;
    unsigned int typesLen;
    unsigned char cName[SCODE_NAME_LEN];
    unsigned char cVer[SCODE_NAME_LEN];
    unsigned int pad;
    unsigned int checksum;
	SCODE_KIT_TYPE * kit_type_list;
} SCODE_KIT, *PSCODE_KIT;

typedef struct _SCODE_LOG
{
    unsigned int id;
    unsigned int nameBix;
    unsigned char cName[SCODE_LOGNAME_LEN];
} SCODE_LOG, *PSCODE_LOG;



typedef struct _SCODE_METHOD
{
    unsigned char numParamsOpCode;
    unsigned char numLocalsOpCode;
	std::vector<std::string> opcodeInfo; 
} SCODE_METHOD, *PSCODE_METHOD;

typedef struct _SCODE_TEST_QNAMESLOT
{
    unsigned short qnameBix;
    unsigned short methodBix;

    unsigned short typeNameBix;
    unsigned short slotNameBix;

    unsigned char typeName[SCODE_LOGNAME_LEN];
    unsigned char slotName[SCODE_LOGNAME_LEN];
    unsigned char methodName[SCODE_LOGNAME_LEN];
} SCODE_TEST_QNAMESLOT, *PSCODE_TEST_QNAMESLOT;

typedef struct _SCODE_TEST
{
    unsigned short slotBix;
    unsigned short codeBix;
	SCODE_TEST_QNAMESLOT qnameSlot;
	SCODE_METHOD   method; 
} SCODE_TEST, *PSCODE_TEST;

// Based on SCodeValidator.java
class SCodeReader  
{
public:
	bool readSCodeFile(char * cFileName);
	void releaseSCodeFileBuffer();
	SCODE_KIT * getSCodeKits() { return m_scode_kit_list; }
	unsigned char * getSCodeKitName(unsigned int iCompID) { return m_scode_kit_list[iCompID].cName; }
	int         getNumberOfKits() { return (int)m_head_numberOfKits; }
	
	SCodeReader();
	virtual ~SCodeReader();

public:
	bool matchRtFlagsProp(unsigned char filter, unsigned int rtFlags);
	bool isRtFlagsProp(unsigned int rtFlags)     { return (rtFlags & RTFLAGS_ACTION) == 0; }
	bool isRtFlagsAction(unsigned int rtFlags)   { return (rtFlags & RTFLAGS_ACTION) != 0; }
	bool isRtFlagsConfig(unsigned int rtFlags)   { return (rtFlags & RTFLAGS_CONFIG) != 0; }
	bool isRtFlagsAsStr(unsigned int rtFlags)    { return (rtFlags & RTFLAGS_AS_STR) != 0; }
	bool isRtFlagsOperator(unsigned int rtFlags) { return (rtFlags & RTFLAGS_OPERATOR) != 0; }

private:
	void parseStr(unsigned char * strBuf, unsigned int iOffset);

	void parseKit(SCODE_KIT& objScodeKit, unsigned int iOffset);
	void parseType(SCODE_KIT_TYPE& objScodeKitType, unsigned int typeBix);
	void parseSlot(SCODE_KIT_TYPE_SLOT& objScodeKitTypeSlot, unsigned int typeBix);
	void parseLogs(unsigned int endSlotBix);
	void parseMethods(unsigned int endLogBix);
	int  parseMethod(SCODE_METHOD& objMethod, int codeBix, bool bRestorePos);
	void parseTests();
	void parseQnameSlot(SCODE_TEST_QNAMESLOT& objQnameSlot, int bi);
	void parseBootstrap();

	void setSCodeHeader();
	void printSCodeHeader();
	void printSCodeKits();
	void generateRtFlagsInfo(char * cInfo, int rtFlags);
	void printSCodeLogs();
	void printTests();
	
	void printMethods();
	void printMethod(SCODE_METHOD objMethod, bool bPrintOpCodeInfo);
	int getArgString(char *argStr, int op);

	FileReader         m_objFileReader;
	
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

private:
	unsigned char *    m_fileBuf;
	unsigned char *    m_cFileBufPtr;
	// head Data.
	unsigned char       m_head_magic[4];    // 0x5ED0BA07 for big endian, 0x07BAD05E for little endian
	unsigned char       m_head_majorVer;
	unsigned char       m_head_minorVer;
	unsigned char       m_head_blockSize;   // addr = blockIndex*blockSize + codeBaseAddr
	unsigned char       m_head_refSize;     // num bytes in address pointers
	unsigned int        m_head_imageSize;   // num bytes of whole image including full header
	unsigned int        m_head_dataSize;    // num bytes for static field data
	unsigned short      m_head_mainMethodBlockIndex; 
	unsigned short      m_head_testBlockIndex; 
	unsigned short      m_head_kitsArrayBlockIndex; 
	unsigned char       m_head_numberOfKits;
	
	unsigned char       m_head_scodeFlags;
	unsigned char       m_head_debugFlag;
	unsigned char       m_head_testFlag;

	SCODE_KIT *         m_scode_kit_list;
	std::vector<SCODE_LOG>   m_scode_log_veclist;
	int                 m_sysKitBix;
	// Use these to identify ends of type & slot segments
	int                 m_endTypeBix;
	int                 m_endSlotBix;
	int                 m_endLogBix;
	
	std::vector<SCODE_METHOD>   m_scode_method_veclist;
	
	int                 m_numTests;
	SCODE_TEST *        m_scode_test_table;
};

#endif // !defined(AFX_SCODEREADER_H)

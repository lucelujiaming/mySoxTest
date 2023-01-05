// SCodeReader.h: interface for the SCodeReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCODEREADER_H)
#define AFX_SCODEREADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileReader.h"
#include <vector>

#define SCODE_ARG_LEN          256 
#define SCODE_LOGNAME_LEN      256 
#define SCODE_NAME_LEN   16 // (8 * 2)

#define  SCODE_DEBUG_FLAG  0x01   // is debug compiled in
#define  SCODE_TEST_FLAG   0x02   // are unit tests compiled in

typedef struct _SCODE_KIT_TYPE_SLOT
{
    int id;
    int rtFlags;
    int nameBix;
    char cName[SCODE_NAME_LEN];
    int fpBix;
    int codeBix;
} SCODE_KIT_TYPE_SLOT, *PSCODE_KIT_TYPE_SLOT;

typedef struct _SCODE_KIT_TYPE
{
    int id;
    int slotsLen;
    int nameBix;
    char cName[SCODE_NAME_LEN];
    int kitBix;
    int baseBix;
    int size;
    int initBix;
	SCODE_KIT_TYPE_SLOT * kit_type_slots_list;
} SCODE_KIT_TYPE, *PSCODE_KIT_TYPE;

typedef struct _SCODE_KIT
{
    int id;
    int typesLen;
    char cName[SCODE_NAME_LEN];
    char cVer[SCODE_NAME_LEN];
    int pad;
    int checksum;
	SCODE_KIT_TYPE * kit_type_list;
} SCODE_KIT, *PSCODE_KIT;

typedef struct _SCODE_LOG
{
    int id;
    int nameBix;
    char cName[SCODE_LOGNAME_LEN];
} SCODE_LOG, *PSCODE_LOG;

typedef struct _SCODE_METHOD 
{
    unsigned char numParamsOpCode;
    unsigned char numLocalsOpCode;
	std::vector<unsigned char> opcodes; 
} SCODE_METHOD, *PSCODE_METHOD;

typedef struct _SCODE_TEST
{
    unsigned short slotBix;
    unsigned short codeBix;
	SCODE_METHOD   method; 
} SCODE_TEST, *PSCODE_TEST;

class SCodeReader  
{
public:
	void readSCodeFile(char * cFileName);
	SCodeReader();
	virtual ~SCodeReader();

private:
	void parseStr(char * strBuf, unsigned int iOffset);

	void parseKit(SCODE_KIT& objScodeKit, unsigned int iOffset);
	void parseType(SCODE_KIT_TYPE& objScodeKitType, unsigned int typeBix);
	void parseSlot(SCODE_KIT_TYPE_SLOT& objScodeKitTypeSlot, unsigned int typeBix);
	void parseLogs(unsigned int endSlotBix);
	void parseMethods(unsigned int endLogBix);
	void parseMethod(SCODE_METHOD& objMethod, int codeBix, bool bRestorePos);
	void parseTests();
	void parseBootstrap();

	void setSCodeHeader();
	void printSCodeHeader();
	void printSCodeKits();
	void printSCodeLogs();
	void printTests();
	
	void printMethods();
	void printMethod(SCODE_METHOD objMethod);

	FileReader         m_objFileReader;
	
public:
	void           setBigEndian() { m_bBigEndian = true; }
	void           setLittleEndian() { m_bBigEndian = false; }
	bool           isBigEndian()     { return m_bBigEndian;  }

private:
	bool           m_bBigEndian;
	unsigned short calcAndSkipUnsignedShortValue();
	unsigned int   calcAndSkipUnsignedIntValue();

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

// SCodeReader.h: interface for the SCodeReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCODEREADER_H)
#define AFX_SCODEREADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileReader.h"

#define SCODE_NAME_LEN   16 // (8 * 2)

#define  SCODE_DEBUG_FLAG  0x01   // is debug compiled in
#define  SCODE_TEST_FLAG   0x02   // are unit tests compiled in

typedef struct _SCODE_KIT
{
    int id;
    int typesLen;
    char cName[SCODE_NAME_LEN];
    char cVer[SCODE_NAME_LEN];
    int pad;
    int checksum;
} SCODE_KIT, *PSCODE_KIT;

// It is LittleEndian 
class SCodeReader  
{
public:
	void readSCodeFile(char * cFileName);
	SCodeReader();
	virtual ~SCodeReader();

private:
	void parseStr(char * strBuf, unsigned int iOffset);

	void parseKit(int iSeq, unsigned int iOffset);
	void parseType(int iSeq, unsigned int typeBix);
	void parseSlot(int iSeq, unsigned int typeBix);
	void parseLogs(unsigned int endSlotBix);
	void parseMethods(unsigned int endLogBix);
	void parseTests();
	void parseBootstrap();

	void setSCodeHeader();
	void printSCodeHeader();
	void printSCodeKits();

	FileReader         m_objFileReader;
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
	int                 m_sysKitBix;
	// Use these to identify ends of type & slot segments
	int                 m_endTypeBix;
	int                 m_endSlotBix;
	int                 m_endLogBix;
};

#endif // !defined(AFX_SCODEREADER_H)

// SCodeReader.cpp: implementation of the SCodeReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCodeReader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma pack(1)
typedef struct _SCODE_HEADER
{
	unsigned char       magic[4];    // 0x5ED0BA07 for big endian, 0x07BAD05E for little endian
	unsigned char       majorVer;
	unsigned char       minorVer;
	unsigned char       blockSize;   // addr = blockIndex*blockSize + codeBaseAddr
	unsigned char       refSize;     // num bytes in address pointers
	unsigned int        imageSize;   // num bytes of whole image including full header
	unsigned int        dataSize;    // num bytes for static field data
	unsigned short      mainMethodBlockIndex; 
	unsigned short      testTable; 
	unsigned short      kitsArrayBlockIndex; 
	unsigned char       numberOfKits;
} SCODE_HEADER, *PSCODE_HEADER;

//	item
//	{
//	  vtable | kit | type | int | float | long | double | str | 
//	  slot | log | method | tests | qnameType | qnameSlot
//	}

typedef struct _SCODE_VTABLE_ELEMENT
{
    unsigned short      blockIndex;   // block indexes to each virtual method
} SCODE_VTABLE_ELEMENT, *PSCODE_VTABLE_ELEMENT;

//	kit
//	{
//	  // see sys::Kit field memory layout
//	}
//
//	type
//	{
//	  // see sys::Type field memory layout
//	}
//
//	slot
//	{
//	  // see sys::Slot field memory layout
//	}
//
//	log
//	{
//	  // see sys::Log field memory layout
//	}

typedef struct _SCODE_METHOD
{
	unsigned char       numParams;  // (including implicit this)
	unsigned char       numLocals;
	unsigned char       opcodes;
} SCODE_METHOD, *PSCODE_METHOD;

typedef struct _SCODE_TEST
{
	unsigned short      qnameSlot;
	unsigned short      testMethod;
} SCODE_TEST, *PSCODE_TEST;

typedef struct _SCODE_TESTS
{
	unsigned short      count;
    SCODE_TEST *        table;
} SCODE_TESTS, *PSCODE_TESTS;

typedef struct _SCODE_QNAMETYPE
{
  unsigned short      kitNameStrIndex;
  unsigned short      typeNameStrIndex;
} SCODE_QNAMETYPE, *PSCODE_QNAMETYPE;

typedef struct _SCODE_QNAMESLOT
{
	unsigned short      qnameTypeIndex; 
	unsigned short      slotNameStrIndex;
} SCODE_QNAMESLOT, *PSCODE_QNAMESLOT;



SCodeReader::SCodeReader()
{

}

SCodeReader::~SCodeReader()
{

}

void SCodeReader::readSCodeFile(char * cFileName)
{
	SCODE_HEADER objSCodeHeader;
	FILE * fpSCode = fopen(cFileName, "rb");
	int iLen = fread(&objSCodeHeader, 1, sizeof(SCODE_HEADER), fpSCode);
	printf("SCODE_HEADER.magic[0] = %02X \r\n", objSCodeHeader.magic[0]);
	printf("SCODE_HEADER.magic[1] = %02X \r\n", objSCodeHeader.magic[1]);
	printf("SCODE_HEADER.magic[2] = %02X \r\n", objSCodeHeader.magic[2]);
	printf("SCODE_HEADER.magic[3] = %02X \r\n", objSCodeHeader.magic[3]);
	printf("SCODE_HEADER.majorVer = %d \r\n", objSCodeHeader.majorVer);
	printf("SCODE_HEADER.minorVer = %d \r\n", objSCodeHeader.minorVer);
	
	printf("SCODE_HEADER.blockSize = %d \r\n", objSCodeHeader.blockSize);
	printf("SCODE_HEADER.refSize = %d \r\n", objSCodeHeader.refSize);
	printf("SCODE_HEADER.imageSize = %04X \r\n", objSCodeHeader.imageSize);
	printf("SCODE_HEADER.dataSize = %04X \r\n", objSCodeHeader.dataSize);

	printf("SCODE_HEADER.mainMethodBlockIndex = %02X \r\n", objSCodeHeader.mainMethodBlockIndex);
	printf("SCODE_HEADER.testTable = %02X \r\n", objSCodeHeader.testTable);
	printf("SCODE_HEADER.kitsArrayBlockIndex = %02X \r\n", objSCodeHeader.kitsArrayBlockIndex);
	printf("SCODE_HEADER.numberOfKits = %02X \r\n", objSCodeHeader.numberOfKits);
	fclose(fpSCode);
}

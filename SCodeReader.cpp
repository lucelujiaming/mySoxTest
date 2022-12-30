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
	FILE * fpSCode = fopen(cFileName, "rb");
	
}

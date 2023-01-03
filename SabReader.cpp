// SabReader.cpp: implementation of the SabReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SabReader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SabReader::SabReader()
{

}

SabReader::~SabReader()
{

}

void SabReader::readSabFile(char * cFileName)
{
	char cTmp[128];
	FILE * fpSCode = fopen(cFileName, "rb");
	int iLen = fread(cTmp, 1, 128, fpSCode);

	fclose(fpSCode);
}

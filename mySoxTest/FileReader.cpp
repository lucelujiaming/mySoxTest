// FileReader.cpp: implementation of the FileReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "FileReader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}

long FileReader::GetFileSize(char * cFileName)
{
	FILE * fpSize = fopen(cFileName, "rb");
	long file_size = -1;
	long cur_offset = ftell(fpSize);	// 获取当前偏移位置
	if (cur_offset == -1) {
		printf("ftell failed \n");
		return -1;
	}
	if (fseek(fpSize, 0, SEEK_END) != 0) {	// 移动文件指针到文件末尾
		printf("fseek failed \n");
		return -1;
	}
	file_size = ftell(fpSize);	// 获取此时偏移值，即文件大小
	if (file_size == -1) {
		printf("ftell failed \n");
	}
	if (fseek(fpSize, cur_offset, SEEK_SET) != 0) {	// 将文件指针恢复初始位置
		printf("fseek failed \n");
		return -1;
	}
	fclose(fpSize);
	return file_size;
}

/*********************************************************** 
 * 功能:	计算字节对齐位置
 * n:		未对齐前值
 * align:   对齐粒度
 **********************************************************/
int FileReader::CalcAlign(int value , int align)
{
	return (value + align -1) & (~(align -1));
}


int FileReader::readSCodeFileBuffer(unsigned char ** cFileBuf, char * cFileName)
{
	int iFileSize = GetFileSize(cFileName);
	FILE * fpSCode = fopen(cFileName, "rb");
	if(*cFileBuf)
	{
		free(cFileBuf);
		cFileBuf = NULL;
	}
	*cFileBuf = (unsigned char *)malloc(CalcAlign(iFileSize,4));
	int iLen = fread(*cFileBuf, 1, CalcAlign(iFileSize,4), fpSCode);
	fclose(fpSCode);
	return iLen;
}
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
	long cur_offset = ftell(fpSize);	// ��ȡ��ǰƫ��λ��
	if (cur_offset == -1) {
		printf("ftell failed \n");
		return -1;
	}
	if (fseek(fpSize, 0, SEEK_END) != 0) {	// �ƶ��ļ�ָ�뵽�ļ�ĩβ
		printf("fseek failed \n");
		return -1;
	}
	file_size = ftell(fpSize);	// ��ȡ��ʱƫ��ֵ�����ļ���С
	if (file_size == -1) {
		printf("ftell failed \n");
	}
	if (fseek(fpSize, cur_offset, SEEK_SET) != 0) {	// ���ļ�ָ��ָ���ʼλ��
		printf("fseek failed \n");
		return -1;
	}
	fclose(fpSize);
	return file_size;
}

/*********************************************************** 
 * ����:	�����ֽڶ���λ��
 * n:		δ����ǰֵ
 * align:   ��������
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
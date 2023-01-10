// FileReader.h: interface for the FileReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEREADER_H__9702BD9E_B0D5_4316_A97A_A33E0C0724DA__INCLUDED_)
#define AFX_FILEREADER_H__9702BD9E_B0D5_4316_A97A_A33E0C0724DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FileReader  
{
public:
	FileReader();
	virtual ~FileReader();

	long GetFileSize(char * cFileName);
	int  CalcAlign(int value , int align);
	int  readSCodeFileBuffer(unsigned char ** cFileBuf, char * cFileName);
	void freeSCodeFileBuffer(unsigned char * cFileBuf);
};

#endif // !defined(AFX_FILEREADER_H__9702BD9E_B0D5_4316_A97A_A33E0C0724DA__INCLUDED_)

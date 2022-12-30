// SCodeReader.h: interface for the SCodeReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCODEREADER_H)
#define AFX_SCODEREADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SCodeReader  
{
public:
	void readSCodeFile(char * cFileName);
	SCodeReader();
	virtual ~SCodeReader();

};

#endif // !defined(AFX_SCODEREADER_H)

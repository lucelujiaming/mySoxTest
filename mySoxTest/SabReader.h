// SabReader.h: interface for the SabReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SABREADER_H__B4EE535F_3490_4897_A9A6_2888CD958741__INCLUDED_)
#define AFX_SABREADER_H__B4EE535F_3490_4897_A9A6_2888CD958741__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileReader.h"

typedef struct _SCHEMA_KIT
{
	char *     name;
	int        checksum;
} SCHEMA_KIT, *PSCHEMA_KIT;

class SabReader  
{
public:
	void readSabFile(char * cFileName);
	SabReader();
	virtual ~SabReader();

private:
	void loadSchema();
	void loadComponents();
	void loadAppComp();
	void loadProps();
	
	
private:
	FileReader         m_objFileReader;
	
	unsigned char *    m_fileBuf;
	unsigned char *    m_cFileBufPtr;
	
	// head Data.
	unsigned char       m_head_magic[4]; 
	unsigned char       m_head_Version[4];
	unsigned char       m_schema_count; 
	SCHEMA_KIT *        m_schema_kit_list;
	unsigned short      m_maxid; 
	
};

#endif // !defined(AFX_SABREADER_H__B4EE535F_3490_4897_A9A6_2888CD958741__INCLUDED_)

#if !defined(AFX_FACE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_FACE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFace
{
public:
	CFace(void);
	virtual ~CFace(void);
	void SetPtNumber(int ptNumber);
public:
	int ptNumber;		// ��Ķ�����
	int ptIndex[4];		// ��Ķ���������
// ����Obj�ļ�
public:	
	int vtIndex[3];		// ��Ķ���������
};

#endif // !defined(AFX_FACE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

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
	int ptNumber;		// 面的顶点数
	int ptIndex[4];		// 面的顶点索引号
// 用于Obj文件
public:	
	int vtIndex[3];		// 面的顶点索引号
};

#endif // !defined(AFX_FACE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

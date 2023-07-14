#if !defined(AFX_TEXTURE_COORDINATE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TEXTURE_COORDINATE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����ඨ��Ϊ�����ַ��Ҳ���������һ����������λͼ�ϵ�λ�����ꡣ
class CTextureCoordinate
{
public:
	CTextureCoordinate(void);
	virtual ~CTextureCoordinate(void);
	CTextureCoordinate(double u, double v);
	friend CTextureCoordinate operator + (const CTextureCoordinate &t0, const CTextureCoordinate &t1);//���������
	friend CTextureCoordinate operator - (const CTextureCoordinate &t0, const CTextureCoordinate &t1);
	friend CTextureCoordinate operator * (const CTextureCoordinate &t, double scalar);
	friend CTextureCoordinate operator * (double scalar, const CTextureCoordinate &t);
	friend CTextureCoordinate operator / (const CTextureCoordinate &t, double scalar);
	friend CTextureCoordinate operator += (CTextureCoordinate &t0, CTextureCoordinate &t1);
	friend CTextureCoordinate operator -= (CTextureCoordinate &t0, CTextureCoordinate &t1);
	friend CTextureCoordinate operator *= (CTextureCoordinate &t, double scalar);
	friend CTextureCoordinate operator /= (CTextureCoordinate &t, double scalar);
public:
	// ��һ�����������λͼ�ϵ�λ�����ꡣ
	double u, v;
};

#endif // !defined(AFX_TEXTURE_COORDINATE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

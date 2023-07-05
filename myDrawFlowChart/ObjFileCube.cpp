#include "stdafx.h"
#include "ObjFileCube.h"
#include <stdlib.h>
#include <stdio.h>
#define ROUND(d) int(d + 0.5)

CObjFileCube::CObjFileCube(void)
{
	FileName = _T("cube.obj");
	nTotalVertex = 0;//�������
	nTotalNormal = 0;//��ʸ������
	nTotalTexture = 0;//�������
	nTotalFace = 0;//�������
}

CObjFileCube::~CObjFileCube(void)
{
	if (V != NULL)
	{
		delete[] V;
		V = NULL;
	}
	/*if (N != NULL)
	{
		delete[] N;
		N = NULL;
	}*/
	/*if (T != NULL)
	{
		delete[] T;
		T = NULL;
	}*/
	if (F != NULL)
	{
		delete[] F;
		F = NULL;
	}
}

CP3* CObjFileCube::GetVertexName(void)
{
	return V;
}

int CObjFileCube::GetVertexNumber(void)
{
	return nTotalVertex;
}

void CObjFileCube::ReadNumber(void)
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
	{
		MessageBox(NULL, _T("�ļ�������!"), _T("Warning"), MB_ICONEXCLAMATION);
		return;
	}
	int index = 0;//������������
	CString strLine;//���ַ���
	while (file.ReadString(strLine))//���ж�ȡ
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')//��ǰ����"v+�ո�"��ͷʱ��ȡ
		{
			nTotalVertex++;
		}
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')//��ǰ����"vn+�ո�"��ͷʱ��ȡ
		{
			nTotalNormal++;
		}
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')//��ǰ����"vt+�ո�"��ͷʱ��ȡ
		{
			nTotalTexture++;
		}
		if (strLine[0] == 'f' && strLine[1] == ' ')//��ǰ����"f+�ո�"��ͷʱ��ȡ
			nTotalFace++;
	}
	file.Close();
}

void CObjFileCube::ReadVertex(void)//������
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
		return;
	V = new CP3[nTotalVertex];
	int index = 0;
	CString strLine;//����ļ���ÿ���ַ����Ļ�����
	while (file.ReadString(strLine))//���ж�ȡ
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')//��ǰ����"v+�ո�"��ͷʱ��ȡ���
		{
			CString str[3];//��strLine��'�ո�'Ϊ������ָ� ���ѵ�i+1�θ�ֵ��str[i](��ͷ�ַ�'v'Ϊ��0��)			
			for (int i = 0; i < 3; i++)
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
//			V[index].x = _wtol(str[0]);//_wtof������CSting����ת��Ϊdouble����
//			V[index].y = _wtol(str[1]);
//			V[index].z = _wtol(str[2]);
			V[index].x = atof(str[0]);//_wtof������CSting����ת��Ϊdouble����
			V[index].y = atof(str[1]);
			V[index].z = atof(str[2]);
			index++;//��������Լ�
		}
	}
	file.Close();
}

void CObjFileCube::ReadTexture(void)//��������
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
		return;
	/*T = new CT2[nTotalTexture];*/
	file.Close();
}

void CObjFileCube::ReadNormal(void)//���뷨ʸ��
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
		return;
	/*N = new CVector3[nTotalNormal];*/
	file.Close();
}

void CObjFileCube::ReadFace(void)//�������
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
		return;
	F = new CFace[nTotalFace];
	int index = 0;
	CString strLine;//���ַ���
	while (file.ReadString(strLine))//���ж�ȡ
	{
		if (strLine[0] == 'f' && strLine[1] == ' ')//��ǰ����"f+�ո�"��ͷʱ��ȡ���
		{
			CString str[3];
			//��strLine��'�ո�'Ϊ������ָ� ���ѵ�j+1�θ�ֵ��str[j](��ͷ�ַ�'f'Ϊ��0��)
			for (int i = 0; i < 3; i++)
			{
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
//				F[index].vtIndex[i] = _wtoi(str[i]) - 1;//�ָ���ÿ���ַ�����һ���ַ�Ϊ��������+1
				F[index].vtIndex[i] = atol(str[i]) - 1;//�ָ���ÿ���ַ�����һ���ַ�Ϊ��������+1
			}
			index++;//�����Լ�
		}
	}
	file.Close();
}

void CObjFileCube::Draw(CDC* pDC)//͸��ͶӰ
{
	CP2 ScreenPoint[3];//��άͶӰ��
	CP3 Eye = projection.GetEye();
	for (int nMesh = 0; nMesh < nTotalFace; nMesh++)//��ѭ��
	{
		CVector3 ViewVector(V[F[nMesh].vtIndex[0]], Eye);//�����ʸ��
		ViewVector = ViewVector.Normalize();//��һ����ʸ��
		CVector3 Vector01(V[F[nMesh].vtIndex[0]], V[F[nMesh].vtIndex[1]]);//���һ����ʸ��
		CVector3 Vector02(V[F[nMesh].vtIndex[0]], V[F[nMesh].vtIndex[2]]);//�����һ����ʸ��
		CVector3 NormalVector = CrossProduct(Vector01, Vector02);//��ķ�ʸ��
		NormalVector = NormalVector.Normalize();//��һ����ʸ��
		if (DotProduct(ViewVector, NormalVector) >= 0)//�����޳�
		{
			for (int nVertex = 0; nVertex < 3; nVertex++)//����ѭ��
			{
				ScreenPoint[nVertex] = projection.PerspectiveProjection(V[F[nMesh].vtIndex[nVertex]]);//͸��ͶӰ
			}
			pDC->MoveTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));//���ƶ����
			pDC->LineTo(ROUND(ScreenPoint[1].x), ROUND(ScreenPoint[1].y));
			pDC->LineTo(ROUND(ScreenPoint[2].x), ROUND(ScreenPoint[2].y));
			pDC->LineTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
		}
	}
}

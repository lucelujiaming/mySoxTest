#include "stdafx.h"
#include "ObjFileTeapot.h"
#define ROUND(d) int(d + 0.5)

CObjFileTeapot::CObjFileTeapot(void)
{
	FileName = _T("teapot.obj");
	nTotalVertex = 0;//�������	
	nTotalTexture = 0;//�������
	nTotalNormal = 0;//��ʸ������
	nTotalFace = 0;//�������
}

CObjFileTeapot::~CObjFileTeapot(void)
{
	if (V != NULL)
	{
		delete[] V;
		V = NULL;
	}
	/*if (T != NULL)
	{
		delete[] T;
		T = NULL;
	}*/
	if (N != NULL)
	{
		delete[] N;
		N = NULL;
	}
	if (F != NULL)
	{
		delete[] F;
		F = NULL;
	}
}

CColorP3* CObjFileTeapot::GetVertexName(void)
{
	return V;
}

int CObjFileTeapot::GetVertexNumber(void)
{
	return nTotalVertex;
}

CVector3* CObjFileTeapot::GetNormalName(void)
{
	return N;
}

int CObjFileTeapot::GetNormalNumber(void)
{
	return nTotalNormal;
}

void CObjFileTeapot::ReadNumber(void)
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
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')//��ǰ����"vt+�ո�"��ͷʱ��ȡ
		{
			nTotalTexture++;
		}
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')//��ǰ����"vn+�ո�"��ͷʱ��ȡ
		{
			nTotalNormal++;
		}
		if (strLine[0] == 'f' && strLine[1] == ' ')//��ǰ����"f+�ո�"��ͷʱ��ȡ
			nTotalFace++;
	}
	file.Close();
}

void CObjFileTeapot::ReadVertex(void)//������
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
		return;
	V = new CColorP3[nTotalVertex];
	int index = 0;
	CString strLine;//����ļ���ÿ���ַ����Ļ�����
	while (file.ReadString(strLine))//���ж�ȡ
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')//��ǰ����"v+�ո�"��ͷʱ��ȡ���
		{
			CString str[3];//��strLine��'�ո�'Ϊ������ָ� ���ѵ�i+1�θ�ֵ��str[i](��ͷ�ַ�'v'Ϊ��0��)			
			for (int i = 0; i < 3; i++)
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
//			V[index].x = _wtof(str[0]); //_wtof������CSting����ת��Ϊdouble����
//			V[index].y = _wtof(str[1]);
//			V[index].z = _wtof(str[2]);
			V[index].x = atof(str[0]);//_wtof������CSting����ת��Ϊdouble����
			V[index].y = atof(str[1]);
			V[index].z = atof(str[2]);
			index++;
		}
	}
	file.Close();
}

void CObjFileTeapot::ReadTexture(void)//��������
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
		return;
	T = new CTextureCoordinate[nTotalTexture];
	int index = 0;
	CString strLine;//���ַ���
	while (file.ReadString(strLine))//���ж�ȡ
	{
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')//��ǰ����"f+�ո�"��ͷʱ��ȡ���
		{
			CString str[3];
			//��strLine��'�ո�'Ϊ������ָ� ���ѵ�j+1�θ�ֵ��str[j](��ͷ�ַ�'f'Ϊ��0��)
			for (int i = 0; i < 3; i++)
			{
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
//				T[index].u = _wtof(str[0]);
//				T[index].v = _wtof(str[1]);
				T[index].u = atof(str[0]);
				T[index].v = atof(str[1]);
			}
			index++;
		}
	}
	file.Close();
}

void CObjFileTeapot::ReadNormal(void)//���뷨ʸ��
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
		return;
	N = new CVector3[nTotalNormal];
	int index = 0;
	CString strLine;//���ַ���
	while (file.ReadString(strLine))//���ж�ȡ
	{
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')//��ǰ����"vn+�ո�"��ͷʱ��ȡ���
		{
			CString str[3];
			//��strLine��'�ո�'Ϊ������ָ� ���ѵ�j+1�θ�ֵ��str[j](��ͷ�ַ�'f'Ϊ��0��)
			for (int i = 0; i < 3; i++)
			{
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
//				N[index].x = _wtof(str[0]);
//				N[index].y = _wtof(str[1]);
//				N[index].z = _wtof(str[2]);
				N[index].x = atof(str[0]);
				N[index].y = atof(str[1]);
				N[index].z = atof(str[2]);
			}
			index++;
		}
	}
	file.Close();
}

void CObjFileTeapot::ReadFace(void)//�������
{
	CStdioFile file;//�ļ�������
	if (!file.Open(FileName, CFile::modeRead))//ֻ����ʽ���ļ�
		return;
	F = new CColorFace[nTotalFace];
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
//				F[index].Index[i] = _wtoi(str[i]) - 1;//�ָ���ÿ���ַ�����һ���ַ�Ϊ��������+1
				F[index].Index[i] = atol(str[i]) - 1;//�ָ���ÿ���ַ�����һ���ַ�Ϊ��������+1
			}
			index++;
		}
	}
	file.Close();
}

void CObjFileTeapot::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CObjFileTeapot::Draw(CDC* pDC, CDepthLinearInterpZBuffer* pZBuffer)//�������
{
	CColorP3 ScreenPoint[3];//��άͶӰ��
	CVector3 vtNormal[3];//�㷨ʸ��
	CColorP3 Eye = projection.GetColorEye();
	for (int nMesh = 0; nMesh < nTotalFace; nMesh++)//��ѭ��
	{
		for (int nVertex = 0; nVertex < 3; nVertex++)//����ѭ��
		{
			ScreenPoint[nVertex] = projection.ThreeDimColorPerspectiveProjection(V[F[nMesh].Index[nVertex]]);
			vtNormal[nVertex] = N[F[nMesh].Index[nVertex]];			
		}
		pZBuffer->SetPoint(ScreenPoint, vtNormal);
		pZBuffer->FillTriangle(pDC, Eye, pScene);
	}
}

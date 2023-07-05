#include "stdafx.h"
#include "ObjFileCube.h"
#include <stdlib.h>
#include <stdio.h>
#define ROUND(d) int(d + 0.5)

CObjFileCube::CObjFileCube(void)
{
	FileName = _T("cube.obj");
	nTotalVertex = 0;//顶点个数
	nTotalNormal = 0;//法矢量个数
	nTotalTexture = 0;//纹理个数
	nTotalFace = 0;//表面个数
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
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
	{
		MessageBox(NULL, _T("文件不存在!"), _T("Warning"), MB_ICONEXCLAMATION);
		return;
	}
	int index = 0;//用于区分数字
	CString strLine;//行字符串
	while (file.ReadString(strLine))//按行读取
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')//当前行以"v+空格"开头时读取
		{
			nTotalVertex++;
		}
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')//当前行以"vn+空格"开头时读取
		{
			nTotalNormal++;
		}
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')//当前行以"vt+空格"开头时读取
		{
			nTotalTexture++;
		}
		if (strLine[0] == 'f' && strLine[1] == ' ')//当前行以"f+空格"开头时读取
			nTotalFace++;
	}
	file.Close();
}

void CObjFileCube::ReadVertex(void)//读入点表
{
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
		return;
	V = new CP3[nTotalVertex];
	int index = 0;
	CString strLine;//存放文件中每行字符串的缓冲区
	while (file.ReadString(strLine))//按行读取
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')//当前行以"v+空格"开头时读取点表
		{
			CString str[3];//将strLine以'空格'为间隔符分割 并把第i+1段赋值给str[i](开头字符'v'为第0段)			
			for (int i = 0; i < 3; i++)
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
//			V[index].x = _wtol(str[0]);//_wtof函数将CSting类型转换为double类型
//			V[index].y = _wtol(str[1]);
//			V[index].z = _wtol(str[2]);
			V[index].x = atof(str[0]);//_wtof函数将CSting类型转换为double类型
			V[index].y = atof(str[1]);
			V[index].z = atof(str[2]);
			index++;//顶点个数自加
		}
	}
	file.Close();
}

void CObjFileCube::ReadTexture(void)//读入纹理
{
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
		return;
	/*T = new CT2[nTotalTexture];*/
	file.Close();
}

void CObjFileCube::ReadNormal(void)//读入法矢量
{
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
		return;
	/*N = new CVector3[nTotalNormal];*/
	file.Close();
}

void CObjFileCube::ReadFace(void)//读入面表
{
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
		return;
	F = new CFace[nTotalFace];
	int index = 0;
	CString strLine;//行字符串
	while (file.ReadString(strLine))//按行读取
	{
		if (strLine[0] == 'f' && strLine[1] == ' ')//当前行以"f+空格"开头时读取面表
		{
			CString str[3];
			//将strLine以'空格'为间隔符分割 并把第j+1段赋值给str[j](开头字符'f'为第0段)
			for (int i = 0; i < 3; i++)
			{
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
//				F[index].vtIndex[i] = _wtoi(str[i]) - 1;//分割后的每段字符串第一个字符为面表顶点序号+1
				F[index].vtIndex[i] = atol(str[i]) - 1;//分割后的每段字符串第一个字符为面表顶点序号+1
			}
			index++;//面数自加
		}
	}
	file.Close();
}

void CObjFileCube::Draw(CDC* pDC)//透视投影
{
	CP2 ScreenPoint[3];//二维投影点
	CP3 Eye = projection.GetEye();
	for (int nMesh = 0; nMesh < nTotalFace; nMesh++)//面循环
	{
		CVector3 ViewVector(V[F[nMesh].vtIndex[0]], Eye);//面的视矢量
		ViewVector = ViewVector.Normalize();//归一化视矢量
		CVector3 Vector01(V[F[nMesh].vtIndex[0]], V[F[nMesh].vtIndex[1]]);//面的一个边矢量
		CVector3 Vector02(V[F[nMesh].vtIndex[0]], V[F[nMesh].vtIndex[2]]);//面的另一个边矢量
		CVector3 NormalVector = CrossProduct(Vector01, Vector02);//面的法矢量
		NormalVector = NormalVector.Normalize();//归一化法矢量
		if (DotProduct(ViewVector, NormalVector) >= 0)//背面剔除
		{
			for (int nVertex = 0; nVertex < 3; nVertex++)//顶点循环
			{
				ScreenPoint[nVertex] = projection.PerspectiveProjection(V[F[nMesh].vtIndex[nVertex]]);//透视投影
			}
			pDC->MoveTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));//绘制多边形
			pDC->LineTo(ROUND(ScreenPoint[1].x), ROUND(ScreenPoint[1].y));
			pDC->LineTo(ROUND(ScreenPoint[2].x), ROUND(ScreenPoint[2].y));
			pDC->LineTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
		}
	}
}

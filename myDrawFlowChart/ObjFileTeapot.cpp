#include "stdafx.h"
#include "ObjFileTeapot.h"
#define ROUND(d) int(d + 0.5)

CObjFileTeapot::CObjFileTeapot(void)
{
	FileName = _T("teapot.obj");
	nTotalVertex = 0;//顶点个数	
	nTotalTexture = 0;//纹理个数
	nTotalNormal = 0;//法矢量个数
	nTotalFace = 0;//表面个数
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
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')//当前行以"vt+空格"开头时读取
		{
			nTotalTexture++;
		}
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')//当前行以"vn+空格"开头时读取
		{
			nTotalNormal++;
		}
		if (strLine[0] == 'f' && strLine[1] == ' ')//当前行以"f+空格"开头时读取
			nTotalFace++;
	}
	file.Close();
}

void CObjFileTeapot::ReadVertex(void)//读入点表
{
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
		return;
	V = new CColorP3[nTotalVertex];
	int index = 0;
	CString strLine;//存放文件中每行字符串的缓冲区
	while (file.ReadString(strLine))//按行读取
	{
		if (strLine[0] == 'v' && strLine[1] == ' ')//当前行以"v+空格"开头时读取点表
		{
			CString str[3];//将strLine以'空格'为间隔符分割 并把第i+1段赋值给str[i](开头字符'v'为第0段)			
			for (int i = 0; i < 3; i++)
				AfxExtractSubString(str[i], strLine, i + 1, ' ');
//			V[index].x = _wtof(str[0]); //_wtof函数将CSting类型转换为double类型
//			V[index].y = _wtof(str[1]);
//			V[index].z = _wtof(str[2]);
			V[index].x = atof(str[0]);//_wtof函数将CSting类型转换为double类型
			V[index].y = atof(str[1]);
			V[index].z = atof(str[2]);
			index++;
		}
	}
	file.Close();
}

void CObjFileTeapot::ReadTexture(void)//读入纹理
{
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
		return;
	T = new CTextureCoordinate[nTotalTexture];
	int index = 0;
	CString strLine;//行字符串
	while (file.ReadString(strLine))//按行读取
	{
		if (strLine[0] == 'v' && strLine[1] == 't' && strLine[2] == ' ')//当前行以"f+空格"开头时读取面表
		{
			CString str[3];
			//将strLine以'空格'为间隔符分割 并把第j+1段赋值给str[j](开头字符'f'为第0段)
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

void CObjFileTeapot::ReadNormal(void)//读入法矢量
{
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
		return;
	N = new CVector3[nTotalNormal];
	int index = 0;
	CString strLine;//行字符串
	while (file.ReadString(strLine))//按行读取
	{
		if (strLine[0] == 'v' && strLine[1] == 'n' && strLine[2] == ' ')//当前行以"vn+空格"开头时读取面表
		{
			CString str[3];
			//将strLine以'空格'为间隔符分割 并把第j+1段赋值给str[j](开头字符'f'为第0段)
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

void CObjFileTeapot::ReadFace(void)//读入面表
{
	CStdioFile file;//文件流对象
	if (!file.Open(FileName, CFile::modeRead))//只读方式打开文件
		return;
	F = new CColorFace[nTotalFace];
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
//				F[index].Index[i] = _wtoi(str[i]) - 1;//分割后的每段字符串第一个字符为面表顶点序号+1
				F[index].Index[i] = atol(str[i]) - 1;//分割后的每段字符串第一个字符为面表顶点序号+1
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

void CObjFileTeapot::Draw(CDC* pDC, CDepthLinearInterpZBuffer* pZBuffer)//茶壶光照
{
	CColorP3 ScreenPoint[3];//二维投影点
	CVector3 vtNormal[3];//点法矢量
	CColorP3 Eye = projection.GetColorEye();
	for (int nMesh = 0; nMesh < nTotalFace; nMesh++)//面循环
	{
		for (int nVertex = 0; nVertex < 3; nVertex++)//顶点循环
		{
			ScreenPoint[nVertex] = projection.ThreeDimColorPerspectiveProjection(V[F[nMesh].Index[nVertex]]);
			vtNormal[nVertex] = N[F[nMesh].Index[nVertex]];			
		}
		pZBuffer->SetPoint(ScreenPoint, vtNormal);
		pZBuffer->FillTriangle(pDC, Eye, pScene);
	}
}

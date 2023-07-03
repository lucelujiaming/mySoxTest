#include "stdafx.h"
#include "DepthFace.h"

CDepthFace::CDepthFace(void)
{
	fMaxDepth = 3000.0;
}

void CDepthFace::SetPtNumber(int ptNumber)
{
	this->ptNumber = ptNumber;
}

CDepthFace::~CDepthFace(void)
{
}

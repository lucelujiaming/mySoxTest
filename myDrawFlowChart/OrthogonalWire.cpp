// OrthogonalWire.cpp: implementation of the COrthogonalWire class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrthogonalWire.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrthogonalWire::COrthogonalWire()
{

}

COrthogonalWire::~COrthogonalWire()
{

}

void COrthogonalWire::calculateDirectVector(CPoint startPos, CPoint endPos)
{
	int iDiff = 0;
	iDiff = startPos.x - endPos.x ;
	if(iDiff >= 0)
	{
		m_objDirectVector.x_direction = ORTHOGONALWIRE_WEST;	
	}
	else
	{
		m_objDirectVector.x_direction = ORTHOGONALWIRE_EAST;
	}
	iDiff = startPos.y - endPos.y;
	if(iDiff >= 0)
	{
		m_objDirectVector.y_direction = ORTHOGONALWIRE_NORTH;	
	}
	else
	{
		m_objDirectVector.y_direction = ORTHOGONALWIRE_SOUTH;
	}
	// return m_objDirectVector;
}

int COrthogonalWire::calculateDirection(CWirePoint point)
{
	if(point.direction == m_objDirectVector.x_direction)
		return m_objDirectVector.x_direction;
	else if(point.direction == m_objDirectVector.y_direction)
		return m_objDirectVector.y_direction;
	else if(point.direction == m_objDirectVector.x_direction * (-1))
		return m_objDirectVector.y_direction;
	else if(point.direction == m_objDirectVector.y_direction * (-1))
		return m_objDirectVector.x_direction;
	else
	{
		printf("NOTICE:We can not find the direction");
		return ORTHOGONALWIRE_MAX;
	}
}

int COrthogonalWire::calculateOrthogonalWire(CWirePoint startPos, CWirePoint endPos)
{
	calculateDirectVector(startPos.ptPosition, endPos.ptPosition);
	int startDirection = calculateDirection(startPos);
	int endDirection   = calculateDirection(endPos);
	if(startDirection != endDirection)
	{
		// 直接向量的垂直方向
		if(abs(startDirection) == ORTHOGONALWIRE_SOUTH)
		{
			m_ptBend[0].x = startPos.ptPosition.x;
		}
		else if(abs(endDirection) == ORTHOGONALWIRE_SOUTH)
		{
			m_ptBend[0].x = endPos.ptPosition.x;
		}

		if(abs(startDirection) == ORTHOGONALWIRE_EAST)
		{
			m_ptBend[0].y = startPos.ptPosition.y;
		}
		else if(abs(endDirection) == ORTHOGONALWIRE_EAST)
		{
			m_ptBend[0].y = endPos.ptPosition.y;
		}
		return 1;
	}
	else 
	{
		if(abs(startDirection) == ORTHOGONALWIRE_EAST)
		{
			m_ptBend[0].y = startPos.ptPosition.y;
			m_ptBend[1].y = endPos.ptPosition.y;

			m_ptBend[0].x = (startPos.ptPosition.x + endPos.ptPosition.x) / 2;
			m_ptBend[1].x = (startPos.ptPosition.x + endPos.ptPosition.x) / 2;
		}
		else if(abs(startDirection) == ORTHOGONALWIRE_SOUTH)
		{
			m_ptBend[0].x = startPos.ptPosition.x;
			m_ptBend[1].x = endPos.ptPosition.x;

			m_ptBend[0].y = (startPos.ptPosition.y + endPos.ptPosition.y) / 2;
			m_ptBend[1].y = (startPos.ptPosition.y + endPos.ptPosition.y) / 2;
		}
		return 2;
	}
	return 0;
}

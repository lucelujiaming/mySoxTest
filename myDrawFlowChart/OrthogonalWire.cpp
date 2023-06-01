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
	m_fBendPercent = 0.5;
}

COrthogonalWire::~COrthogonalWire()
{

}

void COrthogonalWire::calculateDirectVector(CPoint& startPos, CPoint& endPos)
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

int COrthogonalWire::calculateDirection(CWirePoint& point)
{
	if(point.endDirection == m_objDirectVector.x_direction)
		return m_objDirectVector.x_direction;
	else if(point.endDirection == m_objDirectVector.y_direction)
		return m_objDirectVector.y_direction;
	else if(point.endDirection == m_objDirectVector.x_direction * (-1))
		return m_objDirectVector.y_direction;
	else if(point.endDirection == m_objDirectVector.y_direction * (-1))
		return m_objDirectVector.x_direction;
	else
	{
		printf("NOTICE:We can not find the direction");
		return ORTHOGONALWIRE_MAX;
	}
}

int COrthogonalWire::calculateOrthogonalWire(CWirePoint& startPos, CWirePoint& endPos)
{
	calculateDirectVector(startPos.ptPosition, endPos.ptPosition);
	int startDirection = calculateDirection(startPos);
	int endDirection   = calculateDirection(endPos);
	if(startDirection != endDirection)
	{
		// m_ptBend[0] always use the startDirection
		m_ptBend[0].endDirection = startDirection;
		// 直接向量的垂直方向
		if(abs(startDirection) == ORTHOGONALWIRE_SOUTH)
		{
			m_ptBend[0].ptPosition.x = startPos.ptPosition.x;
		}
		else if(abs(endDirection) == ORTHOGONALWIRE_SOUTH)
		{
			m_ptBend[0].ptPosition.x = endPos.ptPosition.x;
		}

		if(abs(startDirection) == ORTHOGONALWIRE_EAST)
		{
			m_ptBend[0].ptPosition.y = startPos.ptPosition.y;
		}
		else if(abs(endDirection) == ORTHOGONALWIRE_EAST)
		{
			m_ptBend[0].ptPosition.y = endPos.ptPosition.y;
		}
		return 1;
	}
	else 
	{
		// m_ptBend[0] always use the startDirection
		m_ptBend[0].endDirection = startDirection;
		
		if(abs(startDirection) == ORTHOGONALWIRE_EAST)
		{
			m_ptBend[0].ptPosition.y = startPos.ptPosition.y;
			m_ptBend[1].ptPosition.y = endPos.ptPosition.y;
			m_ptBend[0].ptPosition.x = 
				// (startPos.ptPosition.x + endPos.ptPosition.x) / 2;
				startPos.ptPosition.x + (int)(m_fBendPercent *
							(endPos.ptPosition.x - startPos.ptPosition.x));
			m_ptBend[1].ptPosition.x = 
				// (startPos.ptPosition.x + endPos.ptPosition.x) / 2;
				endPos.ptPosition.x - (int)((1 - m_fBendPercent) *
							(endPos.ptPosition.x - startPos.ptPosition.x));
			
			if(m_ptBend[0].ptPosition.y >= m_ptBend[1].ptPosition.y)
			{
				m_ptBend[1].endDirection = ORTHOGONALWIRE_NORTH;
			}
			else
			{
				m_ptBend[1].endDirection = ORTHOGONALWIRE_SOUTH;
			}
		}
		else if(abs(startDirection) == ORTHOGONALWIRE_SOUTH)
		{
			m_ptBend[0].ptPosition.x = startPos.ptPosition.x;
			m_ptBend[1].ptPosition.x = endPos.ptPosition.x;

			m_ptBend[0].ptPosition.y = 
				// (startPos.ptPosition.y + endPos.ptPosition.y) / 2;
				startPos.ptPosition.y + (int)(m_fBendPercent *
							(endPos.ptPosition.y - startPos.ptPosition.y));
			m_ptBend[1].ptPosition.y = 
				// (startPos.ptPosition.y + endPos.ptPosition.y) / 2;
				endPos.ptPosition.y - (int)((1 - m_fBendPercent) *
							(endPos.ptPosition.y - startPos.ptPosition.y));
						
			if(m_ptBend[0].ptPosition.x >= m_ptBend[1].ptPosition.x)
			{
				m_ptBend[1].endDirection = ORTHOGONALWIRE_WEST;
			}
			else
			{
				m_ptBend[1].endDirection = ORTHOGONALWIRE_EAST;
			}
		}
		
		return 2;
	}
	return 0;
}

int COrthogonalWire::calculateWireDirection(CWirePoint& startPos, CWirePoint& endPos)
{
	if(abs(startPos.endDirection) != abs(endPos.endDirection))
	{
		return endPos.endDirection;
	}
	else
	{
		if(startPos.endDirection == endPos.endDirection * (-1))
		{
			// m_ptBend[0] always use the startDirection
			if(abs(startPos.endDirection) == ORTHOGONALWIRE_EAST)
			{
				if(startPos.ptPosition.x >= startPos.ptPosition.x)
				{
					return ORTHOGONALWIRE_SOUTH;
				}
				else 
				{
					return ORTHOGONALWIRE_NORTH;
				}
			}
			else if(abs(startPos.endDirection) == ORTHOGONALWIRE_SOUTH)
			{
				if(startPos.ptPosition.y >= startPos.ptPosition.y)
				{
					return ORTHOGONALWIRE_EAST;
				}
				else 
				{
					return ORTHOGONALWIRE_WEST;
				}
			}
			return ORTHOGONALWIRE_MAX;
		}
		else if(startPos.endDirection == endPos.endDirection)
		{
			// m_ptBend[0] always use the startDirection
			return startPos.endDirection;
		}
	}
	return ORTHOGONALWIRE_MAX;
}
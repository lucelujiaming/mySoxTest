// OrthogonalWire.h: interface for the COrthogonalWire class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORTHOGONALWIRE_H__B34F204D_BD00_42DB_86F4_DADDECD45E7B__INCLUDED_)
#define AFX_ORTHOGONALWIRE_H__B34F204D_BD00_42DB_86F4_DADDECD45E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Use screen direction
#define ORTHOGONALWIRE_EAST               (1)     // 向东
#define ORTHOGONALWIRE_WEST               (-1)    // 向西
#define ORTHOGONALWIRE_SOUTH              (2)     // 向南
#define ORTHOGONALWIRE_NORTH              (-2)    // 向北
#define ORTHOGONALWIRE_MAX                 3  

class CWirePoint {
public:
	int     direction;
	CPoint  ptPosition;
};

class CDirectVector {
public:
	int x_direction;
	int y_direction;
} ;


class COrthogonalWire  
{
public:
	COrthogonalWire();
	virtual ~COrthogonalWire();

	int calculateOrthogonalWire(CWirePoint startPos, CWirePoint endPos);
	void calculateDirectVector(CPoint startPos, CPoint endPos);
	int calculateDirection(CWirePoint point);

private:
	CDirectVector m_objDirectVector;
public:
	CPoint m_ptBend[2];
};

#endif // !defined(AFX_ORTHOGONALWIRE_H__B34F204D_BD00_42DB_86F4_DADDECD45E7B__INCLUDED_)

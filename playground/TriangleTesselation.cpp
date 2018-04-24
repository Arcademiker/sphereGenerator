#include <cstdlib>
//#include "stdafx.h"
#include "TriangleTesselation.h"
#include <cmath>

CTriangleTesselation::CTriangleTesselation(float fRadius)
{
	m_fRadius = fRadius;
	m_vecTriangleList[0] = new std::vector<CTriangle>();
	m_vecTriangleList[1] = new std::vector<CTriangle>();
}

CTriangleTesselation::~CTriangleTesselation()
{
	delete m_vecTriangleList[0];
	delete m_vecTriangleList[1];
}

void CTriangleTesselation::Tesselate(uint32_t nIterations)
{
	GenerateTetraeder();
	for (size_t i = 0; i < nIterations;++i)
	{
		m_vecTriangleList[1 - m_nArrayResult]->clear();
		for (size_t nIterator = 0; nIterator < m_vecTriangleList[m_nArrayResult]->size(); ++nIterator)
		{
			CTriangle::SPoint3D PointNew1;
			CTriangle::SPoint3D PointNew2;
			CTriangle::SPoint3D PointNew3;
			CTriangle::SPoint3D Point1 = *m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint1();
			CTriangle::SPoint3D Point2 = *m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint2();
			CTriangle::SPoint3D Point3 = *m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint3();
			PointNew1.fX = (Point2.fX + Point3.fX) / 2.0f;
			PointNew1.fY = (Point2.fY + Point3.fY) / 2.0f;
			PointNew1.fZ = (Point2.fZ + Point3.fZ) / 2.0f;
			PointNew1.Normalize();
			PointNew1 = PointNew1 * m_fRadius;
			PointNew2.fX = (Point3.fX + Point1.fX) / 2.0f;
			PointNew2.fY = (Point3.fY + Point1.fY) / 2.0f;
			PointNew2.fZ = (Point3.fZ + Point1.fZ) / 2.0f;
			PointNew2.Normalize();
			PointNew2 = PointNew2 * m_fRadius;
			PointNew3.fX = (Point1.fX + Point2.fX) / 2.0f;
			PointNew3.fY = (Point1.fY + Point2.fY) / 2.0f;
			PointNew3.fZ = (Point1.fZ + Point2.fZ) / 2.0f;
			PointNew3.Normalize();
			PointNew3 = PointNew3 * m_fRadius;
			m_vecTriangleList[1 - m_nArrayResult]->push_back(CTriangle(Point1, PointNew2, PointNew3));
			m_vecTriangleList[1 - m_nArrayResult]->push_back(CTriangle(Point2, PointNew3, PointNew1));
			m_vecTriangleList[1 - m_nArrayResult]->push_back(CTriangle(Point3, PointNew1, PointNew2));
			m_vecTriangleList[1 - m_nArrayResult]->push_back(CTriangle(PointNew1, PointNew3, PointNew2));
		}
		m_nArrayResult = 1 - m_nArrayResult;
	}
	//ComputeTextureCoordinates();
}
/*
float CalculateAngle(float fX, float fY)
{
	static float fPI = 3.1415926f;

	float fResult;
	if (fX > 0 && fY > 0) { fResult = atan(fY / fX); }
	else if (fX < 0 && fY > 0) { fResult = atan(fX / fY) + fPI / 2.0f; }
	else if (fX < 0 && fY < 0) { fResult = atan(fY / fX) + fPI; }
	else if (fX > 0 && fY < 0) { fResult = atan(fX / fY) + fPI / 3.0f * 4.0f; }
	else if (fX == 0 && fY > 0) { fResult = fPI / 2.0f; }
	else if (fX == 0 && fY < 0) { fResult = fPI / 3.0f * 4.0f; }
	else if (fX > 0 && fY == 0) { fResult = 0; }
	else if (fX < 0 && fY == 0) { fResult = fPI; }
	else { fResult = 0; }

	return fResult;
}


void CorrectTextureOverflowU(CTriangle& Triangle)
{
	static float fPI = 3.1415926f;
	CTriangle::SPoint3D* Point1 = Triangle.GetPoint1();
	CTriangle::SPoint3D* Point2 = Triangle.GetPoint2();
	CTriangle::SPoint3D* Point3 = Triangle.GetPoint3();

	if (abs(Point1->fU - Point2->fU) > fPI)
	{
		if (Point1->fU < Point2->fU) 
		{	
			Point1->fU += 2.0f * fPI;
		}
		else
		{
			Point2->fU += 2.0f * fPI;
		}
	}
	if (abs(Point2->fU - Point3->fU) > fPI)
	{
		if (Point2->fU < Point3->fU)
		{
			Point2->fU += 2.0f * fPI;
		}
		else
		{
			Point3->fU += 2.0f * fPI;
		}
	}
}

void CTriangleTesselation::ComputeTextureCoordinates()
{
	for (size_t nIterator = 0; nIterator < m_vecTriangleList[m_nArrayResult]->size(); ++nIterator)
	{
		CTriangle::SPoint3D* Point1 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint1();
		CTriangle::SPoint3D* Point2 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint2();
		CTriangle::SPoint3D* Point3 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint3();
		Point1->fU = CalculateAngle(Point1->fX, Point1->fZ);
		Point2->fU = CalculateAngle(Point2->fX, Point2->fZ);
		Point3->fU = CalculateAngle(Point3->fX, Point3->fZ);
		CorrectTextureOverflowU(m_vecTriangleList[m_nArrayResult]->at(nIterator));
		Point1->fV = acos(Point1->fY / m_fRadius);
		Point2->fV = acos(Point2->fY / m_fRadius);
		Point3->fV = acos(Point3->fY / m_fRadius);
	}
}
*/
const std::vector<CTriangle>* CTriangleTesselation::GetTriangleList() const
{
	return m_vecTriangleList[m_nArrayResult];
}

void CTriangleTesselation::GenerateTetraeder()
{
	//static float fPI = 3.1415926f;
	
	m_vecTriangleList[0]->clear();
	m_vecTriangleList[1]->clear();
	m_nArrayResult = 0;

    /* //tetraeder
	CTriangle::SPoint3D Point1;
	Point1.fX = -1.0f;
	Point1.fY = -1.0f;
	Point1.fZ = -1.0f;
	Point1.Normalize();
	Point1 = Point1 * m_fRadius;

	CTriangle::SPoint3D Point2;
	Point2.fX = -1.0f;
	Point2.fY = 1.0f;
	Point2.fZ = 1.0f;
	Point2.Normalize();
	Point2 = Point2 * m_fRadius;

	CTriangle::SPoint3D Point3;
	Point3.fX = 1.0f;
	Point3.fY = -1.0f;
	Point3.fZ = 1.0f;
	Point3.Normalize();
	Point3 = Point3 * m_fRadius;

	CTriangle::SPoint3D Point4;
	Point4.fX = 1.0f;
	Point4.fY = 1.0f;
	Point4.fZ = -1.0f;
	Point4.Normalize();
	Point4 = Point4 * m_fRadius;

	m_vecTriangleList[0]->push_back(CTriangle(Point1, Point3, Point4));
	m_vecTriangleList[0]->push_back(CTriangle(Point4, Point2, Point1));
	m_vecTriangleList[0]->push_back(CTriangle(Point1, Point2, Point3));
	m_vecTriangleList[0]->push_back(CTriangle(Point2, Point4, Point3));
     */

    //icosahedron:
    float phi = 1.0f / 2.0f*(1.0f + std::sqrt(5.0f));

    CTriangle::SPoint3D Point1;
    Point1.fX = 0.0f;
    Point1.fY = 1.0f;
    Point1.fZ = phi;
    Point1.Normalize();
    Point1 = Point1 * m_fRadius;

    CTriangle::SPoint3D Point2;
    Point2.fX = 0.0f;
    Point2.fY = 1.0f;
    Point2.fZ = -phi;
    Point2.Normalize();
    Point2 = Point2 * m_fRadius;

    CTriangle::SPoint3D Point3;
    Point3.fX = 0.0f;
    Point3.fY = -1.0f;
    Point3.fZ = phi;
    Point3.Normalize();
    Point3 = Point3 * m_fRadius;

    CTriangle::SPoint3D Point4;
    Point4.fX = 0.0f;
    Point4.fY = -1.0f;
    Point4.fZ = -phi;
    Point4.Normalize();
    Point4 = Point4 * m_fRadius;

    CTriangle::SPoint3D Point5;
    Point5.fX = 1.0f;
    Point5.fY = phi;
    Point5.fZ = 0.0f;
    Point5.Normalize();
    Point5 = Point5 * m_fRadius;

    CTriangle::SPoint3D Point6;
    Point6.fX = 1.0f;
    Point6.fY = -phi;
    Point6.fZ = 0.0f;
    Point6.Normalize();
    Point6 = Point6 * m_fRadius;

    CTriangle::SPoint3D Point7;
    Point7.fX = -1.0f;
    Point7.fY = phi;
    Point7.fZ = 0.0f;
    Point7.Normalize();
    Point7 = Point7 * m_fRadius;

    CTriangle::SPoint3D Point8;
    Point8.fX = -1.0f;
    Point8.fY = -phi;
    Point8.fZ = 0.0f;
    Point8.Normalize();
    Point8 = Point8 * m_fRadius;

    CTriangle::SPoint3D Point9;
    Point9.fX = phi;
    Point9.fY = 0.0f;
    Point9.fZ = 1.0f;
    Point9.Normalize();
    Point9 = Point9 * m_fRadius;

    CTriangle::SPoint3D PointA;
    PointA.fX = -phi;
    PointA.fY = 0.0f;
    PointA.fZ = 1.0f;
    PointA.Normalize();
    PointA = PointA * m_fRadius;

    CTriangle::SPoint3D PointB;
    PointB.fX = phi;
    PointB.fY = 0.0f;
    PointB.fZ = -1.0f;
    PointB.Normalize();
    PointB = PointB * m_fRadius;

    CTriangle::SPoint3D PointC;
    PointC.fX = -phi;
    PointC.fY = 0.0f;
    PointC.fZ = -1.0f;
    PointC.Normalize();
    PointC = PointC * m_fRadius;

    //northpol
    m_vecTriangleList[0]->push_back(CTriangle(Point7, Point5, Point1));
    m_vecTriangleList[0]->push_back(CTriangle(Point7, Point2, Point5));
    m_vecTriangleList[0]->push_back(CTriangle(Point7, PointC, Point2));
    m_vecTriangleList[0]->push_back(CTriangle(Point7, PointA, PointC));
    m_vecTriangleList[0]->push_back(CTriangle(Point7, Point1, PointA));
    //southpol
    m_vecTriangleList[0]->push_back(CTriangle(Point6, Point4, Point8));
    m_vecTriangleList[0]->push_back(CTriangle(Point6, Point8, Point3));
    m_vecTriangleList[0]->push_back(CTriangle(Point6, Point3, Point9));
    m_vecTriangleList[0]->push_back(CTriangle(Point6, Point9, PointB));
    m_vecTriangleList[0]->push_back(CTriangle(Point6, PointB, Point4));
    //belt
    m_vecTriangleList[0]->push_back(CTriangle(PointA, Point8, PointC));
    m_vecTriangleList[0]->push_back(CTriangle(Point8, Point4, PointC));
    m_vecTriangleList[0]->push_back(CTriangle(PointC, Point4, Point2));
    m_vecTriangleList[0]->push_back(CTriangle(Point4, PointB, Point2));
    m_vecTriangleList[0]->push_back(CTriangle(Point2, PointB, Point5));
    m_vecTriangleList[0]->push_back(CTriangle(PointB, Point9, Point5));
    m_vecTriangleList[0]->push_back(CTriangle(Point5, Point9, Point1));
    m_vecTriangleList[0]->push_back(CTriangle(Point9, Point3, Point1));
    m_vecTriangleList[0]->push_back(CTriangle(Point1, Point3, PointA));
    m_vecTriangleList[0]->push_back(CTriangle(Point3, Point8, PointA));
}

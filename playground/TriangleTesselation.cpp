#include <cstdlib>
//#include "stdafx.h"
#include "TriangleTesselation.h"

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

/*
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
}

#include <cstdlib>
//#include "stdafx.h"
#include "TriangleTesselation.h"
#include <iostream>


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

            PointNew1.fPos = (Point2.fPos + Point3.fPos) / 2.0f;
            PointNew1.fPos = glm::normalize(PointNew1.fPos);
            PointNew1.fPos = PointNew1.fPos * m_fRadius;

            PointNew2.fPos = (Point3.fPos + Point1.fPos) / 2.0f;
            PointNew2.fPos = glm::normalize(PointNew2.fPos);
            PointNew2.fPos = PointNew2.fPos * m_fRadius;

            PointNew3.fPos = (Point1.fPos + Point2.fPos) / 2.0f;
            PointNew3.fPos = glm::normalize(PointNew3.fPos);
            PointNew3.fPos = PointNew3.fPos * m_fRadius;

			m_vecTriangleList[1 - m_nArrayResult]->push_back(CTriangle(Point1, PointNew2, PointNew3));
			m_vecTriangleList[1 - m_nArrayResult]->push_back(CTriangle(Point2, PointNew3, PointNew1));
			m_vecTriangleList[1 - m_nArrayResult]->push_back(CTriangle(Point3, PointNew1, PointNew2));
			m_vecTriangleList[1 - m_nArrayResult]->push_back(CTriangle(PointNew1, PointNew3, PointNew2));
		}
		m_nArrayResult = 1 - m_nArrayResult;
	}
	ComputeTextureCoordinates();
    ComputeTangentBitangent();
}

/*
float CalculateAngle(float fX, float fY)
{
	static float fPI = 3.1415926f;

	float fResult;
	if (fX > 0 && fY > 0) { fResult = atanf(fY / fX)/(2*fPI); }
	else if (fX < 0 && fY > 0) { fResult = atanf(fX / fY)/(2*fPI) + fPI / 2.0f; }
	else if (fX < 0 && fY < 0) { fResult = atanf(fY / fX)/(2*fPI) + fPI; }
	else if (fX > 0 && fY < 0) { fResult = atanf(fX / fY)/(2*fPI) + fPI / 3.0f * 4.0f; }
	else if (fX == 0 && fY > 0) { fResult = fPI / 2.0f; }
	else if (fX == 0 && fY < 0) { fResult = fPI / 3.0f * 4.0f; }
	else if (fX > 0 && fY == 0) { fResult = 0; }
	else if (fX < 0 && fY == 0) { fResult = fPI; }
	else { fResult = 0; }

	return fResult;
}
*/

void CorrectTextureOverflowU(CTriangle& Triangle)
{
	static float fPI = 3.1415926f;
	CTriangle::SPoint3D* Point1 = Triangle.GetPoint1();
	CTriangle::SPoint3D* Point2 = Triangle.GetPoint2();
	CTriangle::SPoint3D* Point3 = Triangle.GetPoint3();

	if (fabsf(Point1->fUV.x - Point2->fUV.x) > 0.5)
	{
		if (Point1->fUV.x < Point2->fUV.x)
		{	
			Point1->fUV.x += 1.0f;//2.0f * fPI;
		}
		else
		{
			Point2->fUV.x += 1.0f;//2.0f * fPI;
		}
	}
	if (fabsf(Point2->fUV.x - Point3->fUV.x) > 0.5)
	{
		if (Point2->fUV.x < Point3->fUV.x)
		{
			Point2->fUV.x += 1.0f;//2.0f * fPI;
		}
		else
		{
			Point3->fUV.x += 1.0f;//2.0f * fPI;
		}
	}
    if (fabsf(Point1->fUV.x - Point3->fUV.x) > 0.5)
    {
        if (Point1->fUV.x < Point3->fUV.x)
        {
            Point1->fUV.x += 1.0f;//2.0f * fPI;
        }
        else
        {
            Point3->fUV.x += 1.0f;//2.0f * fPI;
        }
    }
}

void CTriangleTesselation::ComputeTextureCoordinates()
{
    static float fPI = 3.1415926f; //..7
    for (size_t nIterator = 0; nIterator < m_vecTriangleList[m_nArrayResult]->size(); ++nIterator)
    {
        CTriangle::SPoint3D* Point1 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint1();
        CTriangle::SPoint3D* Point2 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint2();
        CTriangle::SPoint3D* Point3 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint3();
        Point1->fUV.x = 0.5f+atan2f(Point1->fPos.x / m_fRadius, Point1->fPos.z / m_fRadius)/(2.0f*fPI);
        Point2->fUV.x = 0.5f+atan2f(Point2->fPos.x / m_fRadius, Point2->fPos.z / m_fRadius)/(2.0f*fPI);
        Point3->fUV.x = 0.5f+atan2f(Point3->fPos.x / m_fRadius, Point3->fPos.z / m_fRadius)/(2.0f*fPI);
        CorrectTextureOverflowU(m_vecTriangleList[m_nArrayResult]->at(nIterator));
        Point1->fUV.y = 0.5f-asinf(Point1->fPos.y / m_fRadius)/fPI;
        Point2->fUV.y = 0.5f-asinf(Point2->fPos.y / m_fRadius)/fPI;
        Point3->fUV.y = 0.5f-asinf(Point3->fPos.y / m_fRadius)/fPI;
    }
}

void CTriangleTesselation::ComputeTangentBitangent()
{
    for (size_t nIterator = 0; nIterator < m_vecTriangleList[m_nArrayResult]->size(); ++nIterator)
    {
        CTriangle::SPoint3D *Point1 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint1();
        CTriangle::SPoint3D *Point2 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint2();
        CTriangle::SPoint3D *Point3 = m_vecTriangleList[m_nArrayResult]->at(nIterator).GetPoint3();

        if(Point1->fPos.x == 0.0f && Point1->fPos.z == 0.0f)
        {
            Point1->fT = glm::vec3(0,0,1);
        }
        else
        {
            ComputePointTangent(Point1);
        }
        if(Point2->fPos.x == 0.0f && Point2->fPos.z == 0.0f)
        {
            Point2->fT = glm::vec3(0,0,1);
        }
        else
        {
            ComputePointTangent(Point2);
        }
        if(Point3->fPos.x == 0.0f && Point3->fPos.z == 0.0f)
        {
            Point3->fT = glm::vec3(0,0,1);
        }
        else
        {
            ComputePointTangent(Point3);
        }
    }
}

void CTriangleTesselation::ComputePointTangent(CTriangle::SPoint3D *Point) {
    glm::vec3 normal = Point->fPos;
    glm::vec3 tangent = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), normal);
    tangent = glm::normalize(tangent);
    Point->fT = tangent;
}


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
    float phi = 1.0f / 2.0f*(1.0f + sqrtf(5.0f));

    CTriangle::SPoint3D Point1;
    //Point1.fPos(0.0f,1.0f,phi);
    Point1.fPos.x = 0.0f;
    Point1.fPos.y = 1.0f;
    Point1.fPos.z = phi;
    Point1.fPos = glm::normalize(Point1.fPos);
    Point1.fPos = Point1.fPos * m_fRadius;

    //rim+
    CTriangle::SPoint3D Point2;
    Point2.fPos.x = 0.0f;
    Point2.fPos.y = 1.0f;
    Point2.fPos.z = -phi;
    Point2.fPos = glm::normalize(Point2.fPos);
    Point2.fPos = Point2.fPos * m_fRadius;

    CTriangle::SPoint3D Point3;
    Point3.fPos.x = 0.0f;
    Point3.fPos.y = -1.0f;
    Point3.fPos.z = phi;
    Point3.fPos = glm::normalize(Point3.fPos);
    Point3.fPos = Point3.fPos * m_fRadius;

    //rim+
    CTriangle::SPoint3D Point4;
    Point4.fPos.x = 0.0f;
    Point4.fPos.y = -1.0f;
    Point4.fPos.z = -phi;
    Point4.fPos = glm::normalize(Point4.fPos);
    Point4.fPos = Point4.fPos * m_fRadius;

    //rim+
    CTriangle::SPoint3D Point5;
    Point5.fPos.x = 1.0f;
    Point5.fPos.y = phi;
    Point5.fPos.z = 0.0f;
    Point5.fPos = glm::normalize(Point5.fPos);
    Point5.fPos = Point5.fPos * m_fRadius;

    //rim+
    CTriangle::SPoint3D Point6;
    Point6.fPos.x = 1.0f;
    Point6.fPos.y = -phi;
    Point6.fPos.z = 0.0f;
    Point6.fPos = glm::normalize(Point6.fPos);
    Point6.fPos = Point6.fPos * m_fRadius;

    CTriangle::SPoint3D Point7;
    Point7.fPos.x = -1.0f;
    Point7.fPos.y = phi;
    Point7.fPos.z = 0.0f;
    Point7.fPos = glm::normalize(Point7.fPos);
    Point7.fPos = Point7.fPos * m_fRadius;

    CTriangle::SPoint3D Point8;
    Point8.fPos.x = -1.0f;
    Point8.fPos.y = -phi;
    Point8.fPos.z = 0.0f;
    Point8.fPos = glm::normalize(Point8.fPos);
    Point8.fPos = Point8.fPos * m_fRadius;

    CTriangle::SPoint3D Point9;
    Point9.fPos.x = phi;
    Point9.fPos.y = 0.0f;
    Point9.fPos.z = 1.0f;
    Point9.fPos = glm::normalize(Point9.fPos);
    Point9.fPos = Point9.fPos * m_fRadius;

    CTriangle::SPoint3D PointA;
    PointA.fPos.x = -phi;
    PointA.fPos.y = 0.0f;
    PointA.fPos.z = 1.0f;
    PointA.fPos = glm::normalize(PointA.fPos);
    PointA.fPos = PointA.fPos * m_fRadius;

    CTriangle::SPoint3D PointB;
    PointB.fPos.x = phi;
    PointB.fPos.y = 0.0f;
    PointB.fPos.z = -1.0f;
    PointB.fPos = glm::normalize(PointB.fPos);
    PointB.fPos = PointB.fPos * m_fRadius;

    CTriangle::SPoint3D PointC;
    PointC.fPos.x = -phi;
    PointC.fPos.y = 0.0f;
    PointC.fPos.z = -1.0f;
    PointC.fPos = glm::normalize(PointC.fPos);
    PointC.fPos = PointC.fPos * m_fRadius;

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
    //rim
    //m_vecTriangleList[0]->push_back(CTriangle(Point2, Point1, Point3));
    //m_vecTriangleList[0]->push_back(CTriangle(Point4, Point2, Point3));


}

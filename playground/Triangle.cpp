//#include "stdafx.h"
#include "Triangle.h"


CTriangle::CTriangle()
{
}

CTriangle::CTriangle(SPoint3D Point1, SPoint3D Point2, SPoint3D Point3)
{
	m_Point1 = Point1;
	m_Point2 = Point2;
	m_Point3 = Point3;
}

CTriangle::~CTriangle()
{
}

CTriangle::SPoint3D* CTriangle::GetPoint1() 
{
	return &m_Point1;
}

CTriangle::SPoint3D* CTriangle::GetPoint2()
{
	return &m_Point2;
}

CTriangle::SPoint3D* CTriangle::GetPoint3()
{
	return &m_Point3;
}

void CTriangle::SetPoint1(SPoint3D Point)
{
	m_Point1 = Point;
}

void CTriangle::SetPoint2(SPoint3D Point)
{
	m_Point2 = Point;
}

void CTriangle::SetPoint3(SPoint3D Point)
{
	m_Point3 = Point;
}

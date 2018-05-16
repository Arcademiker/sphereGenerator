#pragma once
#include <cmath>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CTriangle
{
public:
	struct SPoint3D
	{
        glm::vec3 fPos = glm::vec3(0.0f, 0.0f, 0.0f); //same as normals
        glm::vec2 fUV = glm::vec2(0.0f,0.0f);
        glm::vec3 fT = glm::vec3(0.0f,0.0f,0.0f);
	};

	//CTriangle();
	CTriangle(SPoint3D Point1, SPoint3D Point2, SPoint3D Point3);
	~CTriangle();

	SPoint3D* GetPoint1();
	SPoint3D* GetPoint2();
	SPoint3D* GetPoint3();
    const SPoint3D* GetPoints(int n) const;

	//void SetPoint1(SPoint3D Point);
	//void SetPoint2(SPoint3D Point);
	//void SetPoint3(SPoint3D Point);

private:
	//SPoint3D m_Point1;
	//SPoint3D m_Point2;
	//SPoint3D m_Point3;
	SPoint3D m_Points[3];
};


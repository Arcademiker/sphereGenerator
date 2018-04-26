#pragma once
#include <vector>
#include <cstdint>
#include "Triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CTriangleTesselation
{
public:
	CTriangleTesselation(float fRadius);
	~CTriangleTesselation();

	void Tesselate(uint32_t nIterations = 5);
	const std::vector<CTriangle>* GetTriangleList() const;

private:

	void GenerateTetraeder();
	void ComputeTextureCoordinates();
    void ComputeTangentBitangent();
    void ComputePointTangent(CTriangle::SPoint3D *Point);

	size_t m_nArrayResult = 0;
	float m_fRadius = 1.0f;
	std::vector<CTriangle>* m_vecTriangleList[2];
};


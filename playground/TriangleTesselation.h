#pragma once
#include <vector>
#include <cstdint>
#include "Triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graph.h"

class CTriangleTesselation
{
public:
	CTriangleTesselation(float fRadius, uint32_t nIterations);
	~CTriangleTesselation();

	void Tesselate(uint32_t nIterations = 5);
	const std::vector<CTriangle>* GetTriangleList() const;
    const CGraph* GetGraph() const;

private:

	void GenerateTetraeder();
    void GenerateGraph();
	void ComputeTextureCoordinates();
    void ComputeTangentBitangent();
    void ComputePointTangent(CTriangle::SPoint3D *Point);


	size_t m_nArrayResult = 0;
	float m_fRadius = 1.0f;
	std::vector<CTriangle>* m_vecTriangleList[2];
    CGraph* m_graph;
};


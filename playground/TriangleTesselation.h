#pragma once
#include <vector>
#include <cstdint>
#include "Triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "Graph.h"
// Include OpenCV for image loading
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

class CTriangleTesselation
{
public:
	CTriangleTesselation(float fRadius, uint32_t nIterations);
	~CTriangleTesselation();

	void Tesselate(uint32_t nIterations = 5);
	const std::vector<CTriangle>* GetTriangleList() const;
	//const CTriangle::SPoint3D* GetPointofTriangleList(std::pair<int,int> TPID) const;
    CGraph* GetGraph();
    //int updateNVertices() { this->m_nNVertices = this->m_nNVertices*4-6; }

private:

	void GenerateTetraeder();
    //void GenerateGraph();
	void ComputeTextureCoordinates();
    void ComputeTangentBitangent();
    void ComputePointTangent(CTriangle::SPoint3D *Point);
    void loadHeightMap();
    void calcEdgeWeight();
    void CorrectTextureOverflowU(CTriangle::SPoint3D& Point1,CTriangle::SPoint3D& Point2,CTriangle::SPoint3D& Point3);

	cv::Mat* heightMap;


	size_t m_nMeshSwitcher = 0;
	float m_fRadius = 1.0f;
	std::vector<CTriangle>* m_dualTriangleList[2];
    
    size_t m_nGraphSwitcher = 0;
    CGraph* m_dualGraph[2];
    //CGraph* m_graph;
    size_t m_nNVertices = 12;
};


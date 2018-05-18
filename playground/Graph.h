//
// Created by Thomas Engels on 09.05.18.
//

#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>
//#include "Triangle.h"



class CGraph {
private:
    std::vector<std::unordered_map<int,int>>* G;
    std::vector<std::pair<int,int>>* G3DTable;
    //std::pair<std::unordered_map<int,int>::iterator, bool> itAdjcent;
    std::vector<std::vector<int>>* m_matPointsofTraingle; //todo rename
    std::unordered_map<int,float>* m_edgeList;
    size_t size;
    int edgeCounter;
    int triangleCounter;



public:
    CGraph(size_t size);
    ~CGraph();
    //std::vector<CTriangle::SPoint3D*>* G3D; //todo addEdge and datastructure changes
    bool addEdge(int u, int v, float w);
    float getEdgeWeight(int e);
    std::unordered_map<int,int> getAdjacent(int u);
    void addTriangle(int point1,int point2,int point3);
    std::pair<int,int> get3DPointIDofVertexID(int VertexID);
    void reconstructGraph(size_t size);
    std::vector<int> getPointsofTriangle(int triangleID);
    size_t getSize();
    void setEdgeWeights(int VertexUID, int VertexVID, float w);
    int getNTriangles();
    int getEdgeCounter();

    void printGraph();
};

//std::vector<std::vector<std::pair<int,int>>>* G;

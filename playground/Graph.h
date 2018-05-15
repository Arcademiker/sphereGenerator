//
// Created by Thomas Engels on 09.05.18.
//

#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

class CGraph {
private:
    std::vector<std::unordered_map<int,int>>* G;
    //std::pair<std::unordered_map<int,int>::iterator, bool> itAdjcent;
    std::vector<std::vector<int>>* m_matPointsofTraingle;
    std::unordered_map<int,int>* m_edgeList;
    size_t size;
    int edgeCounter;
    int triangleCounter;

public:
    CGraph(size_t size);
    ~CGraph();
    bool addEdge(int u, int v, int w);
    int getVerticesOfEdge(int e);
    std::unordered_map<int,int> getAdjacent(int u);
    void addTriangle(int point1,int point2,int point3);
    void reconstructGraph(size_t size);
    std::vector<int> getPointsofTriangle(int triangleID);
    size_t getSize();
    int getNTriangles();

    void printGraph();
};

//std::vector<std::vector<std::pair<int,int>>>* G;

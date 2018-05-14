//
// Created by Thomas Engels on 09.05.18.
//

#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

class CGraph {
private:
    std::vector<std::vector<std::pair<int,int>>>* G;
    std::vector<std::vector<unsigned int>>* m_matPointsofTraingle;
    std::unordered_map<int,int>* m_edgeList;
    unsigned int size;
    int edgeCounter;

public:
    explicit CGraph(unsigned int size);
    ~CGraph();
    void addEdge(int u, int v, int e);
    int getEdge(unsigned int u, unsigned int triangleID);
    std::vector<std::pair<int,int>> getAdjacent(unsigned int u);
    void addTriangle(unsigned int point1,unsigned int point2,unsigned int point3, unsigned int triangleID);
    void reconstructGraph(unsigned int size);
    std::vector<unsigned int> getPointsofTriangle(size_t triangleID);
    int getSize();

    void printGraph();
};


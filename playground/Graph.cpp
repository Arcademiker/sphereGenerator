//
// Created by Thomas on 09.05.18.
//

#include "Graph.h"

/* todo:
 * vertex reference to 3D point and vice versa: solution same numbering
 * weights pair vector
 * order doesnt matter
 * 3D heuristik
 * DOD vs OOD

 */


CGraph::CGraph(const unsigned int size) {
    this->size = size;
    this->G = new std::vector<std::vector<std::pair<int,int>>>(size);
    this->m_matPointsofTraingle = new std::vector<std::vector<unsigned int>>;
    this->m_edgeList = new std::unordered_map<int,int>;
    this->edgeCounter = 0;
}

CGraph::~CGraph() {
    delete this->G;
    delete this->m_matPointsofTraingle;
    delete this->m_edgeList;
}


///water low weight but land to water high cost
void CGraph::addEdge(int u, int v, int e) {
    //e = edge id
    //if(this->G->at(v)) test if u  exist
    this->G->at(u).push_back({v,edgeCounter});
    //this->G->at(v).push_back({u,w});
    this->m_edgeList->at(edgeCounter) = u;
    this->m_edgeList->at(-edgeCounter) = v;
    edgeCounter++;
}


void CGraph::addTriangle(unsigned int point1, unsigned int point2, unsigned int point3, unsigned int triangleID) {

    addEdge(point1, point2,1);

    addEdge(point2, point3,1);

    addEdge(point3, point1,1);
    //std::vector<unsigned int> triangle {point1,point2,point3};
    this->m_matPointsofTraingle[triangleID].push_back({point1,point2,point3});
}

void CGraph::reconstructGraph(unsigned int size) {
    delete this->G;
    delete this->m_matPointsofTraingle;
    delete this->m_edgeList;
    this->G = new std::vector<std::vector<std::pair<int,int>>>(size);
    this->m_matPointsofTraingle = new std::vector<std::vector<unsigned int>>;
    this->m_edgeList = new std::unordered_map<int,int>;
    this->edgeCounter = 0;
}

void CGraph::printGraph() {
    for (int v = 0; v < this->G->size(); ++v) {
        std::cout << "\n Adjacency list of vertex " << v << "\n head ";
        for (auto x : this->G->at(v)) {
            std::cout << "-> [" << x.first << ", " << x.second << "] ";
        }
        std::printf("\n");
    }
}

std::vector<unsigned int> CGraph::getPointsofTriangle(size_t triangleID) {
    //todo this performs a deep copy for the case this datastructure gets more complex
    std::vector<unsigned int> triangle((*m_matPointsofTraingle)[triangleID]);
    return triangle;
}

int CGraph::getSize() {
    return this->size;
}

int CGraph::getEdge(unsigned int u, unsigned int triangleID) {
    //todo return edge pointing on vertex withing specific triangle
    return this->m_edgeList->find(u)->second;
}

std::vector<std::pair<int, int>> CGraph::getAdjacent(unsigned int u) {
    return this->G->at(u);
}






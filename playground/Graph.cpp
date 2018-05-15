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


CGraph::CGraph(size_t size) {
    this->size = size;
    this->G = new std::vector<std::unordered_map<int,int>>(size,std::unordered_map<int,int>(6));
    //this->G = std::vector<std::unordered_map<int,int>>();
    this->m_matPointsofTraingle = new std::vector<std::vector<int>>;
    this->m_edgeList = new std::unordered_map<int,int>;
    this->edgeCounter = 1;
    this->triangleCounter = 0;
}

CGraph::~CGraph() {
    delete this->G;
    delete this->m_matPointsofTraingle;
    delete this->m_edgeList;
}


///water low weight but land to water high cost
bool CGraph::addEdge(int u, int v, int w) {
    //e = edge id
    //std::cout << "insert " << u << "--" << v << std::endl;
    //int test = this->G->at(v).count(u);
    //itAdjcent = G->at(u).insert(std::make_pair(v, 0));
    if(this->G->at(u).count(v)==0) // test if edge to u does not exist in v. average case const time
    {
        //todo: more intelligent insertion check
        this->G->at(u).insert({v,edgeCounter}); // = edgeCounter;
        this->G->at(v).insert({u,-edgeCounter});
        (*this->m_edgeList)[edgeCounter] = u; //pair for weights w
        (*this->m_edgeList)[-edgeCounter] = v;
        edgeCounter++;
        //todo: add edge w=weight list
        //this->printGraph();
        return true;
    }
    return false;
}


void CGraph::addTriangle(int point1, int point2, int point3) {

    addEdge(point1, point2,1);

    addEdge(point2, point3,1);

    addEdge(point3, point1,1);
    std::vector<int> triangle {point1,point2,point3};
    this->m_matPointsofTraingle->push_back(triangle);
    this->triangleCounter++;
}

void CGraph::reconstructGraph(size_t size) {
    delete this->G;
    delete this->m_matPointsofTraingle;
    delete this->m_edgeList;
    this->G = new std::vector<std::unordered_map<int,int>>(size,std::unordered_map<int,int>(6));
    this->m_matPointsofTraingle = new std::vector<std::vector<int>>;
    this->m_edgeList = new std::unordered_map<int,int>;
    this->edgeCounter = 1;
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

std::vector<int> CGraph::getPointsofTriangle(int triangleID) {
    //todo this performs a deep copy for the case this datastructure gets more complex
    std::vector<int> triangle((*m_matPointsofTraingle)[triangleID]);
    return triangle;
}

size_t CGraph::getSize() {
    return this->size;
}

int CGraph::getVerticesOfEdge(int e) {
    //todo return edge pointing on vertex withing specific triangle
    return this->m_edgeList->at(e);
}

std::unordered_map<int,int> CGraph::getAdjacent(int u) {
    return this->G->at(u);
}






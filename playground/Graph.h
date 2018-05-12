//
// Created by Thomas Engels on 09.05.18.
//

#pragma once

#include <vector>
#include <iostream>

class CGraph {
private:
    std::vector<std::vector<std::pair<int,int>>>* G;

public:
    explicit CGraph(unsigned int size);
    ~CGraph();
    void addEdge(int u, int v, int w);

    void printGraph();
};


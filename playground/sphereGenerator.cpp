// sphereGenerator.cpp : Defines the entry point for the console application.
//
/*
#include "stdafx.h"
#include "TriangleTesselation.h"
#include <vector>

int sphereGenerator()
{
	CTriangleTesselation TriangleTesselation(5);
	TriangleTesselation.Tesselate(5);
	const std::vector<CTriangle>* triangles = TriangleTesselation.GetTriangleList();

    //testzugriff
    CTriangle tmpTriangle;
    for(int i=0; i<triangles->size();i++)
    {
        tmpTriangle = triangles->at(i);
        std::cout << tmpTriangle.GetPoint1()->fU << " " << tmpTriangle.GetPoint1()->fV << std::endl;
        std::cout << tmpTriangle.GetPoint1()->fX << " " << tmpTriangle.GetPoint1()->fX << " "
                  << tmpTriangle.GetPoint1()->fZ << std::endl;
        std::cout << tmpTriangle.GetPoint2()->fU << " " << tmpTriangle.GetPoint2()->fV << std::endl;
        std::cout << tmpTriangle.GetPoint2()->fX << " " << tmpTriangle.GetPoint2()->fX << " "
                  << tmpTriangle.GetPoint2()->fZ << std::endl;
        std::cout << tmpTriangle.GetPoint3()->fU << " " << tmpTriangle.GetPoint3()->fV << std::endl;
        std::cout << tmpTriangle.GetPoint3()->fX << " " << tmpTriangle.GetPoint3()->fX << " "
                  << tmpTriangle.GetPoint3()->fZ << std::endl;
    }

    return 0;
}

*/
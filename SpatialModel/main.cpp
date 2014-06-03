//
//  main.cpp
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include "Globals.h"
#include "ModelWorld.h"

int main(int argc, const char * argv[])
{
    const int numParams = 1;
    int numRuns = 100, numSims = 1;
    int numSeed[numParams] = {1};
    int numPatches[numParams] = {1};
    int t_init[numParams] = {0};
    int t_max[numParams] = {50};
    bool rSeed[numParams] = {false};
    
    std::vector<SeedParam> param;

    // Make directories for output params.
    /*
     
     std::string dir = "paramset"+std::to_string(i);
     int check = mkdir(dir.c_str(), S_IRWXU);
     etc...
     */
    
    for (int i = 0; i < numParams; ++i)
    {
        SeedParam P( numSeed[i], numPatches[i], t_init[i], t_max[i] );

        if (rSeed[i]) P.randomSeed = true;
        
        param.push_back(P);
    }
    
    for (int i = 0; i < numRuns; ++i)
    {
        ModelWorld graph( numSims, i, param );
        graph.CallSim();
    }

}
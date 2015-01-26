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
#include <unistd.h>
#include "Globals.h"
#include "ModelWorld.h"


int main(int argc, const char * argv[])
{
    const int numParams = 1;
    int pSet = 3;
    int numRuns = 100, numSims = 1;
    int numSeed[numParams] = {1};
    int numPatches[numParams] = {1};
    int seedLevel = 2;
    int t_init[numParams] = {0};
    int t_max[numParams] = {10};
    bool rSeed[numParams] = {false};

    

    std::vector<SeedParam> param;

    // Make directories for output params.
    int size = 1000;;
    char path[size];
    getcwd(path, size);
    std::string cwd(path);
    std::string dir = "/Users/prianna/Documents/Dev/spatial-sim/Data/paramset"+std::to_string(pSet);
    
    if (path[0])
    {
        if (cwd != dir)
        {
            if (chdir(dir.c_str()))
            {
                int err = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                if (err)
                    std::cout << "Error in directory creation." << std::endl;
                else
                    chdir(dir.c_str());

            }
        }
    }
    else
        std::cout << "Null path specificed." << std::endl;

    // Initialize SeedParam vector.
    for (int i = 0; i < numParams; ++i)
    {
        SeedParam P( numSeed[i], numPatches[i], t_init[i], t_max[i], seedLevel );

        if (rSeed[i]) P.randomSeed = true;
        
        param.push_back(P);
    }
    
    // Main loop.
    for (int i = 0; i < numRuns; ++i)
    {
        ModelWorld graph( numSims, i, pSet, param );
        graph.CallSim();
    }

    std::cout << BETA_L0 << " " << KAPPA_L0 << " " << R_L0 << std::endl;
    std::cout << BETA_L1 << " " << KAPPA_L1 << " " << R_L1 << std::endl;
    std::cout << BETA_L2 << " " << KAPPA_L2 << " " << R_L2 << std::endl;

}
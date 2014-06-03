//
//  main.cpp
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#include <iostream>
#include "Globals.h"
#include "ModelWorld.h"

int main(int argc, const char * argv[])
{
    int numRuns = 1000, numParamSets = 1,
        numSeed = 2, numPatches = 1,
        t_init = 0, t_max = 50;
    
    int params[4] = {numSeed, numPatches, t_init, t_max};
    
    for (int i = 0; i < numRuns; ++i)
    {
        ModelWorld graph( numParamSets, i );
        graph.CallSim(params[0], params[1], params[2], params[3]);
    }
}
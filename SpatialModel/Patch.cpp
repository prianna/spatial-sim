//
//  Patch.cpp
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#include "Patch.h"

Patch::Patch( int L )
:nI(INIT_I), nR(INIT_R), level(L)
{
    switch (L)
    {
        case LEVEL_0:
            nS = INIT_S_0;
            beta = BETA_L0;
            break;
            
        case LEVEL_1:
            nS = INIT_S_1;
            beta = BETA_L1;
            break;
            
        case LEVEL_2:
            nS = INIT_S_2;
            beta = BETA_L2;
            break;
            
        default:
            break;
    }
    /*for (int i = 0; i < nS; ++i)
    {
        S.push_back(1);
    }*/
    nT = nS+nI+nR;
}

void Patch::AddNeighbor( const Patch *adjPatch, int i )
{
    Neighbor patch;
    patch.neighbor = adjPatch;
    patch.index = i;
    neighbors.push_back(patch);
}

std::vector<int> Patch::GetAdjIndices()
{
    std::vector<int> indices;
    
    for (std::vector<Neighbor>::iterator it = neighbors.begin();
         it != neighbors.end(); ++it)
    {
        indices.push_back(it->index);
    }
    
    return indices;
}
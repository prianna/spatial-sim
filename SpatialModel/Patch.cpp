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
        case '0':
            nS = INIT_S_0;
            beta = BETA_L0;
            break;
            
        case '1':
            nS = INIT_S_1;
            beta = BETA_L1;
            break;
            
        case '2':
            nS = INIT_S_2;
            beta = BETA_L2;
            break;
            
        default:
            break;
    }
    
    nT = nS;
    
    for (int i = 0; i < nS; ++i)
    {
        S.push_back(1);
    }
}

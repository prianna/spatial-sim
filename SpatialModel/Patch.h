//
//  Patch.h
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#ifndef __SpatialModel__Patch__
#define __SpatialModel__Patch__

#include "Globals.h"
#include <vector>

class Patch
{
    
public:
    // Constructor
    Patch( int L );
    
    // Accessor functions.
    int GetPopS(){ return nS; }
    int GetPopI(){ return nI; }
    int GetPopR(){ return nR; }
    int GetLevel(){ return level; }
    int GetTotalPop(){ return nT; }
    double GetBeta(){ return beta; }
    const std::vector<int> GetAdjIndices();
    
    template <typename T>
    void InsertPop( std::vector<T> &X )
    {
        X.push_back(&S);
        X.push_back(&I);
        X.push_back(&R);
    }
    
    void AddNeighbor( const Patch* adjacent, int index );
    
private:
    // Containers to hold susceptible, infected, and recovered individuals.
    // TEMPLATE THIS.
    std::vector<int> S, I, R, A;
    
    struct Neighbor
    {
        Patch* neighbor;
        int index;
    };
    std::vector<Neighbor> neighbors;
    std::vector<int> adjacentIndices;
    // Current number of suseceptibles, infecteds, recovereds, and total.
    int nS, nI, nR, nT;
    int level;
    double beta;
};

#endif /* defined(__SpatialModel__Patch__) */

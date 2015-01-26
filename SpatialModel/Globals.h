//
//  Globals.h
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#ifndef SpatialModel_Globals_h
#define SpatialModel_Globals_h

// HELPER FUNCTIONS

// Need this to generate powers at compile time.
template<class T>
inline constexpr T POW( const T base, unsigned const power )
{
    return (power == 0) ? 1 : (base * POW(base, power-1));
}


// STRUCTURAL CONSTANTS
constexpr int BRANCH = 4; // Branching ratio for tree.
const int HEIGHT = 3; // Height of tree.
const int MAX_LEVEL = 2; // Index of max level, counting from 0.
const int MIN_INDEX_L0 = 0; // Min index of L0 nodes.
const int MIN_INDEX_L1 = 1; // Min index of L1 nodes.
const int MIN_INDEX_L2 = 5; // Min index of L2 nodes.
constexpr int LEVELS[HEIGHT] = {0, 1, 2};
constexpr int LEVEL_0 = 0; // Root node level.
constexpr int LEVEL_1 = 1; // Nonleaf node level.
constexpr int LEVEL_2 = 2; // Leaf node level.

const int NUM_L0 = POW(BRANCH, LEVEL_0);
const int NUM_L1 = POW(BRANCH, LEVEL_1);
const int NUM_L2 = POW(BRANCH, LEVEL_2);
const int NUM_NODES = NUM_L0+NUM_L1+NUM_L2;

const int INIT_I = 0; // Initially, infected pop is 0.
const int INIT_R = 0; // Initially, recovered pop is 0.



// POPULATION
const int MAGNITUDE_L0 = 1000; // Population scaling magnitudes.
const int MAGNITUDE_L1 = 25;
const int MAGNITUDE_L2 = 25;

const int NUM_STATES = 3;
enum{ SUS, INF, REC } STATES;
const int DIM = NUM_NODES*NUM_STATES;
// Initial populations at root, nonleaf, leaf node levels.
const int INIT_S_0 = static_cast<int>(POW(BRANCH, MAX_LEVEL-LEVEL_0)
                                      *1000);
const int INIT_S_1 = static_cast<int>(POW(BRANCH, MAX_LEVEL-LEVEL_1)
                                      *25);
const int INIT_S_2 = static_cast<int>(POW(BRANCH, MAX_LEVEL-LEVEL_2)
                                      *25);

// PARAMETER
const double EPSILON_L0 = 0.195;
const double EPSILON_L1 = EPSILON_L0;
const double EPSILON_L2 = EPSILON_L0;
const double RISK = 1.0;
const double GAMMA = 5.0;
const double FLOW = 0.10;

const double KAPPA_L0 = 1/(1-EPSILON_L0+EPSILON_L0*INIT_S_0);
const double KAPPA_L1 = 1/(1-EPSILON_L1+EPSILON_L1*INIT_S_1);
const double KAPPA_L2 = 1/(1-EPSILON_L2+EPSILON_L2*INIT_S_2);

const double BETA_L0 = RISK*KAPPA_L0;
const double BETA_L1 = RISK*KAPPA_L1;
const double BETA_L2 = RISK*KAPPA_L2;

// DESIRED REPRODUCTIVE NUMBER
const double R_L0 = (BETA_L0*INIT_S_0)/GAMMA;
const double R_L1 = (BETA_L1*INIT_S_1)/GAMMA;
const double R_L2 = (BETA_L2*INIT_S_2)/GAMMA;



#endif
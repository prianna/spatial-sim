//
//  Gillespie.h
//  SpatialModel
//
//  Created by Prianna Ahsan on 5/21/14.
//  Copyright (c) 2014 Prianna Ahsan. All rights reserved.
//

#ifndef __SpatialModel__Gillespie__
#define __SpatialModel__Gillespie__

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "Globals.h"
#include "Patch.h"

static const int MAX_EVENTS = NUM_STATES+BRANCH;

//! brief State struct holds set of events associated w/ element in curStates.
struct State
{
    double event[MAX_EVENTS] = {};
    std::vector<int> adj;
    int S, I, R, N;
    double sup = 0, inf = 0, totalRate = 0;

};

//! brief Helper function for struct State, overloads == operator.
//! param LHS State object.
//! param RHS Value to compare.
//! return Boolean true or false.
inline bool operator==( const State &LHS, const double &RHS )
{ return ( LHS.inf < RHS && LHS.sup <= RHS ) ? true:false; }

//! brief Helper function for struct State, overloads < operator.
//! param LHS State object.
//! param RHS Value to compare.
//! return Boolean true or false.
inline bool operator<( const State &LHS, const double &RHS )
{ return ( LHS.sup < RHS ) ? true:false; }

//! brief Helper function for struct State, overloads > operator.
//! param LHS State object.
//! param RHS Value to compare.
//! return Boolean true or false.
inline bool operator>( const State &LHS, const double &RHS )
{ return ( LHS.inf >= RHS ) ? true:false; }

class Gillespie
{
 public:
    Gillespie( std::vector<Patch*> States );
    double Iterate();

 private:
    double ComputeTotalRate();
    int TypeToMove( const State &i );
    void MakeMove( int from, int to, int type );

    std::vector<State> curStates;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> runif;
};

#endif /* defined(__SpatialModel__Gillespie__) */

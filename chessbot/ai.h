// Andrew Lee
// 1 May 2021
// ai.h
/*
* This header defines the engine's AI using an imperitive model
* The board is an object, but the AI is a set of functions
*/
# pragma once
#include "board.h"

namespace chessbot
{

double getHeuristic( board& b );
double minimax( board b, int depth );
double alphabeta( board b, int depth, double alpha, double beta );

}
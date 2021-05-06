// Andrew Lee
// 1 May 2021
// ai.h
/*
* This header defines the engine's AI using an imperitive model
* The board is an object, but the AI is a set of functions
*/
# pragma once
class board;

namespace chessbot
{

int getHeuristic( const board& b );
double minimax( board b, int depth );
board alphabeta( board b, int depth, int maxdepth, int alpha, int beta );

}
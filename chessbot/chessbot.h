// chessbot.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <future>
#include <chrono>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// TODO: Reference additional headers your program requires here.
#include "board.h"
#include "ai.h"
namespace py = pybind11;
namespace chessbot
{ 


std::array<int, 64> getCurrentBoard();
unsigned char getGameCtrlFlags();
std::array<int, 64> getAutomaticMove(int depth);
std::string makeAutomaticMove(int depth);
bool makeManualMove( std::string UCImove );
bool whiteCheckmated();
bool blackCheckmated();
bool stalemate();
bool thriceRepetition();

namespace game
{
	board currentBoard = board::startingboard();
	std::vector<board> boardHistory;
}





PYBIND11_MODULE( chessbot, m )
{
	m.doc() = "Pybind chessbot engine";
	m.def( "getCurrentBoard", &getCurrentBoard, "Gets the current board state." );
	m.def( "getGameCtrlFlags", &getGameCtrlFlags, "Gets other info for board state." );
	m.def( "getAutomaticMove", &getAutomaticMove, "Use the AI to get its best move, without affecting the game state." );
	m.def( "makeAutomaticMove", &makeAutomaticMove, "Have the AI make its best move, updating the game state." );
	m.def( "makeManualMove", &makeManualMove, "Enter a move on the board manually." );
	m.def( "whiteCheckmated", &whiteCheckmated, "Returns True if white has been checkmated." );
	m.def( "blackCheckmated", &blackCheckmated, "Returns True if black has been checkmated." );
	m.def( "stalemate", &stalemate, "Returns True if the board is at a stalemate position." );
	m.def( "thriceRepetition", &thriceRepetition, "Returns true if the same position has been repeated 3 times, resulting in a draw." );
}

}
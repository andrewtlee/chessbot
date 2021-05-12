// chessbot.cpp : Defines the entry point for the application.
//
#include "chessbot.h"
namespace chessbot
{ 

void printchessboard( chessbot::board b )
{
	for( int i = 7; i >= 0; i-- )
	{
		for( int j = 0; j < 8; j++ )
		{
			std::cout << static_cast<int>(b.getSpace( j, i )) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

std::array<int, 64> getCurrentBoard()
{
	std::array<int, 64> currboard;
	for( int i = 0; i < 64; i++ )
	{
		currboard.at( i ) = game::currentBoard.getSpace( i % 8, i / 8 );
	}
	return currboard;
}

unsigned char getGameCtrlFlags()
{
	return game::currentBoard.gameCtrlFlags;
}

std::array<int, 64> getAutomaticMove( int depth )
{
	auto gen = game::currentBoard.getMove();
	std::vector<std::future<board>> fmoves;
	while( gen )
	{
		fmoves.push_back( std::async( alphabeta, gen(), 0, depth, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max() ) );
	}
	board bestmove;
	bestmove.heuristicVal = game::currentBoard.gameCtrlFlags & whiteToMove ? std::numeric_limits<int>::lowest() : std::numeric_limits<int>::max();
	for( auto& m : fmoves )
	{
		board b = m.get();
		std::cout << b.heuristicVal << "\n";
		if( game::currentBoard.gameCtrlFlags & whiteToMove )
		{
			if( b.heuristicVal > bestmove.heuristicVal )
			{
				bestmove = b;
			}
		}
		else
		{
			if( b.heuristicVal < bestmove.heuristicVal )
			{
				bestmove = b;
			}
		}
	}
	std::array<int, 64> currboard;
	for( int i = 0; i < 64; i++ )
	{
		currboard.at( i ) = bestmove.getSpace( i % 8, i / 8 );
	}
	return currboard;
}

std::string makeAutomaticMove(int depth)
{
	auto gen = game::currentBoard.getMove();
	std::vector<std::future<board>> fmoves;
	if( !gen )
	{
		game::currentBoard.gameCtrlFlags |= gameOver;
		return std::string(""); // no legal moves
	}
	while( gen )
	{
		auto b = gen();
		//printchessboard( b );
		fmoves.push_back( std::async( alphabeta, b, 0, depth, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max() ) );
	}
	board bestmove;
	bestmove.heuristicVal = game::currentBoard.gameCtrlFlags & whiteToMove ? std::numeric_limits<int>::lowest() : std::numeric_limits<int>::max();
	for( auto& m : fmoves )
	{
		board b = m.get();
		if( game::currentBoard.gameCtrlFlags & whiteToMove )
		{
			if( b.heuristicVal > bestmove.heuristicVal )
			{
				bestmove = b;
			}
		}
		else
		{
			if( b.heuristicVal < bestmove.heuristicVal )
			{
				bestmove = b;
			}
		}
	}
	game::currentBoard = bestmove;
	game::boardHistory.push_back( game::currentBoard );
	return game::currentBoard.getUCIMove(); // report success
}

bool makeManualMove( std::string UCImove )
{
	auto legalMoves = game::currentBoard.getLegalMoves();
	if( legalMoves.empty() )
	{
		game::currentBoard.gameCtrlFlags |= gameOver;
		return false;
	}
	for( auto& m : legalMoves )
	{
		//std::cout << m.getUCIMove() << "\n";
		if( UCImove == m.getUCIMove() )
		{
			game::currentBoard = m;
			game::boardHistory.push_back( game::currentBoard );
			return true;
		}
	}
	return false;
}

bool whiteCheckmated()
{
	return !game::currentBoard.getLegalMoves().size() && game::currentBoard.isWhiteInCheck() && (game::currentBoard.gameCtrlFlags & whiteToMove);
}

bool blackCheckmated()
{
	return !game::currentBoard.getLegalMoves().size() && game::currentBoard.isBlackInCheck() && !(game::currentBoard.gameCtrlFlags & whiteToMove);
}

bool stalemate()
{
	return (!game::currentBoard.getLegalMoves().size() && !game::currentBoard.isWhiteInCheck() && (game::currentBoard.gameCtrlFlags & whiteToMove))
	|| (!game::currentBoard.getLegalMoves().size() && !game::currentBoard.isBlackInCheck() && !(game::currentBoard.gameCtrlFlags & whiteToMove));
	return false;
}

bool thriceRepetition()
{
	for( auto& b : game::boardHistory )
	{
		//std::cout << std::count( game::boardHistory.begin(), game::boardHistory.end(), b );
		if( std::count( game::boardHistory.begin(), game::boardHistory.end(), b ) >= 3 )
			return true;
	}
	return false;
}



}
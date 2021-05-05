// chessbot.cpp : Defines the entry point for the application.
//
#include "chessbot.h"
namespace chessbot
{ 

game G;

void printchessboard( chessbot::board b )
{
	for( int i = 7; i >= 0; i-- )
	{
		for( int j = 0; j < 8; j++ )
		{
			std::cout << static_cast<int>(b.getSpace( i, j )) << " ";
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
		currboard.at( i ) = G.currentBoard.getSpace( i / 8, i % 8 );
	}
	return currboard;
}

unsigned char getGameCtrlFlags()
{
	return G.currentBoard.gameCtrlFlags;
}

std::array<int, 64> getAutomaticMove( int depth )
{
	auto gen = G.currentBoard.getMove();
	std::vector<std::future<board>> fmoves;
	while( gen )
	{
		fmoves.push_back( std::async( alphabeta, gen(), depth, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() ) );
	}
	board bestmove;
	bestmove.heuristicVal = G.currentBoard.gameCtrlFlags & whiteToMove ? std::numeric_limits<double>::lowest() : std::numeric_limits<double>::max();
	for( auto& m : fmoves )
	{
		board b = m.get();
		
		//std::cout << v << " ";
		if( G.currentBoard.gameCtrlFlags & whiteToMove )
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
		currboard.at( i ) = bestmove.getSpace( i / 8, i % 8 );
	}
	return currboard;
}

bool makeAutomaticMove(int depth)
{
	auto gen = G.currentBoard.getMove();
	std::vector<std::future<board>> fmoves;
	if( !gen )
	{
		return false; // no legal moves
	}
	while( gen )
	{
		fmoves.push_back( std::async( alphabeta, gen(), depth, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() ) );
	}
	board bestmove;
	bestmove.heuristicVal = G.currentBoard.gameCtrlFlags & whiteToMove ? std::numeric_limits<double>::lowest() : std::numeric_limits<double>::max();
	for( auto& m : fmoves )
	{
		//printchessboard( moves.at( i ) );
		board b = m.get();
		//std::cout << v << " ";
		if( G.currentBoard.gameCtrlFlags & whiteToMove )
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
	//std::array<int, 64> currboard;
	//for( int i = 0; i < 64; i++ )
	//{
	//	currboard.at( i ) = bestmove.getSpace( i / 8, i % 8 );
	//}
	G.currentBoard = bestmove;
	return true; // report success
}

bool makeManualMove( std::array<int, 64> move )
{
	auto legalMoves = G.currentBoard.getLegalMoves();
	if( legalMoves.empty() )
	{
		return false; // no legal moves
	}
	for( auto& m : legalMoves )
	{
		bool match = true;
		for( int i = 0; i < 64; i++ )
		{
			if( move.at( i ) != m.spaces.at( i ) )
			{
				match = false;
				break;
			}
		}
		if( match )
		{
			G.currentBoard = m;
			return true;
		}
	}
	return false;
}



}
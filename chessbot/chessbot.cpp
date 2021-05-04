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
	std::vector<std::future<double>> vals;
	std::vector<board> moves;
	while( gen )
	{
		moves.push_back( gen() );
		vals.push_back( std::async( alphabeta, moves.back(), depth, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() ) );
	}
	board bestmove;
	
	for( int i = 0; i < moves.size(); i++ )
	{
		double v = vals.at( i ).get();
		
		//std::cout << v << " ";
		if( G.currentBoard.gameCtrlFlags & whiteToMove )
		{
			double best = std::numeric_limits<double>::lowest();
			if( v > best )
			{
				best = v;
				bestmove = moves.at( i );
			}
		}
		else
		{
			double best = std::numeric_limits<double>::max();
			if( v < best )
			{
				best = v;
				bestmove = moves.at( i );
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

void makeAutomaticMove(int depth)
{
	auto gen = G.currentBoard.getMove();
	std::vector<std::future<double>> vals;
	std::vector<board> moves;
	while( gen )
	{
		moves.push_back( gen() );
		vals.push_back( std::async( alphabeta, moves.back(), depth, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() ) );
	}
	board bestmove;
	auto best = G.currentBoard.gameCtrlFlags & whiteToMove ? std::numeric_limits<double>::lowest() : std::numeric_limits<double>::max();
	for( int i = 0; i < moves.size(); i++ )
	{
		//printchessboard( moves.at( i ) );
		double v = vals.at( i ).get();
		//std::cout << v << " ";
		if( G.currentBoard.gameCtrlFlags & whiteToMove )
		{
			if( v > best )
			{
				best = v;
				bestmove = moves.at( i );
			}
		}
		else
		{
			if( v < best )
			{
				best = v;
				bestmove = moves.at( i );
			}
		}
	}
	//std::array<int, 64> currboard;
	//for( int i = 0; i < 64; i++ )
	//{
	//	currboard.at( i ) = bestmove.getSpace( i / 8, i % 8 );
	//}
	G.currentBoard = bestmove;
}

}
/*
int main()
{

	board b = board::startingboard();
	auto moves = b.getLegalMoves();
	std::cout << moves.size() << "\n";
	auto best = std::numeric_limits<double>::lowest();
	board bestmove;
	std::vector<std::future<double>> vals;
	auto start = std::chrono::steady_clock::now();
	auto gen = b.getMove();
	while( gen )
	{
		vals.push_back( std::async( alphabeta, gen(), 7, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() ) );
	}
	for( int i = 0; i < moves.size(); i++ )
	{
		double v = vals.at( i ).get();
		std::cout << v << " ";
		if( v > best )
		{
			best = v;
			bestmove = moves.at( i );
		}
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "\n";
	std::cout << "Async time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
	printchessboard( bestmove);
	return 0;
}
*/
// chessbot.cpp : Defines the entry point for the application.
//

#include "chessbot.h"
#include <chrono>
using namespace chessbot; 

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
}
// chessbot.cpp : Defines the entry point for the application.
//

#include "chessbot.h"
using namespace chessbot; 

int main()
{
	board b = board::startingboard();
	auto moves = b.getLegalMoves();
	std::cout << moves.size() << "\n";
	auto best = std::numeric_limits<double>::lowest();
	board bestmove;
	std::vector<std::future<double>> vals;
	for( auto& m : moves )
	{
		vals.push_back( std::async( minimax, m, 1 ) );
	}
	for( int i = 0; i < moves.size(); i++ )
	{
		if(vals.at( i ).get() > best )
		{
			best = vals.at( i ).get();
			bestmove = moves.at( i );
		}
	}
	std::cout << "\n";
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
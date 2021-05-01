// chessbot.cpp : Defines the entry point for the application.
//

#include "chessbot.h"
using namespace chessbot; 

int main()
{
	board b = board::startingboard();
	auto moves = board::getLegalMoves( b );
	std::cout << moves.size() << "\n";
	auto responses = board::getLegalMoves( moves.at( 0 ) );
	moves = board::getLegalMoves( responses.at( 0 ) );
	for( auto& m : moves )
	{
		printchessboard( m );
	}
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
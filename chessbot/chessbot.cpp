// chessbot.cpp : Defines the entry point for the application.
//

#include "chessbot.h"
using namespace chessbot; 

int main()
{
	board b = board::startingboard();
	auto moves = b.getLegalMoves();
	std::cout << moves.size() << "\n";
	auto responses = moves.at(0).getLegalMoves( );
	moves = responses.at(0).getLegalMoves();
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
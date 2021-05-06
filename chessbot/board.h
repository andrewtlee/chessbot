// Andrew Lee
// 30 April 2021
// board.h
/*
* This header file defines the board class for the chess engine.
* The board is responsible for tracking the entire game-state,
* and therefore the board must also be able to generate all boards 
* resulting from leval moves given its own state.
* 
* We define control flags as bits so that all flags
* (currently 5 but up to 8 possible) take only one byte in the class.
* Using 1 byte instead of 5 is a small improvement, but we expect thousands of
* boards to be in memory at a given time, so it's not nothing.
* 
* startingboard() yeilds the standard chess starting board. This may be expanded
* in the future to provide handicapped boards.
*/
# pragma once
#include <array>
#include <stdint.h>
#include <vector>
#include "generator.h"

namespace chessbot
{
  
constexpr unsigned char whiteToMove = 0x01;
constexpr unsigned char whiteCanNotCastleKingside = 0x02;
constexpr unsigned char whiteCanNotCastleQueenside = 0x04;
constexpr unsigned char blackCanNotCastleKingside = 0x08;
constexpr unsigned char blackCanNotCastleQueenside = 0x10;
constexpr unsigned char gameOver = 0x20;
constexpr int8_t OFFBOARD = -128;

class board
{
public:
   [[nodiscard]] std::vector<board> getLegalMoves();
   [[nodiscard]] Generator<board> getMove();
   [[nodiscard]] static board startingboard();

   board() = default;
   ~board() = default;
   bool operator==( board b );
   bool operator==( board&& b );

   void placePiece( int8_t id, int8_t row, int8_t col );
   void removePiece( int8_t row, int8_t col );

   int8_t getSpace( int8_t row, int8_t col ) const;
   bool isWhiteInCheck() const;
   bool isBlackInCheck() const;

   unsigned char gameCtrlFlags = whiteToMove;
   int heuristicVal = 0;
   std::array<int8_t, 64> spaces{ 0 };
protected:
   std::pair<int8_t, int8_t> enpassant = { OFFBOARD, OFFBOARD };
   
   // helper functions to make getLegalMoves readable

   Generator<board> generateWhitePawnMoves(int8_t row, int8_t col ) const;
   Generator<board> generateBlackPawnMoves( int8_t row, int8_t col ) const;
   Generator<board> generateWhiteKnightMoves( int8_t row, int8_t col ) const;
   Generator<board> generateBlackKnightMoves( int8_t row, int8_t col ) const;
   Generator<board> generateWhiteBishopMoves( int8_t row, int8_t col ) const;
   Generator<board> generateBlackBishopMoves( int8_t row, int8_t col ) const;
   Generator<board> generateWhiteRookMoves( int8_t row, int8_t col ) const;
   Generator<board> generateBlackRookMoves( int8_t row, int8_t col ) const;
   Generator<board> generateWhiteQueenMoves( int8_t row, int8_t col ) const;
   Generator<board> generateBlackQueenMoves( int8_t row, int8_t col ) const;
   Generator<board> generateWhiteKingMoves( int8_t row, int8_t col ) const;
   Generator<board> generateBlackKingMoves( int8_t row, int8_t col ) const;
};

// Pieces are represented as constant int8s with unique identifiers
constexpr int8_t PAWN = 1;
constexpr int8_t KNIGHT = 2;
constexpr int8_t BISHOP = 3;
constexpr int8_t ROOK = 5;
constexpr int8_t QUEEN = 8;
constexpr int8_t KING = 9;


}
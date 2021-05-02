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
#include <coroutine>

namespace chessbot
{
  
constexpr unsigned char whiteToMove = 0x01;
constexpr unsigned char whiteCanNotCastleKingside = 0x02;
constexpr unsigned char whiteCanNotCastleQueenside = 0x04;
constexpr unsigned char blackCanNotCastleKingside = 0x08;
constexpr unsigned char blackCanNotCastleQueenside = 0x10;

class board
{
public:
   [[nodiscard]] std::vector<board> getLegalMoves();
   board() = default;
   [[nodiscard]] static board startingboard();
   ~board() = default;
   inline void placePiece( int8_t id, int8_t row, int8_t col );
   inline void removePiece( int8_t row, int8_t col );
   int8_t getSpace( int8_t row, int8_t col ) const;
   bool isWhiteInCheck() const;
   bool isBlackInCheck() const;
   unsigned char gameCtrlFlags = whiteToMove;
   double heuristicVal = 0.;

protected:
   std::pair<int8_t, int8_t> enpassant = { 0, 0 };
   std::array<int8_t, 64> spaces{ 0 };
   // helper functions to make getLegalMoves readable
   // boardsAfterMove is an outparam that each valid move gets appended to
   static void findWhitePawnMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findBlackPawnMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findWhiteKnightMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findBlackKnightMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findWhiteBishopMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findBlackBishopMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findWhiteRookMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findBlackRookMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findWhiteQueenMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findBlackQueenMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findWhiteKingMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
   static void findBlackKingMoves( std::vector<board>& boardsAfterMove, board b, int8_t row, int8_t col );
};

// Pieces are represented as constant int8s with unique identifiers
constexpr int8_t pawn = 1;
constexpr int8_t knight = 2;
constexpr int8_t bishop = 3;
constexpr int8_t rook = 5;
constexpr int8_t queen = 8;
constexpr int8_t king = 9;
constexpr int8_t OFFBOARD = -128;

}
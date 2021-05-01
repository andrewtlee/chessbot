// Andrew Lee
// 30 April 2021
// board.h
// defines 0x88 board representation
# pragma once
#include <array>
#include <stdint.h>
#include <vector>

namespace chessbot
{
  
constexpr unsigned char whiteToMove = 0x01;
constexpr unsigned char whiteCanNotCastle = 0x02;
constexpr unsigned char blackCanNotCastle = 0x04;

class board
{
public:
   [[nodiscard]] static std::vector<board> getLegalMoves( const board& b);
   board() = default;
   [[nodiscard]] static board startingboard();
   ~board() = default;
   inline void placePiece( int8_t id, int8_t row, int8_t col );
   inline void removePiece( int8_t row, int8_t col );
   int8_t getSpace( int8_t row, int8_t col ) const;
   bool isWhiteInCheck() const;
   bool isBlackInCheck() const;
protected:
   unsigned char gameCtrlFlags = whiteToMove;
   std::pair<int8_t, int8_t> enpassant = std::make_pair( 0, 0 );
   std::array<int8_t, 64> spaces{ 0 };
   // helper functions to make getLegalMoves readable
   // boardsAfterMove is an outparam that each valid move gets appended to
   static void findWhitePawnMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findBlackPawnMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findWhiteKnightMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findBlackKnightMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findWhiteBishopMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findBlackBishopMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findWhiteRookMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findBlackRookMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findWhiteQueenMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findBlackQueenMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findWhiteKingMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
   static void findBlackKingMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col );
};



}
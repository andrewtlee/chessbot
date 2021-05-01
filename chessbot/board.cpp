#include "board.h"
#include <iostream>

namespace chessbot
{

constexpr int8_t pawn = 1;
constexpr int8_t knight = 2;
constexpr int8_t bishop = 3;
constexpr int8_t rook = 5;
constexpr int8_t queen = 8;
constexpr int8_t king = 9;
constexpr int8_t OFFBOARD = -128;

std::vector<board> board::getLegalMoves()
{
   std::vector<board> boardsAfterMove;
   for( int8_t row = 0; row < 8; row++ )
   {
      for( int8_t col = 0; col < 8; col++ )
      {
         auto p = this->spaces.at( col + 8 * row );
         if( p > 0 && ( this->gameCtrlFlags & whiteToMove ) ) // white piece and white's turn 
         {
            
            switch( p )
            {
            case pawn:
               findWhitePawnMoves( boardsAfterMove, *this, row, col );
               break;
            case knight:
               findWhiteKnightMoves( boardsAfterMove, *this, row, col );
               break;
            case bishop:
               findWhiteBishopMoves( boardsAfterMove, *this, row, col );
               break;
            case rook:
               findWhiteRookMoves( boardsAfterMove, *this, row, col );
               break;
            case queen:
               findWhiteQueenMoves( boardsAfterMove, *this, row, col );
               break;
            case king:
               findWhiteKingMoves( boardsAfterMove, *this, row, col );
               break;
            default:
            {
               std::cout << "Unrecognized piece!\n";
            }
            }
         }
         else if( p < 0  && !(this->gameCtrlFlags & whiteToMove)) // black piece and black's turn
         {
            switch( -p ) // invert sign because I don't like negative cases
            {
            case pawn:
               findBlackPawnMoves( boardsAfterMove, *this, row, col );
               break;
            case knight:
               findBlackKnightMoves( boardsAfterMove, *this, row, col );
               break;
            case bishop:
               findBlackBishopMoves( boardsAfterMove, *this, row, col );
               break;
            case rook:
               findBlackRookMoves( boardsAfterMove, *this, row, col );
               break;
            case queen:
               findBlackQueenMoves( boardsAfterMove, *this, row, col );
               break;
            case king:
               findBlackKingMoves( boardsAfterMove, *this, row, col );
               break;
            default:
            {
               std::cout << "Unrecognized piece!\n";
            }
            }
         }
      }
   }
   return boardsAfterMove;
}

void board::placePiece( int8_t id, int8_t row, int8_t col )
{
   this->spaces.at( col + 8 * row ) = id;
}

void board::removePiece( int8_t row, int8_t col )
{
   this->spaces.at( col + 8 * row ) = 0;
}

int8_t board::getSpace( int8_t row, int8_t col ) const
{
   if( row >= 0 && row < 8 && col >= 0 && col < 8 )
      return this->spaces.at( col + 8 * row );
   else
      return OFFBOARD;
}

bool board::isWhiteInCheck() const
{
   int8_t row = 0, col = 0;
   for(auto i = 0; i < this->spaces.size(); i++ )
   {
      if( this->spaces.at( i ) == king )
      {
         col = i % 8;
         row = i / 8;
         break;
      }
   }

   if( this->getSpace( row + 1, col + 1 ) == -pawn || this->getSpace( row + 1, col - 1 ) == -pawn )
      return true; // black pawn checks the king

   for( auto r = row + 1; r < 8; r++ )
   {
      if( this->getSpace( r, col ) == -rook || this->getSpace( r, col ) == -queen )
         return true; // black queen or rook checks the king from above
      if( this->getSpace( r, col ) )
         break; // row blocked
   }
   for( auto r = row - 1; r >= 0; r-- )
   {
      if( this->getSpace( r, col ) == -rook || this->getSpace( r, col ) == -queen )
         return true; // black queen or rook checks the king from below
      if( this->getSpace( r, col ) )
         break; // row blocked
   }
   for( auto c = col + 1; c < 8; c++ )
   {
      if( this->getSpace( row, c ) == -rook || this->getSpace( row, c ) == -queen )
         return true; // a black rook or queen check the king from the right
      if( this->getSpace( row, c ) )
         break;
   }
   for( auto c = col - 1; c >= 0; c-- )
   {
      if( this->getSpace( row, c ) == -rook || this->getSpace( row, c ) == -queen )
         return true; // a black rook or queen checks the king from the left
      if( this->getSpace( row, c ) )
         break;
   }

   for(auto r=row+1, c=col+1;;) // this is disgusting but efficient; 
   {
      if( this->getSpace( r, c ) == -bishop || this->getSpace( r, c ) == -queen )
      {
         return true; // black queen or bishop checks the king from upper-right diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r++; c++;
   }
   for( auto r = row + 1, c = col - 1;; )
   {
      if( this->getSpace( r, c ) == -bishop || this->getSpace( r, c ) == -queen )
      {
         return true; // black queen or bishop checks the king from upper-left diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r++; c--;
   }
   for( auto r = row - 1, c = col - 1;; )
   {
      if( this->getSpace( r, c ) == -bishop || this->getSpace( r, c ) == -queen )
      {
         return true; // black queen or bishop checks the king from lower-left diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r--; c--;
   }
   for( auto r = row - 1, c = col + 1;; )
   {
      if( this->getSpace( r, c ) == -bishop || this->getSpace( r, c ) == -queen )
      {
         return true; // black queen or bishop checks the king from lower-right diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r--; c++;
   }
   // check for knights
   if( this->getSpace( row + 1, col + 2 ) == -knight || this->getSpace( row + 2, col + 1 ) == -knight
      || this->getSpace( row + 2, col - 1 ) == -knight || this->getSpace( row + 1, col - 2 ) == -knight
      || this->getSpace( row - 1, col - 2 ) == -knight || this->getSpace( row - 2, col - 1 ) == -knight
      || this->getSpace( row - 1, col + 2 ) == -knight || this->getSpace( row - 2, col + 1 ) == -knight )
      return true; // a black knight checks the king from... somewhere
   if( this->getSpace( row + 1, col + 1 ) == -king || this->getSpace( row + 1, col ) == -king
      || this->getSpace( row + 1, col - 1 ) == -king || this->getSpace( row, col - 1 ) == -king
      || this->getSpace( row, col + 1 ) == -king || this->getSpace( row - 1, col - 1 ) == -king
      || this->getSpace( row - 1, col ) == -king || this->getSpace( row - 1, col + 1 ) == -king )
      return true; // the black king checks the white king (absurd, but we need to check for move generation process)

   return false;
}

bool board::isBlackInCheck() const
{
   int8_t row = 0, col = 0;
   for( auto i = 0; i < this->spaces.size(); i++ )
   {
      if( this->spaces.at( i ) == -king )
      {
         col = i % 8;
         row = i / 8;
         break;
      }
   }

   if( this->getSpace( row - 1, col + 1 ) == pawn || this->getSpace( row - 1, col - 1 ) == pawn )
      return true; // white pawn checks the king

   for( auto r = row + 1; r < 8; r++ )
   {
      if( this->getSpace( r, col ) == rook || this->getSpace( r, col ) == queen )
         return true; // white queen or rook checks the king from above
      if( this->getSpace( r, col ) )
         break; // row blocked
   }
   for( auto r = row - 1; r >= 0; r-- )
   {
      if( this->getSpace( r, col ) == rook || this->getSpace( r, col ) == queen )
         return true; // white queen or rook checks the king from below
      if( this->getSpace( r, col ) )
         break; // row blocked
   }
   for( auto c = col + 1; c < 8; c++ )
   {
      if( this->getSpace( row, c ) == rook || this->getSpace( row, c ) == queen )
         return true; // a white rook or queen check the king from the right
      if( this->getSpace( row, c ) )
         break;
   }
   for( auto c = col - 1; c >= 0; c-- )
   {
      if( this->getSpace( row, c ) == rook || this->getSpace( row, c ) == queen )
         return true; // a white rook or queen checks the king from the left
      if( this->getSpace( row, c ) )
         break;
   }

   for( auto r = row + 1, c = col + 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( this->getSpace( r, c ) == bishop || this->getSpace( r, c ) == queen )
      {
         return true; // white queen or bishop checks the king from upper-right diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r++; c++;
   }
   for( auto r = row + 1, c = col - 1;; )
   {
      if( this->getSpace( r, c ) == bishop || this->getSpace( r, c ) == queen )
      {
         return true; // white queen or bishop checks the king from upper-left diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r++; c--;
   }
   for( auto r = row - 1, c = col - 1;; )
   {
      if( this->getSpace( r, c ) == bishop || this->getSpace( r, c ) == queen )
      {
         return true; // white queen or bishop checks the king from lower-left diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r--; c--;
   }
   for( auto r = row - 1, c = col + 1;; )
   {
      if( this->getSpace( r, c ) == bishop || this->getSpace( r, c ) == queen )
      {
         return true; // white queen or bishop checks the king from lower-right diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r--; c++;
   }
   // check for knights
   if( this->getSpace( row + 1, col + 2 ) == knight || this->getSpace( row + 2, col + 1 ) == knight
      || this->getSpace( row + 2, col - 1 ) == knight || this->getSpace( row + 1, col - 2 ) == knight
      || this->getSpace( row - 1, col - 2 ) == knight || this->getSpace( row - 2, col - 1 ) == knight
      || this->getSpace( row - 1, col + 2 ) == knight || this->getSpace( row - 2, col + 1 ) == knight )
      return true; // a white knight checks the king from... somewhere
   if( this->getSpace( row + 1, col + 1 ) == king || this->getSpace( row + 1, col ) == king
      || this->getSpace( row + 1, col - 1 ) == king || this->getSpace( row, col - 1 ) == king
      || this->getSpace( row, col + 1 ) == king || this->getSpace( row - 1, col - 1 ) == king
      || this->getSpace( row - 1, col ) == king || this->getSpace( row - 1, col + 1 ) == king )
      return true; // the white king checks the black king (absurd, but we need to check for move generation process)

   return false;
}

void board::findWhitePawnMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   auto oneSpaceFwd = b.getSpace( row + 1, col );
   if( !oneSpaceFwd && row != 7 )
   {
      temp.placePiece( pawn, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
      if( row == 1 )
      {
         auto twoSpacesFwd = b.getSpace( row + 2, col );
         if( !twoSpacesFwd )
         {
            temp.placePiece( pawn, row + 2, col );
            temp.removePiece( row, col );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.enpassant = std::make_pair( row + 1, col );
            if( !temp.isWhiteInCheck() )
               boardsAfterMove.push_back( temp );
            temp = b;
         }
      }
   }
   else if( !oneSpaceFwd )
   {
      temp.placePiece( queen, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
      temp.placePiece( knight, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
   auto leftDiag = b.getSpace( row + 1, col - 1 );
   if( leftDiag && leftDiag != OFFBOARD )
   {
      temp.placePiece( pawn, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
   else if( b.enpassant.first == row + 1 && b.enpassant.second == col - 1 )
   {
      temp.placePiece( pawn, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.removePiece( row, col-1 );
      temp.enpassant = std::make_pair( 0, 0 );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
   auto rightDiag = b.getSpace( row + 1, col + 1 );
   if( rightDiag && rightDiag != OFFBOARD )
   {
      temp.placePiece( pawn, row + 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if (!temp.isWhiteInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
   else if( b.enpassant.first == row + 1 && b.enpassant.second == col + 1 )
   {
      temp.placePiece( pawn, row + 1, col + 1 );
      temp.removePiece( row, col );
      temp.removePiece( row, col + 1 );
      temp.enpassant = std::make_pair( 0, 0 );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
}

void board::findBlackPawnMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   auto oneSpaceFwd = b.getSpace( row - 1, col );
   if( !oneSpaceFwd && row != 1 )
   {
      temp.placePiece( -pawn, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
      if( row == 6 )
      {
         auto twoSpacesFwd = b.getSpace( row - 2, col );
         if( !twoSpacesFwd )
         {
            temp.placePiece( -pawn, row - 2, col );
            temp.removePiece( row, col );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.enpassant = std::make_pair( row - 1, col );
            if( !temp.isBlackInCheck() )
               boardsAfterMove.push_back( temp );
            temp = b;
         }
      }
   }
   else if( !oneSpaceFwd )
   {
      temp.placePiece( -queen, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
      temp.placePiece( -knight, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
   auto leftDiag = b.getSpace( row - 1, col - 1 );
   if( leftDiag && leftDiag != OFFBOARD )
   {
      temp.placePiece( -pawn, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
   else if( b.enpassant.first == row - 1 && b.enpassant.second == col - 1 )
   {
      temp.placePiece( -pawn, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.removePiece( row, col - 1 );
      temp.enpassant = std::make_pair( 0, 0 );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
   auto rightDiag = b.getSpace( row - 1, col + 1 );
   if( rightDiag && rightDiag != OFFBOARD )
   {
      temp.placePiece( -pawn, row - 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
   else if( b.enpassant.first == row - 1 && b.enpassant.second == col + 1 )
   {
      temp.placePiece( -pawn, row - 1, col + 1 );
      temp.removePiece( row, col );
      temp.removePiece( row, col + 1 );
      temp.enpassant = std::make_pair( 0, 0 );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         boardsAfterMove.push_back( temp );
      temp = b;
   }
}

void board::findWhiteKnightMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   if( b.getSpace( row + 1, col + 2 ) <= 0 && b.getSpace( row + 1, col + 2 ) != OFFBOARD )
   {
      temp.placePiece( knight, row + 1, col + 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 2, col + 1 ) <= 0 && b.getSpace( row + 2, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( knight, row + 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 2, col - 1 ) <= 0 && b.getSpace( row + 2, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( knight, row + 2, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 1, col - 2 ) <= 0 && b.getSpace( row + 1, col - 2 ) != OFFBOARD )
   {
      temp.placePiece( knight, row + 1, col - 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col - 2 ) <= 0 && b.getSpace( row - 1, col - 2 ) != OFFBOARD )
   {
      temp.placePiece( knight, row - 1, col - 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 2, col - 1 ) <= 0 && b.getSpace( row - 2, col - 1) != OFFBOARD )
   {
      temp.placePiece( knight, row - 2, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col + 2 ) <= 0 && b.getSpace( row - 1, col + 2 ) != OFFBOARD )
   {
      temp.placePiece( knight, row - 1, col + 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 2, col + 1 ) <= 0 && b.getSpace( row - 2, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( knight, row - 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }

}

void board::findBlackKnightMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   if( b.getSpace( row + 1, col + 2 ) >= 0 )
   {
      temp.placePiece( -knight, row + 1, col + 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 2, col + 1 ) >= 0 )
   {
      temp.placePiece( -knight, row + 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 2, col - 1 ) >= 0 )
   {
      temp.placePiece( -knight, row + 2, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 1, col - 2 ) >= 0 )
   {
      temp.placePiece( -knight, row + 1, col - 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col - 2 ) >= 0 )
   {
      temp.placePiece( -knight, row - 1, col - 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 2, col - 1 ) >= 0 )
   {
      temp.placePiece( -knight, row - 2, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col + 2 ) >= 0 )
   {
      temp.placePiece( -knight, row - 1, col + 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 2, col + 1 ) >= 0 )
   {
      temp.placePiece( -knight, row - 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
}

void board::findWhiteBishopMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   for( auto r = row + 1, c = col + 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
      r++; c++;
   }
   for( auto r = row + 1, c = col - 1;;) 
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
      r++; c--;
   }
   for( auto r = row - 1, c = col - 1;;) 
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
      r--; c--;
   }
   for( auto r = row - 1, c = col + 1;;) 
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
      r--; c++;
   }
}

void board::findBlackBishopMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   for( auto r = row + 1, c = col + 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) > 0)
      {
         temp.placePiece( -bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
      r++; c++;
   }
   for( auto r = row + 1, c = col - 1;;) 
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) > 0)
      {
         temp.placePiece( -bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) < 0)
      {
         break;
      }
      r++; c--;
   }
   for( auto r = row - 1, c = col - 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
      r--; c--;
   }
   for( auto r = row - 1, c = col + 1;;)
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -bishop, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
      r--; c++;
   }
}

void board::findWhiteRookMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastle;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, col ) < 0)
      {
         temp.placePiece( rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastle;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         break;
      }
   }
   for( auto r = row - 1; r >= 0; r-- )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastle;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         temp.placePiece( rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastle;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         break;
      }
   }
   for( auto c = col + 1; c < 8; c++ )
   {
      if( temp.getSpace( row, c ) == 0 )
      {
         temp.placePiece( rook, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastle;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         temp.placePiece( rook, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastle;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         break;
      }
   }
   for( auto c = col - 1; c >= 0; c-- )
   {
      if( temp.getSpace( row, c ) == 0 )
      {
         temp.placePiece( rook, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastle;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         temp.placePiece( rook, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastle;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         break;
      }
   }
}

void board::findBlackRookMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( -rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastle;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastle;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         break;
      }
   }
   for( auto r = row - 1; r >= 0; r-- )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( -rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastle;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastle;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         break;
      }
   }
   for( auto c = col + 1; c < 8; c++ )
   {
      if( temp.getSpace( row, c ) == 0 )
      {
         temp.placePiece( -rook, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastle;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         temp.placePiece( -rook, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastle;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         break;
      }
   }
   for( auto c = col - 1; c >= 0; c-- )
   {
      if( temp.getSpace( row, c ) == 0 )
      {
         temp.placePiece( -rook, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastle;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         temp.placePiece( -rook, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastle;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         break;
      }
   }
}

void board::findWhiteQueenMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   for( auto r = row + 1, c = col + 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
      r++; c++;
   }
   for( auto r = row + 1, c = col - 1;;)
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
      r++; c--;
   }
   for( auto r = row - 1, c = col - 1;;)
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
      r--; c--;
   }
   for( auto r = row - 1, c = col + 1;;)
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
      r--; c++;
   }
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( queen, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         temp.placePiece( queen, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         break;
      }
   }
   for( auto r = row - 1; r >= 0; r-- )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( queen, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         temp.placePiece( queen, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         break;
      }
   }
   for( auto c = col + 1; c < 8; c++ )
   {
      if( temp.getSpace( row, c ) == 0 )
      {
         temp.placePiece( queen, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         temp.placePiece( queen, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         break;
      }
   }
   for( auto c = col - 1; c >= 0; c-- )
   {
      if( temp.getSpace( row, c ) == 0 )
      {
         temp.placePiece( queen, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         temp.placePiece( queen, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         break;
      }
   }
}

void board::findBlackQueenMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   for( auto r = row + 1, c = col + 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
      r++; c++;
   }
   for( auto r = row + 1, c = col - 1;;)
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
      r++; c--;
   }
   for( auto r = row - 1, c = col - 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
      r--; c--;
   }
   for( auto r = row - 1, c = col + 1;;)
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -queen, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
      r--; c++;
   }
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( -queen, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -queen, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         break;
      }
   }
   for( auto r = row - 1; r >= 0; r-- )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( -queen, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -queen, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         break;
      }
   }
   for( auto c = col + 1; c < 8; c++ )
   {
      if( temp.getSpace( row, c ) == 0 )
      {
         temp.placePiece( -queen, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         temp.placePiece( -queen, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         break;
      }
   }
   for( auto c = col - 1; c >= 0; c-- )
   {
      if( temp.getSpace( row, c ) == 0 )
      {
         temp.placePiece( -queen, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         temp.placePiece( -queen, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            boardsAfterMove.push_back( temp );
         }
         temp = b;
         break;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         break;
      }
   }
}

void board::findWhiteKingMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   if( b.getSpace( row + 1, col + 1 ) <= 0 && b.getSpace( row + 1, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( king, row + 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastle;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 1, col ) <= 0 && b.getSpace( row + 1, col ) != OFFBOARD )
   {
      temp.placePiece( king, row, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastle;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 1, col - 1 ) <= 0 && b.getSpace( row + 1, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( king, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastle;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row, col - 1 ) <= 0 && b.getSpace( row, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( king, row, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastle;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row, col + 1 ) <= 0 && b.getSpace( row, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( king, row, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastle;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col - 1 ) <= 0 && b.getSpace( row - 1, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( king, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastle;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col ) <= 0 && b.getSpace( row - 1, col ) != OFFBOARD )
   {
      temp.placePiece( king, row - 1, col);
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastle;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col + 1 ) <= 0 && b.getSpace( row - 1, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( king, row - 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastle;
      if( !temp.isWhiteInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   // TODO: Let you move one rook and still castle
   if( !(b.gameCtrlFlags & whiteCanNotCastle) && !b.getSpace(0, 5) && !b.getSpace( 0, 6 ) && (b.getSpace(0, 7) == rook) ) // kingside castle
   {
      temp.placePiece( king, row, col + 1 ); // king can't move through check to castle
      temp.removePiece( row, col );
      if( !temp.isWhiteInCheck() )
      {
         temp.placePiece( king, row, col + 2 );
         temp.placePiece( rook, row, col + 1 ); // rook overwrites phantom king
         if( !temp.isWhiteInCheck() )
         {
            temp.removePiece( 0, 7 );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.gameCtrlFlags |= whiteCanNotCastle;
            boardsAfterMove.push_back( temp );
         }
      }
      temp = b;
   }
   if( !(b.gameCtrlFlags & whiteCanNotCastle) && !b.getSpace( 0, 1 ) && !b.getSpace( 0, 2 ) && !b.getSpace( 0, 3 ) && (b.getSpace(0,0) == rook))
   {
      temp.placePiece( king, row, col - 1 );
      temp.removePiece( row, col );
      if( !temp.isWhiteInCheck() )
      {
         temp.placePiece( king, row, col - 2 );
         temp.placePiece( rook, 0, 3 );
         if( !temp.isWhiteInCheck() )
         {
            temp.removePiece( 0, 0 );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.gameCtrlFlags |= whiteCanNotCastle;
            boardsAfterMove.push_back( temp );
         }
      }
      temp = b;
   }
}

void board::findBlackKingMoves( std::vector<board>& boardsAfterMove, const board& b, int8_t row, int8_t col )
{
   board temp = b;
   if( b.getSpace( row + 1, col + 1 ) >= 0 )
   {
      temp.placePiece( -king, row + 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 1, col ) >= 0 )
   {
      temp.placePiece( -king, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row + 1, col - 1 ) >= 0 )
   {
      temp.placePiece( -king, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row, col - 1 ) >= 0 )
   {
      temp.placePiece( -king, row, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row, col + 1 ) >= 0 )
   {
      temp.placePiece( -king, row, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col - 1 ) >= 0 )
   {
      temp.placePiece( -king, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col ) >= 0 )
   {
      temp.placePiece( -king, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( b.getSpace( row - 1, col + 1 ) >= 0 )
   {
      temp.placePiece( -king, row - 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         boardsAfterMove.push_back( temp );
      }
      temp = b;
   }
   if( !(b.gameCtrlFlags & blackCanNotCastle) && b.getSpace( 7, 5 ) && b.getSpace( 7, 6 ) && (b.getSpace( 7, 7 ) == rook) ) // kingside castle
   {
      temp.placePiece( -king, row, col + 1 ); // king can't move through check to castle
      temp.removePiece( row, col );
      if( !temp.isBlackInCheck() )
      {
         temp.placePiece( -king, row, col + 2 );
         temp.placePiece( -rook, row, col + 1 ); // rook overwrites phantom king
         if( !temp.isBlackInCheck() )
         {
            temp.removePiece( 7, 7 );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.gameCtrlFlags |= blackCanNotCastle;
            boardsAfterMove.push_back( temp );
         }
      }
      temp = b;
   }
   if( !(b.gameCtrlFlags & blackCanNotCastle) && b.getSpace( 7, 1 ) && b.getSpace( 7, 2 ) && b.getSpace( 7, 3 ) && (b.getSpace( 7, 0 ) == rook) )
   {
      temp.placePiece( -king, row, col - 1 );
      temp.removePiece( row, col );
      if( !temp.isBlackInCheck() )
      {
         temp.placePiece( -king, row, col - 2 );
         temp.placePiece( rook, 7, 3 );
         if( !temp.isBlackInCheck() )
         {
            temp.removePiece( 7, 0 );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.gameCtrlFlags |= blackCanNotCastle;
            boardsAfterMove.push_back( temp );
         }
      }
      temp = b;
   }
}

board board::startingboard()
{     
   board b;
   b.placePiece( rook, 0, 0 );
   b.placePiece( knight, 0, 1 );
   b.placePiece( bishop, 0, 2 );
   b.placePiece( queen, 0, 3 );
   b.placePiece( king, 0, 4 );
   b.placePiece( bishop, 0, 5 );
   b.placePiece( knight, 0, 6 );
   b.placePiece( rook, 0, 7 );
   for( uint8_t col = 0; col < 8; col++ )
   {
      b.placePiece( pawn, 1, col );
      b.placePiece( -pawn, 6, col );
   }
   b.placePiece( -rook, 7, 0 );
   b.placePiece( -knight, 7, 1 );
   b.placePiece( -bishop, 7, 2 );
   b.placePiece( -queen, 7, 3 );
   b.placePiece( -king, 7, 4 );
   b.placePiece( -bishop, 7, 5 );
   b.placePiece( -knight, 7, 6 );
   b.placePiece( -rook, 7, 7 );
   return b;
}

} // namespace chessbot
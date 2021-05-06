// Andrew Lee
// 1 May 2021
// board.cpp
/*
* Implements the board.h class. This looks horrendously long but a lot of it is copy-paste or brute-force move availability checKING.
* The logic in getMove is fairly simple, and the helper functions hide the pain.
*/
#include "board.h"

namespace chessbot
{

bool board::operator==( board b )
{
   return b.gameCtrlFlags == this->gameCtrlFlags && b.spaces == this->spaces;
}

bool board::operator==( board&& b )
{
   return b.gameCtrlFlags == this->gameCtrlFlags && b.spaces == this->spaces;
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
      if( this->spaces.at( i ) == KING )
      {
         col = i % 8;
         row = i / 8;
         break;
      }
   }

   if( this->getSpace( row + 1, col + 1 ) == -PAWN || this->getSpace( row + 1, col - 1 ) == -PAWN )
      return true; // black PAWN checks the KING

   for( auto r = row + 1; r < 8; r++ )
   {
      if( this->getSpace( r, col ) == -ROOK || this->getSpace( r, col ) == -QUEEN )
         return true; // black QUEEN or ROOK checks the KING from above
      if( this->getSpace( r, col ) )
         break; // row blocked
   }
   for( auto r = row - 1; r >= 0; r-- )
   {
      if( this->getSpace( r, col ) == -ROOK || this->getSpace( r, col ) == -QUEEN )
         return true; // black QUEEN or ROOK checks the KING from below
      if( this->getSpace( r, col ) )
         break; // row blocked
   }
   for( auto c = col + 1; c < 8; c++ )
   {
      if( this->getSpace( row, c ) == -ROOK || this->getSpace( row, c ) == -QUEEN )
         return true; // a black ROOK or QUEEN check the KING from the right
      if( this->getSpace( row, c ) )
         break;
   }
   for( auto c = col - 1; c >= 0; c-- )
   {
      if( this->getSpace( row, c ) == -ROOK || this->getSpace( row, c ) == -QUEEN )
         return true; // a black ROOK or QUEEN checks the KING from the left
      if( this->getSpace( row, c ) )
         break;
   }

   for(auto r=row+1, c=col+1;;) // this is disgusting but efficient; 
   {
      if( this->getSpace( r, c ) == -BISHOP || this->getSpace( r, c ) == -QUEEN )
      {
         return true; // black QUEEN or BISHOP checks the KING from upper-right diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r++; c++;
   }
   for( auto r = row + 1, c = col - 1;; )
   {
      if( this->getSpace( r, c ) == -BISHOP || this->getSpace( r, c ) == -QUEEN )
      {
         return true; // black QUEEN or BISHOP checks the KING from upper-left diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r++; c--;
   }
   for( auto r = row - 1, c = col - 1;; )
   {
      if( this->getSpace( r, c ) == -BISHOP || this->getSpace( r, c ) == -QUEEN )
      {
         return true; // black QUEEN or BISHOP checks the KING from lower-left diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r--; c--;
   }
   for( auto r = row - 1, c = col + 1;; )
   {
      if( this->getSpace( r, c ) == -BISHOP || this->getSpace( r, c ) == -QUEEN )
      {
         return true; // black QUEEN or BISHOP checks the KING from lower-right diagonal
      }
      if( this->getSpace( r, c ) )
         break;
      r--; c++;
   }
   // check for KNIGHTs
   if( this->getSpace( row + 1, col + 2 ) == -KNIGHT || this->getSpace( row + 2, col + 1 ) == -KNIGHT
      || this->getSpace( row + 2, col - 1 ) == -KNIGHT || this->getSpace( row + 1, col - 2 ) == -KNIGHT
      || this->getSpace( row - 1, col - 2 ) == -KNIGHT || this->getSpace( row - 2, col - 1 ) == -KNIGHT
      || this->getSpace( row - 1, col + 2 ) == -KNIGHT || this->getSpace( row - 2, col + 1 ) == -KNIGHT )
      return true; // a black KNIGHT checks the KING from... somewhere
   if( this->getSpace( row + 1, col + 1 ) == -KING || this->getSpace( row + 1, col ) == -KING
      || this->getSpace( row + 1, col - 1 ) == -KING || this->getSpace( row, col - 1 ) == -KING
      || this->getSpace( row, col + 1 ) == -KING || this->getSpace( row - 1, col - 1 ) == -KING
      || this->getSpace( row - 1, col ) == -KING || this->getSpace( row - 1, col + 1 ) == -KING )
      return true; // the black KING checks the white KING (absurd, but we need to check for move generation process)

   return false;
}

bool board::isBlackInCheck() const
{
   int8_t row = 0, col = 0;
   for( auto i = 0; i < this->spaces.size(); i++ )
   {
      if( this->spaces.at( i ) == -KING )
      {
         col = i % 8;
         row = i / 8;
         break;
      }
   }

   if( this->getSpace( row - 1, col + 1 ) == PAWN || this->getSpace( row - 1, col - 1 ) == PAWN )
      return true; // white PAWN checks the KING

   for( auto r = row + 1; r < 8; r++ )
   {
      if( this->getSpace( r, col ) == ROOK || this->getSpace( r, col ) == QUEEN )
         return true; // white QUEEN or ROOK checks the KING from above
      if( this->getSpace( r, col ) )
         break; // row blocked
   }
   for( auto r = row - 1; r >= 0; r-- )
   {
      if( this->getSpace( r, col ) == ROOK || this->getSpace( r, col ) == QUEEN )
         return true; // white QUEEN or ROOK checks the KING from below
      if( this->getSpace( r, col ) )
         break; // row blocked
   }
   for( auto c = col + 1; c < 8; c++ )
   {
      if( this->getSpace( row, c ) == ROOK || this->getSpace( row, c ) == QUEEN )
         return true; // a white ROOK or QUEEN check the KING from the right
      if( this->getSpace( row, c ) )
         break;
   }
   for( auto c = col - 1; c >= 0; c-- )
   {
      if( this->getSpace( row, c ) == ROOK || this->getSpace( row, c ) == QUEEN )
         return true; // a white ROOK or QUEEN checks the KING from the left
      if( this->getSpace( row, c ) )
         break;
   }

   for( auto r = row + 1, c = col + 1;; r++, c++ ) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( this->getSpace( r, c ) == BISHOP || this->getSpace( r, c ) == QUEEN )
      {
         return true; // white queen or bishop checks the king from upper-right diagonal
      }
      if( this->getSpace( r, c ) )
         break;
   }
   for( auto r = row + 1, c = col - 1;; r++, c-- )
   {
      if( this->getSpace( r, c ) == BISHOP || this->getSpace( r, c ) == QUEEN )
      {
         return true; // white queen or bishop checks the king from upper-left diagonal
      }
      if( this->getSpace( r, c ) )
         break;
   }
   for( auto r = row - 1, c = col - 1;; r--, c-- )
   {
      if( this->getSpace( r, c ) == BISHOP || this->getSpace( r, c ) == QUEEN )
      {
         return true; // white queen or bishop checks the king from lower-left diagonal
      }
      if( this->getSpace( r, c ) )
         break;
     
   }
   for( auto r = row - 1, c = col + 1;; r--, c++ )
   {
      if( this->getSpace( r, c ) == BISHOP || this->getSpace( r, c ) == QUEEN )
      {
         return true; // white queen or bishop checks the KING from lower-right diagonal
      }
      if( this->getSpace( r, c ) )
         break;
   }
   // check for KNIGHTs
   if( this->getSpace( row + 1, col + 2 ) == KNIGHT || this->getSpace( row + 2, col + 1 ) == KNIGHT
      || this->getSpace( row + 2, col - 1 ) == KNIGHT || this->getSpace( row + 1, col - 2 ) == KNIGHT
      || this->getSpace( row - 1, col - 2 ) == KNIGHT || this->getSpace( row - 2, col - 1 ) == KNIGHT
      || this->getSpace( row - 1, col + 2 ) == KNIGHT || this->getSpace( row - 2, col + 1 ) == KNIGHT )
      return true; // a white knight checks the king from... somewhere
   if( this->getSpace( row + 1, col + 1 ) == KING || this->getSpace( row + 1, col ) == KING
      || this->getSpace( row + 1, col - 1 ) == KING || this->getSpace( row, col - 1 ) == KING
      || this->getSpace( row, col + 1 ) == KING || this->getSpace( row - 1, col - 1 ) == KING
      || this->getSpace( row - 1, col ) == KING || this->getSpace( row - 1, col + 1 ) == KING )
      return true; // the white king checks the black king (absurd, but we need to check for move generation process)

   return false;
}
 // This version of getMove() goes through the board once and finds the moves of the pieces it comes across, in order
/*Generator<board> board::getMove()
{
   for( int8_t row = 0; row < 8; row++ )
   {
      for( int8_t col = 0; col < 8; col++ )
      {
         auto p = this->spaces.at( col + 8 * row );
         if( p > 0 && (this->gameCtrlFlags & whiteToMove) ) // white piece and white's turn 
         {
            switch( p )
            {
            case PAWN:
            {
               auto genMove = this->generateWhitePawnMoves( row, col ); // generic ordering
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case KNIGHT:
            {
               auto genMove = this->generateWhiteKnightMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case BISHOP:
            {
               auto genMove = this->generateWhiteBishopMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case ROOK:
            {
               auto genMove = this->generateWhiteRookMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case QUEEN:
            {
               auto genMove = this->generateWhiteQueenMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case KING:
            {
               auto genMove = this->generateWhiteKingMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            default:
            {
               throw "Unkown piece on board!";
            }
            }
         }
         else if( p < 0 && !(this->gameCtrlFlags & whiteToMove) )
         {
            switch( -p )
            {
            case PAWN:
            {
               auto genMove = this->generateBlackPawnMoves( row, col ); // generic ordering
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case KNIGHT:
            {
               auto genMove = this->generateBlackKnightMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case BISHOP:
            {
               auto genMove = this->generateBlackBishopMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case ROOK:
            {
               auto genMove = this->generateBlackRookMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case QUEEN:
            {
               auto genMove = this->generateBlackQueenMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case KING:
            {
               auto genMove = this->generateBlackKingMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            default:
            {
               throw "Unknown piece on board!";
            }
            }
         }
      }
   }
}*/

std::vector<board> board::getLegalMoves()
{
   std::vector<board> legalMoves;
   auto gen = this->getMove();
   while( gen )
   {
      legalMoves.push_back( gen() );
   }
   return legalMoves;
}

// Thes version of getMove tries to be clever and generate likely good moves first
Generator<board> board::getMove()
{
   if( this->gameCtrlFlags & whiteToMove )
   {
      for( int8_t row = 0; row < 8; row++ )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == KNIGHT )
            {
               auto gen = this->generateWhiteKnightMoves(row, col);
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 0; row < 8; row++ )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == BISHOP )
            {
               auto gen = this->generateWhiteBishopMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 0; row < 8; row++ )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == PAWN )
            {
               auto gen = this->generateWhitePawnMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 0; row < 8; row++ )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == ROOK )
            {
               auto gen = this->generateWhiteRookMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 0; row < 8; row++ )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == QUEEN )
            {
               auto gen = this->generateWhiteQueenMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 0; row < 8; row++ )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == KING )
            {
               auto gen = this->generateWhiteKingMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
   }
   else
   {
      for( int8_t row = 7; row >= 0; row-- )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == -KNIGHT )
            {
               auto gen = this->generateBlackKnightMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 7; row >= 0; row-- )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == -BISHOP )
            {
               auto gen = this->generateBlackBishopMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 7; row >= 0; row-- )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == -PAWN )
            {
               auto gen = this->generateBlackPawnMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 7; row >= 0; row-- )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == -ROOK )
            {
               auto gen = this->generateBlackRookMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 7; row >= 0; row-- )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == -QUEEN )
            {
               auto gen = this->generateBlackQueenMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
      for( int8_t row = 7; row >= 0; row-- )
      {
         for( int8_t col = 0; col < 8; col++ )
         {
            if( this->getSpace( row, col ) == -KING )
            {
               auto gen = this->generateBlackKingMoves( row, col );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
   }
}

Generator<board> board::generateWhitePawnMoves( int8_t row, int8_t col ) const
{
   board temp = *this;
   auto oneSpaceFwd = this->getSpace( row + 1, col );
   if( !oneSpaceFwd && row != 7 )
   {
      temp.placePiece( PAWN, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      if( row == 1 )
      {
         auto twoSpacesFwd = this->getSpace( row + 2, col );
         if( !twoSpacesFwd )
         {
            temp.placePiece( PAWN, row + 2, col );
            temp.removePiece( row, col );
            temp.gameCtrlFlags ^= whiteToMove;
            //temp.enpassant = { row + 1, col };
            if( !temp.isWhiteInCheck() )
               co_yield temp;
            temp = *this;
         }
      }
   }
   else if( !oneSpaceFwd )
   {
      temp.placePiece( QUEEN, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( KNIGHT, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   auto leftDiag = this->getSpace( row + 1, col - 1 );
   if( leftDiag < 0 && leftDiag != OFFBOARD && row != 6)
   {
      temp.placePiece( PAWN, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   else if( leftDiag < 0 && leftDiag != OFFBOARD)
   {
      temp.placePiece( KNIGHT, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( QUEEN, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   else if( this->enpassant.first == row + 1 && this->enpassant.second == col - 1 )
   {
      temp.placePiece( PAWN, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.removePiece( row, col - 1 );
      //temp.enpassant = { -1, -1 };
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   auto rightDiag = this->getSpace( row + 1, col + 1 );
   if( rightDiag < 0 && rightDiag != OFFBOARD && row != 6)
   {
      temp.placePiece( PAWN, row + 1, col + 1 );
      temp.removePiece( row, col );
      if( row == 7 )
      {
         temp.placePiece( PAWN, row + 1, col + 1 );
      }
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   else if( rightDiag < 0 && rightDiag != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( QUEEN, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   else if( this->enpassant.first == row + 1 && this->enpassant.second == col + 1 )
   {
      temp.placePiece( PAWN, row + 1, col + 1 );
      temp.removePiece( row, col );
      temp.removePiece( row, col + 1 );
      //temp.enpassant = { -1, -1 };
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
}

Generator<board> board::generateWhiteKnightMoves( int8_t row, int8_t col ) const
{
   board temp = *this;
   if(  this->getSpace( row + 1, col + 2 ) <= 0 &&  this->getSpace( row + 1, col + 2 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row + 1, col + 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( row + 2, col + 1 ) <= 0 &&  this->getSpace( row + 2, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row + 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( row + 2, col - 1 ) <= 0 &&  this->getSpace( row + 2, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row + 2, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( row + 1, col - 2 ) <= 0 &&  this->getSpace( row + 1, col - 2 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row + 1, col - 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( row - 1, col - 2 ) <= 0 &&  this->getSpace( row - 1, col - 2 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row - 1, col - 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( row - 2, col - 1 ) <= 0 &&  this->getSpace( row - 2, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row - 2, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( row - 1, col + 2 ) <= 0 &&  this->getSpace( row - 1, col + 2 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row - 1, col + 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( row - 2, col + 1 ) <= 0 &&  this->getSpace( row - 2, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, row - 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
}

Generator<board> board::generateWhiteBishopMoves( int8_t row, int8_t col ) const
{
   board temp = *this;
   for( auto r = row + 1, c = col + 1;; r++, c++) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
   }
   for( auto r = row + 1, c = col - 1;; r++, c--)
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
   }
   for( auto r = row - 1, c = col - 1;; r--, c--)
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
   }
   for( auto r = row - 1, c = col + 1;; r--, c++ )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
   }
}

Generator<board> board::generateWhiteRookMoves( int8_t row, int8_t col ) const
{
   board temp = *this;
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( ROOK, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         /* This bears some explaining: castling rules are more complex than most other chess rules
         If the ROOK is moving from his starting square, we want to forbid castling; all other moves have no effect.
         But we don't want extra branches that we can avoid, so instead we do bitwise arithmetic. This statement is equivalent to
         if (row==0 && col==7) temp.gameCtrlFlags |= whiteCanNotCastleKingside;
         */
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         temp.placePiece( ROOK, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
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
         temp.placePiece( ROOK, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         temp.placePiece( ROOK, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
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
         temp.placePiece( ROOK, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         temp.placePiece( ROOK, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
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
         temp.placePiece( ROOK, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         temp.placePiece( ROOK, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         break;
      }
   }
}
Generator<board> board::generateWhiteQueenMoves( int8_t row, int8_t col ) const
{
   board temp = *this;
   for( auto r = row + 1, c = col + 1;; r++, c++ )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
   }
   for( auto r = row + 1, c = col - 1;; r++, c-- )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
   }
   for( auto r = row - 1, c = col - 1;; r--, c-- )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
   }
   for( auto r = row - 1, c = col + 1;; r--, c++ )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( r, c ) > 0 || temp.getSpace( r, c ) == OFFBOARD )
      {
         break;
      }
   }
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( QUEEN, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         temp.placePiece( QUEEN, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
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
         temp.placePiece( QUEEN, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         temp.placePiece( QUEEN, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
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
         temp.placePiece( QUEEN, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         temp.placePiece( QUEEN, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
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
         temp.placePiece( QUEEN, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         temp.placePiece( QUEEN, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         break;
      }
   }
}

Generator<board> board::generateWhiteKingMoves( int8_t row, int8_t col ) const
{
   board temp = *this;
   if( this->getSpace( row + 1, col + 1 ) <= 0 && this->getSpace( row + 1, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, row + 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         co_yield  temp;
      }
      temp = *this;
   }
   if( this->getSpace( row + 1, col ) <= 0 && this->getSpace( row + 1, col ) != OFFBOARD )
   {
      temp.placePiece( KING, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( row + 1, col - 1 ) <= 0 && this->getSpace( row + 1, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( row, col - 1 ) <= 0 && this->getSpace( row, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, row, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( row, col + 1 ) <= 0 && this->getSpace( row, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, row, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( row - 1, col - 1 ) <= 0 && this->getSpace( row - 1, col - 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( row - 1, col ) <= 0 && this->getSpace( row - 1, col ) != OFFBOARD )
   {
      temp.placePiece( KING, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( row - 1, col + 1 ) <= 0 && this->getSpace( row - 1, col + 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, row - 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }
   if( !(this->gameCtrlFlags & whiteCanNotCastleKingside) && !this->getSpace( 0, 5 ) && !this->getSpace( 0, 6 ) && (this->getSpace( 0, 7 ) == ROOK) ) // KINGside castle
   {
      temp.placePiece( KING, row, col + 1 ); // KING can't move through check to castle
      temp.removePiece( row, col );
      if( !temp.isWhiteInCheck() )
      {
         temp.placePiece( KING, row, col + 2 );
         temp.placePiece( ROOK, row, col + 1 ); // ROOK overwrites phantom KING
         if( !temp.isWhiteInCheck() )
         {
            temp.removePiece( 0, 7 );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.gameCtrlFlags |= whiteCanNotCastleKingside;
            temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
            co_yield temp;
         }
      }
      temp = *this;
   }
   if( !(this->gameCtrlFlags & whiteCanNotCastleQueenside) && !this->getSpace( 0, 1 ) && !this->getSpace( 0, 2 ) && !this->getSpace( 0, 3 ) && (this->getSpace( 0, 0 ) == ROOK) )
   {
      temp.placePiece( KING, row, col - 1 );
      temp.removePiece( row, col );
      if( !temp.isWhiteInCheck() )
      {
         temp.placePiece( KING, row, col - 2 );
         temp.placePiece( ROOK, 0, 3 );
         if( !temp.isWhiteInCheck() )
         {
            temp.removePiece( 0, 0 );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.gameCtrlFlags |= whiteCanNotCastleKingside;
            temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
            co_yield temp;
         }
      }
      temp = *this;
   }
}

Generator<board> board::generateBlackPawnMoves( int8_t row, int8_t col ) const
{
   board temp =*this;
   auto oneSpaceFwd = this->getSpace( row - 1, col );
   if( !oneSpaceFwd && row != 1 )
   {
      temp.placePiece( -PAWN, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
      if( row == 6 )
      {
         auto twoSpacesFwd = this->getSpace( row - 2, col );
         if( !twoSpacesFwd )
         {
            temp.placePiece( -PAWN, row - 2, col );
            temp.removePiece( row, col );
            temp.gameCtrlFlags ^= whiteToMove;
            //temp.enpassant = { row - 1, col };
            if( !temp.isBlackInCheck() )
               co_yield temp;
            temp =*this;
         }
      }
   }
   else if( !oneSpaceFwd )
   {
      temp.placePiece( -QUEEN, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
      temp.placePiece( -KNIGHT, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
   }
   auto leftDiag = this->getSpace( row - 1, col - 1 );
   if( leftDiag > 0 && row != 1 )
   {
      temp.placePiece( -PAWN, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
   }
   else if (leftDiag > 0 )
   {
      temp.placePiece( -KNIGHT, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( -QUEEN, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   auto rightDiag = this->getSpace( row - 1, col + 1 );
   if( rightDiag > 0 && row != 1)
   {
      temp.placePiece( -PAWN, row - 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
   }
   else if( rightDiag > 0 )
   {
      temp.placePiece( -KNIGHT, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( -QUEEN, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
}

Generator<board> board::generateBlackKnightMoves( int8_t row, int8_t col ) const
{
   board temp =*this;
   if( this->getSpace( row + 1, col + 2 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, row + 1, col + 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row + 2, col + 1 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, row + 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row + 2, col - 1 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, row + 2, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row + 1, col - 2 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, row + 1, col - 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row - 1, col - 2 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, row - 1, col - 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row - 2, col - 1 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, row - 2, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row - 1, col + 2 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, row - 1, col + 2 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row - 2, col + 1 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, row - 2, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
}

Generator<board> board::generateBlackBishopMoves( int8_t row, int8_t col ) const
{
   board temp =*this;
   for( auto r = row + 1, c = col + 1;; r++, c++ ) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
      
   }
   for( auto r = row + 1, c = col - 1;; r++, c-- )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
   }
   for( auto r = row - 1, c = col - 1;; r--, c--) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
   }
   for( auto r = row - 1, c = col + 1;; r--, c++ )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -BISHOP, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
   }
}

Generator<board> board::generateBlackRookMoves( int8_t row, int8_t col ) const
{
   board temp =*this;
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( -ROOK, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         // See explanation in findWhiteRookMoves
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -ROOK, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
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
         temp.placePiece( -ROOK, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -ROOK, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
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
         temp.placePiece( -ROOK, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         temp.placePiece( -ROOK, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
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
         temp.placePiece( -ROOK, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         temp.placePiece( -ROOK, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         break;
      }
   }
}

Generator<board> board::generateBlackQueenMoves( int8_t row, int8_t col ) const
{
   board temp =*this;
   for( auto r = row + 1, c = col + 1;; r++, c++ )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
   }
   for( auto r = row + 1, c = col - 1;; r++, c-- )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }  
   }
   for( auto r = row - 1, c = col - 1;; r--, c-- )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
   }
   for( auto r = row - 1, c = col + 1;; r--, c++ )
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -QUEEN, r, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( r, c ) < 0 )
      {
         break;
      }
   }
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( -QUEEN, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -QUEEN, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
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
         temp.placePiece( -QUEEN, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -QUEEN, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
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
         temp.placePiece( -QUEEN, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         temp.placePiece( -QUEEN, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
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
         temp.placePiece( -QUEEN, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( row, c ) > 0 )
      {
         temp.placePiece( -QUEEN, row, c );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( row, c ) < 0 )
      {
         break;
      }
   }
}

Generator<board> board::generateBlackKingMoves( int8_t row, int8_t col ) const
{
   board temp =*this;
   if( this->getSpace( row + 1, col + 1 ) >= 0 )
   {
      temp.placePiece( -KING, row + 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row + 1, col ) >= 0 )
   {
      temp.placePiece( -KING, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row + 1, col - 1 ) >= 0 )
   {
      temp.placePiece( -KING, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row, col - 1 ) >= 0 )
   {
      temp.placePiece( -KING, row, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row, col + 1 ) >= 0 )
   {
      temp.placePiece( -KING, row, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row - 1, col - 1 ) >= 0 )
   {
      temp.placePiece( -KING, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row - 1, col ) >= 0 )
   {
      temp.placePiece( -KING, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( row - 1, col + 1 ) >= 0 )
   {
      temp.placePiece( -KING, row - 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         co_yield temp;
      }
      temp =*this;
   }
   if( !(this->gameCtrlFlags & blackCanNotCastleKingside) && !this->getSpace( 7, 5 ) && !this->getSpace( 7, 6 ) && (this->getSpace( 7, 7 ) == -ROOK) ) // KINGside castle
   {
      temp.placePiece( -KING, row, col + 1 ); // KING can't move through check to castle
      temp.removePiece( row, col );
      if( !temp.isBlackInCheck() )
      {
         temp.placePiece( -KING, row, col + 2 );
         temp.placePiece( -ROOK, row, col + 1 ); 
         if( !temp.isBlackInCheck() )
         {
            temp.removePiece( 7, 7 );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.gameCtrlFlags |= blackCanNotCastleKingside;
            temp.gameCtrlFlags |= blackCanNotCastleQueenside;
            
            co_yield temp;
         }
      }
      temp =*this;
   }
   if( !(this->gameCtrlFlags & blackCanNotCastleQueenside) && !this->getSpace( 7, 1 ) && !this->getSpace( 7, 2 ) && !this->getSpace( 7, 3 ) && (this->getSpace( 7, 0 ) == -ROOK) )
   {
      temp.placePiece( -KING, row, col - 1 );
      temp.removePiece( row, col );
      if( !temp.isBlackInCheck() )
      {
         temp.placePiece( -KING, row, col - 2 );
         temp.placePiece( -ROOK, 7, 3 );
         if( !temp.isBlackInCheck() )
         {
            temp.removePiece( 7, 0 );
            temp.gameCtrlFlags ^= whiteToMove;
            temp.gameCtrlFlags |= blackCanNotCastleKingside;
            temp.gameCtrlFlags |= blackCanNotCastleQueenside;
            co_yield temp;
         }
      }
      temp =*this;
   }
}

board board::startingboard()
{
   board b;
   b.placePiece( ROOK, 0, 0 );
   b.placePiece( KNIGHT, 0, 1 );
   b.placePiece( BISHOP, 0, 2 );
   b.placePiece( QUEEN, 0, 3 );
   b.placePiece( KING, 0, 4 );
   b.placePiece( BISHOP, 0, 5 );
   b.placePiece( KNIGHT, 0, 6 );
   b.placePiece( ROOK, 0, 7 );
   for( uint8_t col = 0; col < 8; col++ )
   {
      b.placePiece( PAWN, 1, col );
      b.placePiece( -PAWN, 6, col );
   }
   b.placePiece( -ROOK, 7, 0 );
   b.placePiece( -KNIGHT, 7, 1 );
   b.placePiece( -BISHOP, 7, 2 );
   b.placePiece( -QUEEN, 7, 3 );
   b.placePiece( -KING, 7, 4 );
   b.placePiece( -BISHOP, 7, 5 );
   b.placePiece( -KNIGHT, 7, 6 );
   b.placePiece( -ROOK, 7, 7 );
   return b;
}


} // namespace chessbot
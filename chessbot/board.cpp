// Andrew Lee
// 1 May 2021
// board.cpp
/*
* Implements the board.h class. This looks horrendously long but a lot of it is copy-paste or brute-force move availability checKING.
* The logic in getMove is fairly simple, and the helper functions hide the pain.
*/
#include "board.h"
#include <iostream>

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

void board::placePiece( int8_t id, int8_t col, int8_t row )
{
   this->spaces.at( col + 8 * row ) = id;
}

void board::removePiece( int8_t col, int8_t row )
{
   this->spaces.at( col + 8 * row ) = 0;
}

int8_t board::getSpace( int8_t col, int8_t row ) const
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

   if( this->getSpace( col + 1, row + 1 ) == -PAWN || this->getSpace( col - 1, row + 1 ) == -PAWN )
      return true; // black PAWN checks the KING

   for( int8_t r = row + 1; r < 8; r++ )
   {
      if( this->getSpace( col, r ) == -ROOK || this->getSpace( col, r ) == -QUEEN )
         return true; // black QUEEN or ROOK checks the KING from above
      if( this->getSpace( col, r ) )
         break; // row blocked
   }
   for( int8_t r = row - 1; r >= 0; r-- )
   {
      if( this->getSpace( col, r ) == -ROOK || this->getSpace( col, r ) == -QUEEN )
         return true; // black QUEEN or ROOK checks the KING from below
      if( this->getSpace( col, r ) )
         break; // row blocked
   }
   for( int8_t c = col + 1; c < 8; c++ )
   {
      if( this->getSpace( c, row ) == -ROOK || this->getSpace( c, row ) == -QUEEN )
         return true; // a black ROOK or QUEEN check the KING from the right
      if( this->getSpace( c, row ) )
         break;
   }
   for( int8_t c = col - 1; c >= 0; c-- )
   {
      if( this->getSpace( c, row ) == -ROOK || this->getSpace( c, row ) == -QUEEN )
         return true; // a black ROOK or QUEEN checks the KING from the left
      if( this->getSpace( c, row ) )
         break;
   }

   for( int8_t r = row + 1, c = col + 1;; r++, c++ ) // this is disgusting but efficient; 
   {
      if( this->getSpace( c, r ) == -BISHOP || this->getSpace( c, r ) == -QUEEN )
      {
         return true; // black QUEEN or BISHOP checks the KING from upper-right diagonal
      }
      if( this->getSpace( c, r ) )
         break;
   }
   for( int8_t r = row + 1, c = col - 1;; r++, c--)
   {
      if( this->getSpace( c, r ) == -BISHOP || this->getSpace( c, r ) == -QUEEN )
      {
         return true; // black QUEEN or BISHOP checks the KING from upper-left diagonal
      }
      if( this->getSpace( c, r ) )
         break;
   }
   for( int8_t r = row - 1, c = col - 1;; r--, c-- )
   {
      if( this->getSpace( c, r ) == -BISHOP || this->getSpace( c, r ) == -QUEEN )
      {
         return true; // black QUEEN or BISHOP checks the KING from lower-left diagonal
      }
      if( this->getSpace( c, r ) )
         break;
   }
   for( int8_t r = row - 1, c = col + 1;; r--, c++ )
   {
      if( this->getSpace( c, r ) == -BISHOP || this->getSpace( c, r ) == -QUEEN )
      {
         return true; // black QUEEN or BISHOP checks the KING from lower-right diagonal
      }
      if( this->getSpace( c, r ) )
         break;
   }
   // check for KNIGHTs
   if( this->getSpace( col + 2, row + 1 ) == -KNIGHT || this->getSpace( col + 1, row + 2 ) == -KNIGHT
      || this->getSpace( col - 1, row + 2 ) == -KNIGHT || this->getSpace( col - 2, row + 1 ) == -KNIGHT
      || this->getSpace( col - 2, row - 1 ) == -KNIGHT || this->getSpace( col - 1, row - 2 ) == -KNIGHT
      || this->getSpace( col + 2, row - 1 ) == -KNIGHT || this->getSpace( col + 1, row - 2 ) == -KNIGHT )
      return true; // a black KNIGHT checks the KING from... somewhere
   if( this->getSpace( col + 1, row + 1 ) == -KING || this->getSpace( col, row + 1 ) == -KING
      || this->getSpace( col - 1, row + 1 ) == -KING || this->getSpace( col - 1, row ) == -KING
      || this->getSpace( col + 1, row ) == -KING || this->getSpace( col - 1, row - 1 ) == -KING
      || this->getSpace( col, row - 1 ) == -KING || this->getSpace( col + 1, row - 1 ) == -KING )
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

   if( this->getSpace( col + 1, row - 1 ) == PAWN || this->getSpace( col - 1, row - 1 ) == PAWN )
      return true; // white PAWN checks the KING

   for( int8_t r = row + 1; r < 8; r++ )
   {
      if( this->getSpace( col, r ) == ROOK || this->getSpace( col, r ) == QUEEN )
         return true; // white QUEEN or ROOK checks the KING from above
      if( this->getSpace( col, r ) )
         break; // row blocked
   }
   for( int8_t r = row - 1; r >= 0; r-- )
   {
      if( this->getSpace( col, r ) == ROOK || this->getSpace( col, r ) == QUEEN )
         return true; // white QUEEN or ROOK checks the KING from below
      if( this->getSpace( col, r ) )
         break; // row blocked
   }
   for( int8_t c = col + 1; c < 8; c++ )
   {
      if( this->getSpace( c, row ) == ROOK || this->getSpace( c, row ) == QUEEN )
         return true; // a white ROOK or QUEEN check the KING from the right
      if( this->getSpace( c, row ) )
         break;
   }
   for( int8_t c = col - 1; c >= 0; c-- )
   {
      if( this->getSpace( c, row ) == ROOK || this->getSpace( c, row ) == QUEEN )
         return true; // a white ROOK or QUEEN checks the KING from the left
      if( this->getSpace( c, row ) )
         break;
   }

   for( int8_t r = row + 1, c = col + 1;; r++, c++ ) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( this->getSpace( c, r ) == BISHOP || this->getSpace( c, r ) == QUEEN )
      {
         return true; // white queen or bishop checks the king from upper-right diagonal
      }
      if( this->getSpace( c, r ) )
         break;
   }
   for( int8_t r = row + 1, c = col - 1;; r++, c-- )
   {
      if( this->getSpace( c, r ) == BISHOP || this->getSpace( c, r ) == QUEEN )
      {
         return true; // white queen or bishop checks the king from upper-left diagonal
      }
      if( this->getSpace( c, r ) )
         break;
   }
   for( int8_t r = row - 1, c = col - 1;; r--, c-- )
   {
      if( this->getSpace( c, r ) == BISHOP || this->getSpace( c, r ) == QUEEN )
      {
         return true; // white queen or bishop checks the king from lower-left diagonal
      }
      if( this->getSpace( c, r ) )
         break;
     
   }
   for( int8_t r = row - 1, c = col + 1;; r--, c++ )
   {
      if( this->getSpace( c, r ) == BISHOP || this->getSpace( c, r ) == QUEEN )
      {
         return true; // white queen or bishop checks the KING from lower-right diagonal
      }
      if( this->getSpace( c, r ) )
         break;
   }
   // check for KNIGHTs
   if( this->getSpace( col + 2, row + 1 ) == KNIGHT || this->getSpace( col + 1, row + 2 ) == KNIGHT
      || this->getSpace( col - 1, row + 2 ) == KNIGHT || this->getSpace( col - 2, row + 1 ) == KNIGHT
      || this->getSpace(  col - 2, row - 1 ) == KNIGHT || this->getSpace( col - 1, row - 2 ) == KNIGHT
      || this->getSpace( col + 2, row - 1 ) == KNIGHT || this->getSpace( col + 1, row - 2 ) == KNIGHT )
      return true; // a white knight checks the king from... somewhere
   if( this->getSpace( col + 1, row + 1 ) == KING || this->getSpace( col, row + 1 ) == KING
      || this->getSpace( col - 1, row + 1 ) == KING || this->getSpace( col - 1, row ) == KING
      || this->getSpace( col + 1, row ) == KING || this->getSpace( col - 1, row - 1 ) == KING
      || this->getSpace( col, row - 1 ) == KING || this->getSpace( col + 1, row - 1 ) == KING )
      return true; // the white king checks the black king (absurd, but we need to check for move generation process)

   return false;
}
std::string board::getUCIMove() const
{
   auto ret = std::string{ static_cast<char>(this->UCInotationMove.at( 0 ) + (uint8_t)97), // ascii a-g
   static_cast<char>(this->UCInotationMove.at( 1 ) + (uint8_t)49),
   static_cast<char>(this->UCInotationMove.at( 2 ) + (uint8_t)97),
   static_cast<char>(this->UCInotationMove.at( 3 ) + (uint8_t)49) };
   if( this->UCInotationMove.at( 4 ) )
   {
      return ret + this->UCInotationMove.at( 4 );
   }
   return ret;
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
               auto genMove = this->generateWhitePawnMoves( col, row ); // generic ordering
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case KNIGHT:
            {
               auto genMove = this->generateWhiteKnightMoves( col, row );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case BISHOP:
            {
               auto genMove = this->generateWhiteBishopMoves( col, row );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case ROOK:
            {
               auto genMove = this->generateWhiteRookMoves( col, row );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case QUEEN:
            {
               auto genMove = this->generateWhiteQueenMoves( col, row );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case KING:
            {
               auto genMove = this->generateWhiteKingMoves( col, row );
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
               auto genMove = this->generateBlackPawnMoves( col, row ); // generic ordering
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case KNIGHT:
            {
               auto genMove = this->generateBlackKnightMoves( col, row );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case BISHOP:
            {
               auto genMove = this->generateBlackBishopMoves( col, row );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case ROOK:
            {
               auto genMove = this->generateBlackRookMoves( col, row );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case QUEEN:
            {
               auto genMove = this->generateBlackQueenMoves( col, row );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case KING:
            {
               auto genMove = this->generateBlackKingMoves( col, row );
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
            if( this->getSpace( col, row ) == KNIGHT )
            {
               auto gen = this->generateWhiteKnightMoves( col, row );
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
            if( this->getSpace( col, row ) == BISHOP )
            {
               auto gen = this->generateWhiteBishopMoves( col, row );
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
            if( this->getSpace( col, row ) == PAWN )
            {
               auto gen = this->generateWhitePawnMoves( col, row );
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
            if( this->getSpace( col, row ) == ROOK )
            {
               auto gen = this->generateWhiteRookMoves( col, row );
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
            if( this->getSpace( col, row ) == QUEEN )
            {
               auto gen = this->generateWhiteQueenMoves( col, row );
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
            if( this->getSpace( col, row ) == KING )
            {
               auto gen = this->generateWhiteKingMoves( col, row );
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
            if( this->getSpace( col, row ) == -KNIGHT )
            {
               auto gen = this->generateBlackKnightMoves( col, row );
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
            if( this->getSpace( col, row ) == -BISHOP )
            {
               auto gen = this->generateBlackBishopMoves( col, row );
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
            if( this->getSpace( col, row ) == -PAWN )
            {
               auto gen = this->generateBlackPawnMoves( col, row );
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
            if( this->getSpace( col, row ) == -ROOK )
            {
               auto gen = this->generateBlackRookMoves( col, row );
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
            if( this->getSpace( col, row ) == -QUEEN )
            {
               auto gen = this->generateBlackQueenMoves( col, row );
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
            if( this->getSpace( col, row ) == -KING )
            {
               auto gen = this->generateBlackKingMoves( col, row );
               while( gen )
               {
                  co_yield gen();
               }
            }
         }
      }
   }
}

Generator<board> board::generateWhitePawnMoves( int8_t col, int8_t row ) const
{
   board temp = *this;
   int8_t oneSpaceFwd = this->getSpace( col, row + 1 );
   if( !oneSpaceFwd && row != 6 )
   {
      temp.placePiece( PAWN, col, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row + 1 };
         co_yield temp;
      }
      temp = *this;
      if( row == 1 )
      {
         int8_t twoSpacesFwd = this->getSpace( col, row + 2 );
         if( !twoSpacesFwd )
         {
            temp.placePiece( PAWN, col, row + 2 );
            temp.removePiece( col, row  );
            temp.gameCtrlFlags ^= whiteToMove;
            //temp.enpassant = { row + 1, col };
            if( !temp.isWhiteInCheck() )
            {
               temp.UCInotationMove = { col, row, col, row + 2 };
               co_yield temp;
            }
            temp = *this;
         }
      }
   }
   else if( !oneSpaceFwd )
   {
      temp.placePiece( QUEEN, col, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      //std::cout << "Pawn should promote\n";
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row + 1, 'q' };
         co_yield temp;
      }
      temp = *this;
      temp.placePiece( KNIGHT, col, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row + 1, 'n' };
         co_yield temp;
      }
      temp = *this;
   }
   int8_t leftDiag = this->getSpace( col - 1, row + 1 );
   if( leftDiag < 0 && leftDiag != OFFBOARD && row != 6)
   {
      temp.placePiece( PAWN, col - 1, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row + 1 };
         co_yield temp;
      }
      temp = *this;
   }
   else if( leftDiag < 0 && leftDiag != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col - 1, row + 1 );
      temp.removePiece( col, row );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row + 1, 'n' };
         co_yield temp;
      }
      temp = *this;
      temp.placePiece( QUEEN, col - 1, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row + 1, 'q' };
         co_yield temp;
      }
      temp = *this;
   }
   /*else if( this->enpassant.first == row + 1 && this->enpassant.second == col - 1 )
   {
      temp.placePiece( PAWN, col - 1, row + 1 );
      temp.removePiece( col, row  );
      temp.removePiece( col - 1, row );
      //temp.enpassant = { -1, -1 };
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }*/
   int8_t rightDiag = this->getSpace( col + 1, row + 1 );
   if( rightDiag < 0 && rightDiag != OFFBOARD && row != 6)
   {
      temp.placePiece( PAWN, col + 1, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row + 1};
         co_yield temp;
      }
      temp = *this;
   }
   else if( rightDiag < 0 && rightDiag != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col + 1, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row + 1, 'n' };
         co_yield temp;
      }
      temp = *this;
      temp.placePiece( QUEEN, col + 1, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row + 1, 'q' };
         co_yield temp;
      }
      temp = *this;
   }
   /*else if( this->enpassant.first == row + 1 && this->enpassant.second == col + 1 )
   {
      temp.placePiece( PAWN, col + 1, row + 1 );
      temp.removePiece( col, row  );
      temp.removePiece( col + 1, row );
      //temp.enpassant = { -1, -1 };
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         co_yield temp;
      }
      temp = *this;
   }*/
}

Generator<board> board::generateWhiteKnightMoves( int8_t col, int8_t row ) const
{
   board temp = *this;
   if(  this->getSpace( col + 2, row + 1 ) <= 0 &&  this->getSpace( col + 2, row + 1 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col + 2, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 2, row + 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( col + 1, row + 2 ) <= 0 &&  this->getSpace( col + 1, row + 2 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col + 1, row + 2 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row + 2 };
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( col - 1, row + 2  ) <= 0 &&  this->getSpace( col - 1, row + 2 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col - 1, row + 2  );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row + 2 };
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( col - 2, row + 1  ) <= 0 &&  this->getSpace( col - 2, row + 1  ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col - 2, row + 1  );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 2, row + 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( col - 2, row - 1  ) <= 0 &&  this->getSpace( col - 2, row - 1 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col - 2, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 2, row - 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( col - 1, row - 2  ) <= 0 &&  this->getSpace( col - 1, row - 2 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col - 1, row - 2 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col -2, row - 2 };
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( col + 2, row - 1 ) <= 0 &&  this->getSpace( col + 2, row - 1  ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col + 2, row - 1  );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 2, row - 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if(  this->getSpace( col + 1, row - 2 ) <= 0 &&  this->getSpace( col + 1, row - 2 ) != OFFBOARD )
   {
      temp.placePiece( KNIGHT, col + 1, row - 2 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row - 2 };
         co_yield temp;
      }
      temp = *this;
   }
}

Generator<board> board::generateWhiteBishopMoves( int8_t col, int8_t row ) const
{
   board temp = *this;
   for( int8_t r = row + 1, c = col + 1;; r++, c++) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, r ) < 0 && temp.getSpace( c, r ) != OFFBOARD )
      {
         temp.placePiece( BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, r ) > 0 || temp.getSpace( c, r ) == OFFBOARD )
      {
         break;
      }
   }
   for( int8_t r = row + 1, c = col - 1;; r++, c--)
   {
      if( !temp.getSpace( c, r )  )
      {
         temp.placePiece( BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, r ) < 0 && temp.getSpace( c, r ) != OFFBOARD )
      {
         temp.placePiece( BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, r ) > 0 || temp.getSpace( c, r ) == OFFBOARD )
      {
         break;
      }
   }
   for( int8_t r = row - 1, c = col - 1;; r--, c--)
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, r ) < 0 && temp.getSpace( c, r ) != OFFBOARD )
      {
         temp.placePiece( BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, r ) > 0 || temp.getSpace( c, r ) == OFFBOARD )
      {
         break;
      }
   }
   for( int8_t r = row - 1, c = col + 1;; r--, c++ )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, r ) < 0 && temp.getSpace( c, r ) != OFFBOARD )
      {
         temp.placePiece( BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, r ) > 0 || temp.getSpace( c, r ) == OFFBOARD )
      {
         break;
      }
   }
}

Generator<board> board::generateWhiteRookMoves( int8_t col, int8_t row ) const
{
   board temp = *this;
   for( int8_t r = row + 1; r < 8; r++ )
   {
      if( !temp.getSpace( col, r ) )
      {
         temp.placePiece( ROOK, col, r );
         temp.removePiece( col, row  );
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
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( col, r ) < 0 )
      {
         temp.placePiece( ROOK, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( col, r ) > 0 )
      {
         break;
      }
   }
   for( int8_t r = row - 1; r >= 0; r-- )
   {
      if( !temp.getSpace( col, r ) )
      {
         temp.placePiece( ROOK, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( col, r ) < 0 )
      {
         temp.placePiece( ROOK, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( col, r ) > 0 )
      {
         break;
      }
   }
   for( int8_t c = col + 1; c < 8; c++ )
   {
      if( !temp.getSpace( c, row ) )
      {
         temp.placePiece( ROOK, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, row ) < 0 )
      {
         temp.placePiece( ROOK, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, row ) > 0 )
      {
         break;
      }
   }
   for( int8_t c = col - 1; c >= 0; c-- )
   {
      if( !temp.getSpace( c, row ) )
      {
         temp.placePiece( ROOK, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, row ) < 0 )
      {
         temp.placePiece( ROOK, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= whiteCanNotCastleKingside * (row == 0) * (col == 7);
         temp.gameCtrlFlags |= whiteCanNotCastleQueenside * (row == 0) * (col == 0);
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, row ) > 0 )
      {
         break;
      }
   }
}
Generator<board> board::generateWhiteQueenMoves( int8_t col, int8_t row ) const
{
   board temp = *this;
   for( int8_t r = row + 1, c = col + 1;; r++, c++ )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, r ) < 0 && temp.getSpace( c, r ) != OFFBOARD )
      {
         temp.placePiece( QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, r ) > 0 || temp.getSpace( c, r ) == OFFBOARD )
      {
         break;
      }
   }
   for( int8_t r = row + 1, c = col - 1;; r++, c-- )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, r ) < 0 && temp.getSpace( c, r ) != OFFBOARD )
      {
         temp.placePiece( QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, r ) > 0 || temp.getSpace( c, r ) == OFFBOARD )
      {
         break;
      }
   }
   for( int8_t r = row - 1, c = col - 1;; r--, c-- )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, r ) < 0 && temp.getSpace( c, r ) != OFFBOARD )
      {
         temp.placePiece( QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, r ) > 0 || temp.getSpace( c, r ) == OFFBOARD )
      {
         break;
      }
   }
   for( int8_t r = row - 1, c = col + 1;; r--, c++ )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, r ) < 0 && temp.getSpace( c, r ) != OFFBOARD )
      {
         temp.placePiece( QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, r ) > 0 || temp.getSpace( c, r ) == OFFBOARD )
      {
         break;
      }
   }
   for( int8_t r = row + 1; r < 8; r++ )
   {
      if( !temp.getSpace( col, r ) )
      {
         temp.placePiece( QUEEN, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( col, r ) < 0 )
      {
         temp.placePiece( QUEEN, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( col, r ) > 0 )
      {
         break;
      }
   }
   for( int8_t r = row - 1; r >= 0; r-- )
   {
      if( !temp.getSpace( col, r ) )
      {
         temp.placePiece( QUEEN, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( col, r ) < 0 )
      {
         temp.placePiece( QUEEN, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( col, r ) > 0 )
      {
         break;
      }
   }
   for( int8_t c = col + 1; c < 8; c++ )
   {
      if( !temp.getSpace( c, row ) )
      {
         temp.placePiece( QUEEN, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, row ) < 0 )
      {
         temp.placePiece( QUEEN, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, row ) > 0 )
      {
         break;
      }
   }
   for( int8_t c = col - 1; c >= 0; c-- )
   {
      if( !temp.getSpace( c, row ) )
      {
         temp.placePiece( QUEEN, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( c, row ) < 0 )
      {
         temp.placePiece( QUEEN, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isWhiteInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp = *this;
         break;
      }
      else if( temp.getSpace( c, row ) > 0 )
      {
         break;
      }
   }
}

Generator<board> board::generateWhiteKingMoves( int8_t col, int8_t row ) const
{
   board temp = *this;
   if( this->getSpace( col + 1, row + 1 ) <= 0 && this->getSpace( col + 1, row + 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, col + 1, row + 1  );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row + 1 };
         co_yield  temp;
      }
      temp = *this;
   }
   if( this->getSpace( col, row + 1 ) <= 0 && this->getSpace( col, row + 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, col, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row + 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( col - 1, row + 1 ) <= 0 && this->getSpace( col - 1, row + 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, col - 1, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row + 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( col - 1, row ) <= 0 && this->getSpace( col -  1, row ) != OFFBOARD )
   {
      temp.placePiece( KING, col - 1, row );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row };
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( col + 1, row ) <= 0 && this->getSpace( col + 1, row ) != OFFBOARD )
   {
      temp.placePiece( KING, col + 1, row );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row };
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( col - 1, row - 1 ) <= 0 && this->getSpace( col - 1, row - 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, col - 1, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row - 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( col, row - 1 ) <= 0 && this->getSpace( col, row - 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, col, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row - 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if( this->getSpace( col + 1, row - 1 ) <= 0 && this->getSpace( col + 1, row - 1 ) != OFFBOARD )
   {
      temp.placePiece( KING, col + 1, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= whiteCanNotCastleKingside;
      temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
      if( !temp.isWhiteInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row - 1 };
         co_yield temp;
      }
      temp = *this;
   }
   if( !temp.isWhiteInCheck() )
   {
      if( !(this->gameCtrlFlags & whiteCanNotCastleKingside) && !this->getSpace( 5, 0 ) && !this->getSpace( 6, 0 ) && (this->getSpace( 7, 0 ) == ROOK) ) // KINGside castle
      {
         temp.placePiece( KING, col + 1, row ); // KING can't move through check to castle
         temp.removePiece( col, row );
         if( !temp.isWhiteInCheck() )
         {
            temp.placePiece( KING, col + 2, row );
            temp.placePiece( ROOK, col + 1, row ); // ROOK overwrites phantom KING
            if( !temp.isWhiteInCheck() )
            {
               temp.removePiece( 7, 0 );
               temp.gameCtrlFlags ^= whiteToMove;
               temp.gameCtrlFlags |= whiteCanNotCastleKingside;
               temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
               temp.UCInotationMove = { col, row, col + 2, row };
               co_yield temp;
            }
         }
         temp = *this;
      }
      if( !(this->gameCtrlFlags & whiteCanNotCastleQueenside) && !this->getSpace( 1, 0 ) && !this->getSpace( 2, 0 ) && !this->getSpace( 3, 0 ) && (this->getSpace( 0, 0 ) == ROOK) )
      {
         temp.placePiece( KING, col - 1, row );
         temp.removePiece( col, row );
         if( !temp.isWhiteInCheck() )
         {
            temp.placePiece( KING, col - 2, row );
            temp.placePiece( ROOK, 0, 3 );
            if( !temp.isWhiteInCheck() )
            {
               temp.removePiece( 0, 0 );
               temp.gameCtrlFlags ^= whiteToMove;
               temp.gameCtrlFlags |= whiteCanNotCastleKingside;
               temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
               temp.UCInotationMove = { col, row, col - 1, row };
               co_yield temp;
            }
         }
         temp = *this;
      }
   }
}

Generator<board> board::generateBlackPawnMoves( int8_t col, int8_t row ) const
{
   board temp =*this;
   int8_t oneSpaceFwd = this->getSpace( col, row - 1 );
   if( !oneSpaceFwd && row != 1 )
   {
      temp.placePiece( -PAWN, col, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row - 1 };
         co_yield temp;
      }
      temp =*this;
      if( row == 6 )
      {
         int8_t twoSpacesFwd = this->getSpace( col, row - 2 );
         if( !twoSpacesFwd )
         {
            temp.placePiece( -PAWN, col, row - 2 );
            temp.removePiece( col, row  );
            temp.gameCtrlFlags ^= whiteToMove;
            //temp.enpassant = { col, row - 1 };
            if( !temp.isBlackInCheck() )
            {
               temp.UCInotationMove = { col, row, col, row - 2 };
               co_yield temp;
            }
            temp =*this;
         }
      }
   }
   else if( !oneSpaceFwd )
   {
      temp.placePiece( -QUEEN, col, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row - 1, 'q' };
         co_yield temp;
      }
      temp =*this;
      temp.placePiece( -KNIGHT, col, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row -1, 'n' };
         co_yield temp;
      }
      temp =*this;
   }
   int8_t leftDiag = this->getSpace( col - 1, row - 1 );
   if( leftDiag > 0 && row != 1 )
   {
      temp.placePiece( -PAWN, col - 1, row - 1  );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row - 1 };
         co_yield temp;
      }
      temp =*this;
   }
   else if (leftDiag > 0 )
   {
      temp.placePiece( -KNIGHT, col - 1, row - 1  );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row - 1, 'n' };
         co_yield temp;
      }
      temp = *this;
      temp.placePiece( -QUEEN, col - 1, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row - 1, 'q' };
         co_yield temp;
      }
      temp = *this;
   }
   int8_t rightDiag = this->getSpace( col + 1, row - 1  );
   if( rightDiag > 0 && row != 1)
   {
      temp.placePiece( -PAWN, col + 1, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row - 1 };
         co_yield temp;
      }
      temp =*this;
   }
   else if( rightDiag > 0 )
   {
      temp.placePiece( -KNIGHT, col + 1, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row - 1, 'n' };
         co_yield temp;
      }
      temp = *this;
      temp.placePiece( -QUEEN, col + 1, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row - 1, 'q' };
         co_yield temp;
      }
      temp = *this;
   }
}

Generator<board> board::generateBlackKnightMoves( int8_t col, int8_t row ) const
{
   board temp =*this;
   if( this->getSpace( col + 2, row + 1  ) >= 0 )
   {
      temp.placePiece( -KNIGHT, col + 2, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 2, row + 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col + 1, row + 2  ) >= 0 )
   {
      temp.placePiece( -KNIGHT, col + 1, row + 2 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row + 2 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col - 1, row + 2 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, col - 1, row + 2 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row + 2 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col - 2, row + 1 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, col - 2, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 2, row + 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col - 2, row - 1 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, col - 2, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 2, row - 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col - 1, row - 2 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, col - 1, row - 2 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row - 2 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col + 2, row - 1  ) >= 0 )
   {
      temp.placePiece( -KNIGHT, col + 2, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 2, row - 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col + 1, row - 2 ) >= 0 )
   {
      temp.placePiece( -KNIGHT, col + 1, row - 2 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row - 2 };
         co_yield temp;
      }
      temp =*this;
   }
}

Generator<board> board::generateBlackBishopMoves( int8_t col, int8_t row ) const
{
   board temp =*this;
   for( int8_t r = row + 1, c = col + 1;; r++, c++ ) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( -BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, r ) > 0 )
      {
         temp.placePiece( -BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, r ) < 0 )
      {
         break;
      }
      
   }
   for( int8_t r = row + 1, c = col - 1;; r++, c-- )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( -BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, r ) > 0 )
      {
         temp.placePiece( -BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t r = row - 1, c = col - 1;; r--, c--) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( -BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, r ) > 0 )
      {
         temp.placePiece( -BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t r = row - 1, c = col + 1;; r--, c++ )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( -BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, r ) > 0 )
      {
         temp.placePiece( -BISHOP, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, r ) < 0 )
      {
         break;
      }
   }
}

Generator<board> board::generateBlackRookMoves( int8_t col, int8_t row ) const
{
   board temp =*this;
   for( int8_t r = row + 1; r < 8; r++ )
   {
      if( !temp.getSpace( col, r ) )
      {
         temp.placePiece( -ROOK, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         // See explanation in findWhiteRookMoves
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( col, r ) > 0 )
      {
         temp.placePiece( -ROOK, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( col, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t r = row - 1; r >= 0; r-- )
   {
      if( !temp.getSpace( col, r ) )
      {
         temp.placePiece( -ROOK, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( col, r ) > 0 )
      {
         temp.placePiece( -ROOK, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, col , r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( col, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t c = col + 1; c < 8; c++ )
   {
      if( !temp.getSpace( c, row ) )
      {
         temp.placePiece( -ROOK, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, row ) > 0 )
      {
         temp.placePiece( -ROOK, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, row ) < 0 )
      {
         break;
      }
   }
   for( int8_t c = col - 1; c >= 0; c-- )
   {
      if( !temp.getSpace( c, row ) )
      {
         temp.placePiece( -ROOK, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, row ) > 0 )
      {
         temp.placePiece( -ROOK, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         temp.gameCtrlFlags |= blackCanNotCastleKingside * (row == 7) * (col == 7);
         temp.gameCtrlFlags |= blackCanNotCastleQueenside * (row == 7) * (col == 0);
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, row ) < 0 )
      {
         break;
      }
   }
}

Generator<board> board::generateBlackQueenMoves( int8_t col, int8_t row ) const
{
   board temp =*this;
   for( int8_t r = row + 1, c = col + 1;; r++, c++ )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( -QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, r ) > 0 )
      {
         temp.placePiece( -QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t r = row + 1, c = col - 1;; r++, c-- )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( -QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, r ) > 0 )
      {
         temp.placePiece( -QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, r ) < 0 )
      {
         break;
      }  
   }
   for( int8_t r = row - 1, c = col - 1;; r--, c-- )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( -QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, r ) > 0 )
      {
         temp.placePiece( -QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t r = row - 1, c = col + 1;; r--, c++ )
   {
      if( !temp.getSpace( c, r ) )
      {
         temp.placePiece( -QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, r ) > 0 )
      {
         temp.placePiece( -QUEEN, c, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t r = row + 1; r < 8; r++ )
   {
      if( !temp.getSpace( col, r ) )
      {
         temp.placePiece( -QUEEN, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( col, r ) > 0 )
      {
         temp.placePiece( -QUEEN, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( col, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t r = row - 1; r >= 0; r-- )
   {
      if( !temp.getSpace( col, r ) )
      {
         temp.placePiece( -QUEEN, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( col, r ) > 0 )
      {
         temp.placePiece( -QUEEN, col, r );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, col, r };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( col, r ) < 0 )
      {
         break;
      }
   }
   for( int8_t c = col + 1; c < 8; c++ )
   {
      if( !temp.getSpace( c, row ) )
      {
         temp.placePiece( -QUEEN, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, row ) > 0 )
      {
         temp.placePiece( -QUEEN, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, row ) < 0 )
      {
         break;
      }
   }
   for( int8_t c = col - 1; c >= 0; c-- )
   {
      if( !temp.getSpace( c, row ) )
      {
         temp.placePiece( -QUEEN, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( c, row ) > 0 )
      {
         temp.placePiece( -QUEEN, c, row );
         temp.removePiece( col, row  );
         temp.gameCtrlFlags ^= whiteToMove;
         if( !temp.isBlackInCheck() )
         {
            temp.UCInotationMove = { col, row, c, row };
            co_yield temp;
         }
         temp =*this;
         break;
      }
      else if( temp.getSpace( c, row ) < 0 )
      {
         break;
      }
   }
}

Generator<board> board::generateBlackKingMoves( int8_t col, int8_t row ) const
{
   board temp =*this;
   if( this->getSpace( col + 1, row + 1 ) >= 0 )
   {
      temp.placePiece( -KING, col + 1, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row + 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col, row + 1 ) >= 0 )
   {
      temp.placePiece( -KING, col, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row + 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col - 1, row + 1 ) >= 0 )
   {
      temp.placePiece( -KING, col - 1, row + 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row + 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col - 1, row ) >= 0 )
   {
      temp.placePiece( -KING, col - 1, row );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col + 1, row ) >= 0 )
   {
      temp.placePiece( -KING, col + 1, row );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col - 1, row - 1 ) >= 0 )
   {
      temp.placePiece( -KING, col - 1, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col - 1, row - 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col, row - 1 ) >= 0 )
   {
      temp.placePiece( -KING, col, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col, row - 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( this->getSpace( col + 1, row - 1 ) >= 0 )
   {
      temp.placePiece( -KING, col + 1, row - 1 );
      temp.removePiece( col, row  );
      temp.gameCtrlFlags ^= whiteToMove;
      temp.gameCtrlFlags |= blackCanNotCastleKingside;
      temp.gameCtrlFlags |= blackCanNotCastleQueenside;
      if( !temp.isBlackInCheck() )
      {
         temp.UCInotationMove = { col, row, col + 1, row + 1 };
         co_yield temp;
      }
      temp =*this;
   }
   if( !this->isWhiteInCheck() )
   {
      if( !(this->gameCtrlFlags & blackCanNotCastleKingside) && !this->getSpace( 5, 7 ) && !this->getSpace( 6, 7 ) && (this->getSpace( 7, 7 ) == -ROOK) ) // KINGside castle
      {
         temp.placePiece( -KING, col + 1, row ); // KING can't move through check to castle
         temp.removePiece( col, row );
         if( !temp.isBlackInCheck() )
         {
            temp.placePiece( -KING, col + 2, row );
            temp.placePiece( -ROOK, col + 1, row );
            if( !temp.isBlackInCheck() )
            {
               temp.removePiece( 7, 7 );
               temp.gameCtrlFlags ^= whiteToMove;
               temp.gameCtrlFlags |= blackCanNotCastleKingside;
               temp.gameCtrlFlags |= blackCanNotCastleQueenside;
               temp.UCInotationMove = { col, row, col + 2, row };
               co_yield temp;
            }
         }
         temp = *this;
      }
      if( !(this->gameCtrlFlags & blackCanNotCastleQueenside) && !this->getSpace( 7, 1 ) && !this->getSpace( 7, 2 ) && !this->getSpace( 7, 3 ) && (this->getSpace( 7, 0 ) == -ROOK) )
      {
         temp.placePiece( -KING, col - 1, row );
         temp.removePiece( col, row );
         if( !temp.isBlackInCheck() )
         {
            temp.placePiece( -KING, col - 2, row );
            temp.placePiece( -ROOK, 7, 3 );
            if( !temp.isBlackInCheck() )
            {
               temp.removePiece( 0, 7 );
               temp.gameCtrlFlags ^= whiteToMove;
               temp.gameCtrlFlags |= blackCanNotCastleKingside;
               temp.gameCtrlFlags |= blackCanNotCastleQueenside;
               temp.UCInotationMove = { col, row, col - 1, row };
               co_yield temp;
            }
         }
         temp = *this;
      }
   }
}

board board::startingboard()
{
   board b;
   b.placePiece( ROOK, 0, 0 );
   b.placePiece( KNIGHT, 1, 0 );
   b.placePiece( BISHOP, 2, 0 );
   b.placePiece( QUEEN, 3, 0 );
   b.placePiece( KING, 4, 0 );
   b.placePiece( BISHOP, 5, 0 );
   b.placePiece( KNIGHT, 6, 0 );
   b.placePiece( ROOK, 7, 0 );
   for( uint8_t col = 0; col < 8; col++ )
   {
      b.placePiece( PAWN, col, 1 );
      b.placePiece( -PAWN, col, 6 );
   }
   b.placePiece( -ROOK, 0, 7 );
   b.placePiece( -KNIGHT, 1, 7 );
   b.placePiece( -BISHOP, 2, 7 );
   b.placePiece( -QUEEN, 3, 7 );
   b.placePiece( -KING, 4, 7 );
   b.placePiece( -BISHOP, 5, 7 );
   b.placePiece( -KNIGHT, 6, 7 );
   b.placePiece( -ROOK, 7, 7 );
   return b;
}


} // namespace chessbot
// Andrew Lee
// 2 May 2021
// getmove.cpp
// This file implements a C++20 coroutine approach to generating one move at a time.

#include "board.h"

namespace chessbot
{

Generator<board> board::getMove()
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
            case pawn:
            {
               auto genMove = this->generateWhitePawnMoves( row, col ); // generic ordering
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case knight:
            {
               auto genMove = this->generateWhiteKnightMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case bishop:
            {
               auto genMove = this->generateWhiteBishopMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case rook:
            {
               auto genMove = this->generateWhiteRookMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case queen:
            {
               auto genMove = this->generateWhiteQueenMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case king:
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
            case pawn:
            {
               auto genMove = this->generateBlackPawnMoves( row, col ); // generic ordering
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case knight:
            {
               auto genMove = this->generateBlackKnightMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case bishop:
            {
               auto genMove = this->generateBlackBishopMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case rook:
            {
               auto genMove = this->generateBlackRookMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case queen:
            {
               auto genMove = this->generateBlackQueenMoves( row, col );
               while( genMove )
               {
                  co_yield genMove();
               }
               break;
            }
            case king:
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
}

Generator<board> board::generateWhitePawnMoves( int8_t row, int8_t col ) const
{
   board temp = *this;
   auto oneSpaceFwd = this->getSpace( row + 1, col );
   if( !oneSpaceFwd && row != 7 )
   {
      temp.placePiece( pawn, row + 1, col );
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
            temp.placePiece( pawn, row + 2, col );
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
      temp.placePiece( queen, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( knight, row + 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   auto leftDiag = this->getSpace( row + 1, col - 1 );
   if( leftDiag < 0 && leftDiag != OFFBOARD && row != 6)
   {
      temp.placePiece( pawn, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   else if( leftDiag < 0 && leftDiag != OFFBOARD)
   {
      temp.placePiece( knight, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( queen, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   else if( this->enpassant.first == row + 1 && this->enpassant.second == col - 1 )
   {
      temp.placePiece( pawn, row + 1, col - 1 );
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
      temp.placePiece( pawn, row + 1, col + 1 );
      temp.removePiece( row, col );
      if( row == 7 )
      {
         temp.placePiece( pawn, row + 1, col + 1 );
      }
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   else if( rightDiag < 0 && rightDiag != OFFBOARD )
   {
      temp.placePiece( knight, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( queen, row + 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   else if( this->enpassant.first == row + 1 && this->enpassant.second == col + 1 )
   {
      temp.placePiece( pawn, row + 1, col + 1 );
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
      temp.placePiece( knight, row + 1, col + 2 );
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
      temp.placePiece( knight, row + 2, col + 1 );
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
      temp.placePiece( knight, row + 2, col - 1 );
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
      temp.placePiece( knight, row + 1, col - 2 );
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
      temp.placePiece( knight, row - 1, col - 2 );
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
      temp.placePiece( knight, row - 2, col - 1 );
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
      temp.placePiece( knight, row - 1, col + 2 );
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
      temp.placePiece( knight, row - 2, col + 1 );
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
         temp.placePiece( bishop, r, c );
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
         temp.placePiece( bishop, r, c );
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
         temp.placePiece( bishop, r, c );
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
         temp.placePiece( bishop, r, c );
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
         temp.placePiece( bishop, r, c );
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
         temp.placePiece( bishop, r, c );
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
         temp.placePiece( bishop, r, c );
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
         temp.placePiece( bishop, r, c );
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
         temp.placePiece( rook, r, col );
         temp.removePiece( row, col );
         temp.gameCtrlFlags ^= whiteToMove;
         /* This bears some explaining: castling rules are more complex than most other chess rules
         If the rook is moving from his starting square, we want to forbid castling; all other moves have no effect.
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
         temp.placePiece( rook, r, col );
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
         temp.placePiece( rook, r, col );
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
         temp.placePiece( rook, r, col );
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
         temp.placePiece( rook, row, c );
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
         temp.placePiece( rook, row, c );
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
         temp.placePiece( rook, row, c );
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
         temp.placePiece( rook, row, c );
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
   for( auto r = row + 1, c = col + 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( queen, r, c );
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
         temp.placePiece( queen, r, c );
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
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( queen, r, c );
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
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( queen, r, c );
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
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, c ) < 0 && temp.getSpace( r, c ) != OFFBOARD )
      {
         temp.placePiece( queen, r, c );
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
            co_yield temp;
         }
         temp = *this;
      }
      else if( temp.getSpace( r, col ) < 0 )
      {
         temp.placePiece( queen, r, col );
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
         temp.placePiece( queen, r, col );
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
         temp.placePiece( queen, r, col );
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
         temp.placePiece( queen, row, c );
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
         temp.placePiece( queen, row, c );
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
         temp.placePiece( queen, row, c );
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
         temp.placePiece( queen, row, c );
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
      temp.placePiece( king, row + 1, col + 1 );
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
      temp.placePiece( king, row + 1, col );
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
      temp.placePiece( king, row + 1, col - 1 );
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
      temp.placePiece( king, row, col - 1 );
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
      temp.placePiece( king, row, col + 1 );
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
      temp.placePiece( king, row - 1, col - 1 );
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
      temp.placePiece( king, row - 1, col );
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
      temp.placePiece( king, row - 1, col + 1 );
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
   if( !(this->gameCtrlFlags & whiteCanNotCastleKingside) && !this->getSpace( 0, 5 ) && !this->getSpace( 0, 6 ) && (this->getSpace( 0, 7 ) == rook) ) // kingside castle
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
            temp.gameCtrlFlags |= whiteCanNotCastleKingside;
            temp.gameCtrlFlags |= whiteCanNotCastleQueenside;
            co_yield temp;
         }
      }
      temp = *this;
   }
   if( !(this->gameCtrlFlags & whiteCanNotCastleQueenside) && !this->getSpace( 0, 1 ) && !this->getSpace( 0, 2 ) && !this->getSpace( 0, 3 ) && (this->getSpace( 0, 0 ) == rook) )
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
      temp.placePiece( -pawn, row - 1, col );
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
            temp.placePiece( -pawn, row - 2, col );
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
      temp.placePiece( -queen, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
      temp.placePiece( -knight, row - 1, col );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
   }
   auto leftDiag = this->getSpace( row - 1, col - 1 );
   if( leftDiag > 0 && row != 1 )
   {
      temp.placePiece( -pawn, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
   }
   else if (leftDiag > 0 )
   {
      temp.placePiece( -knight, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( -queen, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
   }
   auto rightDiag = this->getSpace( row - 1, col + 1 );
   if( rightDiag > 0 && row != 1)
   {
      temp.placePiece( -pawn, row - 1, col + 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      //temp.enpassant = { -1, -1 };
      if( !temp.isBlackInCheck() )
         co_yield temp;
      temp =*this;
   }
   else if( rightDiag > 0 )
   {
      temp.placePiece( -knight, row - 1, col - 1 );
      temp.removePiece( row, col );
      temp.gameCtrlFlags ^= whiteToMove;
      if( !temp.isWhiteInCheck() )
         co_yield temp;
      temp = *this;
      temp.placePiece( -queen, row - 1, col - 1 );
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
      temp.placePiece( -knight, row + 1, col + 2 );
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
      temp.placePiece( -knight, row + 2, col + 1 );
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
      temp.placePiece( -knight, row + 2, col - 1 );
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
      temp.placePiece( -knight, row + 1, col - 2 );
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
      temp.placePiece( -knight, row - 1, col - 2 );
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
      temp.placePiece( -knight, row - 2, col - 1 );
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
      temp.placePiece( -knight, row - 1, col + 2 );
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
      temp.placePiece( -knight, row - 2, col + 1 );
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
   for( auto r = row + 1, c = col + 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -bishop, r, c );
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
         temp.placePiece( -bishop, r, c );
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
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -bishop, r, c );
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
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -bishop, r, c );
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
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -bishop, r, c );
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
      r--; c++;
   }
}

Generator<board> board::generateBlackRookMoves( int8_t row, int8_t col ) const
{
   board temp =*this;
   for( auto r = row + 1; r < 8; r++ )
   {
      if( temp.getSpace( r, col ) == 0 )
      {
         temp.placePiece( -rook, r, col );
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
         temp.placePiece( -rook, r, col );
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
         temp.placePiece( -rook, r, col );
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
         temp.placePiece( -rook, r, col );
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
         temp.placePiece( -rook, row, c );
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
         temp.placePiece( -rook, row, c );
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
         temp.placePiece( -rook, row, c );
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
         temp.placePiece( -rook, row, c );
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
   for( auto r = row + 1, c = col + 1;;) // this is disgusting but efficient; probably ought be a while-loop
   {
      if( temp.getSpace( r, c ) == 0 )
      {
         temp.placePiece( -queen, r, c );
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
         temp.placePiece( -queen, r, c );
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
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -queen, r, c );
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
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -queen, r, c );
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
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, c ) > 0 )
      {
         temp.placePiece( -queen, r, c );
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
            co_yield temp;
         }
         temp =*this;
      }
      else if( temp.getSpace( r, col ) > 0 )
      {
         temp.placePiece( -queen, r, col );
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
         temp.placePiece( -queen, r, col );
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
         temp.placePiece( -queen, r, col );
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
         temp.placePiece( -queen, row, c );
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
         temp.placePiece( -queen, row, c );
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
         temp.placePiece( -queen, row, c );
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
         temp.placePiece( -queen, row, c );
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
      temp.placePiece( -king, row + 1, col + 1 );
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
      temp.placePiece( -king, row + 1, col );
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
      temp.placePiece( -king, row + 1, col - 1 );
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
      temp.placePiece( -king, row, col - 1 );
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
      temp.placePiece( -king, row, col + 1 );
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
      temp.placePiece( -king, row - 1, col - 1 );
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
      temp.placePiece( -king, row - 1, col );
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
      temp.placePiece( -king, row - 1, col + 1 );
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
   if( !(this->gameCtrlFlags & blackCanNotCastleKingside) && this->getSpace( 7, 5 ) && this->getSpace( 7, 6 ) && (this->getSpace( 7, 7 ) == rook) ) // kingside castle
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
            temp.gameCtrlFlags |= blackCanNotCastleKingside;
            temp.gameCtrlFlags |= blackCanNotCastleQueenside;
            co_yield temp;
         }
      }
      temp =*this;
   }
   if( !(this->gameCtrlFlags & blackCanNotCastleQueenside) && this->getSpace( 7, 1 ) && this->getSpace( 7, 2 ) && this->getSpace( 7, 3 ) && (this->getSpace( 7, 0 ) == rook) )
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
            temp.gameCtrlFlags |= blackCanNotCastleKingside;
            temp.gameCtrlFlags |= blackCanNotCastleQueenside;
            co_yield temp;
         }
      }
      temp =*this;
   }
}

} // namespace chessbot
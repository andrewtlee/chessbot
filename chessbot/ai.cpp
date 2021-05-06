#include "ai.h"
#include "board.h"
#include <iostream>

namespace chessbot
{

constexpr int PAWN_VALUE = 10;
constexpr int KNIGHT_VALUE = 29;
constexpr int BISHOP_VALUE = 30;
constexpr int ROOK_VALUE = 50;
constexpr int QUEEN_VALUE = 90;

// For now, the heuristic is a naive piece count
int getHeuristic( const board& b )
{
   double material = 0;
   for( int row = 0; row < 8; row++ )
   {
      for( int col = 0; col < 8; col++ )
      {
         auto sp = b.getSpace( row, col );
         switch( sp )
         {
         case PAWN:
         {
            material += PAWN_VALUE;
            break;
         }
         case -PAWN:
         {
            material -= PAWN_VALUE;
            break;
         }
         case KNIGHT:
         {
            material += KNIGHT_VALUE;
            break;
         }
         case -KNIGHT:
         {
            material -= KNIGHT_VALUE;
            break;
         }
         case BISHOP:
         {
            material += BISHOP_VALUE;
            break;
         }
         case -BISHOP:
         {
            material -= BISHOP_VALUE;
            break;
         }
         case ROOK:
         {
            material += ROOK_VALUE;
            break;
         }
         case -ROOK:
         {
            material -= ROOK_VALUE;
            break;
         }
         case QUEEN:
         {
            material += QUEEN_VALUE;
            break;
         }
         case -QUEEN:
         {
            material -= QUEEN_VALUE;
            break;
         }
         case 0:
         {
            break;
         }
         default:
         {
            break;
         }
         }
      }
   }
   //std::cout << material << " ";
   return material;
}

double minimax( board b, int depth )
{
   if( !depth )
   {
      return getHeuristic( b );
   }
   auto moves = b.getLegalMoves();
   double val = 0.;
   if( moves.empty() && b.isWhiteInCheck() ) // white is in checkmate
   {
      return std::numeric_limits<int>::max() + depth*1000;
   }
   else if( moves.empty() && b.isBlackInCheck() )
   {
      return std::numeric_limits<int>::lowest() - depth*10000; // white wins
   }
   if( b.gameCtrlFlags & whiteToMove )
   {
      val = std::numeric_limits<double>::lowest();
      for( auto& m : moves )
      {
         val = std::max( val, minimax( m, depth - 1 ) );
      }
   }
   else
   {
      val = std::numeric_limits<double>::max();
      for( auto& m : moves )
      {
         val = std::min( val, minimax( m, depth - 1 ) );
      }
   }
   return val;
}

board alphabeta( board b, int depth, int maxdepth, int alpha, int beta )
{
   if( depth == maxdepth )
   {
      b.heuristicVal = getHeuristic( b );
      return b;
   }
   //auto moves = b.getLegalMoves();
   auto gen = b.getMove();
   if( !gen && b.isWhiteInCheck() ) // white is in checkmate
   {
      b.heuristicVal = std::numeric_limits<int>::lowest() - depth * 10000;
      return b;
   }
   else if( !gen && b.isBlackInCheck() )
   {
      b.heuristicVal = std::numeric_limits<int>::max() - depth * 10000; // white wins
      return b;
   }
   if( b.gameCtrlFlags & whiteToMove )
   {
      b.heuristicVal = std::numeric_limits<int>::lowest();
      //std::sort( moves.begin(), moves.end(), []( board x, board y ) {return x.heuristicVal > y.heuristicVal; } );
      while( gen )
      {
         b.heuristicVal = std::max( b.heuristicVal, alphabeta( gen(), depth + 1, maxdepth, alpha, beta ).heuristicVal );
         alpha = std::max( alpha, b.heuristicVal );
         if( alpha >= beta )
         {
            break;
         }
      }
      return b;
   }
   else
   {
      b.heuristicVal = std::numeric_limits<int>::max();
      //std::sort( moves.begin(), moves.end(), []( board a, board b ) {return a.heuristicVal < b.heuristicVal; } );
      while( gen )
      {
         b.heuristicVal = std::min( b.heuristicVal, alphabeta( gen(), depth + 1, maxdepth, alpha, beta ).heuristicVal );
         beta = std::min( beta, b.heuristicVal );
         if( beta <= alpha )
         {
            break;
         }
      }
      return b;
   }
}

} // namespace chessbot
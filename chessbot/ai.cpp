#include "ai.h"
#include <stack>
#include <algorithm>
#include <iostream>

namespace chessbot
{

constexpr double pawn_value = 1.;
constexpr double knight_value = 2.9;
constexpr double bishop_value = 3.;
constexpr double rook_value = 5.;
constexpr double queen_value = 9.;

// For now, the heuristic is a naive piece count
double getHeuristic( const board& b )
{
   double material = 0;
   for( int row = 0; row < 8; row++ )
   {
      for( int col = 0; col < 8; col++ )
      {
         auto sp = b.getSpace( row, col );
         switch( sp )
         {
         case pawn:
         {
            material += pawn_value;
            break;
         }
         case -pawn:
         {
            material -= pawn_value;
            break;
         }
         case knight:
         {
            material += knight_value;
            break;
         }
         case -knight:
         {
            material -= knight_value;
            break;
         }
         case bishop:
         {
            material += bishop_value;
            break;
         }
         case -bishop:
         {
            material -= bishop_value;
            break;
         }
         case rook:
         {
            material += rook_value;
            break;
         }
         case -rook:
         {
            material -= rook_value;
            break;
         }
         case queen:
         {
            material += queen_value;
            break;
         }
         case -queen:
         {
            material -= queen_value;
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
      return std::numeric_limits<double>::lowest(); 
   }
   else if( moves.empty() && b.isBlackInCheck() )
   {
      return std::numeric_limits<double>::max(); // white wins
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

board alphabeta( board b, int depth, double alpha, double beta )
{
   if( depth == 0 )
   {
      b.heuristicVal = getHeuristic( b );
      return b;
   }
   //auto moves = b.getLegalMoves();
   auto gen = b.getMove();
   if( !gen && b.isWhiteInCheck() ) // white is in checkmate
   {
      b.heuristicVal = std::numeric_limits<double>::lowest()+depth;
      return b;
   }
   else if( !gen && b.isBlackInCheck() )
   {
      b.heuristicVal = std::numeric_limits<double>::max()-depth; // white wins
      return b;
   }
   if( b.gameCtrlFlags & whiteToMove )
   {
      b.heuristicVal = std::numeric_limits<double>::lowest()+depth;
      //std::sort( moves.begin(), moves.end(), []( board x, board y ) {return x.heuristicVal > y.heuristicVal; } );
      while( gen )
      {
         b.heuristicVal = std::max( b.heuristicVal, alphabeta( gen(), depth - 1, alpha, beta ).heuristicVal );
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
      b.heuristicVal = std::numeric_limits<double>::max()-depth;
      //std::sort( moves.begin(), moves.end(), []( board a, board b ) {return a.heuristicVal < b.heuristicVal; } );
      while( gen )
      {
         b.heuristicVal = std::min( b.heuristicVal, alphabeta( gen(), depth - 1, alpha, beta ).heuristicVal );
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
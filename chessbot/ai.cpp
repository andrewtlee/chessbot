#include "ai.h"
#include <stack>
#include <algorithm>

namespace chessbot
{

constexpr double pawn_value = 1.;
constexpr double knight_value = 2.9;
constexpr double bishop_value = 3.;
constexpr double rook_value = 5.;
constexpr double queen_value = 9.f;

// For now, the heuristic is a naive piece count
double getHeuristic( board& b )
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
            material += pawn_value;
            break;
         case -pawn:
            material -= pawn_value;
            break;
         case knight:
            material += knight_value;
            break;
         case -knight:
            material -= knight_value;
            break;
         case bishop:
            material += bishop_value;
            break;
         case -bishop:
            material -= bishop_value;
            break;
         case rook:
            material += rook_value;
            break;
         case -rook:
            material -= rook_value;
            break;
         case queen:
            material += queen_value;
            break;
         case -queen:
            material -= queen_value;
            break;
         }
      }
   }
   b.heuristicVal = material;
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

double alphabeta( board b, int depth, double alpha, double beta )
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
      //std::sort( moves.begin(), moves.end(), []( board x, board y ) {return x.heuristicVal > y.heuristicVal; } );
      for( auto& m : moves )
      {
         val = std::max( val, alphabeta( m, depth - 1, alpha, beta ) );
         alpha = std::max( alpha, val );
         if( alpha >= beta )
         {
            break;
         }
      }
      return val;
   }
   else
   {
      val = std::numeric_limits<double>::max();
      //std::sort( moves.begin(), moves.end(), []( board a, board b ) {return a.heuristicVal < b.heuristicVal; } );
      for( auto& m : moves )
      {
         val = std::min( val, alphabeta( m, depth - 1, alpha, beta ) );
         beta = std::min( beta, val );
         if( beta <= alpha )
         {
            break;
         }
      }
      return val;
   }
}

double iterativeDFS(board b, int depth )
{
   for( int i = 0; i < depth; i++ )
   {
      alphabeta( b, i, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() );
   }
}

} // namespace chessbot
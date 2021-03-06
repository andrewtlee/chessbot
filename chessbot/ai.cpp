#include "ai.h"
#include "board.h"
#include "transpositiontable.h"
//#include <mutex>
#include <iostream>

namespace chessbot
{

constexpr int PAWN_VALUE = 10;
constexpr int KNIGHT_VALUE = 29;
constexpr int BISHOP_VALUE = 30;
constexpr int ROOK_VALUE = 50;
constexpr int QUEEN_VALUE = 90;

//std::mutex mtx;
transpositionTable tTable;
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
   else if( !gen )
   {
      b.heuristicVal = 0;
      return b;
   }
   if( b.gameCtrlFlags & whiteToMove )
   {
      b.heuristicVal = std::numeric_limits<int>::lowest();
      while( gen )
      {
         board temp = gen();
         auto qresult = tTable.queryTable( temp );
         auto intable = qresult.first;
         if( intable && qresult.second.depthEncountered >= depth && false )
         {
            temp = qresult.second;
            b.heuristicVal = std::max( b.heuristicVal, temp.heuristicVal );
         }
         else
         {
            b.heuristicVal = std::max( b.heuristicVal, alphabeta( temp, depth + 1, maxdepth, alpha, beta ).heuristicVal );
         }
         
         alpha = std::max( alpha, b.heuristicVal );
         if( alpha >= beta )
         {
            break;
         }
      }
   }
   else
   {
      b.heuristicVal = std::numeric_limits<int>::max();
      while( gen )
      {
         auto temp = gen();
         auto qresult = tTable.queryTable( temp );
         if( qresult.first && qresult.second.depthEncountered >= depth && false )
         {
            temp = qresult.second;
            b.heuristicVal = std::min( b.heuristicVal, temp.heuristicVal );
         }
         else 
         {
            b.heuristicVal = std::min( b.heuristicVal, alphabeta( temp, depth + 1, maxdepth, alpha, beta ).heuristicVal );
         }
         beta = std::min( beta, b.heuristicVal );
         if( beta <= alpha )
         {
            break;
         }
      }
   }
   b.depthEncountered = depth;
   tTable.addBoard( b );
   return b;
}

board negamax( board b, int depth, int maxdepth, int alpha, int beta )
{
   if( depth == maxdepth )
   {
      b.heuristicVal = getHeuristic( b );
      return b;
   }
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
   else if( !gen )
   {
      b.heuristicVal = 0;
      return b;
   }
   b.heuristicVal = std::numeric_limits<int>::lowest();
   while( gen )
   {
      b.heuristicVal = std::max( b.heuristicVal, -negamax( gen(), depth + 1, maxdepth, -beta, -alpha ).heuristicVal);
      alpha = std::max( alpha, b.heuristicVal );
      if( alpha > beta )
      {
         break;
      }
   }
   return b;
}

} // namespace chessbot
#pragma once
#include <concepts>
#include <coroutine>
#include <exception>

/*
* Alright, what is this? 
* 
* When I was implementing minimax with Alpha/Beta Pruning, I realized that 
* a lot of A/B pruning's benefit is that partway through the list of child
* moves, you can realize, "I'm not searching here anymore" and stop generating 
* moves. With the board.getLegalMoves() model, that doesn't save a lot of time
* because you already went through all the work of generating all the moves.
* 
* So I thought, "Python generators would be great here. What if instead of finding every
* legal move from a position, I had a resumable function that generated one move at a time?"
* Enter C++20 and coroutines, which are essentially that. Ideally, I wanted:
* def getMove(b: board):
*    while ...:
*       yield move
* But in C++. After a few hours, I found boilerplate code to handle this use case.
* Why is this 50 lines of code? I don't know. But now we can generate one move at a time.
*/

// Thanks David Mazieres (at https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html ) for this code
// What in God's name are these wrappers??

template<typename T>
struct Generator 
{
   struct promise_type;
   using handle_type = std::coroutine_handle<promise_type>;
   struct promise_type
   {
      T value_;
      std::exception_ptr exception_;
      Generator get_return_object()
      {
         return Generator( handle_type::from_promise( *this ) );
      }
      std::suspend_always initial_suspend() { return {}; }
      std::suspend_always final_suspend() noexcept { return {}; }
      void unhandled_exception() { exception_ = std::current_exception(); }
      template<std::convertible_to<T> From> // C++20 concept
      std::suspend_always yield_value( From&& from ) 
      {
         value_ = std::forward<From>( from );
         return {};
      }
      void return_void() {}
   };
   handle_type h_;
   Generator( handle_type h ) : h_( h ) {}
   ~Generator() { h_.destroy(); }
   explicit operator bool() {
      fill();
      return !h_.done();
   }
   T operator()() {
      fill();
      full_ = false;
      return std::move( h_.promise().value_ );
   }

private:
   bool full_ = false;

   void fill() {
      if( !full_ ) {
         h_();
         if( h_.promise().exception_ )
            std::rethrow_exception( h_.promise().exception_ );
         full_ = true;
      }
   }
};
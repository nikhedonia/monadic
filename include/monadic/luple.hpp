#ifndef MONADIC_LUPLE_HPP
#define MONADIC_LUPLE_HPP

#include <tuple>
#include <type_traits>
#include "./macro.hpp"

namespace monadic {

using std::forward;
using std::is_same;
using std::declval;
using std::decay_t;
using std::enable_if_t;
using std::size_t;

template<class T>
struct Capture{ // container for perfectly forwarded values with value-semantics
  T value;

  template<class X,
    REQUIRES(std::is_convertible<X,T>()) >
  constexpr Capture(X&&x)
  : value( std::forward<X>(x) ){}

  decltype(auto) get() const{
    return value;
  }

  decltype(auto) get() {
    return value;
  }
};

template<class X>
constexpr auto capture(X&&x){
  return Capture<X>(forward<X>(x));
}

template<class...X>
constexpr auto luple(X&&...x){ // this pragmatic tuple implementation compiles faster than ordinary tuple classes
  return ([](auto...y){
    return [=](auto F)->decltype(auto){
      return F(y.get()...);
    };
  })(capture(forward<X>(x))...); // this allows to store references and move temporaries without overhead...
}

auto nop = [](auto...){return 1;}; // required for parameter pack expansion

auto call = [](auto f, auto&&...x){
  f( FORWARD(x)... );
  return 1; //
};

// f(x)... not allowed ; requires context within the expansion can take place
// nop( f(x)... ) ; fails if f(x) returns void;
auto each=[](auto...x){
  return [=](auto f){
    nop( call(f, x)... );
    return luple(x...);
  };
};

auto map=[](auto&&...x){
  return [=](auto F){
    return luple( F(x)... );
  };
};


auto append=[](auto&&...x){
  return ([](auto...y){
    return [=](auto&&...E){
      return luple(y.get()...,forward<decltype(E)>(E)...);
    };
  })(capture(forward<decltype(x)>(x))...);
};

auto prepend=[](auto&&...x){
  return ([](auto...y){
    return [=](auto&&...E){
      return luple(forward<decltype(E)>(E)...,y.get()...);
    };
  })(capture(forward<decltype(x)>(x))...);
};

auto concat=[](auto&&...x){
  return ([](auto...y){
    return [=](auto L){
      return L([=](auto&&...e){
        return luple(y.get()... , forward<decltype(e)>(e)... );
      });
    };
  })(capture(forward<decltype(x)>(x))...);
};



}

#endif

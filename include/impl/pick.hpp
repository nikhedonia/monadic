#ifndef MONADIC_PICK_HPP
#define MONADIC_PICK_HPP

#include "macro.hpp"

namespace monadic {

template<size_t i>
struct Get{

  template<
    size_t j=i,
    class X, class...Xs,
    REQUIRES(j>0)>
  auto operator()(X&&x, Xs&&...xs)const{
    return Get<i-1>()( FORWARD(xs)...);
  }

  template<
    size_t j=i,
    class X, class...Xs,
    REQUIRES(j==0)>
  auto operator()(X&&x, Xs...)const{
    return FORWARD(x);
  }
};

template<size_t i>
constexpr static auto get = Get<i>();


template<char...X> constexpr auto operator "" _N(){
  return integral_constant<size_t,C2N( (X-'0')... )>();
}


static auto pick = [](auto...x){
  return [=](auto...i){
    return apply( get<decltype(i)::value%sizeof...(x)>(x...) ... );
  };
};


}

#endif

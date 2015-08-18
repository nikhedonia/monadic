#ifndef MONADIC_FIND_HPP
#define MONADIC_FIND_HPP

#include "macro.hpp"
#include "apply.hpp"

namespace monadic {

template<template<class,class> class check=is_same>
struct FindIn{

  template<unsigned i=0,class X=bool>
  constexpr auto operator()(X=false)const
  {}


  template<unsigned i=0, class X, class Y, class...Ys,
  REQUIRES(check<decay_t<X>,decay_t<Y>>::value) >
  constexpr auto operator()(X,Y,Ys...)const {
    return get<i>;
  }

  template<unsigned i=0, class X, class Y, class...Ys,
  REQUIRES(!check<decay_t<X>,decay_t<Y>>::value) >
  constexpr auto operator()(X&&x, Y y, Ys...ys)const {
    return this->operator()<i+1>(x,ys...);
  }

};


static auto findIn = FindIn<>();

struct RemoveDupTypes{

  template<class X>
  auto operator()(X&&x){
    return apply(forward<X>(x));
  }

  auto operator()(){
    return apply();
  }


  template<class X,  class...Ys, // when X is unique
  REQUIRES(is_same<decltype(findIn(declval<X>(),declval<Ys>()...)),void>::value)>
  auto operator()(X&&x, Ys&&...ys){
    return apply(forward<X>(x))(concat)((*this)(forward<Ys>(ys)...));
  }

  template<class X,  class...Ys, // otherwise, remove X
  REQUIRES(!is_same<decltype(findIn(declval<X>(),declval<Ys>()...)),void>::value)>
  decltype(auto) operator()(X&&x, Ys&&...ys){
    return (*this)(forward<Ys>(ys)...);
  }

};


static auto removeDuplicateTypes = RemoveDupTypes();

}

#endif

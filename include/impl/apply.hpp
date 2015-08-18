#ifndef MONADIC_LUPLE_HPP
#define MONADIC_LUPLE_HPP

#include <tuple>
#include <type_traits>
#include "macro.hpp"
#include "capture.hpp"

namespace monadic {

static auto apply = [](auto&&...x){
  return ([](auto...y){
    return [=](auto F)->decltype(auto){
      return F(y.get()...);
    };
  })(capture(FORWARD(x))...);
};

static auto one = [](auto...){return 1;};
static auto call = [](auto f, auto&&...x){
  f( FORWARD(x)... );
  return 1; //
};

static auto each=[](auto...x){
  return [=](auto f){
    one( call(f, x)... );
    return apply(x...);
  };
};

static auto map=[](auto&&...x){
  return [=](auto F){
    return apply( F(x)... );
  };
};










}


#endif

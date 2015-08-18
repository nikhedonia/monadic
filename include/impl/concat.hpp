#ifndef MONADIC_CONCAT_HPP
#define MONADIC_CONCAT_HPP

#include "capture.hpp"
#include "apply.hpp"

namespace monadic {

static auto append=[](auto&&...x){
  return ([](auto...y){
    return [=](auto&&...E){
      return apply(y.get()...,forward<decltype(E)>(E)...);
    };
  })(capture(forward<decltype(x)>(x))...);
};

static auto prepend=[](auto&&...x){
  return ([](auto...y){
    return [=](auto&&...E){
      return apply(forward<decltype(E)>(E)...,y.get()...);
    };
  })(capture(forward<decltype(x)>(x))...);
};

static auto concat=[](auto&&...x){
  return ([](auto...y){
    return [=](auto L){
      return L([=](auto&&...e){
        return apply(y.get()... , FORWARD(e)... );
      });
    };
  })(capture(forward<decltype(x)>(x))...);
};

}


#endif

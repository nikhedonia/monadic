#ifndef MONADIC_FOLD_HPP
#define MONADIC_FOLD_HPP

#include "apply.hpp"
#include "concat.hpp"

namespace monadic {

static auto fold = [](auto Z){
  return [Z](auto&&...X){
    auto L=apply(FORWARD(X)...);
    return [L,Z](auto L2){
      return L(concat)(L2)([Z](decltype(X)&&...B, auto&&...A){
          return apply(Z(FORWARD(A),FORWARD(B))...);
      });
    };
  };
};

}

#endif

#ifndef MONADIC_CAPTURE_HPP
#define MONADIC_CAPTURE_HPP

namespace monadic {

template<class T>
struct Capture{ // container for perfectly forwarded values with value-semantics
  T value;

  template<class X,
    REQUIRES(is_convertible<X,T>()) >
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
constexpr decltype(auto) capture(X&&x){
  return Capture<X>(forward<X>(x));
}

}

#endif

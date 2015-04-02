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
using std::is_convertible;
using std::integral_constant;
using std::make_index_sequence;
using std::size_t;

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


static auto luple = [](auto&&...x){ // this pragmatic tuple implementation compiles faster than ordinary tuple classes
  return ([](auto...y){
    return [=](auto F)->decltype(auto){
      return F(y.get()...);
    };
  })(capture(FORWARD(x))...); // this allows to store references and move temporaries without overhead...
};

static auto nop = [](auto...){return 1;}; // required for parameter pack expansion

static auto call = [](auto f, auto&&...x){
  f( FORWARD(x)... );
  return 1; //
};

// f(x)... not allowed ; requires context within the expansion can take place
// nop( f(x)... ) ; fails if f(x) returns void;
static auto each=[](auto...x){
  return [=](auto f){
    nop( call(f, x)... );
    return luple(x...);
  };
};

static auto map=[](auto&&...x){
  return [=](auto F){
    return luple( F(x)... );
  };
};


static auto append=[](auto&&...x){
  return ([](auto...y){
    return [=](auto&&...E){
      return luple(y.get()...,forward<decltype(E)>(E)...);
    };
  })(capture(forward<decltype(x)>(x))...);
};

static auto prepend=[](auto&&...x){
  return ([](auto...y){
    return [=](auto&&...E){
      return luple(forward<decltype(E)>(E)...,y.get()...);
    };
  })(capture(forward<decltype(x)>(x))...);
};

static auto concat=[](auto&&...x){
  return ([](auto...y){
    return [=](auto L){
      return L([=](auto&&...e){
        return luple(y.get()... , FORWARD(e)... );
      });
    };
  })(capture(forward<decltype(x)>(x))...);
};



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
    return luple( get<decltype(i)::value%sizeof...(x)>(x...) ... );
  };
};


struct FindIn{

  template<unsigned i=0,class X=bool>
  constexpr auto operator()(X=false)const
  {}


  template<unsigned i=0, class X, class Y, class...Ys,
  REQUIRES(is_same<decay_t<X>,decay_t<Y>>::value) >
  constexpr auto operator()(X,Y,Ys...)const {
    return get<i>;
  }

  template<unsigned i=0, class X, class Y, class...Ys,
  REQUIRES(!is_same<decay_t<X>,decay_t<Y>>::value) >
  constexpr auto operator()(X&&x, Y y, Ys...ys)const {
    return this->operator()<i+1>(x,ys...);
  }

};


static auto findIn = FindIn();

struct RemoveDupTypes{

  template<class X>
  auto operator()(X&&x){
    return luple(forward<X>(x));
  }

  auto operator()(){
    return luple();
  }


  template<class X,  class...Ys, // when X is unique
  REQUIRES(is_same<decltype(findIn(declval<X>(),declval<Ys>()...)),void>::value)>
  auto operator()(X&&x, Ys&&...ys){
    return luple(forward<X>(x))(concat)((*this)(forward<Ys>(ys)...));
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

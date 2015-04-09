# Monadic C++1y library

## Description

Variadic template meta programming is a very error-prone and (compile-)time consuming task.
This library provides a fast lambda based tuple implementation, enabling a monadic programming style and making meta-programming debuggable...

##Examples:

```c++
using namespace monadic;

auto x = luple(1,2.0,"foo"); //heterogenus tuple;
(x)( get<0> ); // get first element

(x)(each)([](auto x){ //print each element
  cout<<x<<endl;
}); //returns x

(x)(map)([](auto x){
  return is_same<decltype(x),int>{};
}); // transforms x to luple( true_type() , false_type() , true_type() )


(x)(concat)(y)  // combines two luples

(x)(append(1,2,3,4))  // append some elements
(x)(prepend(1,2,3,4)) // prepend some elements

concat(1.0)(x)(findIn) // find first float in x and return a getter
(x)(removeDuplicateTypes) // remove all duplicate types from x

```




#include<igloo/igloo_alt.h>
#include<monadic/luple.hpp>

using namespace monadic;
using namespace igloo;


Describe(monadicTestcase) {

  Describe(lupleTests){
    It(should_consume_elements){
      luple(1, 2, 3);
    }

    It(should_handle_each_continator){
      int i=0;
      int toExpected[3]={1, 2, 3};
      luple(1, 2, 3)(each)([&](auto item){
          Assert::That( item ,  Equals( toExpected[i++] ) );
      });
      Assert::That( i , Equals(3) );
    }

    It(should_handle_map_continator){
      int i=0;
      int toExpected[3]={0, 1, 2};
      luple(1, 2, 3)(map)([](auto x){
          return x-1;
      })(each)([&](auto item){
          Assert::That( item ,  Equals( toExpected[i++] ) );
      });

      Assert::That( i , Equals(3) );
    }



  };

};




int main(int argc, char const* argv[])
{
  return TestRunner::RunAllTests(argc, argv);
}

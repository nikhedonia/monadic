#include<igloo/igloo_alt.h>
#include<monadic>

using namespace monadic;
using namespace igloo;


Describe(monadicTestcase) {

  Describe(applyTests){
    It(should_consume_elements){
      apply(1, 2, 3);
    }

    It(should_handle_each_continator){
      int i=0;
      int toExpected[3]={1, 2, 3};
      apply(1, 2, 3)(each)([&](auto item){
          Assert::That( item ,  Equals( toExpected[i++] ) );
      });
      Assert::That( i , Equals(3) );
    }

    It(should_map){
      int i=0;
      int toExpected[3]={0, 1, 2};
      apply(1, 2, 3)(map)([](auto x){
          return x-1;
      })(each)([&](auto item){
          Assert::That( item ,  Equals( toExpected[i++] ) );
      });

      Assert::That( i , Equals(3) );
    }

    It(should_append){
      int i=0;
      int toExpected[3]={1, 2, 3};
      apply(1)(append)(2,3)(each)([&](auto item){
          Assert::That( item ,  Equals( toExpected[i++] ) );
      });
      Assert::That( i , Equals(3) );
    }

    It(should_get){
      Assert::That( apply(1,2,3)(get<0>), Equals(1) );
      Assert::That( apply(1,2,3)(get<1>), Equals(2) );
      Assert::That( apply(1,2,3)(get<2>), Equals(3) );
    }

    It(should_pick){
      int i=0;
      int toExpected[3]={3, 2, 1};
      apply(1,2,3)(pick)(2_N, 1_N, 0_N)(each)([&](auto item){
          Assert::That( item ,  Equals( toExpected[i++] ) );
      });
      Assert::That( i , Equals(3) );
    }

    It(should_find_element_by_type){
      auto x=apply(true,1.2,3);
      auto getInt = concat(0)(x)(findIn);
      Assert::That( (x)(getInt) , Equals(3) );
    }

    It(should_remove_duplicate_types){
      auto x=apply(1,true,1.2,3);
      (x)(removeDuplicateTypes);
    }


    It(should_fold){
      auto x=apply(1,2);
      auto y=x;
      auto add=[](auto a,auto b){ return a+b; };
      auto r = (x)(fold(add))(y);

      bool c=0;
      r([&c](auto x,auto y){
        Assert::That( x , Equals(2) );
        Assert::That( y , Equals(4) );
        c=1;
      });

      Assert::That(c);

    }



  };
};




int main(int argc, char const* argv[]) {
  return TestRunner::RunAllTests(argc, argv);
}

#ifndef MONADIC_MACRO_HPP
#define MONADIC_MACRO_HPP


#define FORWARD(x) std::forward<decltype(x)>(x)
#define REQUIRES(...) class=std::enable_if_t<(__VA_ARGS__)>


constexpr long MCC( const char* S, const unsigned P=0, long Result=0)
{
    return (S[P])?MCC(S,P+1, 256*Result+S[P]):Result;
}

template<class b, class exp>
constexpr int Pow(b B, exp Exp) { return (Exp)?B*Pow(B,Exp-1):1; }

template<class...>
constexpr int C2N(){ return 0;}

template<class arg, class...args>
constexpr int C2N(arg Arg, args...Args) { return C2N<args...>(Args...) + Arg*Pow(10,sizeof...(args)) ;}


#define STRING(S) #S
#define CONCAT(lhs,rhs) lhs#rhs
#define CHAR(CH) MCC(STRING(CH))




#endif

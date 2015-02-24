#ifndef MONADIC_MACRO_HPP
#define MONADIC_MACRO_HPP


#define FORWARD(x) std::forward<decltype(x)>(x)
#define REQUIRES(...) class=std::enable_if_t<(__VA_ARGS__)>

#endif

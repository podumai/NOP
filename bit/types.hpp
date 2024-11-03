#ifndef __NPL_LIB_TYPES_H__
#define __NPL_LIB_TYPES_H__ 1

namespace npl
{

  using i8 = signed char;
  using u8 = unsigned char;
  using i16 = signed short;
  using u16 = unsigned short;

#if __WORDSIZE == 64
  using i32 = signed int;
  using u32 = unsigned int;
#else
  using i32 = signed long;
  using u32 = unsigned long;
#endif

  using i64 = signed long long;
  using u64 = unsigned long long;
  
  using size_t = unsigned long;
  using ptrdiff_t = long;

  using f32 = float;
  using f64 = double;
  using f128 = long double;

}

#endif
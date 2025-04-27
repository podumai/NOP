#ifndef NOP_TYPES_HPP /* Begin types header file */
#define NOP_TYPES_HPP 1

namespace nop /* Begin namespace nop */
{

#if __cplusplus >= 201103L
  using i8 = signed char;
  using u8 = unsigned char;
  using i16 = short;
  using u16 = unsigned short;

#if __x86_64__ && !defined __ILP32__
  using i32 = int;
  using u32 = unsigned int;
#else
  using i32 = long;
  using u32 = unsigned long;
#endif

  using i64 = long long;
  using u64 = unsigned long long;
  
  using size_t = unsigned long;
  using ptrdiff_t = long;

  using f32 = float;
  using f64 = double;

#if __x86_64__ && !defined __ILP32__
  using f128 = long double;
#endif
#else
  typedef signed char i8;
  typedef unsigned char u8;
  typedef short i16;
  typedef unsigned short u16;

#if __x86_64__ && !defined __ILP32__
  typedef int i32;
  typedef unsigned int u32;
#else
  typedef long i32;
  typedef unsigned long u32;
#endif

  typedef long long i64;
  typedef unsigned long long u64;

  typedef unsigned long size_t;
  typedef long ptrdiff_t;

  typedef float f32;
  typedef double f64;

#if __x86_64__ && !defined __ILP32__
  typedef long double f128;
#endif
#endif

} /* End namespace nop */

#endif /* End types header file */

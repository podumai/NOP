#ifndef __NOP_LIB_TYPES_H__
#define __NOP_LIB_TYPES_H__ 1

namespace nop
{

  using i8 = char;
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

}

#endif

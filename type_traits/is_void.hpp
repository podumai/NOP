#ifndef NOP_TYPE_TRAITS_IS_VOID_HPP /* Begin Is void header file */
#define NOP_TYPE_TRAITS_IS_VOID_HPP 1

#include "basic_traits.hpp"

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct IsVoid : FalseType
  {};

  template<>
  struct IsVoid<void> : TrueType
  {};

  template<>
  struct IsVoid<const void> : TrueType
  {};

  template<>
  struct IsVoid<volatile void> : TrueType
  {};

  template<>
  struct IsVoid<volatile const void> : TrueType
  {};

#if __cplusplus >= 201402L
  template<typename T>
  constexpr bool IsVoidV = IsVoid<T>::value;
#endif

} /* End namespace nop */

#endif /* End Is void header file */

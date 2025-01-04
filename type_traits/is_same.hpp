#ifndef NOP_TYPE_TRAITS_IS_SAME_HPP
#define NOP_TYPE_TRAITS_IS_SAME_HPP 1

#include "basic_traits.hpp"

namespace nop /* Begin namespace nop */
{

  template<typename T, typename U>
  struct IsSame : FalseType
  {};

  template<typename T>
  struct IsSame<T, T> : TrueType
  {};

#if __cplusplus >= 201402L
  template<typename T, typename U>
  constexpr bool IsSameV = IsSame<T, U>::value;

  template<typename T>
  constexpr bool IsSameV<T, T> = IsSame<T, T>::value;
#endif

} /* End namespace nop */

#endif

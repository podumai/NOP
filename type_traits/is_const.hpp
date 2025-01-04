#ifndef NOP_TYPE_TRAITS_IS_CONST_HPP /* Begin Is const header file */
#define NOP_TYPE_TRAITS_IS_CONST_HPP 1

#include "basic_traits.hpp"

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct IsConst : FalseType
  {};

  template<typename T>
  struct IsConst<const T> : TrueType
  {};

  template<typename T>
  struct IsConst<const T&> : TrueType
  {};

  template<typename T>
  struct IsConst<const T&&> : TrueType
  {};

  template<typename T>
  struct IsConst<const T*> : TrueType
  {};

  template<typename T>
  struct IsConst<T* const> : TrueType
  {};

#if __cplusplus >= 201402L
  template<typename T>
  constexpr bool IsConstV = IsConst<T>::value;
#endif

} /* End namespace nop */

#endif /* End Is const header file */

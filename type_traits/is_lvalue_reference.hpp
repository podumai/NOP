#ifndef NOP_TYPE_TRAITS_IS_LVALUE_REFERENCE_HPP /* Begin Is lvalue reference header file */
#define NOP_TYPE_TRAITS_IS_LVALUE_REFERENCE_HPP 1

#include "basic_traits.hpp"

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct IsLvalueReference : FalseType
  {};

  template<typename T>
  struct IsLvalueReference<T&> : TrueType
  {};

#if __cplusplus >= 201402L
  template<typename T>
  constexpr bool IsLvalueReferenceV = IsLvalueReference<T>::value;
#endif

} /* End namespace nop */

#endif /* End Is lvalue reference header file */

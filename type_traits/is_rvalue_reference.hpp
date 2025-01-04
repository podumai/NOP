#ifndef NOP_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP /* Begin Is rvalue reference header file */
#define NOP_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP 1

#include "basic_traits.hpp"

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct IsRvalueReference : FalseType
  {};

  template<typename T>
  struct IsRvalueReference<T&&> : TrueType
  {};

#if __cplusplus >= 201402L
  template<typename T>
  constexpr bool IsRvalueReferenceV = IsRvalueReference<T>::value;
#endif

} /* End namespace nop */

#endif /* End Is rvalue reference header file */

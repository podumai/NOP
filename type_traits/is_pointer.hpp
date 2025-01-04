#ifndef NOP_TYPE_TRAITS_IS_POINTER_HPP /* Begin Is pointer header file */
#define NOP_TYPE_TRAITS_IS_POINTER_HPP 1

#include "basic_traits.hpp"

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct IsPointer : FalseType
  {};

  template<typename T>
  struct IsPointer<T*> : TrueType
  {};

  template<typename T>
  struct IsPointer<T* const> : TrueType
  {};

#if __cplusplus >= 201402L
  template<typename T>
  constexpr bool IsPointerV = IsPointer<T>::value;
#endif

} /* End namespace nop */

#endif /* End Is pointer header file */

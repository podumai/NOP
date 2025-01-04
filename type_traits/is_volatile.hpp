#ifndef NOP_TYPE_TRAITS_IS_VOLATILE_HPP /* Begin Is volatile header file */
#define NOP_TYPE_TRAITS_IS_VOLATILE_HPP 1

#include "basic_traits.hpp"

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct IsVolatile : FalseType
  {};

  template<typename T>
  struct IsVolatile<volatile T> : TrueType
  {};

  template<typename T>
  struct IsVolatile<volatile T&> : TrueType
  {};

  template<typename T>
  struct IsVolatile<volatile T&&> : TrueType
  {};

  template<typename T>
  struct IsVolatile<volatile T*> : TrueType
  {};

  template<typename T>
  struct IsVolatile<volatile T* const> : TrueType
  {};

#if __cplusplus >= 201402L
  template<typename T>
  constexpr bool IsVolatileV = IsVolatile<T>::value;
#endif

} /* End namespace nop */

#endif /* End Is volatile header file */

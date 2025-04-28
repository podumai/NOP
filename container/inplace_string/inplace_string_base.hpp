#ifndef NOP_DETAILS_CONTAINER_INPLACE_STRING_BASE_HPP /* Begin __nop_details::container::inplace_string_base header file */
#define NOP_DETAILS_CONTAINER_INPLACE_STRING_BASE_HPP 1UL

#ifndef NOP_DETAILS_CONTAINER_INPLACE_STRING_IMPL_HPP
  #error "This header file can not be included explicitly. Use inplace_string.hpp instead."
#endif

#pragma once

#include <iterator> /* std::reverse_iterator<T> */
#include <cstdint> /* fixed integral types */
#include "base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_detials */
{

namespace container /* Begin namespace container */
{

template<typename CharT, typename Traits>
class inplace_string_base
{
 public:
  using traits_type = Traits;
  using value_type = CharT;
  using size_type = typename std::size_t;
  using difference_type = typename std::ptrdiff_t;
  using reference = CharT&;
  using const_reference = const CharT&;
  using pointer = CharT*;
  using const_pointer = const CharT*;
  using iterator = CharT*;
  using const_iterator = const CharT*;
  using reverse_iterator = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

 public:
  inline static size_type npos{static_cast<size_type>(-1)};
};

} /* End namespace container */

} /* End namespace __nop_details */

#endif /* End __nop_details::container::inplace_string_base header file */
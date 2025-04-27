#ifndef NOP_DETAILS_CONTAINER_INPLACE_VECTOR_BASE_HPP /* Begin __nop_details::container::inplace_vector_base header file */
#define NOP_DETAILS_CONTAINER_INPLACE_VECTOR_BASE_HPP 1UL

#ifndef NOP_DETAILS_CONTAINER_INPLACE_VECTOR_IMPL_HPP
  #error "This header file cannot be included explicitly. Use inplace_vector instead."
#endif

#pragma once

#include <iterator> /* std::reverse_iterator, std::make_reverse_iterator */
#include <cstdint>  /* std::size_t, std::ptrdiff_t */

#include "base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace container /* Begin namespace container */
{

template<
         typename    T,
         std::size_t N
        >
class inplace_vector_base
{
 public:
  using value_type             = T;
  using size_type              = std::size_t;
  using difference_type        = std::ptrdiff_t;
  using reference              = T&;
  using const_reference        = const T&;
  using pointer                = T*;
  using const_pointer          = const T*;
  using iterator               = T*;
  using const_iterator         = const T*;
  using reverse_iterator       = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

 public:
  [[nodiscard]] static func capacity() noexcept -> size_type
  {
    return N;
  }

  [[nodiscard]] static func max_size() noexcept -> size_type
  {
    return N;
  }

  static func shrink_to_fit() noexcept -> void
  {
    /* Empty */
  }

  static func reserve(size_type size) -> void
  {
    if (size > N)
    {
      throw std::bad_alloc{};
    }
  }

};

} /* End namespace container */

} /* End namespace __nop_details */

#endif /* End __nop_details::container::inplace_vector_base header file */

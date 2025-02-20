#ifndef NOP_DETAILS_ARRAY_BASE_HPP /* Begin nop_details array_base header file */
#define NOP_DETAILS_ARRAY_BASE_HPP 1UL

#pragma once

#ifndef NOP_DETAILS_ARRAY_IMPL_HPP
  #error "This header file cannot be included explicitly. Use array.hpp instead."
#endif

#include <cstdint>

namespace nop_details /* Begin namespace nop_details */
{

namespace container /* Begin namespace container */
{

template<
         typename    T,
         std::size_t N
        >
class array_base
{
 public:
  using value_type      = T;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference       = T&;
  using const_reference = const T&;
  using pointer         = T*;
  using const_pointer   = const T*;

 public:
  [[nodiscard]] constexpr bool empty() const noexcept
  {
    return !N;
  }

  [[nodiscard]] constexpr size_type size() const noexcept
  {
    return N;
  }

  [[nodiscard]] constexpr size_type max_size() const noexcept
  {
    return this->size();
  }

};

} /* End namespace container */

} /* End namespace nop_details */

#endif /* End nop_details array_base header file */

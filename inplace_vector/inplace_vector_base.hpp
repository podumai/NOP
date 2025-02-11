#ifndef NOP_INPLACE_VECTOR_BASE_HPP /* Begin inplace vector base header file */
#define NOP_INPLACE_VECTOR_BASE_HPP 1UL

#pragma once

#include <cstdint>

namespace nop /* Begin namespace nop */
{

namespace details /* Begin namespace details */
{

template<typename T, std::size_t N>
class inplace_vector_base
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
  [[nodiscard]] static size_type capacity() noexcept
  {
    return N;
  }

  [[nodiscard]] static size_type max_size() noexcept
  {
    return N;
  }

  static void shrink_to_fit() noexcept
  {
    /* Empty */
  }

  static void reserve(size_type size)
  {
    if (size > N)
    {
      throw std::bad_alloc{};
    }
  }

};

} /* End namespace details */

} /* End namespace nop */

#endif /* End inplace vector base header file */

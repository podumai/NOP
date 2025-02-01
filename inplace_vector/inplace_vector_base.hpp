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
  using const_reference  = const T&;
  using pointer         = T*;
  using const_pointer   = const T*;

 public:
  [[nodiscard]] size_type capacity() const noexcept
  {
    return N;
  }

  [[nodiscard]] size_type max_size() const noexcept
  {
    return N;
  }

  void shrink_to_fit() const noexcept
  {
    /* Empty */
  }

  void reserve() const noexcept
  {
    /* Empty */
  }

};

} /* End namespace details */

} /* End namespace nop */

#endif /* End inplace vector base header file */

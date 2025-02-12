#ifndef NOP_INPLACE_STACK_BASE_HPP /* Begin inplace stack base header file */
#define NOP_INPLACE_STACK_BASE_HPP 1UL

#pragma once

#include <cstdint>

namespace nop /* Begin namespace nop */
{

namespace details /* Begin namespace details */
{

template<typename T, std::size_t N>
class inplace_stack_base
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
  [[nodiscard]] constexpr static size_type max_size() noexcept
  {
    return N;
  }

};

} /* End namespace details */

} /* End namespace nop */

#endif /* End inplace stack base header file */

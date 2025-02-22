#ifndef NOP_DETAILS_ARRAY_BASE_HPP /* Begin __nop_details array_base header file */
#define NOP_DETAILS_ARRAY_BASE_HPP 1UL

#pragma once

#ifndef NOP_DETAILS_ARRAY_IMPL_HPP
  #error "This header file cannot be included explicitly. Use array.hpp instead."
#endif

#ifndef func
  #define func auto
#endif

#include <iterator>  /* std::input_iterator, std::reverse_iterator, std::make_reverse_iterator */
#include <exception> /* std::exception */
#include <cstdint>   /* std::size_t, std::ptrdiff_t */

namespace __nop_details /* Begin namespace __nop_details */
{

namespace container /* Begin namespace container */
{

struct array_overflow : std::exception
{
  [[nodiscard]] constexpr func what() const noexcept -> const char*
  {
    return "invalid number of elements -> array_overflow";
  }
};

template<
         typename    T,
         std::size_t N
        >
class array_base
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
  [[nodiscard]] constexpr func empty() const noexcept -> bool
  {
    return !N;
  }

  [[nodiscard]] constexpr func size() const noexcept -> size_type
  {
    return N;
  }

  [[nodiscard]] constexpr func max_size() const noexcept -> size_type
  {
    return this->size();
  }

};

} /* End namespace container */

} /* End namespace __nop_details */

#endif /* End __nop_details array_base header file */

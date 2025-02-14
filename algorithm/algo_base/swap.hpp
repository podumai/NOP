#ifndef NOP_ALGORITHM_SWAP_HPP /* Begin algorithm swap header file */
#define NOP_ALGORITHM_SWAP_HPP 1UL

#pragma once

#include <concepts>
#include <type_traits>

namespace nop /* Begin namespace nop */
{

namespace details /* Begin namespace details */
{

template<typename T>
concept valid_swap_type = std::copy_constructible<T> ||
                          std::move_constructible<T>;

} /* End namespace details */

namespace algorithm /* Begin namespace algorithm */
{

template<nop::details::valid_swap_type T>
constexpr void swap(T& value1, T& value2) noexcept((std::is_nothrow_copy_constructible_v<T> &&
                                                    std::is_nothrow_copy_assignable_v<T>)   ||
                                                   (std::is_nothrow_move_constructible_v<T> &&
                                                    std::is_nothrow_move_assignable_v<T>))
{
  if constexpr (std::is_move_constructible_v<T>)
  {
    T temp_value(std::move(value1));
    value1 = std::move(value2);
    value2 = std::move(temp_value);
  }
  else
  {
    T temp_value(value1);
    value1 = value2;
    value2 = temp_value;
  }
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm swap header file */

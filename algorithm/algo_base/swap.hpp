#ifndef NOP_ALGORITHM_BASE_SWAP_HPP /* Begin nop::algorithm::swap header file */
#define NOP_ALGORITHM_BASE_SWAP_HPP 1UL

#pragma once

#include <concepts> /* std::movable<T>, std::copyable<T> */
#include <type_traits>

#include "base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace algorithm /* Begin namespace algorithm */
{

template<typename T>
concept valid_swap_type = std::copyable<T> || std::movable<T>;

} /* End namespace algorithm */

} /* End namespace __nop_details */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<__nop_details::algorithm::valid_swap_type T>
constexpr func swap(T& value1, T& value2) noexcept((std::is_nothrow_copy_constructible_v<T> &&
                                                    std::is_nothrow_copy_assignable_v<T>)   ||
                                                   (std::is_nothrow_move_constructible_v<T> &&
                                                    std::is_nothrow_move_assignable_v<T>)) -> void
{
  if constexpr (std::is_move_constructible_v<T> && std::is_move_assignable_v<T>)
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

#endif /* End nop::algorithm::swap header file */
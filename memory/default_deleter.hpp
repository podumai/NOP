#ifndef NOP_MEMORY_DEFAULT_DELETER_HPP /* Begin nop::memory::default_deleter header file */
#define NOP_MEMORY_DEFAULT_DELETER_HPP 1UL

#pragma once

#include <concepts>    /* std::destructible<T> */
#include <type_traits> /* std::remove_extent_t<T> */

#include "NOP/base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace memory /* Begin namespace memory */
{

template<typename T>
concept valid_default_deleter_t = std::destructible<std::remove_extent_t<T>> &&
                                  (sizeof(std::remove_extent_t<T>) > 0UL);

} /* End namespace memory */

} /* End namespace __nop_details */

namespace nop /* Begin namespace nop */
{

namespace memory /* Begin namespace memory */
{

template<__nop_details::memory::valid_default_deleter_t T>
struct default_deleter
{
  constexpr func operator()(T* ptr) const noexcept -> void
  {
    delete ptr;
  }
};

template<__nop_details::memory::valid_default_deleter_t T>
struct default_deleter<T[]>
{
  constexpr func operator()(T* ptr) const noexcept -> void
  {
    delete[] ptr;
  }
};

} /* End namespace memory */

} /* End namespace nop */

#endif /* End nop::memory::default_deleter header file */

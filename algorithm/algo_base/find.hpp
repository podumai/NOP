#ifndef NOP_ALGORITHM_BASE_FIND_HPP /*Begin nop::algorithm::find header file */
#define NOP_ALGORITHM_BASE_FIND_HPP 1UL

#pragma once

#include <iterator> /* std::input_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         typename T = std::iterator_traits<InIterator>::value_type
        >
[[nodiscard]] constexpr func find(InIterator begin,
                                  InIterator end,
                                  const T& value) noexcept -> InIterator
{
  [[likely]]
  while (!(begin == end) && *begin != value)
  {
    ++begin;
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::find header file */
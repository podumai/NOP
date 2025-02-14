#ifndef NOP_ALGORITHM_FIND_HPP /*Begin algorithm find header file */
#define NOP_ALGORITHM_FIND_HPP 1UL

#pragma once

#include <iterator>

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         typename T = std::iterator_traits<InIterator>::value_type
        >
[[nodiscard]] constexpr InIterator find(InIterator begin, InIterator end, const T& value) noexcept
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

#endif /* End algorithm find header file */

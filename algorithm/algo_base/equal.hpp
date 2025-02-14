#ifndef NOP_ALGORITHM_EQUAL_HPP /* Begin algorithm equal header file */
#define NOP_ALOGRITHM_EQUAL_HPP 1UL

#pragma once

#include <iterator>

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace nop */
{

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2
        >
[[nodiscard]] constexpr bool equal(InIterator1 begin1,
                                   InIterator1 end1,
                                   InIterator2 begin2) noexcept
{
  [[likely]]
  while (!(begin1 == end1) && *begin1++ == *begin2++)
  {
    /* Empty */
  }

  return begin1 == end1;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm equal header file */

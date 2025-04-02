#ifndef NOP_ALGORITHM_BASE_EQUAL_HPP /* Begin nop::algorithm:::equal header file */
#define NOP_ALOGRITHM_BASE_EQUAL_HPP 1UL

#pragma once

#include <iterator> /* std::input_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace nop */
{

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2
        >
[[nodiscard]] constexpr func equal(InIterator1 begin1,
                                   InIterator1 end1,
                                   InIterator2 begin2) noexcept(noexcept(*begin1++ == *begin2++)) -> bool
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

#endif /* End nop::algorithm::equal header file */
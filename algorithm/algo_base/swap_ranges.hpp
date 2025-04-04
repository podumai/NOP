#ifndef NOP_ALGORITHM_BASE_SWAP_RANGES_HPP /* Begin nop::algorithm::swap_ranges header file */
#define NOP_ALGORITHM_BASE_SWAP_RANGES_HPP 1UL

#pragma once

#include <iterator> /* std::forward_iterator<T> */

#include "swap.hpp"
#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator1,
         std::forward_iterator FwdIterator2
        >
constexpr func swap_ranges(FwdIterator1 begin1,
                           FwdIterator1 end1,
                           FwdIterator2 begin2) noexcept(noexcept(nop::algorithm::swap(*begin1++, *begin2++))) -> FwdIterator2
{
  [[likely]]
  while (!(begin1 == end1))
  {
    nop::algorithm::swap(*begin1++, *begin2++);
  }

  return begin2;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::swap_ranges header file */
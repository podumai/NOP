#ifndef NOP_ALGORITHM_BASE_ITER_SWAP_HPP /* Begin nop::algorithm::iter_swap header file */
#define NOP_ALGORITHM_BASE_ITER_SWAP_HPP 1UL

#pragma once

#include <concepts> /* std::forward_iterator<T> */

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
constexpr func iter_swap(FwdIterator1 iter1, FwdIterator2 iter2) noexcept(noexcept(nop::algorithm::swap(*iter1, *iter2))) -> void
{
  nop::algorithm::swap(*iter1, *iter2);
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::iter_swap header file */
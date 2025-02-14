#ifndef NOP_ALGORITHM_ITER_SWAP_HPP /* Begin algorithm iter_swap header file */
#define NOP_ALGORITHM_ITER_SWAP_HPP 1UL

#pragma once

#include <concepts>

#include "swap.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator1,
         std::forward_iterator FwdIterator2
        >
constexpr void iter_swap(FwdIterator1 iter1, FwdIterator2 iter2) noexcept(noexcept(nop::algorithm::swap(*iter1, *iter2)))
{
  nop::algorithm::swap(*iter1, *iter2);
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm iter_swap header file */

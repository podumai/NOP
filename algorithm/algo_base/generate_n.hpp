#ifndef NOP_ALGORITHM_BASE_GENERATE_N_HPP /* Begin nop::algorithm::generate_n header file */
#define NOP_ALGORITHM_BASE_GENERATE_N_HPP 1UL

#pragma once

#include <concepts> /* std::invocable<F, Args&&...>, std::integral<T> */
#include <iterator> /* std::forward_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator,
         std::integral Size,
         std::invocable<> Generator
        >
constexpr func generate_n(FwdIterator begin,
                          Size n,
                          Generator g) noexcept(noexcept(*begin++ = g())) -> FwdIterator
{
  while (n--)
  {
    *begin++ = g();
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::generate_n header file */
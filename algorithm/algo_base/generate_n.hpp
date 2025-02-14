#ifndef NOP_ALGORITHM_GENERATE_N_HPP /* Begin algorithm generate_n header file */
#define NOP_ALGORITHM_GENERATE_N_HPP 1UL

#pragma once

#include <concepts> /* std::invocable<F, Args&&...>, std::integral<T> */
#include <iterator> /* std::forward_iterator<T> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator,
         std::integral         Size,
         std::invocable<>      Generator
        >
constexpr FwdIterator generate_n(FwdIterator begin,
                                 Size n,
                                 Generator g) noexcept(noexcept(*begin++ = g()))
{
  while (n--)
  {
    *begin++ = g();
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm generate_n header file */

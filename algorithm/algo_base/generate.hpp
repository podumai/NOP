#ifndef NOP_ALGORITHM_BASE_GENERATE_HPP /* Begin nop::algorithm::generate header file */
#define NOP_ALGORITHM_BASE_GENERATE_HPP 1UL

#pragma once

#include <concepts> /* std::invocable<F, Args&&...> */
#include <iterator> /* std::forward_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator,
         std::invocable<> Generator
        >
constexpr func generate(FwdIterator begin,
                        FwdIterator end,
                        Generator g) noexcept(noexcept(*begin++ = g())) -> FwdIterator
{
  [[likely]]
  while (!(begin == end))
  {
    *begin++ = g();
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::generate header file */
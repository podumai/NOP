#ifndef NOP_ALGORITHM_BASE_FOR_EACH_N_HPP /* Begin nop::algorithm::for_each_n header file */
#define NOP_ALGORITHM_BASE_FOR_EACH_N_HPP 1UL

#pragma once

#include <concepts> /* std::integral<T>, std::invocable<F, Args&&...> */
#include <iterator> /* std::input_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         std::integral Size,
         std::invocable<typename std::iterator_traits<InIterator>::reference> UnaryFunc
        >
constexpr func for_each_n(InIterator begin,
                          Size n,
                          UnaryFunc f) noexcept(noexcept(f(*begin++))) -> UnaryFunc
{
  [[likely]]
  while (n--)
  {
    f(*begin++);
  }

  return f;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::for_each_n header file */
#ifndef NOP_ALGORITHM_BASE_FOR_EACH_HPP /* Begin nop::algorithm::for_each header file */
#define NOP_ALGORITHM_BASE_FOR_EACH_HPP 1UL

#pragma once

#include <concepts> /* std::invocable<F, Args&&...> */
#include <iterator> /* std::input_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         std::invocable<typename std::iterator_traits<InIterator>::reference> UnaryFunc
        >
constexpr func for_each(InIterator begin,
                        InIterator end,
                        UnaryFunc f) noexcept(noexcept(f(*begin++))) -> UnaryFunc
{
  [[likely]]
  while (!(begin == end))
  {
    f(*begin++);
  }

  return f;
}


} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::for_each header file */
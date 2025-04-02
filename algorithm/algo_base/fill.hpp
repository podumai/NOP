#ifndef NOP_ALGORITHM_BASE_FILL_HPP /* Begin nop::algorithm::fill header file */
#define NOP_ALGORITHM_BASE_FILL_HPP 1UL

#pragma once

#include <iterator> /* std::forward_iterator<T> */

#include "base/func_keyword.hpp"

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator,
         typename T = typename std::iterator_traits<FwdIterator>::value_type
        >
constexpr func fill(FwdIterator begin,
                    FwdIterator end,
                    const T& value) noexcept(noexcept(*begin++ = value)) -> void
{
  [[likely]]
  while (!(begin == end))
  {
    *begin++ = value;
  }
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::fill header file */
#ifndef NOP_ALGORITHM_FILL_HPP /* Begin algorithm fill header file */
#define NOP_ALGORITHM_FILL_HPP 1UL

#pragma once

#include <iterator>

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator,
         typename T = typename std::iterator_traits<FwdIterator>::value_type
        >
constexpr void fill(FwdIterator begin,
                    FwdIterator end,
                    const T&    value) noexcept(noexcept(*begin++ = value))
{
  [[likely]]
  while (!(begin == end))
  {
    *begin++ = value;
  }
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm fill header file */

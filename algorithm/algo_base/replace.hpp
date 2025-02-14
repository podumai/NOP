#ifndef NOP_ALGORITHM_REPLACE_HPP /* Begin algorithm replace header file */
#define NOP_ALGORITHM_REPLACE_HPP 1UL

#pragma once

#include <iterator> /* std::forward_iterator<T> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::forward_iterator FwdIterator,
         typename T = typename std::iterator_traits<FwdIterator>::value_type
        >
constexpr FwdIterator replace(FwdIterator begin,
                              FwdIterator end,
                              const T&    old_value,
                              const T&    new_value) noexcept(noexcept(*begin = new_value))
{
  [[likely]]
  while (!(begin == end))
  {
    if (*begin == old_value)
    {
      *begin = new_value;
    }

    ++begin;
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End algorithm replace header file */

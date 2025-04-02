#ifndef NOP_ALGORITHM_BASE_COUNT_HPP /* Begin nop::algorithm::count header file */
#define NOP_ALGORITHM_BASE_COUNT_HPP 1UL

#pragma once

#include <iterator> /* std::input_iterator<T> */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator,
         typename T = typename std::iterator_traits<InIterator>::value_type
        >
[[nodiscard]] constexpr func count(InIterator begin,
                                   InIterator end,
                                   const T& value) noexcept(noexcept(++c)) -> typename std::iterator_traits<InIterator>::difference_type
{
  typename std::iterator_traits<InIterator>::difference_type c{};

  [[likely]]
  while (!(begin == end))
  {
    if (*begin++ == value)
    {
      ++c;
    }
  }

  return c;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::count header file */
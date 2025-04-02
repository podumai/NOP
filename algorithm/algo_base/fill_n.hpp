#ifndef NOP_ALGORITHM_BASE_FILL_N_HPP /* Begin nop::algorithm::fill_n header file */
#define NOP_ALGORITHM_BASE_FILL_N_HPP 1UL

#pragma once

#include <concepts> /* std::integral<T> */
#include <iterator> /* std::output_iterator<T> */

#include "base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace algorithm /* Begin namespace algorithm */
{

template<typename T>
concept valid_fill_n_iter_type = std::output_iterator<T, typename std::iterator_traits<T>::value_type>;

} /* End namespace algorihtm */

} /* End namespace __nop_details */

namespace nop /* Being namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         __nop_details::algorithm::valid_fill_n_iter_type OutIterator,
         std::integral Size,
         typename T = typename std::iterator_traits<OutIterator>::value_type
        >
constexpr func fill_n(OutIterator begin,
                      Size n,
                      const T& value) noexcept(noexcept(*begin++ = value)) -> OutIterator
{
  [[likely]]
  while (n--)
  {
    *begin++ = value;
  }

  return begin;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::fill_n header file */
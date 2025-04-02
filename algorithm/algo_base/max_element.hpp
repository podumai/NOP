#ifndef NOP_ALGORITHM_BASE_MAX_ELEMENT_HPP /* Begin nop::algorithm::max_element header file */
#define NOP_ALGORITHM_BASE_MAX_ELEMENT_HPP 1UL

#pragma once

#include <concepts> /* std::same_as<T, U> */
#include <iterator> /* std::forward_iterator<T> */
#include <type_traits>

#include "base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace algorithm /* Begin namespace algorithm */
{

template<typename F, typename... Args>
concept valid_max_element_cmp_func = std::same_as<std::invoke_result_t<F, Args...>, bool>;

} /* End namespace algorithm */

} /* End namespace __nop_details */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<std::forward_iterator FwdIterator>
[[nodiscard]] constexpr func max_element(
                                 FwdIterator begin,
                                 FwdIterator end) noexcept -> FwdIterator
{
  FwdIterator result{begin};

  [[likely]]
  while (!(begin == end))
  {
    if (*result < *++begin)
    {
      result = begin;
    }
  }

  return result;
}

template<
         std::forward_iterator FwdIterator,
         __nop_details::algorithm::valid_max_element_cmp_func<
                                                              const typename std::iterator_traits<FwdIterator>::reference,
                                                              const typename std::iterator_traits<FwdIterator>::reference
                                                             > Cmp_func
        >
[[nodiscard]] constexpr func max_element(
                                 FwdIterator begin,
                                 FwdIterator end,
                                 Cmp_func    cmp) noexcept(noexcept(cmp(*begin, *end))) -> FwdIterator
{
  FwdIterator result{begin};

  [[likely]]
  while (!(begin == end))
  {
    if (cmp(*result, *++begin))
    {
      result = begin;
    }
  }

  return result;
}

} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::max_element header file */
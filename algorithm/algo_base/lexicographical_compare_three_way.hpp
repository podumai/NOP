#ifndef NOP_ALGORITHM_BASE_LEXICOGRAPHICAL_COMPARE_THREE_WAY_HPP /* Begin nop::algorithm::lexicographical_compare_three_way header file */
#define NOP_ALGORITHM_BASE_LEXICOGRAPHICAL_COMPARE_THREE_WAY_HPP 1UL

#pragma once

#include <iterator>    /* std::input_iterator<Iter> */
#include <compare>     /* operator <=> overload */
#include <concepts>    /* std::same_as<T1, T2> */
#include <type_traits> /* std::invoke_result_t<F, Args...> */

#include "base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace algorithm /* Begin namespace algorithm */
{

template<typename F, typename... Args>
concept valid_three_way_cmp_func = std::same_as<std::invoke_result_t<F, Args...>, std::strong_ordering>  ||
                                   std::same_as<std::invoke_result_t<F, Args...>, std::partial_ordering> ||
                                   std::same_as<std::invoke_result_t<F, Args...>, std::weak_ordering>;

} /* End namespace algorithm */

} /* End namespace __nop_detials */

namespace nop /* Begin namespace nop */
{

namespace algorithm /* Begin namespace algorithm */
{

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2
        >
[[nodiscard]] constexpr func lexicographical_compare_three_way(
                                 InIterator1 begin1,
                                 InIterator1 end1,
                                 InIterator2 begin2,
                                 InIterator2 end2) noexcept(noexcept(*begin1 <=> *begin2)) -> decltype(*begin1 <=> *begin2)
{
  [[likely]]
  while (!(begin1 == end1) && !(begin2 == end2))
  {
    if (auto result{*begin1++ <=> *begin2++}; !(result == 0))
    {
      break;
    }
  }

  using comp_t = decltype(*begin1 <=> *begin2);

  if constexpr (std::same_as<comp_t, std::strong_ordering>)
  {
    return comp_t::equal;
  }
  else
  {
    return comp_t::equivalent;
  }
}

template<
         std::input_iterator InIterator1,
         std::input_iterator InIterator2,
         __nop_details::algorithm::valid_three_way_cmp_func<
                                                            const typename std::iterator_traits<InIterator1>::reference,
                                                            const typename std::iterator_traits<InIterator2>::reference
                                                           > Cmp_func
        >
[[nodiscard]] constexpr func lexicographical_compare_three_way(
                                 InIterator1 begin1,
                                 InIterator1 end1,
                                 InIterator2 begin2,
                                 InIterator2 end2,
                                 Cmp_func cmp) noexcept(noexcept(cmp(*begin1++, *begin2++))) -> decltype(cmp(*begin1, *begin2))
{
  [[likely]]
  while (!(begin1 == end1) && !(begin2 == end2))
  {
    if (auto result{cmp(*begin1++, *begin2++)}; !(result == 0))
    {
      return result;
    }
  }

  using cmp_t = decltype(cmp(*begin1, *begin2));

  if constexpr (std::same_as<cmp_t, std::strong_ordering>)
  {
    return cmp_t::equal;
  }
  else
  {
    return cmp_t::equivalent;
  }
}


} /* End namespace algorithm */

} /* End namespace nop */

#endif /* End nop::algorithm::lexicographical_compare_three_way header file */
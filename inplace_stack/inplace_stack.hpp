#ifndef NOP_INPLACE_STACK_HPP /* Begin inplace stack header file */
#define NOP_INPLACE_STACK_HPP 1UL

#pragma once

#include "inplace_stack_impl.hpp"

namespace nop /* Begin namespace nop */
{

template<
         nop::details::valid_inplace_stack_t T,
         std::size_t N
        >
class inplace_stack : protected nop::details::inplace_stack_impl<T, N>
{
 private:
  using base = nop::details::inplace_stack_impl<T, N>;

 public:
  using value_type      = typename base::value_type;
  using size_type       = typename base::size_type;
  using difference_type = typename base::difference_type;
  using reference       = typename base::reference;
  using const_reference = typename base::const_reference;
  using pointer         = typename base::pointer;
  using const_pointer   = typename base::const_pointer;

 public:
  inplace_stack() noexcept
    : base()
  {
    /* Empty */
  }

  inplace_stack(size_type n)
  requires nop::details::valid_storage_size<N>
    : base(n)
  {
    /* Empty */
  }

  inplace_stack(size_type n, const value_type& value)
  requires nop::details::valid_storage_size<N>
    : base(n, value)
  {
    /* Empty */
  }

  inplace_stack(std::initializer_list<value_type> ilist)
  requires nop::details::valid_storage_size<N>
    : base(ilist)
  {
    /* Empty */
  }

  inplace_stack(const inplace_stack& other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
    : base(static_cast<base&>(other))
  {
    /* Empty */
  }

  inplace_stack(inplace_stack&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
    : base(static_cast<base&&>(other))
  {
    /* Empty */
  }

  [[nodiscard]] base::iterator begin() noexcept
  {
    return base::begin();
  }

  [[nodiscard]] base::const_iterator cbegin() const noexcept
  {
    return base::cbegin();
  }

  [[nodiscard]] base::iterator end() noexcept
  {
    return base::end();
  }

  [[nodiscard]] base::const_iterator cend() const noexcept
  {
    return base::cend();
  }

  reference push(const value_type& value)
  requires nop::details::valid_storage_size<N>
  {
    return base::push(value);
  }

  reference push(value_type&& value)
  requires nop::details::valid_storage_size<N>
  {
    return base::push(std::move(value));
  }

  template<typename InIterator>
  reference push(InIterator begin, InIterator end)
  requires nop::details::valid_storage_size<N>
  {
    return base::push(begin, end);
  }

  reference push(std::initializer_list<value_type> ilist)
  requires nop::details::valid_storage_size<N>
  {
    return base::push(ilist.begin(), ilist.end());
  }

  pointer try_push(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires nop::details::valid_storage_size<N>
  {
    return base::try_push(value);
  }

  pointer try_push(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires nop::details::valid_storage_size<N>
  {
    return base::try_push(std::move(value));
  }

  template<typename InIterator>
  pointer try_push(InIterator begin, InIterator end)
  requires nop::details::valid_storage_size<N>
  {
    return base::try_push(begin, end);
  }

  pointer try_push(std::initializer_list<value_type> ilist)
  requires nop::details::valid_storage_size<N>
  {
    return base::try_push(ilist.begin(), ilist.end());
  }

  reference unchecked_push(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires nop::details::valid_storage_size<N>
  {
    return base::unchecked_push(value);
  }

  reference unchecked_push(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires nop::details::valid_storage_size<N>
  {
    return base::unchecked_push(std::move(value));
  }

  template<typename InIterator>
  reference unchecked_push(InIterator begin, InIterator end)
  requires nop::details::valid_storage_size<N>
  {
    return base::unchecked_push(begin, end);
  }

  reference unchecked_push(std::initializer_list<value_type> ilist)
  requires nop::details::valid_storage_size<N>
  {
    return base::unchecked_push(ilist.begin(), ilist.end());
  }

  void pop()
  requires nop::details::valid_storage_size<N>
  {
    base::pop();
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return base::empty();
  }

  [[nodiscard]] size_type size() const noexcept
  {
    return base::size();
  }

  [[nodiscard]] reference top() noexcept
  requires nop::details::valid_storage_size<N>
  {
    return base::top();
  }

  [[nodiscard]] const_reference top() const noexcept
  requires nop::details::valid_storage_size<N>
  {
    return base::top();
  }

  void clear() noexcept
  requires nop::details::valid_storage_size<N>
  {
    base::clear();
  }

  [[nodiscard]] size_type max_size() const noexcept
  {
    return base::max_size();
  }

  [[nodiscard]] bool operator==(const inplace_stack& other) const noexcept
  {
    return std::equal(base::cbegin(), base::cend(), other.cbegin());
  }

  [[nodiscard]] bool operator!=(const inplace_stack& other) const noexcept
  {
    return !(*this == other);
  }

};

} /* End namespace nop */

#endif /* End inplace stack header file */

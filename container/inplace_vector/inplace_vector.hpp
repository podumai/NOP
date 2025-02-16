#ifndef NOP_CONTAINER_INPLACE_VECTOR_HPP /* Begin container inplace_vector header file */
#define NOP_CONTAINER_INPLACE_VECTOR_HPP 1UL

#pragma once

#include <algorithm> /* std::copy, std::move, std::lexicographical_compare_three_way */

#include "inplace_vector_impl.hpp"

namespace nop /* Begin namespace nop */
{

namespace container /* Begin namespace container */
{

template<
         typename T,
         std::size_t N
        >
class inplace_vector : public nop::details::inplace_vector_impl<T, N>
{
 private:
  using base = typename nop::details::inplace_vector_impl<T, N>;

 public:
  using value_type             = typename base::value_type;
  using size_type              = typename base::size_type;
  using difference_type        = typename base::difference_type;
  using reference              = typename base::reference;
  using const_reference        = typename base::const_reference;
  using pointer                = typename base::pointer;
  using const_pointer          = typename base::const_pointer;
  using iterator               = typename base::iterator;
  using const_iterator         = typename base::const_iterator;
  using reverse_iterator       = typename base::reverse_iterator;
  using const_reverse_iterator = typename base::const_reverse_iterator;

 public:
  inplace_vector() noexcept
    : base()
  {
    /* Empty */
  }

  explicit inplace_vector(size_type n)
    : base(n)
  {
    /* Empty */
  }

  inplace_vector(size_type n, const value_type& value)
    : base(n, value)
  {
    /* Empty */
  }

  template<typename InIterator>
  inplace_vector(InIterator begin, InIterator end)
    : base(begin, end)
  {
    /* Empty */
  }

  inplace_vector(std::initializer_list<value_type> ilist)
    : base(ilist.begin(), ilist.end())
  {
    /* Empty */
  }

  inplace_vector(const inplace_vector& other) noexcept(noexcept(base(static_cast<const base&>(other))))
    : base(static_cast<const base&>(other))
  {
    /* Empty */
  }

  inplace_vector(inplace_vector&& other) noexcept(noexcept(base(static_cast<base&&>(other))))
    : base(static_cast<base&&>(other))
  {
    /* Empty */
  }

  [[nodiscard]] iterator begin() noexcept
  {
    return base::begin();
  }

  [[nodiscard]] const_iterator cbegin() const noexcept
  {
    return base::cbegin();
  }

  [[nodiscard]] reverse_iterator rbegin() noexcept
  {
    return base::rbegin();
  }

  [[nodiscard]] const_reverse_iterator crbegin() const noexcept
  {
    return base::crbegin();
  }

  [[nodiscard]] iterator end() noexcept
  {
    return base::end();
  }

  [[nodiscard]] const_iterator cend() const noexcept
  {
    return base::cend();
  }

  [[nodiscard]] reverse_iterator rend() noexcept
  {
    return base::rend();
  }

  [[nodiscard]] const_reverse_iterator crend() const noexcept
  {
    return base::crend();
  }

  [[nodiscard]] pointer data() noexcept
  {
    return base::data();
  }

  [[nodiscard]] const_pointer data() const noexcept
  {
    return base::data();
  }

  [[nodiscard]] size_type size() const noexcept
  {
    return base::size();
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return base::empty();
  }

  void clear() noexcept
  {
    base::clear();
  }

  reference push_back(const value_type& value)
  {
    return base::push_back(value);
  }

  reference push_back(value_type&& value)
  {
    return base::push_back(std::move(value));
  }

  pointer try_push_back(const value_type& value) noexcept(noexcept(base::try_push_back(value)))
  {
    return base::try_push_back(value);
  }

  pointer try_push_back(value_type&& value) noexcept(noexcept(base::try_push_back(std::move(value))))
  {
    return base::try_push_back(std::move(value));
  }

  reference unchecked_push_back(const value_type& value) noexcept(noexcept(base::unchecked_push_back(value)))
  {
    return base::unchecked_push_back(value);
  }

  reference unchecked_push_back(value_type&& value) noexcept(noexcept(base::unchecked_push_back(std::move(value))))
  {
    return base::unchecked_push_back(std::move(value));
  }

  template<typename... Args>
  iterator emplace(const_iterator position, Args&&... args)
  {
    return base::emplace(position, std::forward<Args>(args)...);
  }

  template<typename... Args>
  reference emplace_back(Args&&... args)
  {
    return base::emplace_back(std::forward<Args>(args)...);
  }

  template<typename... Args>
  pointer try_emplace_back(Args&&... args)
  {
    return base::try_emplace_back(std::forward<Args>(args)...);
  }

  template<typename... Args>
  reference unchecked_emplace_back(Args&&... args)
  {
    return base::unchecked_emplace_back(std::forward<Args>(args)...);
  }

  void pop_back() noexcept
  {
    base::pop_back();
  }

  [[nodiscard]] reference operator[](size_type index) noexcept
  {
    return base::operator[](index);
  }

  [[nodiscard]] const_reference operator[](size_type index) const noexcept
  {
    return base::operator[](index);
  }

  [[nodiscard]] reference at(size_type index)
  {
    return base::at(index);
  }

  [[nodiscard]] const_reference at(size_type index) const
  {
    return base::at(index);
  }

  [[nodiscard]] reference front() noexcept
  {
    return base::front();
  }

  [[nodiscard]] const_reference front() const noexcept
  {
    return base::front();
  }

  [[nodiscard]] reference back() noexcept
  {
    return base::back();
  }

  [[nodiscard]] const_reference back() const noexcept
  {
    return base::back();
  }

  template<typename InIterator>
  void assign(InIterator begin, InIterator end)
  {
    base::assign(begin, end);
  }

  void assign(std::initializer_list<value_type> ilist)
  {
    base::assign(ilist.begin(), ilist.end());
  }

  void assign(size_type n, const value_type& value)
  {
    base::assign(n, value);
  }

  iterator insert(const_iterator position, const value_type& value)
  {
    return base::insert(position, value);
  }

  iterator insert(const_iterator position, value_type&& value)
  {
    return base::insert(position, std::move(value));
  }

  void swap(inplace_vector& other) noexcept(noexcept(base::swap(static_cast<base&>(other))))
  {
    base::swap(static_cast<base&>(other));
  }

  iterator erase(const_iterator position) noexcept(noexcept(base::erase(position)))
  {
    return base::erase(position);
  }

  iterator erase(const_iterator begin, const_iterator end) noexcept(noexcept(base::erase(begin, end)))
  {
    return base::erase(begin, end);
  }

  void resize(size_type n)
  {
    base::resize(n);
  }

  void resize(size_type n, const value_type& value)
  {
    base::resize(n, value);
  }

  inplace_vector& operator=(std::initializer_list<value_type> ilist)
  {
    base::assign(ilist.begin(), ilist.end());
    return *this;
  }

  inplace_vector& operator=(const inplace_vector& other) noexcept(noexcept(static_cast<inplace_vector&>(static_cast<base&>(*this) = static_cast<const base&>(other))))
  {
    return static_cast<inplace_vector&>(static_cast<base&>(*this) = static_cast<const base&>(other));
  }

  inplace_vector& operator=(inplace_vector&& other) noexcept(noexcept(static_cast<inplace_vector&>(static_cast<base&>(*this) = static_cast<base&&>(other))))
  {
    return static_cast<inplace_vector&>(static_cast<base&>(*this) = static_cast<base&&>(other));
  }

};

} /* End namespace container */

} /* End namespace nop */

namespace std /* Begin namespace std */
{

template<
         typename    T,
         std::size_t N
        >
constexpr void swap(nop::container::inplace_vector<T, N>& lhs,
                    nop::container::inplace_vector<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}

} /* End namespace std */

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] bool operator==(const nop::container::inplace_vector<T, N>& lhs,
                              const nop::container::inplace_vector<T, N>& rhs) noexcept
{
  return std::equal(lhs.cbegin(),
                    lhs.cend(),
                    rhs.cbegin(),
                    rhs.cend());
}

template<
         typename    T,
         std::size_t N
        >
[[nodiscard]] auto operator<=>(const nop::container::inplace_vector<T, N>& lhs,
                               const nop::container::inplace_vector<T, N>& rhs) noexcept
{
  return std::lexicographical_compare_three_way(lhs.cbegin(),
                                                lhs.cend(),
                                                rhs.cbegin(),
                                                rhs.cend());
}


#endif /* End container inplace_vector header file */

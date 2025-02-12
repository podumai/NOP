#ifndef NOP_INPLACE_STACK_IMPL_HPP /* Begin inplace stack impl header file */
#define NOP_INPLACE_STACK_IMPL_HPP 1UL

#pragma once

#include <type_traits>
#include <memory>
#include <iterator>         /* std::input_iterator */
#include <initializer_list> /* std::initializer_list */

#include "inplace_stack_base.hpp"

namespace nop /* Begin namespace nop */
{

namespace details /* Begin namespace details */
{

template<typename T>
concept copy_support = std::is_copy_constructible_v<T> &&
                       std::is_copy_assignable_v<T>;

template<typename T>
concept move_support = std::is_move_constructible_v<T> &&
                       std::is_move_assignable_v<T>;

template<typename T>
concept valid_inplace_stack_t = (nop::details::copy_support<T>  ||
                                 nop::details::move_support<T>) &&
                                 std::is_nothrow_destructible_v<T>;

template<std::size_t N>
concept valid_storage_size = N > 0UL;


template<
         nop::details::valid_inplace_stack_t T,
         std::size_t N
        >
class inplace_stack_impl : public nop::details::inplace_stack_base<T, N>
{
 private:
  using base = nop::details::inplace_stack_base<T, N>;

 public:
  using value_type      = typename base::value_type;
  using size_type       = typename base::size_type;
  using difference_type = typename base::difference_type;
  using reference       = typename base::reference;
  using const_reference = typename base::const_reference;
  using pointer         = typename base::pointer;
  using const_pointer   = typename base::const_pointer;

 public:
  class iterator
  {
   public:
    using value_type        = typename inplace_stack_impl::value_type;
    using difference_type   = typename inplace_stack_impl::difference_type;
    using reference         = typename inplace_stack_impl::reference;
    using const_reference   = typename inplace_stack_impl::const_reference;
    using pointer           = typename inplace_stack_impl::pointer;
    using const_pointer     = typename inplace_stack_impl::const_pointer;
    using iterator_category = typename std::forward_iterator_tag;

   private:
    pointer m_element{nullptr};

   public:
    iterator() noexcept = default;

    iterator(pointer p_element) noexcept
      : m_element{p_element}
    {
      /* Empty */
    }

    iterator(const iterator&) noexcept = default;
    iterator(iterator&&)      noexcept = default;

    [[nodiscard]] reference operator*() noexcept
    {
      return *this->m_element;
    }

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return *this->m_element;
    }

    [[nodiscard]] pointer operator->() noexcept
    {
      return this->m_element;
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return this->m_element;
    }

    iterator& operator++() noexcept
    {
      ++this->m_element;
      return *this;
    }

    iterator operator++(std::int32_t) noexcept
    {
      return {this->m_element++};
    }

    [[nodiscard]] bool operator==(const iterator& other) const noexcept
    {
      return this->m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const iterator& other) const noexcept
    {
      return !(*this == other);
    }

    iterator& operator=(const iterator&) noexcept = default;
    iterator& operator=(iterator&&)      noexcept = default;

  };

  class const_iterator
  {
   public:
    using value_type        = typename inplace_stack_impl::value_type;
    using difference_type   = typename inplace_stack_impl::difference_type;
    using reference         = typename inplace_stack_impl::reference;
    using const_reference   = typename inplace_stack_impl::const_reference;
    using pointer           = typename inplace_stack_impl::pointer;
    using const_pointer     = typename inplace_stack_impl::const_pointer;
    using iterator_category = typename std::forward_iterator_tag;

   private:
    pointer m_element{nullptr};

   public:
    const_iterator() noexcept = default;

    const_iterator(const_pointer p_element) noexcept
      : m_element{const_cast<pointer>(p_element)}
    {
      /* Empty */
    }

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return *this->m_element;
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return this->m_element;
    }

    const_iterator& operator++() noexcept
    {
      ++this->m_element;
      return *this;
    }

    const_iterator operator++(std::int32_t) noexcept
    {
      return {this->m_element++};
    }

    [[nodiscard]] bool operator==(const const_iterator& other) const noexcept
    {
      return this->m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const const_iterator& other) const noexcept
    {
      return !(*this == other);
    }

    const_iterator(const const_iterator&) noexcept = default;
    const_iterator(const_iterator&&)      noexcept = default;
  };

 private:
  alignas(value_type) std::conditional_t<!std::is_fundamental_v<value_type>, std::uint8_t, value_type> m_storage[!std::is_fundamental_v<value_type> ? sizeof(value_type) * N : N];
  size_type m_size{};

 private:
  pointer construct(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
  {
    return ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(value);
  }

  pointer construct(value_type&& value) noexcept(std::is_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
  {
    return ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(std::move(value));
  }

  void destroy() noexcept
  {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      (static_cast<pointer>(static_cast<void*>(this->m_storage)) + --this->m_size)->~value_type();
    }
    else
    {
      --this->m_size;
    }
  }

 protected:
  inplace_stack_impl() noexcept
  {
    /* Empty */
  }

  inplace_stack_impl(size_type n)
  requires std::is_default_constructible_v<value_type>
  {
    if (n > N)
    {
      throw std::bad_alloc{};
    }

    std::uninitialized_default_construct(static_cast<pointer>(static_cast<void*>(this->m_storage)),
                                         static_cast<pointer>(static_cast<void*>(this->m_storage)) + n);

    this->m_size = n;
  }

  inplace_stack_impl(size_type n, const value_type& value)
  requires std::is_copy_constructible_v<value_type>
  {
    if (n > N)
    {
      throw std::bad_alloc{};
    }

    std::uninitialized_fill(static_cast<pointer>(static_cast<void*>(this->m_storage)),
                            static_cast<pointer>(static_cast<void*>(this->m_storage)) + n,
                            value);

    this->m_size = n;
  }

  inplace_stack_impl(std::initializer_list<value_type> ilist)
  requires std::is_copy_constructible_v<value_type>
  {
    if (ilist.size() > N)
    {
      throw std::bad_alloc{};
    }

    std::uninitialized_copy(ilist.begin(),
                            ilist.end(),
                            static_cast<pointer>(static_cast<void*>(this->m_storage)));

    this->m_size = ilist.size();
  }

  inplace_stack_impl(const inplace_stack_impl& other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
    : m_size{other.m_size}
  {
    std::uninitialized_copy(static_cast<pointer>(static_cast<void*>(other.m_storage)),
                            static_cast<pointer>(static_cast<void*>(other.m_storage)) + other.m_size,
                            static_cast<pointer>(static_cast<void*>(this->m_storage)));
  }

  inplace_stack_impl(inplace_stack_impl&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
    : m_size{other.m_size}
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      std::uninitialized_move(static_cast<pointer>(static_cast<void*>(other.m_storage)),
                              static_cast<pointer>(static_cast<void*>(other.m_storage)) + other.m_size,
                              static_cast<pointer>(static_cast<void*>(this->m_storage)));
    }
    else
    {
      std::uninitialized_copy(other.m_storage,
                              other.m_storage + other.m_size,
                              this->m_storage);
    }

    other.clear();
  }

  ~inplace_stack_impl()
  {
    this->clear();
  }

  [[nodiscard]] iterator begin() noexcept
  {
    return {static_cast<pointer>(static_cast<void*>(this->m_storage))};
  }

  [[nodiscard]] const_iterator cbegin() const noexcept
  {
    return {static_cast<const_pointer>(static_cast<const void*>(this->m_storage))};
  }

  [[nodiscard]] iterator end() noexcept
  {
    return {static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size};
  }

  [[nodiscard]] const_iterator cend() const noexcept
  {
    return {static_cast<const_pointer>(static_cast<const void*>(this->m_storage)) + this->m_size};
  }

  reference push(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    return *this->construct(value);
  }

  reference push(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    return *this->construct(std::move(value));
  }

  template<std::input_iterator InIterator>
  reference push(InIterator begin, InIterator end)
  requires std::is_copy_constructible_v<value_type>
  {
    if (static_cast<size_type>(std::distance(begin, end)) + this->m_size > N)
    {
      throw std::bad_alloc{};
    }

    while (begin != end)
    {
      this->construct(*begin++);
    }

    return this->top();
  }

  pointer try_push(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
  {
    if (this->m_size < N)
    {
      return this->construct(value);
    }

    return nullptr;
  }

  pointer try_push(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
  {
    if (this->m_size < N)
    {
      return this->construct(std::move(value));
    }

    return nullptr;
  }

  template<std::input_iterator InIterator>
  pointer try_push(InIterator begin, InIterator end)
  requires std::is_copy_constructible_v<value_type>
  {
    while (this->m_size < N && begin != end)
    {
      this->construct(*begin++);
    }

    if (this->m_size < N)
    {
      return std::addressof(this->top());
    }
    else
    {
      return nullptr;
    }
  }

  reference unchecked_push(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
  {
    return *this->try_push(value);
  }

  reference unchecked_push(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
  {
    return *this->try_push(std::move(value));
  }

  template<std::input_iterator InIterator>
  reference unchecked_push(InIterator begin, InIterator end)
  requires std::is_copy_constructible_v<value_type>
  {
    return *this->try_push(begin, end);
  }

  void pop()
  {
    this->destroy();
  }

  [[nodiscard]] reference top() noexcept
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage))[this->m_size - 1UL];
  }

  [[nodiscard]] const_reference top() const noexcept
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage))[this->m_size - 1UL];
  }

  [[nodiscard]] size_type size() const noexcept
  {
    return this->m_size;
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return !this->m_size;
  }

  void clear() noexcept
  {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      std::destroy(static_cast<pointer>(static_cast<void*>(this->m_storage)),
                   static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size);
    }

    this->m_size = 0UL;
  }

};

template<typename T>
class inplace_stack_impl<T, 0UL> : public nop::details::inplace_stack_base<T, 0UL>
{
 private:
  using base = nop::details::inplace_stack_base<T, 0UL>;

 public:
  using value_type      = typename base::value_type;
  using size_type       = typename base::size_type;
  using difference_type = typename base::difference_type;
  using reference       = typename base::reference;
  using const_reference = typename base::const_reference;
  using pointer         = typename base::pointer;
  using const_pointer   = typename base::const_pointer;
  using iterator        = typename base::pointer;
  using const_iterator  = typename base::const_pointer;

 public:
  constexpr inplace_stack_impl()                          noexcept = default;
  constexpr inplace_stack_impl(const inplace_stack_impl&) noexcept = default;
  constexpr inplace_stack_impl(inplace_stack_impl&&)      noexcept = default;
  constexpr ~inplace_stack_impl()                                  = default;

  [[nodiscard]] constexpr iterator begin() noexcept
  {
    return nullptr;
  }

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept
  {
    return nullptr;
  }

  [[nodiscard]] constexpr iterator end() noexcept
  {
    return nullptr;
  }

  [[nodiscard]] constexpr const_iterator cend() const noexcept
  {
    return nullptr;
  }

  [[nodiscard]] constexpr size_type size() const noexcept
  {
    return 0UL;
  }

  [[nodiscard]] constexpr bool empty() const noexcept
  {
    return true;
  }

  constexpr inplace_stack_impl& operator=(const inplace_stack_impl&) noexcept = default;
  constexpr inplace_stack_impl& operator=(inplace_stack_impl&&)      noexcept = default;

  [[nodiscard]] constexpr bool operator==(const inplace_stack_impl& other) const noexcept
  {
    return true;
  }

  [[nodiscard]] constexpr bool operator!=(const inplace_stack_impl& other) const noexcept
  {
    return false;
  }

};

} /* End namespace details */

} /* End namespace nop */

#endif /* End inplace stack impl header file */

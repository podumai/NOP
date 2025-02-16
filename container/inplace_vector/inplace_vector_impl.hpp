#ifndef NOP_INPLACE_VECTOR_IMPL_HPP /* Begin inplace_vector_impl header file */
#define NOP_INPLACE_VECTOR_IMPL_HPP 1UL

#ifndef NOP_CONTAINER_INPLACE_VECTOR_HPP
  #error "This header file cannot be included explicitly. Use inplace_vector.hpp instead."
#endif

#pragma once

#include <initializer_list> /* std::initializer_list<T> */
#include <iterator>         /* std::reverse_iterator<Iterator> */
#include <memory>
#include <new>              /* std::bad_alloc */
#include <stdexcept>        /* std::out_of_range, std::invalid_argument */
#include <type_traits>

#include "inplace_vector_base.hpp"

namespace nop /* Begin namespace nop */
{

namespace details /* Begin namespace details */
{

template<typename T>
concept valid_inplace_vector_type = std::is_nothrow_destructible_v<T>;

template<std::size_t N>
concept valid_inplace_vector_size = N > 0UL;

template<typename T>
concept is_nothrow_copy_or_move_constructible = std::is_nothrow_copy_constructible_v<T> ||
                                                std::is_nothrow_move_constructible_v<T>;

template<typename T>
concept is_nothrow_copy_or_move_assignable = std::is_nothrow_copy_assignable_v<T> ||
                                             std::is_nothrow_move_assignable_v<T>;

template<
         nop::details::valid_inplace_vector_type T,
         std::size_t                             N
        >
class inplace_vector_impl : public nop::details::inplace_vector_base<T, N>
{
 private:
  using base = typename nop::details::inplace_vector_base<T, N>;

 public:
  class iterator;
  class const_iterator;

 public:
  using value_type             = typename base::value_type;
  using size_type              = typename base::size_type;
  using difference_type        = typename base::difference_type;
  using reference              = typename base::reference;
  using const_reference        = typename base::const_reference;
  using pointer                = typename base::pointer;
  using const_pointer          = typename base::const_pointer;
  using reverse_iterator       = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

 public:
  class iterator
  {
   private:
    friend inplace_vector_impl;

   public:
    using value_type        = typename inplace_vector_impl::value_type;
    using size_type         = typename inplace_vector_impl::size_type;
    using difference_type   = typename inplace_vector_impl::difference_type;
    using reference         = typename inplace_vector_impl::reference;
    using const_reference   = typename inplace_vector_impl::const_reference;
    using pointer           = typename inplace_vector_impl::pointer;
    using const_pointer     = typename inplace_vector_impl::const_pointer;
    using iterator_category = typename std::contiguous_iterator_tag;

   private:
    pointer m_element;

   public:
    iterator() noexcept
      : m_element{nullptr}
    {
      /* Empty */
    }

    iterator(pointer ptr) noexcept
      : m_element{ptr}
    {
      /* Empty */
    }

    iterator(const iterator&) noexcept = default;
    iterator(iterator&&)      noexcept = default;
    ~iterator()                        = default;

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

    [[nodiscard]] reference operator[](difference_type index) noexcept
    {
      return this->m_element[index];
    }

    [[nodiscard]] const_reference operator[](difference_type index) const noexcept
    {
      return this->m_element[index];
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

    iterator& operator--() noexcept
    {
      --this->m_element;
      return *this;
    }

    iterator operator--(std::int32_t) noexcept
    {
      return {this->m_element--};
    }

    iterator& operator+=(difference_type offset) noexcept
    {
      this->m_element += offset;
      return *this;
    }

    iterator& operator-=(difference_type offset) noexcept
    {
      this->m_element -= offset;
      return *this;
    }

    [[nodiscard]] iterator operator+(difference_type offset) const noexcept
    {
      return {this->m_element + offset};
    }

    [[nodiscard]] friend iterator operator+(difference_type offset,
                                            const iterator& iter) noexcept
    {
      return {iter.m_element + offset};
    }

    [[nodiscard]] iterator operator-(difference_type offset) const noexcept
    {
      return {this->m_element - offset};
    }

    [[nodiscard]] difference_type operator-(const iterator& other) const noexcept
    {
      return this->m_element - other.m_element;
    }

    [[nodiscard]] bool operator==(const iterator& other) const noexcept
    {
      return this->m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const iterator& other) const noexcept
    {
      return this->m_element != other.m_element;
    }

    [[nodiscard]] bool operator<(const iterator& other) const noexcept
    {
      return this->m_element < other.m_element;
    }

    [[nodiscard]] bool operator<=(const iterator& other) const noexcept
    {
      return this->m_element <= other.m_element;
    }

    [[nodiscard]] bool operator>(const iterator& other) const noexcept
    {
      return this->m_element > other.m_element;
    }

    [[nodiscard]] bool operator>=(const iterator& other) const noexcept
    {
      return this->m_element >= other.m_element;
    }

    iterator& operator=(const iterator&) noexcept = default;
    iterator& operator=(iterator&&)      noexcept = default;

  };

  class const_iterator
  {
   private:
    friend inplace_vector_impl;

   public:
    using value_type        = typename inplace_vector_impl::value_type;
    using size_type         = typename inplace_vector_impl::size_type;
    using difference_type   = typename inplace_vector_impl::difference_type;
    using reference         = typename inplace_vector_impl::reference;
    using const_reference   = typename inplace_vector_impl::const_reference;
    using pointer           = typename inplace_vector_impl::pointer;
    using const_pointer     = typename inplace_vector_impl::const_pointer;
    using iterator_category = typename std::contiguous_iterator_tag;

   private:
    pointer m_element;

   public:
    const_iterator() noexcept
      : m_element{nullptr}
    {
      /* Empty */
    }

    const_iterator(const_pointer ptr) noexcept
      : m_element{const_cast<pointer>(ptr)}
    {
      /* Empty */
    }

    const_iterator(const const_iterator& other) noexcept = default;
    const_iterator(const_iterator&& other)      noexcept = default;
    ~const_iterator()                                    = default;

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return *this->m_element;
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return this->m_element;
    }

    [[nodiscard]] const_reference operator[](difference_type index) const noexcept
    {
      return this->m_element[index];
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

    const_iterator& operator--() noexcept
    {
      --this->m_element;
      return *this;
    }

    const_iterator operator--(std::int32_t) noexcept
    {
      return {this->m_element--};
    }

    const_iterator& operator+=(difference_type offset) noexcept
    {
      this->m_element += offset;
      return *this;
    }

    [[nodiscard]] const_iterator operator+(difference_type offset) const noexcept
    {
      return {this->m_element + offset};
    }

    [[nodiscard]] friend const_iterator operator+(difference_type offset,
                                                  const const_iterator& iter) noexcept
    {
      return {offset + iter.m_element};
    }

    const_iterator& operator-=(difference_type offset) noexcept
    {
      this->m_element -= offset;
      return *this;
    }

    [[nodiscard]] const_iterator operator-(difference_type offset) const noexcept
    {
      return {this->m_element - offset};
    }

    [[nodiscard]] difference_type operator-(const const_iterator& other) const noexcept
    {
      return this->m_element - other.m_element;
    }

    [[nodiscard]] bool operator==(const const_iterator& other) const noexcept
    {
      return this->m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const const_iterator& other) const noexcept
    {
      return this->m_element != other.m_element;
    }

    [[nodiscard]] bool operator>(const const_iterator& other) const noexcept
    {
      return this->m_element > other.m_element;
    }

    [[nodiscard]] bool operator>=(const const_iterator& other) const noexcept
    {
      return this->m_element >= other.m_element;
    }

    [[nodiscard]] bool operator<(const const_iterator& other) const noexcept
    {
      return this->m_element < other.m_element;
    }

    [[nodiscard]] bool operator<=(const const_iterator& other) const noexcept
    {
      return this->m_element <= other.m_element;
    }

    const_iterator& operator=(const const_iterator&) noexcept = default;
    const_iterator& operator=(const_iterator&&)      noexcept = default;

  };

 private:
  alignas (value_type) std::conditional_t<!std::is_fundamental_v<value_type>, std::uint8_t, value_type> m_storage[!std::is_fundamental_v<value_type> ? sizeof(value_type) * N : N];
  size_type m_size{};

 private:
  constexpr void range_check(size_type index) const
  {
    if (this->m_size <= index)
    {
      throw std::out_of_range{"inplace_vector::range_check(size_type) -> index is out of range"};
    }
  }

  pointer construct(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::copy_constructible<value_type>
  {
    return ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(value);
  }

  pointer construct(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::move_constructible<value_type>
  {
    return ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(std::move(value));
  }

  template<typename... Args>
  pointer emplace_construct(Args&&... args)
  requires std::constructible_from<value_type, Args...>
  {
    return ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(std::forward<Args>(args)...);
  }

  void destroy() noexcept
  requires std::destructible<value_type>
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
  inplace_vector_impl() noexcept = default;

  inplace_vector_impl(const inplace_vector_impl& other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::copy_constructible<value_type>
    : m_size{other.m_size}
  {
    std::uninitialized_copy(static_cast<const_pointer>(static_cast<const void*>(other.m_storage)),
                            static_cast<const_pointer>(static_cast<const void*>(other.m_storage)) + other.m_size,
                            static_cast<pointer>(static_cast<void*>(this->m_storage)));
  }

  inplace_vector_impl(inplace_vector_impl&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::move_constructible<value_type>
    : m_size{other.m_size}
  {
    std::uninitialized_move(static_cast<pointer>(static_cast<void*>(other.m_storage)),
                            static_cast<pointer>(static_cast<void*>(other.m_storage)) + other.m_size,
                            static_cast<pointer>(static_cast<void*>(this->m_storage)));
  }

  inplace_vector_impl(size_type n)
  requires std::default_initializable<value_type>
  {
    if (n > N)
    {
      throw std::bad_alloc{};
    }

    this->m_size = n;

    std::uninitialized_default_construct(static_cast<pointer>(static_cast<void*>(this->m_storage)),
                                         static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size);
  }

  inplace_vector_impl(size_type n, const value_type& value)
  requires std::copy_constructible<value_type>
    : m_size{}
  {
    if (n > N)
    {
      throw std::bad_alloc{};
    }

    this->m_size = n;

    std::uninitialized_fill(static_cast<pointer>(static_cast<void*>(this->m_storage)),
                            static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size,
                            value);
  }

  template<std::input_iterator InIterator>
  inplace_vector_impl(InIterator begin, InIterator end)
  requires std::copy_constructible<value_type>
  {
    size_type range_size{static_cast<size_type>(std::distance(begin, end))};

    if (range_size > N)
    {
      throw std::bad_alloc{};
    }

    this->m_size = range_size;

    std::uninitialized_copy(begin,
                            end,
                            static_cast<pointer>(static_cast<void*>(this->m_storage)));
  }

  ~inplace_vector_impl()
  {
    this->clear();
  }

  [[nodiscard]] iterator begin() noexcept
  {
    return {static_cast<pointer>(static_cast<void*>(this->m_storage))};
  }

  [[nodiscard]] iterator end() noexcept
  {
    return {static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size};
  }

  [[nodiscard]] const_iterator cbegin() const noexcept
  {
    return {static_cast<const_pointer>(static_cast<const void*>(this->m_storage))};
  }

  [[nodiscard]] const_iterator cend() const noexcept
  {
    return {static_cast<const_pointer>(static_cast<const void*>(this->m_storage)) + this->m_size};
  }

  [[nodiscard]] std::reverse_iterator<iterator> rbegin() noexcept
  {
    return std::make_reverse_iterator<iterator>(static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size);
  }

  [[nodiscard]] std::reverse_iterator<iterator> rend() noexcept
  {
    return std::make_reverse_iterator<iterator>(static_cast<pointer>(static_cast<void*>(this->m_storage)));
  }

  [[nodiscard]] std::reverse_iterator<const_iterator> crbegin() const noexcept
  {
    return std::make_reverse_iterator<const_iterator>(static_cast<const_pointer>(static_cast<const void*>(this->m_storage)) + this->m_size);
  }

  [[nodiscard]] std::reverse_iterator<const_iterator> crend() const noexcept
  {
    return std::make_reverse_iterator<const_iterator>(static_cast<const_pointer>(static_cast<const void*>(this->m_storage)));
  }

  [[nodiscard]] pointer data() noexcept
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage));
  }

  [[nodiscard]] const_pointer data() const noexcept
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage));
  }

  [[nodiscard]] size_type size() const noexcept
  {
    return this->m_size;
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return !this->size();
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

  reference push_back(const value_type& value)
  requires std::copy_constructible<value_type>
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    return *this->construct(value);
  }

  reference push_back(value_type&& value)
  requires std::move_constructible<value_type>
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    return *this->construct(std::move(value));
  }

  pointer try_push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::copy_constructible<value_type>
  {
    if (this->m_size < N)
    {
      return this->construct(value);
    }

    return nullptr;
  }

  pointer try_push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::move_constructible<value_type>
  {
    if (this->m_size < N)
    {
      return this->constuct(std::move(value));
    }

    return nullptr;
  }

  reference unchecked_push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::copy_constructible<value_type>
  {
    return *this->try_push_back(value);
  }

  reference unchecked_push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::move_constructible<value_type>
  {
    return *this->try_push_back(std::move(value));
  }

  template<typename... Args>
  iterator emplace(const_iterator position, Args&&... args)
  requires (std::copyable<value_type> ||
            std::movable<value_type>)
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    if (this->m_size)
    {
      pointer temp_ptr{static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++ - 1UL};

      if constexpr (std::is_move_constructible_v<value_type>)
      {
        (void) ::new (temp_ptr + 1UL) value_type(std::move(*temp_ptr));
      }
      else
      {
        (void) ::new (temp_ptr + 1UL) value_type(*temp_ptr);
      }

      while (temp_ptr >= position.m_element)
      {
        if constexpr (std::is_move_assignable_v<value_type>)
        {
          *temp_ptr = std::move(temp_ptr[-1UL]);
        }
        else
        {
          *temp_ptr = temp_ptr[-1UL];
        }

        --temp_ptr;
      }

      if constexpr (!std::is_trivially_destructible_v<value_type>)
      {
        temp_ptr->~value_type();
      }

      return {::new (temp_ptr) value_type(std::forward<Args>(args)...)};
    }
    else if (!this->m_size)
    {
      return {this->emplace_construct(std::forward<Args>(args)...)};
    }
  }

  template<typename... Args>
  reference emplace_back(Args&&... args)
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    return *this->emplace_construct(std::forward<Args>(args)...);
  }

  template<typename... Args>
  pointer try_emplace_back(Args&&... args)
  {
    if (this->m_size < N)
    {
      return this->emplace_construct(std::forward<Args>(args)...);
    }

    return nullptr;
  }

  template<typename... Args>
  reference unchecked_emplace_back(Args&&... args)
  {
    return *this->try_emplace_back(std::forward<Args>(args)...);
  }

  void pop_back() noexcept
  {
    this->destroy();
  }

  [[nodiscard]] reference operator[](size_type index) noexcept
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage))[index];
  }

  [[nodiscard]] const_reference operator[](size_type index) const noexcept
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage))[index];
  }

  [[nodiscard]] reference front() noexcept
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage))[0UL];
  }

  [[nodiscard]] const_reference front() const noexcept
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage))[0UL];
  }

  [[nodiscard]] reference back()
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage))[this->m_size - 1UL];
  }

  [[nodiscard]] const_reference back() const noexcept
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage))[this->m_size - 1UL];
  }

  [[nodiscard]] reference at(size_type index)
  {
    this->range_check(index);

    return static_cast<pointer>(static_cast<void*>(this->m_storage))[index];
  }

  [[nodiscard]] const_reference at(size_type index) const
  {
    this->range_check(index);

    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage))[index];
  }

  template<std::input_iterator InIterator>
  void assign(InIterator begin, InIterator end)
  requires std::copyable<value_type>
  {
    size_type range_size{static_cast<size_type>(std::distance(begin, end))};

    if (range_size > N)
    {
      throw std::bad_alloc{};
    }

    size_type current_size{};

    while (!(begin == end) && current_size < this->m_size)
    {
      static_cast<pointer>(static_cast<void*>(this->m_storage))[current_size++] = *begin++;
    }

    std::uninitialized_copy(begin,
                            end,
                            static_cast<pointer>(static_cast<void*>(this->m_storage)) + current_size);

    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      std::destroy(static_cast<pointer>(static_cast<void*>(this->m_storage)) + current_size,
                   static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size);
    }

    this->m_size = range_size;
  }

  void assign(size_type n, const value_type& value)
  requires std::copyable<value_type>
  {
    if (n > N)
    {
      throw std::bad_alloc{};
    }

    size_type min_size{std::min(this->m_size, n)};
    auto current_position{std::fill(static_cast<pointer>(static_cast<void*>(this->m_storage)),
                                    static_cast<pointer>(static_cast<void*>(this->m_storage)) + min_size,
                                    value)};
    std::uninitialized_fill(current_position,
                            static_cast<pointer>(static_cast<void*>(this->m_storage)) + n,
                            value);

    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      std::destroy(current_position,
                   static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size);
    }

    this->m_size = n;
  }

  iterator insert(const_iterator position, const value_type& value)
  requires (std::copyable<value_type> ||
            std::movable<value_type>)
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    if (this->m_size)
    {
      pointer begin{static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++ - 1UL};

      if constexpr (std::is_move_constructible_v<value_type>)
      {
        (void) ::new (begin + 1UL) value_type(std::move(*begin));
      }
      else
      {
        (void) ::new (begin + 1UL) value_type(*begin);
      }

      pointer end{position.m_element};

      while (begin >= end)
      {
        if constexpr (std::is_move_assignable_v<value_type>)
        {
          *begin = std::move(begin[-1UL]);
        }
        else
        {
          *begin = begin[-1UL];
        }

        --begin;
      }

      *begin = value;

      return {begin};
    }
    else
    {
      return {::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(value)};
    }
  }

  iterator insert(const_iterator position, value_type&& value)
  requires std::movable<value_type>
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    if (this->m_size)
    {
      pointer begin{static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++ - 1UL};

      (void) ::new (begin + 1UL) value_type(std::move(*begin));

      pointer end{position.m_element};

      while (begin >= end)
      {
        *begin = std::move(begin[-1UL]);
        --begin;
      }

      *begin = std::move(value);

      return {begin};
    }
    else
    {
      return {::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(std::move(value))};
    }
  }

  void swap(inplace_vector_impl& other) noexcept(nop::details::is_nothrow_copy_or_move_constructible<value_type> &&
                                                 std::is_nothrow_copy_assignable_v<value_type>)
  requires (std::copyable<value_type> ||
            std::movable<value_type>)
  {
    if (this != std::addressof(other))
    {
      size_type old_self_size{m_size};
      size_type old_other_size{other.m_size};

      for (size_type i{}; i < this->m_size && i < other.m_size; ++i)
      {
        std::swap(static_cast<pointer>(static_cast<void*>(this->m_storage))[i], static_cast<pointer>(static_cast<void*>(other.m_storage))[i]);
      }

      if (this->m_size < other.m_size)
      {
        while (this->m_size < other.m_size)
        {
          if constexpr (std::is_move_constructible_v<value_type>)
          {
            (void) ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size) value_type(std::move(static_cast<pointer>(static_cast<void*>(other.m_storage))[this->m_size]));
          }
          else
          {
            (void) ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size) value_type(static_cast<pointer>(static_cast<void*>(other.m_storage))[this->m_size]);
          }

          if constexpr (!std::is_trivially_destructible_v<value_type>)
          {
            (static_cast<pointer>(static_cast<void*>(other.m_storage)) + this->m_size++)->~value_type();
          }
          else
          {
            ++this->m_size;
          }
        }
      }
      else
      {
        while (other.m_size < this->m_size)
        {
          if constexpr (std::is_move_constructible_v<value_type>)
          {
            (void) ::new (static_cast<pointer>(static_cast<void*>(other.m_storage)) + other.m_size) value_type(std::move(static_cast<pointer>(static_cast<void*>(this->m_storage))[other.m_size]));
          }
          else
          {
            (void) ::new (static_cast<pointer>(static_cast<void*>(other.m_storage)) + other.m_size) value_type(static_cast<pointer>(static_cast<void*>(this->m_storage))[other.m_size]);
          }

          if constexpr (!std::is_trivially_destructible_v<value_type>)
          {
            (static_cast<pointer>(static_cast<void*>(this->m_storage)) + other.m_size++)->~value_type();
          }
          else
          {
            ++other.m_size;
          }
        }
      }

      this->m_size = old_other_size;
      other.m_size = old_self_size;
    }
  }

  iterator erase(const_iterator position) noexcept(nop::details::is_nothrow_copy_or_move_assignable<value_type>)
  requires (std::copyable<value_type> ||
            std::movable<value_type>)
  {
    if (this->m_size)
    {
      if constexpr (std::is_move_assignable_v<value_type>)
      {
        std::move(position.m_element + 1UL,
                  position.m_element + --this->m_size,
                  position.m_element);
      }
      else
      {
        std::copy(position.m_element + 1UL,
                  position.m_element + --this->m_size,
                  position.m_element);
      }

      if constexpr (!std::is_trivially_destructible_v<value_type>)
      {
        (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size)->~value_type();
      }
    }

    return {position.m_element};
  }

  iterator erase(const_iterator first, const_iterator last)
  requires (std::copyable<value_type> ||
            std::movable<value_type>)
  {
    pointer new_last{first.m_element};

    if (first != last)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        pointer end_position{static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size};
        this->m_size -= static_cast<size_type>(last.m_element - first.m_element);

        [[likely]]
        while (last.m_element < end_position)
        {
          if constexpr (std::is_move_assignable_v<value_type>)
          {
            const_cast<reference>(*first++) = std::move(const_cast<reference>(*last++));
          }
          else
          {
            const_cast<reference>(*first++) = const_cast<reference>(*last++);
          }
        }

        if constexpr (!std::is_trivially_destructible_v<value_type>)
        {
          [[likely]]
          while (++first != last)
          {
            const_cast<pointer>(first.m_element)->~value_type();
          }
        }
      }
      else
      {
        pointer end_position{this->m_storage + this->m_size};
        this->m_size -= static_cast<size_type>(last.m_element - first.m_element);

        [[likely]]
        while (last.m_element < end_position)
        {
          (void) ::new (first.m_element) value_type(*last++);
          ++first;
        }
      }
    }

    return {new_last};
  }

  void resize(size_type size)
  requires std::default_initializable<value_type>
  {
    if (this->m_size > size)
    {
      if constexpr (!std::is_trivially_destructible_v<value_type>)
      {
        std::destroy(static_cast<pointer>(static_cast<void*>(this->m_storage)) + size,
                     static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size);
      }
    }
    else if (size <= N)
    {
      std::uninitialized_default_construct(static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size,
                                           static_cast<pointer>(static_cast<void*>(this->m_storage)) + size);
    }
    else
    {
      throw std::bad_alloc{};
    }

    this->m_size = size;
  }

  void resize(size_type size, const value_type& value)
  requires std::copy_constructible<value_type>
  {
    if (this->m_size > size)
    {
      if constexpr (!std::is_trivially_destructible_v<value_type>)
      {
        std::destroy(static_cast<pointer>(static_cast<void*>(this->m_storage) + size),
                     static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size);
      }
    }
    else if (size <= N)
    {
      std::uninitialized_fill(static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size,
                              static_cast<pointer>(static_cast<void*>(this->m_storage)) + size,
                              value);
    }
    else
    {
      throw std::bad_alloc{};
    }

    this->m_size = size;
  }

  inplace_vector_impl& operator=(const inplace_vector_impl& other)
  requires (std::copyable<value_type> ||
            std::movable<value_type>)
  {
    if (this != std::addressof(other))
    {
      this->assign(other.begin(), other.end());
    }

    return *this;
  }

  inplace_vector_impl& operator=(inplace_vector_impl&& other) noexcept(std::is_nothrow_move_constructible_v<value_type> &&
                                                                       std::is_nothrow_move_assignable_v<value_type>)
  requires std::movable<value_type>
  {
    if (this != std::addressof(other))
    {
      auto current_position{std::move(static_cast<pointer>(static_cast<void*>(other.m_storage)),
                                      static_cast<pointer>(static_cast<void*>(other.m_storage)) + std::min(this->m_size, other.m_size),
                                      static_cast<pointer>(static_cast<void*>(this->m_storage)))};

      if (this->m_size <= other.m_size)
      {
        std::uninitialized_move(static_cast<pointer>(static_cast<void*>(other.m_storage)) + this->m_size,
                                static_cast<pointer>(static_cast<void*>(other.m_storage)) + other.m_size,
                                current_position);
      }
      else if constexpr (!std::is_trivially_destructible_v<value_type>)
      {
        std::destroy(static_cast<pointer>(static_cast<void*>(this->m_storage)) + other.m_size,
                     current_position);
      }

      this->m_size = other.m_size;
    }

    return *this;
  }

};

} /* End namespace details */

} /* End namespace nop */

#endif /* End inplace vector header file */

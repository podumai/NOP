#ifndef NOP_DETAILS_CONTAINER_INPLACE_VECTOR_IMPL_HPP /* Begin __nop_details::container::inplace_vector_impl header file */
#define NOP_DETAILS_CONTAINER_INPLACE_VECTOR_IMPL_HPP 1UL

#ifndef NOP_CONTAINER_INPLACE_VECTOR_HPP
  #error "This header file cannot be included explicitly. Use inplace_vector.hpp instead."
#endif

#pragma once

#include <initializer_list> /* std::initializer_list<T> */
#include <memory>
#include <new>              /* std::bad_alloc */
#include <stdexcept>        /* std::out_of_range, std::invalid_argument */
#include <exception>        /* std::exception */
#include <type_traits>

#include "inplace_vector_base.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace container /* Begin namespace container */
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
         __nop_details::container::valid_inplace_vector_type T,
         std::size_t                                         N
        >
class inplace_vector_impl : public __nop_details::container::inplace_vector_base<T, N>
{
 private:
  using base = typename __nop_details::container::inplace_vector_base<T, N>;

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

 private:
  alignas (value_type) std::conditional_t<!std::is_fundamental_v<value_type>, std::uint8_t, value_type> m_storage[!std::is_fundamental_v<value_type> ? sizeof(value_type) * N : N];
  size_type m_size{};

 private:
  constexpr func range_check(size_type index) const -> void
  {
    if (this->m_size <= index)
    {
      throw std::out_of_range{"inplace_vector::range_check(size_type) -> index is out of range"};
    }
  }

  func construct(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>) -> pointer
  requires std::copy_constructible<value_type>
  {
    return ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(value);
  }

  func construct(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>) -> pointer
  requires std::move_constructible<value_type>
  {
    return ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(std::move(value));
  }

  template<typename... Args>
  func emplace_construct(Args&&... args) -> pointer
  requires std::constructible_from<value_type, Args...>
  {
    return ::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(std::forward<Args>(args)...);
  }

  func destroy() noexcept -> void
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

 public:
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

  [[nodiscard]] func begin() noexcept -> iterator
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage));
  }

  [[nodiscard]] func end() noexcept -> iterator
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size;
  }

  [[nodiscard]] func cbegin() const noexcept -> const_iterator
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage));
  }

  [[nodiscard]] func cend() const noexcept -> const_iterator
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage)) + this->m_size;
  }

  [[nodiscard]] func rbegin() noexcept -> reverse_iterator
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size;
  }

  [[nodiscard]] func rend() noexcept -> reverse_iterator
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage));
  }

  [[nodiscard]] func crbegin() const noexcept -> const_reverse_iterator
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage)) + this->m_size;
  }

  [[nodiscard]] func crend() const noexcept -> const_reverse_iterator
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage));
  }

  [[nodiscard]] func data() noexcept -> pointer
  {
    return static_cast<void*>(this->m_storage);
  }

  [[nodiscard]] func data() const noexcept -> const_pointer
  {
    return static_cast<const void*>(this->m_storage);
  }

  [[nodiscard]] func size() const noexcept -> size_type
  {
    return this->m_size;
  }

  [[nodiscard]] func empty() const noexcept -> bool
  {
    return !this->size();
  }

  func clear() noexcept -> void
  {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      std::destroy(static_cast<pointer>(static_cast<void*>(this->m_storage)),
                   static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size);
    }

    this->m_size = 0UL;
  }

  func push_back(const value_type& value) -> reference
  requires std::copy_constructible<value_type>
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    return *this->construct(value);
  }

  func push_back(value_type&& value) -> reference
  requires std::move_constructible<value_type>
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    return *this->construct(std::move(value));
  }

  func try_push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>) -> pointer
  requires std::copy_constructible<value_type>
  {
    if (this->m_size < N)
    {
      return this->construct(value);
    }

    return nullptr;
  }

  func try_push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>) -> pointer
  requires std::move_constructible<value_type>
  {
    if (this->m_size < N)
    {
      return this->constuct(std::move(value));
    }

    return nullptr;
  }

  func unchecked_push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>) -> reference
  requires std::copy_constructible<value_type>
  {
    return *this->try_push_back(value);
  }

  func unchecked_push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>) -> reference
  requires std::move_constructible<value_type>
  {
    return *this->try_push_back(std::move(value));
  }

  template<typename... Args>
  func emplace(const_iterator position, Args&&... args) -> iterator
  requires (std::copyable<value_type> || std::movable<value_type>)
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
  func emplace_back(Args&&... args)
  {
    if (this->m_size == N)
    {
      throw std::bad_alloc{};
    }

    return *this->emplace_construct(std::forward<Args>(args)...);
  }

  template<typename... Args>
  func try_emplace_back(Args&&... args) -> pointer
  {
    if (this->m_size < N)
    {
      return this->emplace_construct(std::forward<Args>(args)...);
    }

    return nullptr;
  }

  template<typename... Args>
  func unchecked_emplace_back(Args&&... args) -> reference
  {
    return *this->try_emplace_back(std::forward<Args>(args)...);
  }

  func pop_back() noexcept -> void
  {
    this->destroy();
  }

  [[nodiscard]] func operator[](size_type index) noexcept -> reference
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage))[index];
  }

  [[nodiscard]] func operator[](size_type index) const noexcept -> const_reference
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage))[index];
  }

  [[nodiscard]] func front() noexcept -> reference
  {
    return *static_cast<pointer>(static_cast<void*>(this->m_storage));
  }

  [[nodiscard]] func front() const noexcept -> const_reference
  {
    return *static_cast<const_pointer>(static_cast<const void*>(this->m_storage));
  }

  [[nodiscard]] func back() -> reference
  {
    return static_cast<pointer>(static_cast<void*>(this->m_storage))[this->m_size - 1UL];
  }

  [[nodiscard]] func back() const noexcept -> const_reference
  {
    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage))[this->m_size - 1UL];
  }

  [[nodiscard]] func at(size_type index) -> reference
  {
    this->range_check(index);

    return static_cast<pointer>(static_cast<void*>(this->m_storage))[index];
  }

  [[nodiscard]] func at(size_type index) const -> const_reference
  {
    this->range_check(index);

    return static_cast<const_pointer>(static_cast<const void*>(this->m_storage))[index];
  }

  template<std::input_iterator InIterator>
  func assign(InIterator begin, InIterator end) -> void
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

  func assign(size_type n, const value_type& value) -> void
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

  func insert(const_iterator position, const value_type& value) -> iterator
  requires (std::copyable<value_type> || std::movable<value_type>)
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

      pointer end{const_cast<pointer>(position)};

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

      return begin;
    }
    else
    {
      return {::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(value)};
    }
  }

  func insert(const_iterator position, value_type&& value) -> iterator
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

      pointer end{const_cast<pointer>(position)};

      while (begin >= end)
      {
        *begin = std::move(begin[-1UL]);
        --begin;
      }

      *begin = std::move(value);

      return begin;
    }
    else
    {
      return {::new (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size++) value_type(std::move(value))};
    }
  }

  func swap(inplace_vector_impl& other) noexcept(__nop_details::container::is_nothrow_copy_or_move_constructible<value_type> &&
                                                 std::is_nothrow_copy_assignable_v<value_type>) -> void
  requires (std::copyable<value_type> || std::movable<value_type>)
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

  func erase(const_iterator position) noexcept(__nop_details::container::is_nothrow_copy_or_move_assignable<value_type>) -> iterator
  requires (std::copyable<value_type> || std::movable<value_type>)
  {
    if (this->m_size)
    {
      if constexpr (std::is_move_assignable_v<value_type>)
      {
        std::move(position + 1UL,
                  position + --this->m_size,
                  position);
      }
      else
      {
        std::copy(position + 1UL,
                  position + --this->m_size,
                  position);
      }

      if constexpr (!std::is_trivially_destructible_v<value_type>)
      {
        (static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size)->~value_type();
      }
    }

    return const_cast<iterator>(position);
  }

  func erase(const_iterator first, const_iterator last) -> iterator
  requires (std::copyable<value_type> || std::movable<value_type>)
  {
    pointer new_last{const_cast<iterator>(first)};

    if (first != last)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        pointer end_position{static_cast<pointer>(static_cast<void*>(this->m_storage)) + this->m_size};
        this->m_size -= static_cast<size_type>(last - first);

        [[likely]]
        while (last < end_position)
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
            const_cast<iterator>(first)->~value_type();
          }
        }
      }
      else
      {
        pointer end_position{this->m_storage + this->m_size};
        this->m_size -= static_cast<size_type>(last - first);

        [[likely]]
        while (last < end_position)
        {
          (void) ::new (first++) value_type(*last++);
        }
      }
    }

    return new_last;
  }

  func resize(size_type size) -> void
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

  func resize(size_type size, const value_type& value) -> void
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

  func operator=(const inplace_vector_impl& other) -> inplace_vector_impl&
  requires (std::copyable<value_type> || std::movable<value_type>)
  {
    if (this != std::addressof(other))
    {
      this->assign(other.begin(), other.end());
    }

    return *this;
  }

  func operator=(inplace_vector_impl&& other) noexcept(std::is_nothrow_move_constructible_v<value_type> &&
                                                       std::is_nothrow_move_assignable_v<value_type>) -> inplace_vector_impl&
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

template<typename T>
class inplace_vector_impl<T, 0UL> : public __nop_details::container::inplace_vector_base<T, 0UL>
{
 private:
  using base = typename __nop_details::container::inplace_vector_base<T, 0UL>;

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

 protected:
  constexpr inplace_vector_impl()                           noexcept = default;
  constexpr inplace_vector_impl(const inplace_vector_impl&) noexcept = default;
  constexpr inplace_vector_impl(inplace_vector_impl&&)      noexcept = default;
  constexpr ~inplace_vector_impl()                                   = default;

  [[nodiscard]] constexpr func size() const noexcept -> size_type
  {
    return 0UL;
  }

  [[nodiscard]] constexpr func empty() const noexcept -> size_type
  {
    return !this->size();
  }

  constexpr func operator=(const inplace_vector_impl&) noexcept -> inplace_vector_impl& = default;
  constexpr func operator=(inplace_vector_impl&&)      noexcept -> inplace_vector_impl& = default;

};

} /* End namespace container */

} /* End namespace __nop_details */

#endif /* End __nop_details::container::inplace_vector_impl header file */

#ifndef NOP_INPLACE_VECTOR_HPP /* Begin inplace vector header file */
#define NOP_INPLACE_VECTOR_HPP 1UL

#include <type_traits>
#include <stdexcept>
#include <initializer_list>

#include "inplace_vector_base.hpp"

namespace nop /* Begin namespace nop */
{

namespace strategy /* Begin namespace strategy */
{

struct inplace_vector_nothrow
{
  /* Empty */
};

struct inplace_vector_throw
{
  /* Empty */
};

} /* End namespace strategy */

namespace details /* Begin namespace details */
{

template<typename T>
concept valid_inplace_vector_policy = std::is_base_of_v<nop::strategy::inplace_vector_nothrow, T> ||
                                      std::is_base_of_v<nop::strategy::inplace_vector_throw, T>;

} /* End namespace details */

template<
         typename T,
         std::size_t N,
         nop::details::valid_inplace_vector_policy Policy = nop::strategy::inplace_vector_nothrow
        >
requires std::is_nothrow_destructible_v<T>
class inplace_vector : public nop::details::inplace_vector_base<T, N>
{
 private:
  using base = nop::details::inplace_vector_base<T, N>;

 public:
  using value_type      = base::value_type;
  using size_type       = base::size_type;
  using difference_type = base::difference_type;
  using reference       = base::reference;
  using const_reference = base::const_reference;
  using pointer         = base::pointer;
  using const_pointer   = base::const_pointer;

 public:
  class iterator
  {
   public:
    using value_type      = inplace_vector::value_type;
    using size_type       = inplace_vector::size_type;
    using difference_type = inplace_vector::difference_type;
    using reference       = inplace_vector::reference;
    using const_reference = inplace_vector::const_reference;
    using pointer         = inplace_vector::pointer;
    using const_pointer   = inplace_vector::const_pointer;

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
    iterator(iterator&&) noexcept = default;
    ~iterator() = default;

    [[nodiscard]] reference operator*() noexcept
    {
      return *m_element;
    }

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return *m_element;
    }

    [[nodiscard]] pointer operator->() noexcept
    {
      return m_element;
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return m_element;
    }

    [[nodiscard]] reference operator[](difference_type index) noexcept
    {
      return m_element[index];
    }

    [[nodiscard]] const_reference operator[](difference_type index) const noexcept
    {
      return m_element[index];
    }

    iterator& operator++() noexcept
    {
      ++m_element;
      return *this;
    }

    iterator operator++(std::int32_t) noexcept
    {
      return {m_element++};
    }

    iterator& operator--() noexcept
    {
      --m_element;
      return *this;
    }

    iterator operator--(std::int32_t) noexcept
    {
      return {m_element--};
    }

    iterator& operator+=(difference_type offset) noexcept
    {
      m_element += offset;
      return *this;
    }

    iterator& operator-=(difference_type offset) noexcept
    {
      m_element -= offset;
      return *this;
    }

    [[nodiscard]] iterator operator+(difference_type offset) noexcept
    {
      return {m_element + offset};
    }

    [[nodiscard]] friend iterator operator+(difference_type offset,
                                            const iterator& iter) noexcept
    {
      return {iter.m_element + offset};
    }

    [[nodiscard]] iterator operator-(difference_type offset) noexcept
    {
      return {m_element - offset};
    }

    [[nodiscard]] difference_type operator-(const iterator& other) noexcept
    {
      return m_element - other.m_element;
    }

    [[nodiscard]] bool operator==(const iterator& other) const noexcept
    {
      return m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const iterator& other) const noexcept
    {
      return m_element != other.m_element;
    }

    [[nodiscard]] bool operator<(const iterator& other) const noexcept
    {
      return m_element < other.m_element;
    }

    [[nodiscard]] bool operator<=(const iterator& other) const noexcept
    {
      return m_element <= other.m_element;
    }

    [[nodiscard]] bool operator>(const iterator& other) const noexcept
    {
      return m_element > other.m_element;
    }

    [[nodiscard]] bool operator>=(const iterator& other) const noexcept
    {
      return m_element >= other.m_element;
    }

  };

  class const_iterator
  {
   public:
    using value_type      = const inplace_vector::value_type;
    using size_type       = inplace_vector::size_type;
    using difference_type = inplace_vector::difference_type;
    using const_reference = inplace_vector::const_reference;
    using const_pointer   = inplace_vector::const_pointer;

   private:
    const_pointer m_element;

   public:
    const_iterator() noexcept
      : m_element{nullptr}
    {
      /* Empty */
    }

    const_iterator(const_pointer ptr) noexcept
      : m_element{ptr}
    {
      /* Empty */
    }

    const_iterator(const const_iterator& other) noexcept = default;
    const_iterator(const_iterator&& other) noexcept = default;
    ~const_iterator() = default;

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return *m_element;
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return m_element;
    }

    [[nodiscard]] const_reference operator[](difference_type index) const noexcept
    {
      return m_element[index];
    }

    const_iterator& operator++() noexcept
    {
      ++m_element;
      return *this;
    }

    const_iterator operator++(std::int32_t) noexcept
    {
      return {m_element++};
    }

    const_iterator& operator--() noexcept
    {
      --m_element;
      return *this;
    }

    const_iterator operator--(std::int32_t) noexcept
    {
      return {m_element--};
    }

    const_iterator& operator+=(difference_type offset) noexcept
    {
      m_element += offset;
      return *this;
    }

    [[nodiscard]] const_iterator operator+(difference_type offset) noexcept
    {
      return {m_element + offset};
    }

    [[nodiscard]] friend const_iterator operator+(difference_type offset,
                                                  const const_iterator& iter) noexcept
    {
      return {offset + iter.m_element};
    }

    const_iterator& operator-=(difference_type offset) noexcept
    {
      m_element -= offset;
      return *this;
    }

    [[nodiscard]] const_iterator operator-(difference_type offset) noexcept
    {
      return {m_element - offset};
    }

    [[nodiscard]] friend const_iterator operator-(difference_type offset,
                                                  const const_iterator& other) noexcept
    {
      return {offset - other.m_element};
    }

    [[nodiscard]] difference_type operator-(const const_iterator& other) const noexcept
    {
      return m_element - other.m_element;
    }

    [[nodiscard]] bool operator==(const const_iterator& other) const noexcept
    {
      return m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const const_iterator& other) const noexcept
    {
      return m_element != other.m_element;
    }

    [[nodiscard]] bool operator>(const const_iterator& other) const noexcept
    {
      return m_element > other.m_element;
    }

    [[nodiscard]] bool operator>=(const const_iterator& other) const noexcept
    {
      return m_element >= other.m_element;
    }

    [[nodiscard]] bool operator<(const const_iterator& other) const noexcept
    {
      return m_element < other.m_element;
    }

    [[nodiscard]] bool operator<=(const const_iterator& other) const noexcept
    {
      return m_element <= other.m_element;
    }

  };

 private:
  std::conditional_t<std::is_fundamental_v<value_type>, value_type, std::uint8_t> m_storage[std::is_fundamental_v<value_type> ? N : sizeof(value_type) * N];
  size_type m_size;

 public:
  inplace_vector() noexcept
    : m_size{}
  {
    /* Empty */
  }

  inplace_vector(const inplace_vector& other)
  requires std::is_copy_constructible_v<value_type>
    : m_size{}
  {
    while (m_size < other.m_size)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size) value_type(reinterpret_cast<pointer>(other.m_storage)[m_size]);
      }
      else
      {
        (void) ::new (m_storage + m_size) value_type(other.m_storage[m_size]);
      }

      ++m_size;
    }
  }

  inplace_vector(inplace_vector&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
    : m_size{}
  {
    while (m_size < other.m_size)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size) value_type(std::move(reinterpret_cast<pointer>(other.m_storage)[m_size]));
      }
      else
      {
        (void) ::new (m_storage + m_size) value_type(std::move(other.m_storage[m_size]));
      }

      ++m_size;
    }
  }

  inplace_vector(size_type n, const value_type& value)
  requires std::is_copy_constructible_v<value_type>
    : m_size{}
  {
    size_type total_size{n > N ? N : n};

    while (m_size < total_size)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(value);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(value);
      }
    }
  }

  inplace_vector(std::initializer_list<value_type> ilist)
  requires (std::is_copy_constructible_v<value_type> ||
            std::is_move_constructible_v<value_type>)
    : m_size{}
  {
    for (auto begin{ilist.begin()}; begin != ilist.end() && m_size < N; ++m_size)
    {
      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size) value_type(std::move(*begin++));
      }
      else if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size) value_type(*begin++);
      }
      else
      {
        (void) ::new (m_storage + m_size) value_type(*begin++);
      }
    }
  }

  template<typename InIterator>
  requires (std::is_copy_constructible_v<value_type>)
  inplace_vector(InIterator begin, InIterator end)
  {
    while (begin != end && m_size < N)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(*begin++);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(*begin++);
      }
    }
  }

  ~inplace_vector()
  {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      while (m_size)
      {
        (reinterpret_cast<pointer>(m_storage) + --m_size)->~value_type();
      }
    }
  }

  [[nodiscard]] iterator begin() noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return {reinterpret_cast<pointer>(m_storage)};
    }
    else
    {
      return {m_storage};
    }
  }

  [[nodiscard]] iterator end() noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return {reinterpret_cast<pointer>(m_storage) + m_size};
    }
    else
    {
      return {m_storage + m_size};
    }
  }

  [[nodiscard]] const_iterator cbegin() const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return {reinterpret_cast<const_pointer>(m_storage)};
    }
    else
    {
      return {m_storage};
    }
  }

  [[nodiscard]] const_iterator cend() const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return {reinterpret_cast<const_pointer>(m_storage) + m_size};
    }
    else
    {
      return {m_storage + m_size};
    }
  }

  [[nodiscard]] pointer data() noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return reinterpret_cast<pointer>(m_storage);
    }
    else
    {
      return m_storage;
    }
  }

  [[nodiscard]] const_pointer data() const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return reinterpret_cast<const_pointer>(m_storage);
    }
    else
    {
      return m_storage;
    }
  }

  [[nodiscard]] size_type size() const noexcept
  {
    return m_size;
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return !m_size;
  }

  void clear() noexcept
  {
    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      while (m_size)
      {
        (reinterpret_cast<pointer>(m_storage) + --m_size)->~value_type();
      }
    }
  }

  void push_back(const value_type& value) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
  requires std::is_copy_constructible_v<value_type>
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(value);
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(value);
        }
      }
    }
    else
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(value);
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(value);
        }
      }
      else
      {
        throw std::out_of_range{"inplace_vector::push_back() -> storage overflow"};
      }
    }
  }

  void push_back(value_type&& value) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires std::is_move_constructible_v<value_type>
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(std::move(value));
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(value);
        }
      }
    }
    else
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(std::move(value));
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(value);
        }
      }
      else
      {
        throw std::length_error{"inplace_vector::push_back() -> storage overflow"};
      }
    }
  }

  template<typename... Args>
  void emplace_back(Args&&... args)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(std::forward<Args>(args)...);
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(std::forward<Args>(args)...);
        }
      }
    }
    else
    {
      if (m_size < N)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(std::forward<Args>(args)...);
        }
        else
        {
          (void) ::new (m_storage + m_size++) value_type(std::forward<Args>(args)...);
        }
      }
      else
      {
        throw std::length_error{"inplace_vector::emplace_back() -> storage overflow"};
      }
    }
  }

  void pop_back() noexcept(std::is_nothrow_destructible_v<value_type> &&
                           std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (!m_size)
      {
        throw std::out_of_range{"inplace_vector::pop_back() -> storage underflow"};
      }
    }

    if constexpr (!std::is_trivially_destructible_v<value_type>)
    {
      (reinterpret_cast<pointer>(m_storage) + --m_size)->~value_type();
    }
    else
    {
      --m_size;
    }
  }

  [[nodiscard]] reference operator[](size_type index) noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return reinterpret_cast<pointer>(m_storage)[index];
    }
    else
    {
      return m_storage[index];
    }
  }

  [[nodiscard]] const_reference operator[](size_type index) const noexcept
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return reinterpret_cast<pointer>(m_storage)[index];
    }
    else
    {
      return m_storage[index];
    }
  }

  [[nodiscard]] reference front() noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (!m_size)
      {
        throw std::out_of_range{"inplace_vector::front() -> storage underflow"};
      }
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return *reinterpret_cast<pointer>(m_storage);
    }
    else
    {
      return *m_storage;
    }
  }

  [[nodiscard]] const_reference front() const noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (!m_size)
      {
        throw std::out_of_range{"inplace_vector::front() -> storage underflow"};
      }
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return *reinterpret_cast<pointer>(m_storage);
    }
    else
    {
      return *m_storage;
    }
  }

  [[nodiscard]] reference back() noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (!m_size)
      {
        throw std::out_of_range{"inplace_vector::front() -> storage underflow"};
      }
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return reinterpret_cast<pointer>(m_storage)[m_size - 1UL];
    }
    else
    {
      return m_storage[m_size - 1UL];
    }
  }

  [[nodiscard]] const_reference back() const noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (!m_size)
      {
        throw std::out_of_range{"inplace_vector::front() -> storage underflow"};
      }
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return reinterpret_cast<pointer>(m_storage)[m_size - 1UL];
    }
    else
    {
      return m_storage[m_size - 1UL];
    }
  }

  [[nodiscard]] reference at(size_type index) noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (index >= m_size)
      {
        throw std::out_of_range{"inplace_vector::at() -> index is out of range"};
      }
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return reinterpret_cast<pointer>(m_storage)[index];
    }
    else
    {
      return m_storage[index];
    }
  }

  [[nodiscard]] const_reference at(size_type index) const noexcept(std::is_base_of_v<nop::strategy::inplace_vector_nothrow, Policy>)
  {
    if constexpr (std::is_base_of_v<nop::strategy::inplace_vector_throw, Policy>)
    {
      if (index >= m_size)
      {
        throw std::out_of_range{"inplace_vector::at() -> index is out of range"};
      }
    }

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      return reinterpret_cast<pointer>(m_storage)[index];
    }
    else
    {
      return m_storage[index];
    }
  }

  void assign(std::initializer_list<value_type> ilist)
  requires ((std::is_copy_constructible_v<value_type> ||
             std::is_move_constructible_v<value_type>) &&
            (std::is_copy_assignable_v<value_type> ||
             std::is_move_assignable_v<value_type>))
  {
    auto begin{ilist.begin()};

    if (m_size)
    {
      for (size_type i{}; i < m_size && begin != ilist.end(); ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type> &&
                      std::is_move_assignable_v<value_type>)
        {
          reinterpret_cast<pointer>(m_storage)[i] = std::move(*begin++);
        }
        else if constexpr (!std::is_fundamental_v<value_type>)
        {
          reinterpret_cast<pointer>(m_storage)[i] = *begin++;
        }
        else
        {
          m_storage[i] = *begin++;
        }
      }
    }

    while (m_size < N && begin != ilist.end())
    {
      if constexpr (!std::is_fundamental_v<value_type> &&
                    std::is_move_constructible_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(std::move(*begin++));
      }
      else if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(*begin++);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(*begin++);
      }
    }
  }

  template<typename InIterator>
  requires (std::is_copy_constructible_v<value_type> &&
            std::is_copy_assignable_v<value_type>)
  void assign(InIterator begin, InIterator end)
  {
    if (m_size)
    {
      for (size_type i{}; i < m_size && begin != end; ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          reinterpret_cast<pointer>(m_storage)[i] = *begin++;
        }
        else
        {
          m_storage[i] = *begin++;
        }
      }
    }

    while (m_size < N && begin != end)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        reinterpret_cast<pointer>(m_storage)[m_size++] = *begin++;
      }
      else
      {
        m_storage[m_size++] = *begin++;
      }
    }
  }

  void assign(size_type n, const value_type& value)
  requires (std::is_copy_constructible_v<value_type> &&
            std::is_copy_assignable_v<value_type>)
  {
    if (m_size)
    {
      for (size_type i{}; i < m_size && i < n; ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          reinterpret_cast<pointer>(m_storage)[i] = value;
        }
        else
        {
          m_storage[i] = value;
        }
      }
    }


    while (m_size < N && m_size < n)
    {
      if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(value);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(value);
      }
    }
  }

  void swap(inplace_vector& other)
  requires (std::is_copy_constructible_v<value_type> ||
            std::is_move_constructible_v<value_type>)
  {
    if (this != std::addressof(other))
    {
      size_type old_self_size{m_size};
      size_type old_other_size{other.m_size};

      for (size_type i{}; i < m_size && i < other.m_size; ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          std::swap(reinterpret_cast<pointer>(m_storage)[i], reinterpret_cast<pointer>(other.m_storage)[i]);
        }
        else
        {
          std::swap(m_storage[i], other.m_storage[i]);
        }
      }

      if (m_size < other.m_size)
      {
        while (m_size < other.m_size)
        {
          if constexpr (std::is_move_constructible_v<value_type> &&
                        !std::is_fundamental_v<value_type>)
          {
            (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size) value_type(std::move(reinterpret_cast<pointer>(other.m_storage)[m_size]));
          }
          else if constexpr (!std::is_fundamental_v<value_type>)
          {
            (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size) value_type(reinterpret_cast<pointer>(other.m_storage)[m_size]);
          }
          else
          {
            (void) ::new (m_storage + m_size) value_type(other.m_storage[m_size]);
          }

          if constexpr (!std::is_trivially_destructible_v<value_type>)
          {
            (reinterpret_cast<pointer>(other.m_storage) + m_size++)->~value_type();
          }
          else
          {
            ++m_size;
          }
        }
      }
      else
      {
        while (other.m_size < m_size)
        {
          if constexpr (std::is_move_constructible_v<value_type> &&
                        !std::is_fundamental_v<value_type>)
          {
            (void) ::new (reinterpret_cast<pointer>(other.m_storage) + other.m_size) value_type(std::move(reinterpret_cast<pointer>(m_storage)[other.m_size]));
          }
          else if constexpr (!std::is_fundamental_v<value_type>)
          {
            (void) ::new (reinterpret_cast<pointer>(other.m_storage) + other.m_size) value_type(reinterpret_cast<pointer>(m_storage)[other.m_size]);
          }
          else
          {
            (void) ::new (other.m_storage + other.m_size) value_type(m_storage[other.m_size]);
          }

          if constexpr (!std::is_trivially_destructible_v<value_type>)
          {
            (reinterpret_cast<pointer>(m_storage) + other.m_size++)->~value_type();
          }
          else
          {
            ++m_size;
          }
        }
      }

      m_size = old_other_size;
      other.m_size = old_self_size;
    }
  }

  inplace_vector& operator=(std::initializer_list<value_type> ilist)
  requires (std::is_copy_constructible_v<value_type> ||
            std::is_move_constructible_v<value_type>)
  {
    auto begin{ilist.begin()};

    if (m_size)
    {
      for (size_type i{}; i < m_size && i < ilist.size(); ++i)
      {
        if constexpr (std::is_move_constructible_v<value_type> &&
                      !std::is_fundamental_v<value_type>)
        {
          reinterpret_cast<pointer>(m_storage)[i] = std::move(*begin++);
        }
        else if constexpr (!std::is_fundamental_v<value_type>)
        {
          reinterpret_cast<pointer>(m_storage)[i] = *begin++;
        }
        else
        {
          (void) ::new (m_storage + i) value_type(*begin++);
        }
      }
    }

    while (m_size < N && begin != ilist.end())
    {
      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(std::move(*begin++));
      }
      else if constexpr (!std::is_fundamental_v<value_type>)
      {
        (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size++) value_type(*begin++);
      }
      else
      {
        (void) ::new (m_storage + m_size++) value_type(*begin++);
      }
    }

    return *this;
  }

  inplace_vector& operator=(const inplace_vector& other)
  requires (std::is_copy_assignable_v<value_type> &&
            std::is_copy_constructible_v<value_type>)
  {
    if (this != std::addressof(other))
    {
      if (m_size)
      {
        for (size_type i{}; i < m_size && i < other.m_size; ++i)
        {
          if constexpr (!std::is_fundamental_v<value_type>)
          {
            reinterpret_cast<pointer>(m_storage)[i] = reinterpret_cast<pointer>(other.m_storage)[i];
          }
          else
          {
            (void) ::new (m_storage + i) value_type(other.m_storage[i]);
          }
        }
      }

      while (m_size < other.m_size)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size) value_type(reinterpret_cast<pointer>(other.m_storage)[m_size]);
        }
        else
        {
          (void) ::new (m_storage + m_size) value_type(other.m_storage[m_size]);
        }

        ++m_size;
      }
    }

    return *this;
  }

  inplace_vector& operator=(inplace_vector&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>)
  requires (std::is_move_assignable_v<value_type> &&
            std::is_move_constructible_v<value_type>)
  {
    if (this != std::addressof(other))
    {
      for (size_type i{}; i < m_size && i < other.m_size; ++i)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          reinterpret_cast<pointer>(m_storage)[i] = std::move(reinterpret_cast<pointer>(other.m_storage)[i]);
        }
        else
        {
          (void) ::new (m_storage + i) value_type(other.m_storage[i]);
        }
      }

      while (m_size < other.m_size)
      {
        if constexpr (!std::is_fundamental_v<value_type>)
        {
          (void) ::new (reinterpret_cast<pointer>(m_storage) + m_size) value_type(std::move(reinterpret_cast<pointer>(other.m_storage)[m_size]));
        }
        else
        {
          (void) ::new (m_storage + m_size) value_type(other.m_storage[m_size]);
        }

        ++m_size;
      }
    }

    return *this;
  }

};

} /* End namespace nop */

template<typename T, std::size_t N, class Policy>
[[nodiscard]] bool operator==(const nop::inplace_vector<T, N, Policy>& lhs,
                              const nop::inplace_vector<T, N, Policy>& rhs) noexcept
{
  if (lhs.size() == rhs.size())
  {
    for (std::size_t i{}; i < lhs.size(); ++i)
    {
      if (lhs[i] != rhs[i])
      {
        return false;
      }
    }

    return true;
  }

  return false;
}

template<typename T, std::size_t N, class Policy>
[[nodiscard]] bool operator!=(const nop::inplace_vector<T, N, Policy>& lhs,
                              const nop::inplace_vector<T, N, Policy>& rhs) noexcept
{
  if (lhs.size() == rhs.size())
  {
    for (std::size_t i{}; i < lhs.size(); ++i)
    {
      if (lhs[i] != rhs[i])
      {
        return true;
      }
    }
  }

  return false;
}

#endif /* End inplace vector header file */

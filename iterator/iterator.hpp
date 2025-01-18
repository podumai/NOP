#ifndef NOP_ITERATOR_HPP /* Begin iterator header file */
#define NOP_ITERATOR_HPP 1UL

#include <cstdint>

#include <type_traits>
#include <iterator>

namespace nop /* Begin namespace nop */
{

namespace detail /* Begin namespace detail */
{

template<typename T>
concept iterator_diff_t = std::is_signed_v<T> &&
                          (sizeof(T) >= sizeof(long));

using fwd_tag = std::forward_iterator_tag;
using bidir_tag = std::bidirectional_iterator_tag;
using ra_tag = std::random_access_iterator_tag;

} /* End namespace detail */

template<typename T, typename Difference, class Category>
requires detail::iterator_diff_t<Difference>
class iterator
{
 public:
  using value_type        = T;
  using reference         = T&;
  using pointer           = T*;
  using difference_type   = Difference;
  using iterator_category = Category;

 private:
  template<typename _C, typename _T>
  using _predicate_i = std::enable_if_t<std::is_base_of_v<_C, _T>, bool>;

  template<typename _C, typename _T>
  using _operator_i = std::enable_if_t<std::is_base_of_v<_C, _T>, iterator>;

 private:
  pointer m_element;

 public:
  constexpr iterator() noexcept
    : m_element{nullptr}
  { /* Empty */ }

  constexpr iterator(pointer ptr) noexcept
    : m_element{ptr}
  { /* Empty */ }

  constexpr iterator(const iterator&) noexcept = default;
  constexpr iterator(iterator&&) noexcept = default;
  constexpr ~iterator() = default;

  [[nodiscard]] constexpr reference operator*() noexcept
  {
    return *m_element;
  }

  template<typename U = T>
  [[nodiscard]] constexpr std::enable_if_t<std::is_class_v<U>, pointer> operator->() noexcept
  {
    return m_element;
  }

  constexpr iterator& operator++() noexcept
  {
    ++m_element;
    return *this;
  }

  [[nodiscard]] constexpr iterator operator++(std::int32_t) noexcept
  {
    auto temp_iterator{m_element};
    ++m_element;
    return temp_iterator;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _operator_i<detail::bidir_tag, U>& operator--() noexcept
  {
    --m_element;
    return *this;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _operator_i<detail::bidir_tag, U> operator--(std::int32_t) noexcept
  {
    auto temp_iterator{m_element};
    --m_element;
    return temp_iterator;
  }

  template<typename U = Category>
  constexpr _operator_i<detail::ra_tag, U>& operator+=(difference_type offset) noexcept
  {
    m_element += offset;
    return *this;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _operator_i<detail::ra_tag, U> operator+(difference_type offset) noexcept
  {
    return iterator{m_element + offset};
  }

  template<typename U = Category>
  [[nodiscard]] friend constexpr _operator_i<detail::ra_tag, U> operator+(difference_type offset,
                                                                          const iterator& iter) noexcept
  {
    return iterator{offset + iter.m_element};
  }

  template<typename U = Category>
  constexpr _operator_i<detail::ra_tag, U>& operator-=(difference_type offset) noexcept
  {
    m_element -= offset;
    return *this;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _operator_i<detail::ra_tag, U> operator-(difference_type offset) noexcept
  {
    return iterator{m_element - offset};
  }

  template<typename U = Category>
  [[nodiscard]] friend constexpr _operator_i<detail::ra_tag, U> operator-(difference_type offset,
                                                                          const iterator& iter) noexcept
  {
    return iterator{offset - iter.m_element};
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<detail::ra_tag, U>, difference_type> operator-(const iterator& other) noexcept
  {
    return m_element - other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<detail::ra_tag, U>, reference> operator[](difference_type index) noexcept
  {
    return m_element[index];
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _predicate_i<detail::fwd_tag, U> operator==(const iterator& other) noexcept
  {
    return m_element == other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _predicate_i<detail::fwd_tag, U> operator!=(const iterator& other) noexcept
  {
    return m_element != other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _predicate_i<detail::ra_tag, U> operator>(const iterator& other) noexcept
  {
    return m_element > other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _predicate_i<detail::ra_tag, U> operator>=(const iterator& other) noexcept
  {
    return m_element >= other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _predicate_i<detail::ra_tag, U> operator<(const iterator& other) noexcept
  {
    return m_element < other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr _predicate_i<detail::ra_tag, U> operator<=(const iterator& other) noexcept
  {
    return m_element <= other.m_element;
  }

  constexpr iterator& operator=(const iterator&) noexcept = default;
  constexpr iterator& operator=(iterator&&) noexcept = default;
};

} /* End namespace nop */

#endif /* End iterator header file */

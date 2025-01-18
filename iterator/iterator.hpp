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
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::bidirectional_iterator_tag, U>, iterator>& operator--() noexcept
  {
    --m_element;
    return *this;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::bidirectional_iterator_tag, U>, iterator> operator--(std::int32_t) noexcept
  {
    auto temp_iterator{m_element};
    --m_element;
    return temp_iterator;
  }

  template<typename U = Category>
  constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, iterator>& operator+=(difference_type offset) noexcept
  {
    m_element += offset;
    return *this;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, iterator> operator+(difference_type offset) noexcept
  {
    return iterator{m_element + offset};
  }

  template<typename U = Category>
  [[nodiscard]] friend constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, iterator> operator+(difference_type offset,
                                                                                                                             const iterator& iter) noexcept
  {
    return iterator{offset + iter.m_element};
  }

  template<typename U = Category>
  constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, iterator>& operator-=(difference_type offset) noexcept
  {
    m_element -= offset;
    return *this;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, iterator> operator-(difference_type offset) noexcept
  {
    return iterator{m_element - offset};
  }

  template<typename U = Category>
  [[nodiscard]] friend constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, iterator> operator-(difference_type offset,
                                                                                                                             const iterator& iter) noexcept
  {
    return iterator{offset - iter.m_element};
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, difference_type> operator-(const iterator& other) noexcept
  {
    return m_element - other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, reference> operator[](difference_type index) noexcept
  {
    return m_element[index];
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, U>, bool> operator==(const iterator& other) noexcept
  {
    return m_element == other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, U>, bool> operator!=(const iterator& other) noexcept
  {
    return m_element != other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, bool> operator>(const iterator& other) noexcept
  {
    return m_element > other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, bool> operator>=(const iterator& other) noexcept
  {
    return m_element >= other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, bool> operator<(const iterator& other) noexcept
  {
    return m_element < other.m_element;
  }

  template<typename U = Category>
  [[nodiscard]] constexpr std::enable_if_t<std::is_base_of_v<std::random_access_iterator_tag, U>, bool> operator<=(const iterator& other) noexcept
  {
    return m_element <= other.m_element;
  }

  constexpr iterator& operator=(const iterator&) noexcept = default;
  constexpr iterator& operator=(iterator&&) noexcept = default;
};

} /* End namespace nop */

#endif /* End iterator header file */

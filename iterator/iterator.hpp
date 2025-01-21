#ifndef NOP_ITERATOR_HPP /* Begin iterator header file */
#define NOP_ITERATOR_HPP 1UL

#include <cstdint>

#include <type_traits>
#include <concepts>
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

  [[nodiscard]] constexpr pointer operator->() noexcept
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
    return iterator{m_element++};
  }

  [[nodiscard]] constexpr iterator& operator--() noexcept
  requires std::derived_from<Category, detail::bidir_tag>
  {
    --m_element;
    return *this;
  }

  [[nodiscard]] constexpr iterator operator--(std::int32_t) noexcept
  requires std::derived_from<Category, detail::bidir_tag>
  {
    return iterator{m_element--};
  }

  constexpr iterator& operator+=(difference_type offset) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    m_element += offset;
    return *this;
  }

  [[nodiscard]] constexpr iterator operator+(difference_type offset) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return iterator{m_element + offset};
  }

  [[nodiscard]] friend constexpr iterator operator+(difference_type offset,
                                                    const iterator& iter) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return iterator{offset + iter.m_element};
  }

  constexpr iterator& operator-=(difference_type offset) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    m_element -= offset;
    return *this;
  }

  [[nodiscard]] constexpr iterator operator-(difference_type offset) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return iterator{m_element - offset};
  }

  [[nodiscard]] friend constexpr iterator operator-(difference_type offset,
                                                    const iterator& iter) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return iterator{offset - iter.m_element};
  }

  [[nodiscard]] constexpr difference_type operator-(const iterator& other) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return m_element - other.m_element;
  }

  [[nodiscard]] constexpr reference operator[](difference_type index) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return m_element[index];
  }

  [[nodiscard]] constexpr bool operator==(const iterator& other) noexcept
  requires std::derived_from<Category, detail::fwd_tag>
  {
    return m_element == other.m_element;
  }

  [[nodiscard]] constexpr bool operator!=(const iterator& other) noexcept
  requires std::derived_from<Category, detail::fwd_tag>
  {
    return m_element != other.m_element;
  }

  [[nodiscard]] constexpr bool operator>(const iterator& other) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return m_element > other.m_element;
  }

  [[nodiscard]] constexpr bool operator>=(const iterator& other) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return m_element >= other.m_element;
  }

  [[nodiscard]] constexpr bool operator<(const iterator& other) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return m_element < other.m_element;
  }

  [[nodiscard]] constexpr bool operator<=(const iterator& other) noexcept
  requires std::derived_from<Category, detail::ra_tag>
  {
    return m_element <= other.m_element;
  }

  constexpr iterator& operator=(const iterator&) noexcept = default;
  constexpr iterator& operator=(iterator&&) noexcept = default;
};

} /* End namespace nop */

#endif /* End iterator header file */

#ifndef NOP_FORWARD_LIST_HPP /* Begin forward list header file */
#define NOP_FORWARD_LIST_HPP 1UL

#pragma once

#include <memory>           /* std::allocator<T> */
#include <type_traits>      /* type_traits */
#include <initializer_list> /* std::initializer_list */
#include <concepts>         /* std::is_same<T> */
#include <cstdint>          /* std::size_t, std::ptrdiff_t */

namespace nop /* Begin namespace nop */
{

namespace details /* Begin namespace details */
{

template<typename T>
struct forward_list_node
{
 public:
  using value_type = T;
  using node_type  = forward_list_node;
  using pointer    = forward_list_node*;

 public:
  value_type value;
  pointer next;

 public:
  forward_list_node()
    : value(),
      next{nullptr}
  {
    /* Empty */
  }

  forward_list_node(const value_type& val)
    : value(val),
      next{nullptr}
  {
    /* Empty */
  }

  forward_list_node(value_type&& val)
  requires (!std::is_fundamental_v<value_type>)
    : value(std::move(val)),
      next{nullptr}
  {
    /* Empty */
  }

  forward_list_node(pointer ptr, const value_type& val)
    : value(val),
      next{ptr}
  {
    /* Empty */
  }

  forward_list_node(pointer ptr, value_type&& val) noexcept
  requires (!std::is_fundamental_v<value_type>)
    : value{std::move(val)},
      next{ptr}
  {
    /* Empty */
  }

  forward_list_node(pointer ptr, value_type&& val) noexcept
  requires std::is_fundamental_v<value_type>
    : value{val},
      next{ptr}
  {
    /* Empty */
  }

  template<typename... Args>
  forward_list_node(Args&&... args)
    : value(std::forward<Args>(args)...),
      next{nullptr}
  {
    /* Empty */
  }

  template<typename... Args>
  forward_list_node(pointer ptr, Args&&... args)
    : value(std::forward<Args>(args)...),
      next{ptr}
  {
    /* Empty */
  }

};

template<class UnaryPredicate, typename T>
concept valid_predicate_forward_list = requires(UnaryPredicate p, const T& val)
                                       {
                                         { p(val) } noexcept -> std::same_as<bool>;
                                       };

template<typename T, class Alloc>
concept valid_tparams_forward_list = std::is_same_v<typename std::allocator_traits<Alloc>::pointer, nop::details::forward_list_node<T>*> &&
                                     std::is_nothrow_destructible_v<T>;

} /* End namespace details */

template<
         typename T,
         class Alloc = std::allocator<nop::details::forward_list_node<T>>
        >
requires nop::details::valid_tparams_forward_list<T, Alloc>
class forward_list
{
 public:
  using value_type      = T;
  using allocator_type  = Alloc;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference       = T&;
  using const_reference = const T&;
  using pointer         = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;

 private:
  using node_type = nop::details::forward_list_node<value_type>;

 public:
  class iterator
  {
   private:
    friend forward_list;

   public:
    using value_type        = T;
    using reference         = T&;
    using const_reference   = const T&;
    using pointer           = nop::details::forward_list_node<T>*;
    using const_pointer     = const nop::details::forward_list_node<T>*;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

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

    iterator(const_pointer ptr) noexcept
      : m_element{const_cast<pointer>(ptr)}
    {
      /* Empty */
    }

    iterator(const iterator&) noexcept = default;
    iterator(iterator&&) noexcept = default;
    ~iterator() = default;

    [[nodiscard]] reference operator*() noexcept
    {
      return m_element->value;
    }

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return m_element->value;
    }

    [[nodiscard]] pointer operator->() noexcept
    {
      return std::addressof(m_element->value);
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return std::addressof(m_element->value);
    }

    iterator& operator++() noexcept
    {
      m_element = m_element->next;
      return *this;
    }

    iterator operator++(std::int32_t) noexcept
    {
      auto temp_iterator{m_element};
      m_element = m_element->next;
      return temp_iterator;
    }

    iterator& operator=(const iterator&) noexcept = default;
    iterator& operator=(iterator&&) noexcept = default;

    [[nodiscard]] bool operator==(const iterator& other) const noexcept
    {
      return m_element == other.m_element;
    }

    [[nodiscard]] bool operator!=(const iterator& other) const noexcept
    {
      return m_element != other.m_element;
    }

  };

 public:
  using const_iterator = const iterator;

 private:
  [[no_unique_address]] allocator_type xmalloc;
  pointer m_head;

 private:
  void clear_list() noexcept
  {
    [[likely]]
    while (m_head)
    {
      pointer temp_ptr{m_head->next};

      if constexpr (!std::is_fundamental_v<value_type>)
      {
        m_head->~node_type();
      }

      xmalloc.deallocate(m_head, 1UL);
      m_head = temp_ptr;
    }
  }

 public:
  forward_list() noexcept(noexcept(allocator_type{}))
    : xmalloc{},
      m_head{nullptr}
  {
    /* Empty */
  }

  explicit forward_list(const allocator_type& allocator) noexcept(noexcept(allocator_type{}))
    : xmalloc{allocator},
      m_head{nullptr}
  {
    /* Empty */
  }

  forward_list(std::initializer_list<value_type> ilist)
  requires (std::is_move_constructible_v<value_type> ||
            std::is_copy_constructible_v<value_type>)
    : xmalloc{},
      m_head{nullptr}
  {
    if (auto begin{ilist.begin()}; begin != ilist.end())
    {
      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        pointer temp_ptr{xmalloc.allocate(1UL)};
        m_head = ::new (temp_ptr) node_type(std::move(*begin++));

        while (begin != ilist.end())
        {
          temp_ptr->next = xmalloc.allocate(1UL);
          temp_ptr = ::new (temp_ptr->next) node_type(std::move(*begin++));
        }
      }
      else
      {
        pointer temp_ptr{xmalloc.allocate(1UL)};
        m_head = ::new (temp_ptr) node_type(*begin++);

        while (begin != ilist.end())
        {
          temp_ptr->next = xmalloc.allocate(1UL);
          temp_ptr = ::new (temp_ptr->next) node_type(*begin++);
        }
      }
    }
  }

  template<typename InIterator>
  forward_list(InIterator begin,
               InIterator end,
               const allocator_type& allocator = allocator_type{})
    : xmalloc{allocator},
      m_head{nullptr}
  {
    if (begin != end)
    {
      m_head = xmalloc.allocate(1UL);
      (void) ::new (m_head) node_type(*begin++);
      pointer temp_ptr{m_head};

      while (begin != end)
      {
        temp_ptr->next = xmalloc.allocate(1UL);
        temp_ptr = ::new (temp_ptr->next) node_type(*begin++);
      }
    }
  }

  forward_list(forward_list&& other) noexcept
    : m_head{other.m_head}
  {
    other.m_head = nullptr;
  }

  ~forward_list()
  {
    clear_list();
  }

  [[nodiscard]] iterator begin() noexcept
  {
    return {m_head};
  }

  [[nodiscard]] iterator end() noexcept
  {
    return {};
  }

  [[nodiscard]] const_iterator cbegin() const noexcept
  {
    return {m_head};
  }

  [[nodiscard]] const_iterator cend() const noexcept
  {
    return {};
  }

  void push_front(const value_type& value)
  {
    pointer temp_node{xmalloc.allocate(1UL)};
    m_head = ::new (temp_node) node_type(m_head, value);
  }

  void push_front(value_type&& value)
  requires std::is_move_constructible_v<value_type>
  {
    pointer temp_node{xmalloc.allocate(1UL)};

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      m_head = ::new (temp_node) node_type(m_head, std::move(value));
    }
    else
    {
      m_head = ::new (temp_node) node_type(m_head, value);
    }
  }

  template<typename... Args>
  void emplace_front(Args&&... args)
  {
    pointer temp_node{xmalloc.allocate(1UL)};
    m_head = ::new (temp_node) node_type(m_head, std::forward<Args>(args)...);
  }

  void pop_front() noexcept
  {
    pointer temp_ptr{m_head};
    m_head = m_head->next;
    temp_ptr->~node_type();
    xmalloc.deallocate(temp_ptr, 1UL);
  }

  iterator push_after(const_iterator position, const value_type& value)
  {
    pointer temp_ptr{xmalloc.allocate(1UL)};
    (void) ::new (temp_ptr) node_type(position.m_element->next, value);
    position.m_element->next = temp_ptr;
    return {temp_ptr};
  }

  iterator push_after(const_iterator position, value_type&& value)
  requires std::is_move_constructible_v<value_type>
  {
    pointer temp_ptr{xmalloc.allocate(1UL)};

    if constexpr (!std::is_fundamental_v<value_type>)
    {
      position.m_element->next = ::new (temp_ptr) node_type(position.m_element->next, std::move(value));
    }
    else
    {
      position.m_element->next = ::new (temp_ptr) node_type(position.m_element->next, value);
    }

    return {temp_ptr};
  }

  [[deprecated("May be potentially dangerous due to creation of linked list (ctor may throw and all info is lost)")]]
  iterator push_after(const_iterator position,
                      size_type n,
                      const value_type& value)
  {
    if (n--)
    {
      pointer temp_last{position.m_element->next};
      pointer temp_ptr{xmalloc.allocate(1UL)};
      position.m_element->next = ::new (temp_ptr) node_type(value);

      while (n--)
      {
        temp_ptr->next = xmalloc.allocate(1UL);
        temp_ptr = ::new (temp_ptr->next) node_type(value);
      }

      temp_ptr->next = temp_last;

      return {temp_ptr};
    }

    return position;
  }

  [[deprecated("May be potentially dangerous due to creation of linked list (ctor may throw and all info is lost)")]]
  iterator push_after(const_iterator position,
                      std::initializer_list<value_type> ilist)
  {
    if (auto begin{ilist.begin()}; begin != ilist.end())
    {
      pointer temp_last{position.m_element->next};
      pointer temp_ptr{xmalloc.allocate(1UL)};

      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        position.m_element->next = ::new (temp_ptr) node_type(std::move(*begin++));
      }
      else
      {
        position.m_element->next = ::new (temp_ptr) node_type(*begin++);
      }

      while (begin != ilist.end())
      {
        temp_ptr->next = xmalloc.allocate(1UL);

        if constexpr (std::is_move_constructible_v<value_type> &&
                      !std::is_fundamental_v<value_type>)
        {
          temp_ptr = ::new (temp_ptr->next) node_type(std::move(*begin++));
        }
        else
        {
          temp_ptr = ::new (temp_ptr->next) node_type(*begin++);
        }
      }

      temp_ptr->next = temp_last;

      return {temp_ptr};
    }

    return position;
  }

  template<typename InIterator>
  [[deprecated("May be potentially dangerous due to creation of linked list (ctor may throw and all info is lost)")]]
  iterator push_after(const_iterator position,
                      InIterator begin,
                      InIterator end)
  {
    if (begin != end)
    {
      pointer temp_last{position.m_element->next};
      pointer temp_ptr{xmalloc.allocate(1UL)};
      position.m_element->next = ::new (temp_ptr) node_type(*begin++);

      while (begin != end)
      {
        temp_ptr->next = xmalloc.allocate(1UL);
        temp_ptr = ::new (temp_ptr->next) node_type(*begin++);
      }

      temp_ptr->next = temp_last;

      return {temp_ptr};
    }

    return position;
  }

  template<typename... Args>
  iterator emplace_after(const_iterator position, Args&&... args)
  {
    pointer temp_ptr{xmalloc.allocate(1UL)};
    position.m_element->next = ::new (temp_ptr) node_type(position.m_element->next, std::forward<Args>(args)...);
    return {temp_ptr};
  }

  [[nodiscard]] reference front()
  {
    return m_head->value;
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return !m_head;
  }

  void clear() noexcept
  {
    clear_list();
  }

  [[nodiscard]] allocator_type get_allocator() const noexcept
  {
    return xmalloc;
  }

  void swap(forward_list& other) noexcept
  {
    std::swap(m_head, other.m_head);
  }

  template<typename InIterator>
  void assign(InIterator begin, InIterator end)
  {
    if (begin != end)
    {
      clear_list();

      m_head = xmalloc.allocate(1UL);
      m_head = ::new (m_head) node_type(*begin++);
      pointer temp_ptr{m_head};

      [[likely]]
      while (begin != end)
      {
        temp_ptr->next = xmalloc.allocate(1UL);
        temp_ptr = ::new (temp_ptr->next) node_type(*begin++);
      }
    }
  }

  void assign(std::initializer_list<value_type> ilist)
  {
    if (auto begin{ilist.begin()}; begin != ilist.end())
    {
      m_head = xmalloc.allocate(1UL);

      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        m_head = ::new (m_head) node_type(std::move(*begin++));
      }
      else
      {
        m_head = ::new (m_head) node_type(*begin++);
      }

      pointer temp_ptr{m_head};

      [[likely]]
      while (begin != end)
      {
        temp_ptr->next = xmalloc.allocate(1UL);

        if constexpr (std::is_move_constructible_v<value_type> &&
                      !std::is_fundamental_v<value_type>)
        {
          temp_ptr = ::new (temp_ptr->next) node_type(std::move(*begin++));
        }
        else
        {
          temp_ptr = ::new (temp_ptr->next) node_type(*begin++);
        }
      }
    }
  }

  size_type remove(const value_type& value) noexcept
  {
    size_type removed_elements{};

    [[likely]]
    while (m_head && m_head->value == value)
    {
      pointer temp_ptr{m_head};
      m_head = m_head->next;

      if constexpr (!std::is_fundamental_v<value_type>)
      {
        temp_ptr->~node_type();
      }

      xmalloc.deallocate(temp_ptr, 1UL);
      ++removed_elements;
    }

    pointer prev{m_head};
    pointer temp{m_head ? m_head->next : m_head};

    [[likely]]
    while (temp)
    {
      if (temp->value == value)
      {
        prev->next = temp->next;

        if constexpr (!std::is_fundamental_v<value_type>)
        {
          temp->~node_type();
        }

        xmalloc.deallocate(temp, 1UL);
        temp = prev->next;
        ++removed_elements;
      }
      else
      {
        prev = prev->next;
        temp = temp->next;
      }
    }

    return removed_elements;
  }

  template<details::valid_predicate_forward_list<value_type> UnaryPredicate>
  size_type remove_if(UnaryPredicate p) noexcept
  {
    size_type removed_elements{};

    [[likely]]
    while (m_head && p(m_head->value))
    {
      pointer temp_ptr{m_head};
      m_head = m_head->next;

      if constexpr (!std::is_fundamental_v<value_type>)
      {
        temp_ptr->~node_type();
      }

      xmalloc.deallocate(temp_ptr, 1UL);
      ++removed_elements;
    }

    pointer prev{m_head};
    pointer temp{m_head ? m_head->next : m_head};

    [[likely]]
    while (temp)
    {
      if (p(temp->value))
      {
        prev->next = temp->next;

        if constexpr (!std::is_fundamental_v<value_type>)
        {
          temp->~node_type();
        }

        xmalloc.deallocate(temp, 1UL);
        temp = prev->next;
        ++removed_elements;
      }
      else
      {
        prev = prev->next;
        temp = temp->next;
      }
    }

    return removed_elements;
  }

  forward_list& operator=(const forward_list& other)
  {
    if (this != std::addressof(other))
    {
      clear_list();

      if (pointer temp_ptr{other.m_head}; temp_ptr)
      {
        m_head = xmalloc.allocate(1UL);
        m_head = ::new (m_head) node_type(*temp_ptr++);

        pointer temp_head{m_head};

        [[likely]]
        while (temp_ptr)
        {
          temp_ptr->next = xmalloc.allocate(1UL);
          temp_ptr = ::new (temp_ptr->next) node_type(*temp_ptr++);
        }
      }
    }

    return *this;
  }

  forward_list& operator=(std::initializer_list<value_type> ilist)
  {
    if (auto begin{ilist.begin()}; begin != ilist.end())
    {
      clear_list();

      m_head = xmalloc.allocate(1UL);

      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        m_head = ::new (m_head) node_type(std::move(*begin++));
      }
      else
      {
        m_head = ::new (m_head) node_type(*begin++);
      }

      pointer temp_ptr{m_head};

      [[likely]]
      while (begin != ilist.end())
      {
        temp_ptr->next = xmalloc.allocate(1UL);

        if constexpr (std::is_move_constructible_v<value_type> &&
                      !std::is_fundamental_v<value_type>)
        {
          temp_ptr = ::new (temp_ptr->next) node_type(std::move(*begin++));
        }
        else
        {
          temp_ptr = ::new (temp_ptr->next) node_type(*begin++);
        }
      }
    }

    return *this;
  }

  forward_list& operator=(forward_list&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      clear_list();

      m_head = other.m_head;
      other.m_head = nullptr;
    }

    return *this;
  }

  [[nodiscard]] bool operator==(const forward_list& other) const noexcept
  {
    pointer temp_ptr{m_head};
    pointer temp_other{other.m_head};

    [[likely]]
    while (temp_ptr && temp_other)
    {
      if (temp_ptr->value != temp_other->value)
      {
        return false;
      }

      temp_ptr = temp_ptr->next;
      temp_other = temp_other->next;
    }

    return temp_ptr == temp_other;
  }

  [[nodiscard]] bool operator!=(const forward_list& other) const noexcept
  {
    pointer temp_ptr{m_head};
    pointer temp_other{other.m_head};

    [[likely]]
    while (temp_ptr && temp_other)
    {
      if (temp_ptr->value != temp_other->value)
      {
        return true;
      }

      temp_ptr = temp_ptr->next;
      temp_other = temp_other->next;
    }

    return temp_ptr != temp_other;
  }

};

} /* End namespace nop */

namespace std /* Begin namespace std */
{

template<typename T, typename Alloc>
void swap(nop::forward_list<T, Alloc>& lhs, nop::forward_list<T, Alloc>& rhs) noexcept
{
  lhs.swap(rhs);
}

} /* End namespace std */

#endif /* End forward list header file */

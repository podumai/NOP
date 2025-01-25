#ifndef NOP_FORWARD_LIST_HPP /* Begin forward list header file */
#define NOP_FORWARD_LIST_HPP 1UL

#pragma once

#include <memory>
#include <type_traits>
#include <stdexcept>
#include <initializer_list>
#include <cstdint>

#include "/home/podumai/NOP/iterator/iterator.hpp"

namespace nop /* Begin namespace nop */
{

namespace details /* Begin namespace details */
{

template<typename T>
struct forward_list_node
{
  T value;
  forward_list_node* next;

  forward_list_node(const T& val)
    : value(val),
      next{nullptr}
  {
    /* Empty */
  }

  forward_list_node(T&& val) noexcept
  requires (!std::is_fundamental_v<T>)
    : value(std::move(val)),
      next{nullptr}
  {
    /* Empty */
  }

  forward_list_node(T&& val) noexcept
  requires std::is_fundamental_v<T>
    : value(val),
      next{nullptr}
  {
    /* Empty */
  }

  template<typename... Args>
  forward_list_node(Args... args)
    : value(std::forward<Args>(args)...),
      next{nullptr}
  {
    /* Empty */
  }

};

} /* End namespace details */

template<
         typename T,
         class Alloc = std::allocator<nop::details::forward_list_node<T>>
        >
class forward_list
{
 public:
  using value_type      = T;
  using reference       = T&;
  using const_reference = const T&;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using allocator_type  = Alloc;

 private:
  using node_type = nop::details::forward_list_node<value_type>;
  using pointer   = nop::details::forward_list_node<value_type>*;

 public:
  class iterator : public nop::iterator<
                                        nop::details::fwd_tag,
                                        nop::details::forward_list_node<value_type>
                                       >
  {
   private:
    using iter_base = nop::iterator<
                                    nop::details::fwd_tag,
                                    nop::details::forward_list_node<value_type>
                                   >;

   public:
    using pointer         = nop::forward_list<T, Alloc>::value_type *;
    using const_pointer   = const nop::forward_list<T, Alloc>::value_type *;
    using reference       = nop::forward_list<T, Alloc>::reference;
    using const_reference = const nop::forward_list<T, Alloc>::reference &;
    using difference_type = typename iter_base::difference_type;

   public:
    iterator() = default;

    iterator(nop::forward_list<T, Alloc>::pointer ptr) noexcept
      : iter_base{ptr}
    {
      /* Empty */
    }

    iterator(const iterator&) = default;
    iterator(iterator&&) = default;
    ~iterator() = default;

    [[nodiscard]] reference operator*() noexcept
    {
      return (*iter_base::base())->value;
    }

    [[nodiscard]] const_reference operator*() const noexcept
    {
      return (*iter_base::base())->value;
    }

    [[nodiscard]] pointer operator->() noexcept
    {
      return std::addressof(iter_base::base()->value);
    }

    [[nodiscard]] const_pointer operator->() const noexcept
    {
      return std::addressof((*iter_base::base())->value);
    }

    iterator& operator++() noexcept
    {
      auto temp_ptr{iter_base::base()};
      *temp_ptr = (*temp_ptr)->next;

      return *this;
    }

    iterator operator++(std::int32_t) noexcept
    {
      auto temp_iterator{*this};
      auto temp_ptr{iter_base::base()};
      *temp_ptr = (*temp_ptr)->next;

      return temp_iterator;
    }

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&) = default;
  };

 private:
  [[no_unique_address]] allocator_type xmalloc;
  pointer m_head;

 private:
  template<typename U>
  pointer create_node(U&& value)
  {
    pointer temp_node{xmalloc.allocate(sizeof(node_type))};
    (void) new (temp_node) node_type(std::forward<U>(value));

    return temp_node;
  }

  template<typename... Args>
  pointer emplace_node(Args... args)
  {
    pointer temp_node{xmalloc.allocate(sizeof(node_type))};
    (void) new (temp_node) node_type(std::forward<Args>(args)...);

    return temp_node;
  }

  void insert_front(const value_type& value)
  {
    if (pointer temp_ptr{create_node(value)}; m_head)
    {
      temp_ptr->next = m_head;
      m_head = temp_ptr;
    }
    else
    {
      m_head = temp_ptr;
    }
  }

  void insert_front(value_type&& value)
  requires std::is_move_constructible_v<value_type>
  {
    if (pointer temp_ptr{create_node(std::move(value))}; m_head)
    {
      temp_ptr->next = m_head;
      m_head = temp_ptr;
    }
    else
    {
      m_head = temp_ptr;
    }
  }

  constexpr void clear_list() noexcept
  {
    while (m_head)
    {
      pointer temp_ptr{m_head->next};
      m_head->~node_type();
      xmalloc.deallocate(m_head, sizeof(node_type));
      m_head = temp_ptr;
    }

    m_head = nullptr;
  }

 public:
  forward_list() noexcept
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

  forward_list(std::initializer_list<value_type> ilist,
               const allocator_type& allocator = allocator_type{})
    : xmalloc{allocator},
      m_head{nullptr}
  {
    auto begin{ilist.begin()};

    if (begin != ilist.end())
    {
      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        m_head = create_node(std::move(*begin++));
      }
      else
      {
        m_head = create_node(*begin++);
      }
    }

    pointer temp_ptr{m_head};

    while (begin != ilist.end())
    {
      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        temp_ptr->next = create_node(std::move(*begin++));
      }
      else
      {
        temp_ptr->next = create_node(*begin++);
      }

      temp_ptr = temp_ptr->next;
    }
  }

  template<typename InIterator>
  forward_list(InIterator begin,
               InIterator end,
               const allocator_type allocator = allocator_type{})
    : xmalloc{allocator},
      m_head{nullptr}
  {
    if (begin != end)
    {
      m_head = create_node(*begin++);
    }

    pointer temp_ptr{m_head};

    while (begin != end)
    {
      temp_ptr->next = create_node(*begin++);
      temp_ptr = temp_ptr->next;
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

  void push_front(const value_type& value)
  {
    insert_front(value);
  }

  void push_front(value_type&& value)
  requires std::is_move_constructible_v<value_type>
  {
    if constexpr (!std::is_fundamental_v<value_type>)
    {
      insert_front(std::move(value));
    }
    else
    {
      insert_front(value);
    }
  }

  template<typename... Args>
  void emplace_front(Args... args)
  {
    if (pointer temp_ptr{emplace_node(std::forward<Args>(args)...)};
        m_head)
    {
      temp_ptr->next = m_head;
      m_head = temp_ptr;
    }
    else
    {
      m_head = temp_ptr;
    }
  }

  constexpr void pop_front() noexcept
  {
    if (!m_head)
    {
      return;
    }

    pointer temp_ptr{m_head};
    m_head = m_head->next;
    temp_ptr->~node_type();
    xmalloc.deallocate(temp_ptr, sizeof(node_type));
  }

  [[nodiscard]] reference front()
  {
    if (!m_head)
    {
      throw std::out_of_range{"forward_list::front() -> list is empty"};
    }

    return m_head->value;
  }

  [[nodiscard]] bool empty() const noexcept
  {
    return m_head == nullptr;
  }

  constexpr void clear() noexcept
  {
    clear_list();
  }

  [[nodiscard]] allocator_type get_allocator() const noexcept
  {
    return xmalloc;
  }

  constexpr void swap(forward_list& other) noexcept
  {
    std::swap(m_head, other.m_head);
  }

  template<typename InIterator>
  void assign(InIterator begin, InIterator end)
  {
    if (begin != end)
    {
      clear_list();
      m_head = create_node(*begin++);
    }

    pointer temp_ptr{m_head};

    while (begin != end)
    {
      temp_ptr->next = create_node(*begin++);
      temp_ptr = temp_ptr->next;
    }
  }

  void assign(std::initializer_list<value_type> ilist)
  {
    auto begin{ilist.begin()};

    if (begin != ilist.end())
    {
      clear_list();

      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        m_head = create_node(std::move(*begin++));
      }
      else
      {
        m_head = create_node(*begin++);
      }
    }
    else
    {
      return;
    }

    pointer temp_ptr{m_head};

    while (begin != ilist.end())
    {
      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        temp_ptr->next = create_node(std::move(*begin++));
      }
      else
      {
        temp_ptr->next = create_node(*begin++);
      }

      temp_ptr = temp_ptr->next;
    }
  }

  forward_list& operator=(const forward_list& other)
  {
    if (this != std::addressof(other))
    {
      clear_list();

      if (pointer temp_ptr{other.m_head}; temp_ptr)
      {
        m_head = create_node(temp_ptr->value);

        pointer temp_head{m_head};

        while (temp_ptr->next)
        {
          temp_ptr = temp_ptr->next;
          temp_head->next = create_node(temp_ptr->value);
          temp_head = temp_head->next;
        }
      }
    }

    return *this;
  }

  forward_list& operator=(std::initializer_list<value_type> ilist)
  {
    clear_list();

    auto begin{ilist.begin()};

    if (begin != ilist.end())
    {
      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        m_head = create_node(std::move(*begin++));
      }
      else
      {
        m_head = create_node(*begin++);
      }
    }

    pointer temp_ptr{m_head};

    while (begin != ilist.end())
    {
      if constexpr (std::is_move_constructible_v<value_type> &&
                    !std::is_fundamental_v<value_type>)
      {
        temp_ptr->next = create_node(std::move(*begin++));
      }
      else
      {
        temp_ptr->next = create_node(*begin++);
      }

      temp_ptr = temp_ptr->next;
    }

    return *this;
  }

  constexpr forward_list& operator=(forward_list&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      clear_list();

      m_head = other.m_head;
      other.m_head = nullptr;
    }

    return *this;
  }

  [[nodiscard]] constexpr bool operator==(const forward_list& other) const noexcept
  {
    pointer temp_ptr{m_head};
    pointer temp_other{other.m_head};

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

  [[nodiscard]] constexpr bool operator!=(const forward_list& other) const noexcept
  {
    pointer temp_ptr{m_head};
    pointer temp_other{other.m_head};

    while (temp_ptr && temp_other)
    {
      if (temp_ptr->value == temp_other->value)
      {
        return false;
      }
      else
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
constexpr void swap(nop::forward_list<T, Alloc>& lhs, nop::forward_list<T, Alloc>& rhs) noexcept
{
  lhs.swap(rhs);
}

} /* End namespace str */

#endif /* End forward list header file */

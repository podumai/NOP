#ifndef NOP_CONTAINER_STACK_HPP /* Begin nop::container::stack header file */
#define NOP_CONTAINER_STACK_HPP 1UL

#pragma once

#include <concepts>

#include "base/func_keyword.hpp"

namespace __nop_details /* Begin namespace __nop_details */
{

namespace container /* Begin namespace container */
{

template<typename Container>
concept valid_container_t = requires(Container c, typename Container::value_type& value)
{
  { c.back() } -> std::same_as<typename Container::reference>;
  { static_cast<const Container&>(c).back() } -> std::same_as<typename Container::const_reference>;
  { c.size() } -> std::same_as<typename Container::size_type>;
  { c.push_back(value) };
  { c.push_back(std::move(value)) };
  { c.pop_back() };
};

} /* End namespace container */

} /* End namespace __nop_details */

namespace nop /* Begin namespace nop */
{

namespace container /* Begin namespace container */
{

template<typename T, __nop_details::container::valid_container_t Container>
class stack
{
 public:
   using value_type = typename Container::value_type;
   using reference = typename Container::reference;
   using const_reference = typename Container::const_reference;
   using size_type = typename Container::size_type;
   using container_type = Container;

 private:
  container_type m_storage;

 public:
  [[nodiscard]] func top() noexcept(noexcept(this->m_storage.back())) -> reference
  {
    return this->m_storage.back();
  }

  [[nodiscard]] func top() const noexcept(noexcept(this->m_storage.back())) -> const_reference
  {
    return this->m_storage.back();
  }

  func push(const value_type& value) -> void
  requires std::copyable<value_type>
  {
    this->m_storage.push_back(value);
  }

  func push(value_type&& value) -> void
  requires std::movable<value_type>
  {
    this->m_storage.push_back(std::move(value));
  }

  func pop() -> void
  {
    this->m_storage.pop_back();
  }

  [[nodiscard]] func size() const noexcept -> size_type
  {
    return this->m_storage.size();
  }

};

} /* End namespace contaiener */

} /* End namespace nop */

#endif
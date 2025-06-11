#ifndef NOP_CONTAINER_ADAPTOR_STACK_HPP /* Begin nop::container::adaptor::stack<T, Container> header file */
#define NOP_CONTAINER_ADAPTOR_STACK_HPP 1UL

#pragma once

#include <stack>
#include <type_traits>
#include <mutex>

#include "base/func_keyword.hpp"

namespace nop::container::adaptor::mt /* Begin namespace nop::container::adaptor::mt */
{

template<typename T, typename Container = std::deque<T>>
class stack
{
 public:
  using value_type = T;
  using return_type = std::conditional_t<std::is_fundamental_v<value_type>, value_type, std::shared_ptr<value_type>>;

 public:
  stack() = default;
  stack(const stack& s)
  {
    std::lock_guard lg{s.m_};
    stack_ = s.stack_;
  }
  func operator=(const stack& s) -> stack& = delete;
  ~stack() = default;

  func push(const value_type& value) -> void
  {
    std::lock_guard lg{m_};
    stack_.push(value);
  }

  func push(value_type&& value) -> void
  {
    std::lock_guard lg{m_};
    stack_.push(std::move(value));
  }

  template<typename... Args>
  func emplace(Args&&... args) -> void
  {
    std::lock_guard lg{m_};
    stack_.emplace(std::forward<Args>(args)...);
  }

  [[nodiscard]] func pop() -> return_type
  {
    std::lock_guard lg{m_};
    if (stack_.empty())
    {
      return return_type{};
    }
    return_type value;
    if constexpr (std::is_fundamental_v<value_type> == false)
    {
      value = std::make_shared<value_type>(stack_.top());
    }
    else
    {
      value = stack_.top();
    }
    stack_.pop();
    return value;
  }

 private:
  std::stack<T, Container> stack_;
  std::mutex m_;
};

} /* End namespace nop::container::adaptor::mt */

#endif
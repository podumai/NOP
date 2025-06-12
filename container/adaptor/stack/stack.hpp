#ifndef NOP_CONTAINER_ADAPTOR_MT_STACK_HPP /* Begin nop::container::adaptor::mt::stack<T, Container> header file */
#define NOP_CONTAINER_ADAPTOR_MT_STACK_HPP 1UL

#pragma once

#include <stack>
#include <type_traits>
#include <mutex>
#include <condition_variable>

#include "base/func_keyword.hpp"

namespace nop::container::adaptor::mt /* Begin namespace nop::container::adaptor::mt */
{

template<typename T, typename Container = std::deque<T>>
class stack
{
 public:
  using value_type = T;
  using container_type = Container;
  using return_type = std::conditional_t<std::is_fundamental_v<value_type>, value_type, std::shared_ptr<value_type>>;

 public:
  stack() = default;
  stack(const stack& s)
  {
    std::lock_guard lg{s.m_};
    stack_ = s.stack_;
  }
  func operator=(const stack& s) -> stack&
  {
    if (this == &s)
    {
      return *this;
    }
    std::scoped_lock sl{m_, s.m_};
    stack_ = s.stack_;
    return *this;
  }
  ~stack() = default;

  func push(const value_type& value) -> void
  {
    std::lock_guard lg{m_};
    stack_.push(value);
    cv_.notify_one();
  }

  func push(value_type&& value) -> void
  {
    std::lock_guard lg{m_};
    stack_.push(std::move(value));
    cv_.notify_one();
  }

  template<typename... Args>
  func emplace(Args&&... args) -> void
  {
    std::lock_guard lg{m_};
    stack_.emplace(std::forward<Args>(args)...);
    cv_.notify_one();
  }

 private:
  [[nodiscard]] func get_value() -> return_type
  {
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

 public:
  [[nodiscard]] func try_pop() -> return_type
  {
    std::unique_lock ul{m_};
    if (stack_.empty())
    {
      return return_type{};
    }
    return get_value();
  }

  [[nodiscard]] func pop() -> return_type
  {
    std::unique_lock ul{m_};
    cv_.wait(ul, [this] -> bool { return stack_.empty() == false; });
    return get_value();
  }

 private:
  std::stack<value_type, container_type> stack_;
  std::mutex m_;
  std::condition_variable cv_;
};

} /* End namespace nop::container::adaptor::mt */

#endif /* End nop::container::adaptor::mt::stack header file */
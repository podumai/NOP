#include "exception.hpp"

namespace nop /* Begin namespace nop */
{

namespace err /* Begin namespace err */
{

  /*
   * Logic error implementation
   */
  LogicError::LogicError()
#if __cplusplus >= 201103L
    : m_error{"LogicError()"}
#else
    : m_error("LogicError()")
#endif
  {}

  LogicError::LogicError(const char* errorMessage)
#if __cplusplus >= 201103L
    : m_error{errorMessage}
#else
    : m_error(errorMessage)
#endif
  {}

  LogicError::LogicError(const std::string& errorMessage)
#if __cplusplus >= 201103L
    : m_error{errorMessage}
  {}
#else
  LogicError::LogicError(const std::string& errorMessage)
    : m_error(errorMessage)
  {}

  LogicError::LogicError(const LogicError& other)
    : m_error(other.m_error)
  {}
#endif

#if __cplusplus >= 201103L
  LogicError::LogicError(std::string&& errorMessage) __NOP_ATTRIBUTE_NOEXCEPT__
    : m_error{std::move(errorMessage)}
  {}
#endif

  const char* LogicError::what() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return m_error.c_str();
  }

  i32 LogicError::errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return 1;
  }

#if __cplusplus < 201103L
  LogicError& LogicError::operator=(const LogicError& other)
  {
    m_error = other.m_error;
    return *this;
  }
#endif

  /*
   * Invalid argument implementation
   */
  InvalidArgument::InvalidArgument()
#if __cplusplus >= 201103L
    : LogicError{"InvalidArgument()"}
#else
    : LogicError("InvalidArgument()")
#endif
  {}

  InvalidArgument::InvalidArgument(const char* errorMessage)
#if __cplusplus >= 201103L
    : LogicError{errorMessage}
#else
    : LogicError(errorMessage)
#endif
  {}

  InvalidArgument::InvalidArgument(const std::string& errorMessage)
#if __cplusplus >= 201103L
    : LogicError{errorMessage}
  {}
#else
    : LogicError(errorMessage)
  {}

  InvalidArgument::InvalidArgument(const InvalidArgument& other)
    : LogicError(other.what())
  {}
#endif

#if __cplusplus >= 201103L
  InvalidArgument::InvalidArgument(std::string&& errorMessage) __NOP_ATTRIBUTE_NOEXCEPT__
    : LogicError{std::move(errorMessage)}
  {}
#endif

  i32 InvalidArgument::errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return 2;
  }

#if __cplusplus < 201103L
  InvalidArgument& InvalidArgument::operator=(const InvalidArgument& other)
  {
    return *this;
  }
#endif

  /*
   * Runtime error implementation
   */
  RuntimeError::RuntimeError()
#if __cplusplus >= 201103L
    : m_error{"RuntimeError()"}
#else
    : m_error("RuntimeError()")
#endif
  {}

  RuntimeError::RuntimeError(const char* errorMessage)
#if __cplusplus >= 201103L
    : m_error{errorMessage}
#else
    : m_error(errorMessage)
#endif
  {}

  RuntimeError::RuntimeError(const std::string& errorMessage)
#if __cplusplus >= 201103L
    : m_error{errorMessage}
  {}
#else
    : m_error(errorMessage)
  {}

  RuntimeError::RuntimeError(const RuntimeError& other)
    : m_error(other.m_error)
  {}
#endif

#if __cplusplus >= 201103L
  RuntimeError::RuntimeError(std::string&& errorMessage) __NOP_ATTRIBUTE_NOEXCEPT__
    : m_error{std::move(errorMessage)}
  {}
#endif

  const char* RuntimeError::what() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return m_error.c_str();
  }

  i32 RuntimeError::errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return 3;
  }

#if __cplusplus < 201103L
  RuntimeError& RuntimeError::operator=(const RuntimeError& other)
  {
    m_error = other.m_error;
    return *this;
  }
#endif

  /*
    * System error implementation
    */
  SystemError::SystemError()
    : RuntimeError{"SystemError()"}
  {}

  SystemError::SystemError(const char* errorMessage)
    : RuntimeError{errorMessage}
  {}

  SystemError::SystemError(const std::string& errorMessage)
    : RuntimeError{errorMessage}
  {}

  SystemError::SystemError(std::string&& errorMessage) noexcept
    : RuntimeError{std::move(errorMessage)}
  {}

  i32 SystemError::errorCode() const noexcept
  {
    return 4;
  }

  /*
   * Bad alloc implementation
   */
  BadAlloc::BadAlloc()
    : RuntimeError{"BadAlloc()"}
  {}

  BadAlloc::BadAlloc(const char* errorMessage)
    : RuntimeError{errorMessage}
  {}

  BadAlloc::BadAlloc(const std::string& errorMessage)
    : RuntimeError{errorMessage}
  {}

#if __cplusplus >= 201103L
  BadAlloc::BadAlloc(std::string&& errorMessage) __NOP_ATTRIBUTE_NOEXCEPT__
    : RuntimeError{std::move(errorMessage)}
  {}
#endif

  i32 BadAlloc::errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return 5;
  }

  /*
   * Format error implementation
   */
  FormatError::FormatError()
    : LogicError{"FormatError()"}
  {}

  FormatError::FormatError(const char* errorMessage)
    : LogicError{errorMessage}
  {}

  FormatError::FormatError(const std::string& errorMessage)
    : LogicError{errorMessage}
  {}

  FormatError::FormatError(std::string&& errorMessage) __NOP_ATTRIBUTE_NOEXCEPT__
    : LogicError{std::move(errorMessage)}
  {}

  i32 FormatError::errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return 6;
  }

  /*
    * Out of range implementation
    */
  OutOfRange::OutOfRange()
    : LogicError{"OutOfRange()"}
  {}

  OutOfRange::OutOfRange(const char* errorMessage)
    : LogicError{errorMessage}
  {}

  OutOfRange::OutOfRange(const std::string& errorMessage)
    : LogicError{errorMessage}
  {}

#if __cplusplus >= 201103L
  OutOfRange::OutOfRange(std::string&& errorMessage) __NOP_ATTRIBUTE_NOEXCEPT__
    : LogicError{std::move(errorMessage)}
  {}
#endif

  i32 OutOfRange::errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return 7;
  }

#if __cplusplus < 201103L
  OutOfRange& OutOfRange::operator=(const OutOfRange& other)
  {
    return *this;
  }
#endif

  /*
   * Length error implementation
   */
  LengthError::LengthError()
#if __cplusplus >= 201103L
    : LogicError{"LengthError()"}
#else
    : :LogicError("LogicError")
#endif
  {}

  LengthError::LengthError(const char* errorMessage)
#if __cplusplus >= 201103L
    : LogicError{errorMessage}
#else
    : LogicError(errorMessage)
#endif
  {}

  LengthError::LengthError(const std::string& errorMessage)
#if __cplusplus >= 201103L
    : LogicError{errorMessage}
  {}
#else
    : LogicError(errorMessage)
  {}

  LengthError(const LengthError& other)
    : LogicError(other.what())
  {}
#endif

#if __cplusplus >= 201103L
  LengthError::LengthError(std::string&& errorMessage) __NOP_ATTRIBUTE_NOEXCEPT__
    : LogicError{std::move(errorMessage)}
  {}
#endif

  i32 LengthError::errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__
  {
    return 8;
  }

#if __cplusplus < 201103L
  LengthError& LengthError::operator=(const LengthError& other)
  {
    return *this;
  }
#endif

} /* End namespace err */

} /* End namespace nop */

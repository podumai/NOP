#include "exception.hpp"

namespace nop /* Begin namespace nop */
{

  namespace err /* Begin namespace err */
  {

    /*
     * Logic error implementation
     */
    LogicError::LogicError()
      : m_error{"LogicError()"}
    {}

    LogicError::LogicError(const char* errorMessage)
      : m_error{errorMessage}
    {}

    LogicError::LogicError(const std::string& errorMessage)
      : m_error{errorMessage}
    {}

    LogicError::LogicError(std::string&& errorMessage) noexcept
      : m_error{std::move(errorMessage)}
    {}

    const char* LogicError::what() const noexcept
    {
      return m_error.c_str();
    }

    i32 LogicError::errorCode() const noexcept
    {
      return 1;
    }

    /*
     * Invalid argument implementation
     */
    InvalidArgument::InvalidArgument()
      : LogicError{"InvalidArgument()"}
    {}

    InvalidArgument::InvalidArgument(const char* errorMessage)
      : LogicError{errorMessage}
    {}

    InvalidArgument::InvalidArgument(const std::string& errorMessage)
      : LogicError{errorMessage}
    {}

    InvalidArgument::InvalidArgument(std::string&& errorMessage) noexcept
      : LogicError{std::move(errorMessage)}
    {}

    i32 InvalidArgument::errorCode() const noexcept
    {
      return 2;
    }

    /*
     * Runtime error implementation
     */
    RuntimeError::RuntimeError()
      : m_error{"RuntimeError()"}
    {}

    RuntimeError::RuntimeError(const char* errorMessage)
      : m_error{errorMessage}
    {}

    RuntimeError::RuntimeError(const std::string& errorMessage)
      : m_error{errorMessage}
    {}

    RuntimeError::RuntimeError(std::string&& errorMessage) noexcept
      : m_error{std::move(errorMessage)}
    {}

    const char* RuntimeError::what() const noexcept
    {
      return m_error.c_str();
    }

    i32 RuntimeError::errorCode() const noexcept
    {
      return 3;
    }

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

    BadAlloc::BadAlloc(std::string&& errorMessage) noexcept
      : RuntimeError{std::move(errorMessage)}
    {}

    i32 BadAlloc::errorCode() const noexcept
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

    FormatError::FormatError(std::string&& errorMessage) noexcept
      : LogicError{std::move(errorMessage)}
    {}

    i32 FormatError::errorCode() const noexcept
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

    OutOfRange::OutOfRange(std::string&& errorMessage) noexcept
      : LogicError{std::move(errorMessage)}
    {}

    i32 OutOfRange::errorCode() const noexcept
    {
      return 7;
    }

    LengthError::LengthError()
      : LogicError{"LengthError()"}
    {}

    LengthError::LengthError(const char* errorMessage)
      : LogicError{errorMessage}
    {}

    LengthError::LengthError(const std::string& errorMessage)
      : LogicError{errorMessage}
    {}

    LengthError::LengthError(std::string&& errorMessage) noexcept
      : LogicError{std::move(errorMessage)}
    {}

    i32 LengthError::errorCode() const noexcept
    {
      return 8;
    }

  } /* End namespace err */

} /* End namespace nop */

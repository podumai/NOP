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

    i32 SystemError::errorCode() const noexcept
    {
      return 4;
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

    i32 FormatError::errorCode() const noexcept
    {
      return 6;
    }

  } /* End namespace err */

} /* End namespace nop */

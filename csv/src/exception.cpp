#include "exception.hpp"

namespace csv /* Begin namespace sndproc */
{

  namespace err /* Begin namespace err */
  {

    /*
     * Logic error implementation
     */
    logic_error::logic_error()
      : m_error{"logic_error()"}
    {}

    logic_error::logic_error(const char* error_message)
      : m_error{error_message}
    {}

    logic_error::logic_error(const std::string& error_message)
      : m_error{error_message}
    {}

    const char* logic_error::what() const noexcept
    {
      return m_error.c_str();
    }

    int32_t logic_error::err_code() const noexcept
    {
      return 1;
    }

    /*
     * Invalid argument implementation
     */
    invalid_argument::invalid_argument()
      : logic_error{"invalid_argument()"}
    {}

    invalid_argument::invalid_argument(const char* error_message)
      : logic_error{error_message}
    {}

    invalid_argument::invalid_argument(const std::string& error_message)
      : logic_error{error_message}
    {}

    int32_t invalid_argument::err_code() const noexcept
    {
      return 2;
    }

    /*
     * Runtime error implementation
     */
    runtime_error::runtime_error()
      : m_error{"runtime_error()"}
    {}

    runtime_error::runtime_error(const char* error_message)
      : m_error{error_message}
    {}

    runtime_error::runtime_error(const std::string& error_message)
      : m_error{error_message}
    {}

    const char* runtime_error::what() const noexcept
    {
      return m_error.c_str();
    }

    int32_t runtime_error::err_code() const noexcept
    {
      return 3;
    }

    /*
     * System error implementation
     */
    system_error::system_error()
      : runtime_error{"system_error()"}
    {}

    system_error::system_error(const char* error_message)
      : runtime_error{error_message}
    {}

    system_error::system_error(const std::string& error_message)
      : runtime_error{error_message}
    {}

    int32_t system_error::err_code() const noexcept
    {
      return 4;
    }

    /*
     * Configuration error implementation
     */
    configuration_error::configuration_error()
      : runtime_error{"configuration_error()"}
    {}

    configuration_error::configuration_error(const char* error_message)
      : runtime_error{error_message}
    {}

    configuration_error::configuration_error(const std::string& error_message)
      : runtime_error{error_message}
    {}

    int32_t configuration_error::err_code() const noexcept
    {
      return 5;
    }

    /*
     * Format error implementation
     */
    format_error::format_error()
      : logic_error{"format_error()"}
    {}

    format_error::format_error(const char* error_message)
      : logic_error{error_message}
    {}

    format_error::format_error(const std::string& error_message)
      : logic_error{error_message}
    {}

    int32_t format_error::err_code() const noexcept
    {
      return 6;
    }

  } /* End namespace err */

} /* End namespace sndproc */

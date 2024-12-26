#ifndef __NOP_EXCEPTION_HPP__   /* Begin exception header file */
#define __NOP_EXCEPTION_HPP__ 1

#include <string>
#include "types.hpp"

namespace nop /* Begin namespace nop */
{

  namespace err /* Begin namespace err */
  {

    class BaseException
    {
    public:
      virtual const char* what() const noexcept = 0;
      virtual i32 errorCode() const noexcept = 0;
      virtual ~BaseException() = default;
    };

    class LogicError : public BaseException
    {
    private:
      std::string m_error;

    public:
      LogicError();
      LogicError(const char*);
      LogicError(const std::string&);
      LogicError(const LogicError&) = default;
      LogicError(LogicError&&) = default;
      ~LogicError() = default;

      [[nodiscard]] const char* what() const noexcept override;
      [[nodiscard]] i32 errorCode() const noexcept override;

      LogicError& operator=(const LogicError&) = default;
      LogicError& operator=(LogicError&&) = default;
    };

    class InvalidArgument : public LogicError
    {
    public:
      InvalidArgument();
      InvalidArgument(const char*);
      InvalidArgument(const std::string&);
      InvalidArgument(const InvalidArgument&) = default;
      InvalidArgument(InvalidArgument&&) = default;
      ~InvalidArgument() = default;

      [[nodiscard]] i32 errorCode() const noexcept override;

      InvalidArgument& operator=(const InvalidArgument&) = default;
      InvalidArgument& operator=(InvalidArgument&&) = default;
    };

    class RuntimeError : public BaseException
    {
    private:
      std::string m_error;

    public:
      RuntimeError();
      RuntimeError(const char*);
      RuntimeError(const std::string&);
      RuntimeError(const RuntimeError&) = default;
      RuntimeError(RuntimeError&&) = default;
      ~RuntimeError() = default;

      [[nodiscard]] const char* what() const noexcept override;
      [[nodiscard]] i32 errorCode() const noexcept override;

      RuntimeError& operator=(const RuntimeError&) = default;
      RuntimeError& operator=(RuntimeError&&) = default;
    };

    class SystemError : public RuntimeError
    {
    public:
      SystemError();
      SystemError(const char*);
      SystemError(const std::string&);
      SystemError(const SystemError&) = default;
      SystemError(SystemError&&) = default;
      ~SystemError() = default;

      [[nodiscard]] i32 errorCode() const noexcept override;

      SystemError& operator=(const SystemError&) = default;
      SystemError& operator=(SystemError&&) = default;
    };

    class FormatError : public LogicError
    {
    public:
      FormatError();
      FormatError(const char*);
      FormatError(const std::string&);
      FormatError(const FormatError&) = default;
      FormatError(FormatError&&) = default;
      ~FormatError() = default;

      [[nodiscard]] i32 errorCode() const noexcept override;

      FormatError& operator=(const FormatError&) = default;
      FormatError& operator=(FormatError&&) = default;
    };

  } /*End namespace err */

} /* End namespace nop */

#endif /* End exception header file */

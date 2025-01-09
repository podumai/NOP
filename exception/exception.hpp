#ifndef NOP_EXCEPTION_HPP /* Begin exception header file */
#define NOP_EXCEPTION_HPP 1

#include <string>
#include "attributes.hpp"
#include "utils.hpp"
#include "types.hpp"

namespace nop /* Begin namespace nop */
{

namespace err /* Begin namespace err */
{

  class BaseException
  {
  public:
    virtual const char* what() const __NOP_ATTRIBUTE_NOEXCEPT__ = 0;
    virtual i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ = 0;

#if __cplusplus >= 201103L
    virtual ~BaseException() = default;
#else
    virtual ~BaseException()
    {}
#endif
  };

  class LogicError : public BaseException
  {
  private:
    std::string m_error;

  public:
    LogicError();
    LogicError(const char*);
    LogicError(const std::string&);

#if __cplusplus >= 201103L
    LogicError(std::string&&) __NOP_ATTRIBUTE_NOEXCEPT__;
    LogicError(const LogicError&) = default;
    LogicError(LogicError&&) = default;
    ~LogicError() = default;
#else
    LogicError(const LogicError&);
#endif

    __NOP_ATTRIBUTE_NODISCARD__ const char* what() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;
    __NOP_ATTRIBUTE_NODISCARD__ i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;

#if __cplusplus >= 201103L
    LogicError& operator=(const LogicError&) = default;
    LogicError& operator=(LogicError&&) = default;
#else
    LogicError& operator(const LogicError&);
#endif
  };

  class InvalidArgument : public LogicError
  {
  public:
    InvalidArgument();
    InvalidArgument(const char*);
    InvalidArgument(const std::string&);

#if __cplusplus >= 201103L
    InvalidArgument(std::string&&) __NOP_ATTRIBUTE_NOEXCEPT__;
    InvalidArgument(const InvalidArgument&) = default;
    InvalidArgument(InvalidArgument&&) = default;
    ~InvalidArgument() = default;
#else
    InvalidArgument(const InvalidArgument&);
#endif

    __NOP_ATTRIBUTE_NODISCARD__ i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;

#if __cplusplus >= 201103L
    InvalidArgument& operator=(const InvalidArgument&) = default;
    InvalidArgument& operator=(InvalidArgument&&) = default;
#else
    InvalidArgument& operator=(const InvalidArgument&);
#endif
  };

  class RuntimeError : public BaseException
  {
  private:
    std::string m_error;

  public:
    RuntimeError();
    RuntimeError(const char*);
    RuntimeError(const std::string&);

#if __cplusplus >= 201103L
    RuntimeError(std::string&&) noexcept;
    RuntimeError(const RuntimeError&) = default;
    RuntimeError(RuntimeError&&) = default;
    ~RuntimeError() = default;
#else
    RuntimeError(const RuntimeError&);
#endif

    __NOP_ATTRIBUTE_NODISCARD__ const char* what() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;
    __NOP_ATTRIBUTE_NODISCARD__ i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;

#if __cplusplus >= 201103L
    RuntimeError& operator=(const RuntimeError&) = default;
    RuntimeError& operator=(RuntimeError&&) = default;
#else
    RuntimeError& operator=(const RuntimeError&);
#endif
  };

  class SystemError : public RuntimeError
  {
  public:
    SystemError();
    SystemError(const char*);
    SystemError(const std::string&);

#if __cplusplus >= 201103L
    SystemError(std::string&&) noexcept;
    SystemError(const SystemError&) = default;
    SystemError(SystemError&&) = default;
    ~SystemError() = default;
#else
    SystemError(const SystemError&);
#endif

    __NOP_ATTRIBUTE_NODISCARD__ i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;

#if __cplusplus >= 201103L
    SystemError& operator=(const SystemError&) = default;
    SystemError& operator=(SystemError&&) = default;
#else
    SystemError& operator=(const SystemError&);
#endif
  };

  class BadAlloc : public RuntimeError
  {
  public:
    BadAlloc();
    BadAlloc(const char*);
    BadAlloc(const std::string&);

#if __cplusplus >= 201103L
    BadAlloc(std::string&&) noexcept;
    BadAlloc(const BadAlloc&) = default;
    BadAlloc(BadAlloc&&) = default;
    ~BadAlloc() = default;
#else
    BadAlloc(const BadAlloc&);
#endif

    __NOP_ATTRIBUTE_NODISCARD__ i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;

#if __cplusplus >= 201103L
    BadAlloc& operator=(const BadAlloc&) = default;
    BadAlloc& operator=(BadAlloc&&) = default;
#else
    BadAlloc& operator=(const BadAlloc&);
#endif
  };

  class FormatError : public LogicError
  {
  public:
    FormatError();
    FormatError(const char*);
    FormatError(const std::string&);
#if __cplusplus >= 201103L
    FormatError(std::string&&) __NOP_ATTRIBUTE_NOEXCEPT__;
    FormatError(const FormatError&) = default;
    FormatError(FormatError&&) = default;
    ~FormatError() = default;
#else
    FormatError(const FormatError&);
#endif

    __NOP_ATTRIBUTE_NODISCARD__ i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;

#if __cplusplus >= 201103L
    FormatError& operator=(const FormatError&) = default;
    FormatError& operator=(FormatError&&) = default;
#else
    FormatError& operator=(const FormatError&);
#endif
  };

  class OutOfRange : public LogicError
  {
  public:
    OutOfRange();
    OutOfRange(const char*);
    OutOfRange(const std::string&);

#if __cplusplus >= 201103L
    OutOfRange(std::string&&) noexcept;
    OutOfRange(const OutOfRange&) = default;
    OutOfRange(OutOfRange&&) = default;
    ~OutOfRange() = default;
#else
    OutOfRange(const OutOfRange&);
#endif

    __NOP_ATTRIBUTE_NODISCARD__ i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;

#if __cplusplus >= 201103L
    OutOfRange& operator=(const OutOfRange&) = default;
    OutOfRange& operator=(OutOfRange&&) = default;
#else
    OutOfRange& operator=(const OutOfRange&);
#endif
  };

  class LengthError : public LogicError
  {
  public:
    LengthError();
    LengthError(const char*);
    LengthError(const std::string&);

#if __cplusplus >= 201103L
    LengthError(std::string&&) noexcept;
    LengthError(const LengthError&) = default;
    LengthError(LengthError&&) = default;
    ~LengthError() = default;
#else
    LengthError(const LengthError&);
#endif

    __NOP_ATTRIBUTE_NODISCARD__ i32 errorCode() const __NOP_ATTRIBUTE_NOEXCEPT__ __NOP_ATTRIBUTE_OVERRIDE__;

#if __cplusplus >= 201103L
    LengthError& operator=(const LengthError&) = default;
    LengthError& operator=(LengthError&&) = default;
#else
    LengthError& operator=(const LengthError&);
#endif
  };

} /*End namespace err */

} /* End namespace nop */

#endif /* End exception header file */

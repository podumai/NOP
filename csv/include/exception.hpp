#ifndef __CSV_PARSER_EXCEPTION_HPP__   /* Begin exception header file */
#define __CSV_PARSER__EXCEPTION_HPP__ 1

#include <string>

namespace csv /* Begin namespace csv */
{

  namespace err /* Begin namespace err */
  {

    class base_exception
    {
    public:
      virtual const char* what() const noexcept = 0;
      virtual int32_t err_code() const noexcept = 0;
      virtual ~base_exception() = default;
    };

    class logic_error : public base_exception
    {
    private:
      std::string m_error;

    public:
      logic_error();
      logic_error(const char*);
      logic_error(const std::string&);
      logic_error(const logic_error&) = delete;
      logic_error(logic_error&&) = delete;
      ~logic_error() = default;

      [[nodiscard]] const char* what() const noexcept override;
      [[nodiscard]] int32_t err_code() const noexcept override;

      logic_error& operator=(const logic_error&) = delete;
      logic_error& operator=(logic_error&&) = delete;
    };

    class invalid_argument : public logic_error
    {
    public:
      invalid_argument();
      invalid_argument(const char*);
      invalid_argument(const std::string&);
      invalid_argument(const invalid_argument&) = delete;
      invalid_argument(invalid_argument&&) = delete;
      ~invalid_argument() = default;

      [[nodiscard]] int32_t err_code() const noexcept override;

      invalid_argument& operator=(const invalid_argument&) = delete;
      invalid_argument& operator=(invalid_argument&&) = delete;
    };

    class runtime_error : public base_exception
    {
    private:
      std::string m_error;

    public:
      runtime_error();
      runtime_error(const char*);
      runtime_error(const std::string&);
      runtime_error(const runtime_error&) = delete;
      runtime_error(runtime_error&&) = delete;
      ~runtime_error() = default;

      [[nodiscard]] const char* what() const noexcept override;
      [[nodiscard]] int32_t err_code() const noexcept override;

      runtime_error& operator=(const runtime_error&) = delete;
      runtime_error& operator=(runtime_error&&) = delete;
    };

    class system_error : public runtime_error
    {
    public:
      system_error();
      system_error(const char*);
      system_error(const std::string&);
      system_error(const system_error&) = delete;
      system_error(system_error&&) = delete;
      ~system_error() = default;

      [[nodiscard]] int32_t err_code() const noexcept override;

      system_error& operator=(const system_error&) = delete;
      system_error& operator=(system_error&&) = delete;
    };

    class configuration_error : public runtime_error
    {
    public:
      configuration_error();
      configuration_error(const char*);
      configuration_error(const std::string&);
      configuration_error(const configuration_error&) = delete;
      configuration_error(configuration_error&&) = delete;
      ~configuration_error() = default;

      [[nodiscard]] int32_t err_code() const noexcept override;

      configuration_error& operator=(const configuration_error&) = delete;
      configuration_error& operator=(configuration_error&&) = delete;
    };

    class format_error : public logic_error
    {
    public:
      format_error();
      format_error(const char*);
      format_error(const std::string&);
      format_error(const format_error&) = delete;
      format_error(format_error&&) = delete;
      ~format_error() = default;

      [[nodiscard]] int32_t err_code() const noexcept override;

      format_error& operator=(const format_error&) = delete;
      format_error& operator=(format_error&&) = delete;
    };

  } /*End namespace err */

} /* End namespace csv */

#endif /* End exception header file */

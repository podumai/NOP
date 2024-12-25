#include <string>
#include "command.hpp"
#include "exception.hpp"

namespace csv /* Begin namespace csv */
{

  namespace cmd /* Begin namespace cmd */
  {

    data_handler::data_handler(int32_t argc, char* argv[])
    {
      if (2 <= argc && argc <= 3)
      {
        m_data.first = argv[1];
        
        if (m_data.first.ends_with(".csv") == false)
          goto ERROR;

        if (argc == 3)
        {
          std::string digit{argv[2]};

          for (const auto& symbol : digit)
            if (symbol > '9' || symbol < '0')
              goto ERROR;

          m_data.second = std::strtoull(digit.c_str(), NULL, 10UL);
        }
        else
          m_data.second = 0;
      }
      else
      {
ERROR:
        using namespace std::string_literals;

        std::string error_message{
            "\033[1;35m[ERROR]\033[0m Invalid parameters.\n"
            "\033[1;35m[MESSAGE]\033[0m Requires <file.csv> <skip_lines> (optional)\n"
            "\033[1;35m[MESSAGE]\033[0m Recieved: "};

        if (argc > 1)
        {
          for (int32_t i{1}; i < argc; ++i)
            error_message += "<"s + argv[i] + "> ";
        }
        else
          error_message += "<None>";

        throw csv::err::invalid_argument{error_message};
      }
    }

    const char* data_handler::get_file_name() const noexcept
    {
      return m_data.first.data();
    }

    size_t data_handler::get_skip_lines() const noexcept
    {
      return m_data.second;
    }

  } /* End namespace cmd */

} /* End namespace csv */

#define __INVALID_FILE__ (7UL)

#include <iostream>
#include "parser.hpp"
#include "command.hpp"

int32_t main(int32_t argc, char* argv[])
{
  std::ios_base::sync_with_stdio(false);

  try
  {
    csv::cmd::data_handler input_data{argc, argv};
    std::ifstream in{input_data.get_file_name()};
    csv::parser<int32_t, std::string> prs{in, input_data.get_skip_lines()};

    for (auto&& i : prs)
      std::cout << i << '\n';
  }
  catch (const csv::err::base_exception& error)
  {
    std::cerr << error.what() << '\n';
    return error.err_code();
  }

  return EXIT_SUCCESS;
}

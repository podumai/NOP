#include <iostream>
#include "parser.hpp"
#include "command.hpp"

int32_t main(int32_t argc, char* argv[])
{
  std::ios_base::sync_with_stdio(false);

  try
  {
    csv::cmd::DataHandler inputData{argc, argv};
    std::ifstream in{inputData.getFileName()};
    nop::csv::Parser<nop::csv::DefaultCfg, int32_t, std::string> prs{in, inputData.getSkipLines()};

    for (auto&& i : prs)
      std::cout << i << '\n';
  }
  catch (const nop::err::BaseException& error)
  {
    std::cerr << error.what() << '\n';
    return error.errorCode();
  }

  return EXIT_SUCCESS;
}

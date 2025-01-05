#include <iostream>
#include <vector>
#include "smart_ptr.hpp"

#define example(...) nop::makeSmartPtr<std::vector<nop::i32>\
                                      , nop::AllowConversion\
                                      , nop::NoSafety\
                                      , nop::MultipleStorage>(__VA_ARGS__)

nop::i32 main()
{
  try
  {
    auto ptr{example(10UL, 2UL, 52)};

    for (nop::size_t i{}; i < 10UL; ++i)
    {
      std::cout << "Vector in Ptr[" << i << "]:\n";
      for (auto&& iterator : ptr[i])
      {
        std::cout << iterator << ' ';
      }
      std::cout << '\n';
    }
  }
  catch (const nop::err::BaseException& error)
  {
    std::cerr << error.what() << '\n';
    return error.errorCode();
  }

  return 0;
}

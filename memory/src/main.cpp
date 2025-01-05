#include <iostream>
#include <vector>
#include "smart_ptr.hpp"

nop::i32 main()
{
  auto ptr{nop::makeSmartPtr<std::vector<nop::i32>, nop::AllowConversion, nop::NoSafety, nop::MultipleStorage>(10UL, 2UL, 52)};

  try
  {
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

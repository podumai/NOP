#include <iostream>
#include "smart_ptr.hpp"

nop::i32 main()
{
  auto ptr{nop::makeSmartPtr<nop::i32>(52)};

  std::cout << *ptr << std::endl;

  return 0;
}

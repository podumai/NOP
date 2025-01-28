#include <iostream>
#include <cstdint>

#include "smart_ptr.hpp"

#define example(...) nop::make_smart_ptr<std::vector<std::int32_t>,\
                                         nop::strategy::all_conversion,\
                                         nop::strategy::no_safety,\
                                         nop::strategy::multiple_storage>(__VA_ARGS__)

std::int32_t main()
{
  try
  {
    auto ptr{example(10UL, 2UL, 52)};

    for (std::size_t i{}; i < 10UL; ++i)
    {
      std::cout << "Vector in Ptr[" << i << "]:\n";
      for (auto&& iterator : ptr[i])
      {
        std::cout << iterator << ' ';
      }
      std::cout.put('\n');
    }
  }
  catch (const nop::err::BaseException& error)
  {
    std::cerr << error.what() << '\n';
    return error.errorCode();
  }
  catch (const std::exception& error)
  {
    std::cerr << error.what() << '\n';
    return 1;
  }

  return 0;
}

#ifndef NOP_TYPE_TRAITS_BASIC_TRAITS_HPP /* Begin basic traits header file */
#define NOP_TYPE_TRAITS_BASIC_TRAITS_HPP 1

namespace nop /* Begin namespace nop */
{

  struct FalseType
  {
  public:
    static constexpr bool value{false};
  };

  struct TrueType
  {
  public:
    static constexpr bool value{true};
  };

} /* End namespace nop */

#endif /* End basic traits header file */

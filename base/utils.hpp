#ifndef NOP_UTILS_HPP /* Begin Utils header file */
#define NOP_UTILS_HPP 1

#if defined(__cplusplus)
  #if __cplusplus >= 201103L
    #define __NOP_THROW_EXCEPTION(excName, message) do { throw excName{message}; } while (false)
    #define __NOP_NULLPTR__ nullptr
    #define __NOP_ATTRIBUTE_OVERRIDE__ override
    #define __NOP_ATTRIBUTE_FUNC_CONSTEXPR__ constexpr
    #define __NOP_ATTRIBUTE_CTOR_CONSTEXPR__ constexpr
  #else
    #define __NOP_THROW_EXCEPTION(excName, message) do { throw excName(message); } while (false)
    #define __NOP_NULLPTR__ NULL
    #define __NOP_ATTRIBUTE_OVERRIDE__
    #define __NOP_ATTRIBUTE_MCTOR_CONSTEXPR__
    #define __NOP_ATTRIBUTE_DTOR_CONSTEXPR__
    #define __NOP_ATTRIBUTE_FUNC_CONSTEVAL__
  #endif
  #if __cplusplus >= 201402L
    #define __NOP_ATTRIBUTE_MCTOR_CONSTEXPR__ constexpr
  #endif
  #if __cplusplus >= 202002L
    #define __NOP_ATTRIBUTE_DTOR_CONSTEXPR__ constexpr
    #define __NOP_ATTRIBUTE_FUNC_CONSTEVAL__ consteval
  #endif
#else
  #define __NOP_THROW_EXCEPTION(excName, message)
  #define __NOP_NULLPTR__ NULL
  #define __NOP_ATTRIBUTE_OVERRIDE__
  #define __NOP_ATTRIBUTE_CTOR_CONSTRXPR__
  #define __NOP_ATTRIBUTE_MCTOR_CONSTEXPR__
  #define __NOP_ATTRIBUTE_DTOR_CONSTEXPR__
  #define __NOP_ATTRIBUTE_FUNC_CONSTEXPR__
  #define __NOP_ATTRIBUTE_FUNC_CONSTEVAL__
#endif

#endif /* End Util header file */

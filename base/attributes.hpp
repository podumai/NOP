#ifndef NOP_ATTRIBUTES_HPP /* Begin Attributes header file */
#define NOP_ATTRIBUTES_HPP 1

#ifndef __NOP_DISABLE_ATTRIBUTES__
  #ifdef __has_attribute
    #define __NOP_HAS_ATTRIBUTE(expr) __has_attribute(expr)
  #else
    #define __NOP_HAS_ATTRIBUTE(expr) 0UL
  #endif

  #if defined(__cplusplus) && defined(__has_cpp_attribute)
    #define __NOP_HAS_CPP_ATTRIBUTE(expr) __has_cpp_attribute(expr)
  #else
    #define __NOP_HAS_CPP_ATTRIBUTE(expr) 0UL
  #endif

  #if __NOP_HAS_ATTRIBUTE(always_inline) ||\
      defined(__GNUC__) && !defined(__clang__)
    #define __NOP_ATTRIBUTE_ALWAYS_INLINE__ __attribute__((always_inline))
    #define __NOP_HAS_ATTRIBUTE_ALWAYS_INLINE__ 1UL
  #else
    #define __NOP___NOP_ATTRIBUTE_ALWAYS_INLINE__
  #endif

  #if __NOP_HAS_ATTRIBUTE(noinline) ||\
      (defined(__GNUC__) && !defined(__clang__))
    #define __NOP_ATTRIBUTE_NOINLINE__ __attribute__((noinline))
    #define __NOP_HAS_ATTRIBUTE_NOINLINE__ 1UL
  #else
    #define __NOP_ATTRIBUTE_NOINLINE__
  #endif

  #if __NOP_HAS_ATTRIBUTE(noreturn) ||\
      (defined(__GNUC__) && !defined(__clang__))
    #if (defined(__cplusplus) && __cplusplus >= 201103L) || __STDC_VERSION__ >= 202311L
      #define __NOP_ATTRIBUTE_NORETURN__ [[noreturn]]
    #else
      #define __NOP_ATTRIBUTE_NORETURN__ __attribute__((noreturn))
    #endif
    #define __NOP_HAS_ATTRIBUTE_NORETURN__ 1UL
  #else
    #define __NOP_ATTRIBUTE_NORETURN__
  #endif

  #if __NOP_HAS_ATTRIBUTE(warn_unused_result) ||\
      (defined(__GNUC__) && !defined(__clang__))
    #if (defined(__cplusplus) && __cplusplus >= 201703L) ||\
        (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L)
      #define __NOP_ATTRIBUTE_NODISCARD__ [[nodiscard]]
    #else
      #define __NOP_ATTRIBUTE_NODISCARD__ __attribute__((warn_unused_result))
    #endif
    #define __NOP_HAS_ATTRIBUTE_NODISCARD__ 1UL
  #else
    #define __NOP_ATTRIBUTE_NODISCARD__
  #endif

  #if __NOP_HAS_CPP_ATTRIBUTE(no_unique_address) && __cplusplus >= 202002L
    #define __NOP_ATTRIBUTE_NO_UNIQUE_ADDRESS__ [[no_unique_address]]
    #define __NOP_HAS_ATTRIBUTE_NO_UNIQUE_ADDRESS 1UL
  #else
    #define __NOP_ATTRIBUTE_NO_UNIQUE_ADDRESS__
  #endif

  #if defined(__cplusplus)
    #if __cplusplus >= 201103L
      #define __NOP_TYPENAME__ typename
    #else
      #define __NOP_TYPENAME__ class
    #endif
  #endif

  #if defined(__cplusplus)
    #if __cplusplus >= 201103L
      #define __NOP_ATTRIBUTE_NOEXCEPT_EXPR(expr) noexcept(expr)
      #define __NOP_ATTRIBUTE_NOEXCEPT__ noexcept
    #else
      #define __NOP_ATTRIBUTE_NOEXCEPT_EXPR(expr)
      #define __NOP_ATTRIBUTE_NOEXCEPT__ throw()
    #endif
  #elif defined(__STDC_VERSION__)
    #if !defined(_MSC_VER)
      #define __NOP_ATTRIBUTE_NOEXCEPT_EXPR(expr)
      #define __NOP_ATTRIBUTE_NOEXCEPT__ __attribute__((nothrow))
    #else
      #define __NOP_ATTRIBUTE_NOEXCEPT_EXPR(expr)
      #define __NOP_ATTRIBUTE_NOEXCEPT__ __declspec(nothrow)
    #endif
  #endif

#endif

#endif /* End Attributes header file */

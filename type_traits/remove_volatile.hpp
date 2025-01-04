#ifndef NOP_TYPE_TRAITS_REMOVE_VOLATILE_HPP /* Begin Remove volatile header file */
#define NOP_TYPE_TRAITS_REMOVE_VOLATILE_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct RemoveVolatile
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemoveVolatile<volatile T>
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemoveVolatile<T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemoveVolatile<volatile T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemoveVolatile<T&&>
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct RemoveVolatile<volatile T&&>
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct RemoveVolatile<T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveVolatile<volatile T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveVolatile<volatile T* const>
  {
  public:
    typedef T* const type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using RemoveVolatileT = typename RemoveVolatile<T>::type;
#endif

} /* End namespace nop */

#endif /* End Remove volatile header file */

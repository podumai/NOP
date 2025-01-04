#ifndef NOP_TYPE_TRAITS_REMOVE_POINTER_HPP /* Begin Remove pointer header file */
#define NOP_TYPE_TRAITS_REMOVE_POINTER_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct RemovePointer
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemovePointer<T*>
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemovePointer<T* const>
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemovePointer<T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemovePointer<T&&>
  {
  public:
    typedef T&& type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using RemovePointerT = typename RemovePointer<T>::type;
#endif

} /* End namespace nop */

#endif /* End Remove pointer header file */

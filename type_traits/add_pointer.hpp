#ifndef NOP_TYPE_TRAITS_ADD_POINTER_HPP /* Begin Add pointer header file */
#define NOP_TYPE_TRAITS_ADD_POINTER_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct AddPointer
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct AddPointer<T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct AddPointer<T&&>
  {
  public:
    typedef T&& type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using AddPointerT = typename AddPointer<T>::type;
#endif

} /* End namespace nop */

#endif /* End Add pointer header file */

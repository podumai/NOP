#ifndef NOP_TYPE_TRAITS_ADD_RVALUE_REFERENCE_HPP /* Begin Add rvalue reference header file */
#define NOP_TYPE_TRAITS_ADD_RVALUE_REFERENCE_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct AddRvalueReference
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct AddRvalueReference<T*>
  {
  public:
    typedef T*&& type;
  };

  template<typename T>
  struct AddRvalueReference<T* const>
  {
  public:
    typedef T* const&& type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using AddRvalueReferenceT = typename AddRvalueReference<T>::type;
#endif

} /* End namespace nop */

#endif /* End Add rvalue reference header file */

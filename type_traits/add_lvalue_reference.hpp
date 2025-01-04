#ifndef NOP_TYPE_TRAITS_ADD_LVALUE_REFERENCE_HPP /* Begin Add lvalue reference header file */
#define NOP_TYPE_TRAITS_ADD_LVALUE_REFERENCE_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct AddLvalueReference
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct AddLvalueReference<T*>
  {
  public:
    typedef T*& type;
  };

  template<typename T>
  struct AddLvalueReference<T* const>
  {
  public:
    typedef T* const& type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using AddLvalueReferenceT = typename AddLvalueReference<T>::type;
#endif

} /* End namespace nop */

#endif /* End Add lvalue reference header file */

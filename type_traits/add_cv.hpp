#ifndef NOP_TYPE_TRAITS_ADD_CV_HPP /* Begin Add cv header file */
#define NOP_TYPE_TRAITS_ADD_CV_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct AddCV
  {
  public:
    typedef volatile const T type;
  };

  template<typename T>
  struct AddCV<T&>
  {
  public:
    typedef volatile const T& type;
  };

  template<typename T>
  struct AddCV<T&&>
  {
  public:
    typedef volatile const T&& type;
  };

  template<typename T>
  struct AddCV<T*>
  {
  public:
    typedef volatile const T* type;
  };

  template<typename T>
  struct AddCV<T* const>
  {
  typedef volatile const T* const type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using AddCVT = typename AddCV<T>::type;
#endif

} /* End namespace nop */

#endif /* End Add cv header file */

#ifndef NOP_TYPE_TRAITS_REMOVE_CV_HPP /* Begin Remove cv header file */
#define NOP_TYPE_TRAITS_REMOVE_CV_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct RemoveCV
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemoveCV<const T>
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemoveCV<volatile const T>
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemoveCV<T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemoveCV<const T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemoveCV<volatile T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemoveCV<volatile const T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemoveCV<T&&>
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct RemoveCV<const T&&>
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct RemoveCV<volatile T&&>
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct RemoveCV<volatile const T&&>
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct RemoveCV<T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveCV<const T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveCV<volatile T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveCV<volatile const T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveCV<T* const>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveCV<volatile T* const>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveCV<volatile const T* const>
  {
  public:
    typedef T* type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using RemoveCVT = typename RemoveCV<T>::type;
#endif

} /* End namespace nop */

#endif /* End Remove cv header file */

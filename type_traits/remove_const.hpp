#ifndef NOP_TYPE_TRAITS_REMOVE_CONST_HPP /* Begin Remove const header file */
#define NOP_TYPE_TRAITS_REMOVE_CONST_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct RemoveConst
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemoveConst<const T>
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemoveConst<T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemoveConst<const T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct RemoveConst<T&&>
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct RemoveConst<const T&&>
  {
  public:
    typedef T&& type;
  };

  template<typename T>
  struct RemoveConst<T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveConst<const T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveConst<T* const>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveConst<const T* const>
  {
  public:
    typedef T* type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using RemoveConstT = typename RemoveConst<T>::type;
#endif

} /* End namespace nop */

#endif /* End Remove const header file */

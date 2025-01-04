#ifndef NOP_TYPE_TRAITS_TYPE_IDENTITY_HPP /* Begin Type identity header file */
#define NOP_TYPE_TRAITS_TYPE_IDENTITY_HPP 1

namespace nop /* Begin namespace nop */
{

  template<typename T>
  struct TypeIdentity
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct TypeIdentity<T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct TypeIdentity<T* const>
  {
  public:
    typedef T* const type;
  };

  template<typename T>
  struct TypeIdentity<T&>
  {
  public:
    typedef T& type;
  };

  template<typename T>
  struct TypeIdentity<T&&>
  {
  public:
    typedef T&& type;
  };

#if __cplusplus >= 201103L
  template<typename T>
  using TypeIdentityT = typename TypeIdentity<T>::type;
#endif

} /* End namespace nop */

#endif /* End Type identity header file */

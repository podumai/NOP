#ifndef NOP_TYPE_TRAITS_REMOVE_REFERENCE_HPP
#define NOP_TYPE_TRAITS_REMOVE_REFERENCE_HPP 1

namespace nop
{

  template<typename T>
  struct RemoveReference
  {
  public:
    typedef T type;
  };

  template<typename T>
  struct RemoveReference<T*>
  {
  public:
    typedef T* type;
  };

  template<typename T>
  struct RemoveReference<T* const>
  {
  public:
    typedef T* const type;
  };

  template<typename T>
  struct RemoveReference<T&>
  {
    typedef T type;
  };

  template<typename T>
  struct RemoveReference<T&&>
  {
    typedef T type;
  };

#if __cplusplus >= 201402L
  template<typename T>
  using RemoveReferenceT = typename RemoveReference<T>::type;
#endif

}

#endif

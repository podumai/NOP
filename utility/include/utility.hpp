#ifndef __LIB_UTILITY__
#define __LIB_UTILITY__ 1

namespace npl
{

  namespace utility
  {

    template<class T1, class T2>
    struct pair
    {
    public:
      using value_type1 = T1;
      using value_type2 = T2;
      using pointer1    = T1*;
      using pointer2    = T2*;

    public:
      value_type1 first;
      value_type2 second;

    public:
      pair()
        : first()
        , second()
      {}

      pair(pointer1 fvalue, pointer2 svalue)
        : first(fvalue)
        , second(svalue)
      {}

      pair(value_type1& fvalue, value_type2& svalue)
        : first(fvalue)
        , second(svalue)
      {}

      pair(value_type1&& fvalue, value_type2&& svalue)
        : first(std::move(fvalue))
        , second(std::move(svalue))
      {}

      pair(const pair& other)
        : first(other.first)
        , second(other.second)
      {}

      pair(pair&& other)
        : first(std::move(other.first))
        , second(std::move(other.second))
      {}

      ~pair() = default;

      pair& operator=(const pair& other)
      {
        first = other.first;
        second = other.second;
      }

      pair& operator=(pair&& other)
      {
        first = std::move(other.first);
        second = std::move(other.second);
      }
    };

    template<class T1, class T2>
    pair<T1, T2> make_pair()
    {
      return pair<T1, T2>();
    }

    template<class T1, class T2>
    pair<T1*, T2*> make_pair(T1* fvalue, T2* svalue)
    {
      return pair<T1*, T2*>(fvalue, svalue);
    }

    template<class T1, class T2>
    pair<T1, T2> make_pair(T1& fvalue, T2& svalue)
    {
      return pair<T1, T2>(fvalue, svalue);
    }

    template<class T1, class T2>
    pair<T1, T2> make_pair(T1&& fvalue, T2&& svalue)
    {
      return pair<T1, T2>(fvalue, svalue);
    }

    

  }

}

template<class T1, class T2>
[[nodiscard]] bool operator==(const npl::utility::pair<T1, T2>& lhs,
                              const npl::utility::pair<T1, T2>& rhs)
{
  return (lhs.first == rhs.first) && (lhs.second == rhs.second);
}

template<class T1, class T2>
[[nodiscard]] bool operator!=(const npl::utility::pair<T1, T2>& lhs,
                              const npl::utility::pair<T1, T2>& rhs)
{
  return !(lhs == rhs);
}

#endif
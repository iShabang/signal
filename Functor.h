#ifndef _ISHABANG_SIGNAL_FUNCTOR_H_
#define _ISHABANG_SIGNAL_FUNCTOR_H_

#include <functional>

namespace sig {

template <typename R, typename ... Types>
class Functor {
 public:
  Functor(std::function<R(Types...)> func) : m_function(func) {}
  std::function<R(Types...)> m_function;
  void operator()(Types...args) const { this->m_function(args...); }
};

} // namespace sig

#endif //_ISHABANG_SIGNAL_FUNCTOR_H_

#ifndef _ISHABANG_SIGNAL_SIGNAL_H_
#define _ISHABANG_SIGNAL_SIGNAL_H_

#include <list>
#include <memory>

#include "Connection.h"
#include "Slot.h"

namespace sig {

template <typename R, typename ... Types>
class Signal {
  using conn_ptr = std::shared_ptr<Connection<R, Types...>>;

 public:
  Signal() : listCount(0) {}
  conn_ptr connect(std::function<R(Types...)> func) {
    auto slot = Slot<R, Types...>(func, ++listCount);
    m_slots.insert(slot);
    return conn_ptr(new Connection<R, Types...>(slot, [this](const Slot<R, Types...> &slot) { m_slots.erase(slot); }));
  }
  void operator()(Types...args) {
    for (auto &&i : this->m_slots) {
      i(args...);
    }
  }
  SlotList<R, Types...> m_slots;
  uint32_t listCount;
};

} //namespace sig
#endif //_ISHABANG_SIGNAL_SIGNAL_H_

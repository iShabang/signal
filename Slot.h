#ifndef _ISHABANG_SIGNAL_SLOT_H_
#define _ISHABANG_SIGNAL_SLOT_H_

#include <unordered_set>
#include <cstdint>

#include "Functor.h"

namespace sig {

template <typename R, typename ... Types>
class Slot {
 public:
  Slot(std::function<R(Types...)> func, std::size_t id) : m_functor(func), m_id(id) {}
  void operator()(Types...args) const { this->m_functor(args...); }
  Functor<R,Types...> m_functor;
  uint32_t m_id;
};

template <typename R, typename ... Types>
struct SlotHash {
  std::size_t operator()(const Slot<R,Types...> &slot) const { return std::hash<uint32_t>()(slot.m_id); }
};

template <typename R, typename ... Types>
struct SlotCompare {
  bool operator()(const Slot<R,Types...> &slot1, const Slot<R,Types...> &slot2) const {
    return slot1.m_id == slot2.m_id;
  }
};

template <typename R, typename ... Types>
class SlotList {
 private:
  using hash_func = SlotHash<R, Types...>;
  using comp_func = SlotCompare<R, Types...>;
  using list_type = std::unordered_set<Slot<R, Types...>, hash_func, comp_func>;
  using slot_iter = typename list_type::iterator;
  using slot_const_iter = typename list_type::const_iterator;

 public:
  using iterator = slot_iter;
  using const_iterator = slot_const_iter;
  void insert(Slot<R, Types...> slot) { m_slots.insert(slot); }
  void erase(Slot<R, Types...> slot) { m_slots.erase(slot); }
  slot_iter begin() { return m_slots.begin(); }
  slot_iter end() { return m_slots.end(); }

 private:
  list_type m_slots;
};

} // namespace sig

#endif //_ISHABANG_SIGNAL_SLOT_H_

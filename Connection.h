#ifndef _ISHABANG_SIGNAL_CONNECTION_H_
#define _ISHABANG_SIGNAL_CONNECTION_H_

#include <memory>

#include "Slot.h"

namespace sig {

/* Connection
 * A connection describes the relationship between a signal and a connected slot.
 * The use of the Connection object allows easy disconnection as well as a way
 * of testing whether a signal is connected to a specific slot.
 *
 * In most cases, this object is not used directly. ScopedConnections will typically be
 * used to manage the connection object.
 *
 */
template <typename R, typename ... Types>
class Connection {
  using slot_type = Slot<R, Types...>;
  using disconnect_func = std::function<void(slot_type)>;

 public:
  Connection(slot_type slot, disconnect_func func) : m_slot(slot), m_disconnectFunc(func) {}

  void disconnect() { m_disconnectFunc(m_slot); }

  bool connected() {
    if (m_disconnectFunc)
    {
      return true;
    }
    return false;
  }

 private:
  slot_type m_slot;
  disconnect_func m_disconnectFunc;
};

/* ScopedConnection
 * A different type of connection object that provides no interaction. The connection object is
 * managed automatically. If the ScopedConnection goes out of scope, the internal connection object
 * is disconnected.
 *
 * TODO: Add operator= overload and a disconnect method. This will allow more dynamic use of the
 * object in the event that a connection object can dynamically change during the life of the
 * program. The current implementation only allows a one time connection that cannot be changed
 * without destroying the object and building a new one.
 */
class ScopedConnection {
 public:
  template <typename R, typename ... Types>
  ScopedConnection(Connection<R, Types...> &conn)
      : destructor([&conn]() { conn.disconnect(); }), connected(true) {}

  template <typename R, typename ... Types>
  ScopedConnection(std::shared_ptr<Connection<R, Types...>> conn)
      : destructor([conn]() { conn->disconnect(); }), connected(true) {}

  ScopedConnection() : connected(false) {}

  void disconnect() {
    if (connected) destructor();
    connected = false;
  };

  ~ScopedConnection() {
    if (connected) destructor();
  }

 private:
  std::function<void()> destructor;
  bool connected;
};

} // namespace sig
#endif //_ISHABANG_SIGNAL_CONNECTION_H_

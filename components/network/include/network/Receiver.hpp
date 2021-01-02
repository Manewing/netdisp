#ifndef NETWORK_RECEIVER_HPP
#define NETWORK_RECEIVER_HPP

#include <cstdint>

namespace network {

class Receiver {
public:
  virtual int recv(void *Dest, std::size_t Len) const = 0;
  virtual void close() = 0;
};

} // namespace network

#endif // #ifndef NETWORK_RECEIVER_HPP
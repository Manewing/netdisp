#ifndef NETWORK_UDP_RECEIVER_HPP
#define NETWORK_UDP_RECEIVER_HPP

#include <network/Receiver.hpp>

namespace network {

class UdpReceiver : public Receiver {
public:
  UdpReceiver(int Port, bool IPv6 = false);
  virtual ~UdpReceiver();

  bool isReady() const;

  int recv(void *Dest, std::size_t Len) const override;

  void close() override;

private:
  int Socket = -1;
};

}

#endif // #ifndef NETWORT_UDP_RECEIVER_HPP
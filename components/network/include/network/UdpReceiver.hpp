#ifndef NETWORK_UDP_RECEIVER_HPP
#define NETWORK_UDP_RECEIVER_HPP

#include <network/AsyncReceiver.hpp>
#include <network/Receiver.hpp>

namespace network {

class UdpReceiver : public Receiver {
public:
  UdpReceiver(int Port, bool Ipv6 = false);
  virtual ~UdpReceiver();

  bool isReady() const;

  int recv(void *Dest, std::size_t Len) const override;

  void close() override;

private:
  int Socket = -1;
};

class AsyncUdpReceiver : protected AsyncReceiver {
public:
  AsyncUdpReceiver(int Port, unsigned MaxMsgLen, bool Ipv6 = false);

  using AsyncReceiver::onRecv;

  bool isReady() const override;

private:
  UdpReceiver UdpRecv;
};

} // namespace network

#endif // #ifndef NETWORT_UDP_RECEIVER_HPP
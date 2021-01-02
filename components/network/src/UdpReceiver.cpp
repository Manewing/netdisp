#include <network/UdpReceiver.hpp>

#include <cstring>
#include <stdexcept>

#include <esp_netif.h>
#include <lwip/sockets.h>

namespace network {

UdpReceiver::UdpReceiver(int Port, bool Ipv6) {
  Socket = socket(Ipv6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (Socket < 0) {
    return;
  }

  struct sockaddr_in6 Addr;
  if (Ipv6) {
    Addr.sin6_family = AF_INET6;
    Addr.sin6_addr = in6addr_any;
    Addr.sin6_port = htons(Port);
  } else {
    struct sockaddr_in *AddrPtr = reinterpret_cast<struct sockaddr_in *>(&Addr);
    AddrPtr->sin_family = AF_INET;
    AddrPtr->sin_port = htons(Port);
    AddrPtr->sin_addr.s_addr = htonl(INADDR_ANY);
  }

  int Enable = 1;
  int Ret = setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &Enable, sizeof(int));
  if (Ret < 0) {
    close();
    return;
  }

  Ret = bind(Socket, reinterpret_cast<struct sockaddr *>(&Addr), sizeof(Addr));
  if (Ret < 0) {
    close();
    return;
  }
}

UdpReceiver::~UdpReceiver() { close(); }

bool UdpReceiver::isReady() const { return Socket >= 0; }

int UdpReceiver::recv(void *Dest, std::size_t Len) const {
  return ::recv(Socket, Dest, Len, 0);
}

void UdpReceiver::close() {
  if (isReady()) {
    ::close(Socket);
  }
  Socket = -1;
}

AsyncUdpReceiver::AsyncUdpReceiver(int Port, unsigned MaxMsgLen, bool Ipv6)
    : AsyncReceiver(UdpRecv, MaxMsgLen), UdpRecv(Port, Ipv6) {
  start();
}

bool AsyncUdpReceiver::isReady() const {
  return UdpRecv.isReady() && AsyncReceiver::isReady();
}

} // namespace network

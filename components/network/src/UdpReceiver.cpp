#include <network/UdpReceiver.hpp>

#include <cstring>
#include <stdexcept>

#include <esp_netif.h>
#include <lwip/sockets.h>

namespace network {

UdpReceiver::UdpReceiver(int Port, bool IPv6) {
  Socket = socket(IPv6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (Socket < 0) {
    return;
  }

  struct sockaddr_in6 Addr;
  if (IPv6) {
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
    close(Socket);
    Socket = -1;
    return;
  }

  Ret = bind(Socket, reinterpret_cast<struct sockaddr *>(&Addr), sizeof(Addr));
  if ( Ret < 0) {
    close(Socket);
    Socket = -1;
    return;
  }
}

bool UdpReceiver::isReady() const {
  return Socket >= 0;
}

int UdpReceiver::recv(void * Dest, std::size_t Len) const {
  return ::recv(Socket, Dest, Len, 0);
}

} // namespace network

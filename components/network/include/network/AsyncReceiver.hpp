#ifndef NETWORK_ASYNC_RECEIVER_HPP
#define NETWORK_ASYNC_RECEIVER_HPP

#include <stdint.h>
#include <functional>
#include <network/Receiver.hpp>

namespace network {

class AsyncReceiver {
public:
  typedef std::function<void(uint8_t*, int)> Callback;

public:
  AsyncReceiver(Receiver &Recv, unsigned MaxMsgLen = 256);
  virtual ~AsyncReceiver();

  void start();

  virtual bool isReady() const;

  void onRecv(Callback OnRecv);

  void close();

  Receiver &getReceiver();

  unsigned getMaxMsgLen() const;

  Callback const &getCallback() const;

private:
  Receiver &Recv;
  unsigned MaxMsgLen;
  Callback OnRecv;
  void *TaskHandle = nullptr;
};

}

#endif // #ifndef NETWORK_ASYNC_RECEIVER_HPP
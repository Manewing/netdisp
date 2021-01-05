#include <cstring>
#include <memory>
#include <network/AsyncReceiver.hpp>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

constexpr const char *TaskName = "async_recv";
constexpr unsigned StackSizeInWords = 4048;
constexpr unsigned Priority = 1;

namespace network {

namespace {

void task(void *Arg) {
  ESP_LOGI("AsyncReceiver", "task started");
  AsyncReceiver &AsyncRecv = *reinterpret_cast<AsyncReceiver *>(Arg);

  auto &Recv = AsyncRecv.getReceiver();
  auto const &Callback = AsyncRecv.getCallback();

  // Allocate memory to buffer received message
  const unsigned MaxMsgLen = AsyncRecv.getMaxMsgLen();
  std::unique_ptr<uint8_t> Buffer(new uint8_t[MaxMsgLen]);

  while (true) {
    std::memset(Buffer.get(), 0, MaxMsgLen);
    int Count = Recv.recv(Buffer.get(), MaxMsgLen);
    Callback(Buffer.get(), Count);
  }
  vTaskDelete(NULL);
}

} // namespace

AsyncReceiver::AsyncReceiver(Receiver &Rcv, unsigned MaxMsgLen)
    : Recv(Rcv), MaxMsgLen(MaxMsgLen), OnRecv([](uint8_t *, int) {}) {}

AsyncReceiver::~AsyncReceiver() { close(); }

void AsyncReceiver::start() {
  auto Ret = xTaskCreate(task, TaskName, StackSizeInWords, this, Priority,
                         &TaskHandle);
  if (Ret != pdPASS) {
    TaskHandle = nullptr;
    ESP_LOGE("AsyncReceiver", "failed to create task");
  }
}

bool AsyncReceiver::isReady() const { return TaskHandle != nullptr; }

void AsyncReceiver::onRecv(Callback OnRcv) { OnRecv = std::move(OnRcv); }

void AsyncReceiver::close() {
  if (TaskHandle) {
    vTaskDelete(TaskHandle);
  }
  Recv.close();
}

Receiver &AsyncReceiver::getReceiver() { return Recv; }

unsigned AsyncReceiver::getMaxMsgLen() const { return MaxMsgLen; }

AsyncReceiver::Callback const &AsyncReceiver::getCallback() const {
  return OnRecv;
}

} // namespace network
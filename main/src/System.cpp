#include <esp_heap_caps.h>
#include <esp_system.h>
#include <netdisp/Log.hpp>
#include <netdisp/System.hpp>

extern "C" {

static void heapAllocErrorHandler(size_t AllocSize, uint32_t /*Caps*/,
                                  const char *FuncName) {
  ND_LOGE("System", "could not allocate %d bytes of memory in '%s'", AllocSize,
          FuncName);
}
}

namespace netdisp {

void setupErrorHandlers() {
  if (heap_caps_register_failed_alloc_callback(&heapAllocErrorHandler) !=
      ESP_OK) {
    ND_LOGE("System", "could not setup heap alloc error handler");
  }
}

uint32_t getFreeHeapSize() { return esp_get_free_heap_size(); }

void logFreeHeapSize() {
  ND_LOGI("System", "free available heap %lu bytes", getFreeHeapSize());
}

} // namespace netdisp
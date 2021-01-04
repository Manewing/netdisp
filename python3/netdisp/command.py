#!/usr/bin/env python3

class CommandBuilder(object):
    MAGIC = bytes([0xef, 0xfa])

    def __init__(self):
        self._bytes = bytes(self.MAGIC)

    def select_view(self, view: int):
        self._bytes += bytes([0x00, view & 0xff])
        return self

    def show_view(self, view: int):
        self._bytes += bytes([0x01, view & 0xff])
        return self

    def set_led(self, led: int, state: bool):
        self._bytes += bytes([0x02, (led & 0x7f) << 1 | state])
        return self

    def show_text(self, text: str, raw: bool = False, encoding: str = 'utf-8'):
        self._bytes += bytes([0x03, 0x00 | raw]) + text.encode(encoding)
        return self

    # TODO blink_led

    def finish(self) -> bytes:
        return self._bytes
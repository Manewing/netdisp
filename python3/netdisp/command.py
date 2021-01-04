#!/usr/bin/env python3

import struct


def uint8(x) -> bytes:
    return bytes([x & 0xff])


def uint16(x) -> bytes:
    return struct.pack("<H", x)


class CommandBuilder(object):
    MAGIC = bytes([0xef, 0xfa])

    def __init__(self):
        self._bytes = bytes(self.MAGIC)

    def select_view(self, view: int):
        self._bytes += uint8(0x00) + uint8(view)
        return self

    def show_view(self, view: int):
        self._bytes += uint8(0x01) + uint8(view)
        return self

    def set_led(self, led: int, state: bool):
        self._bytes += uint8(0x02) + uint8((led & 0x7f) << 1 | state)
        return self

    def show_text(self, text: str, raw: bool = False, encoding: str = 'utf-8'):
        self._bytes += uint8(0x03) + uint8(raw) + text.encode(encoding)
        return self

    # TODO blink_led

    def show_bitmap(self, x: int, y: int, w: int, h: int, bitmap_data: bytes):
        self._bytes += (uint8(0x05) + uint16(x) + uint16(y) + uint16(w) +
                        uint16(h) + uint16(len(bitmap_data)) + bitmap_data)
        return self

    def finish(self) -> bytes:
        return self._bytes
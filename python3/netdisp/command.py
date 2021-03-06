#!/usr/bin/env python3

import struct

from netdisp.image import Bitmap


def uint8(x) -> bytes:
    return bytes([x & 0xff])


def int16(x) -> bytes:
    return struct.pack("<h", x)

def int16_arr(arr) -> bytes:
    result = bytes()
    for x in arr:
      result += int16(x)
    return result


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
        self._bytes += uint8(0x03) + uint8(raw) + uint8(
            len(text)) + text[:0xff + 1].encode(encoding)
        return self

    def blink_led(self, led: int, times: int):
        self._bytes += uint8(0x04) + uint8(led) + uint8(times)
        return self

    def show_bitmap(self, x: int, y: int, w: int, h: int, bitmap_data: bytes):
        self._bytes += (uint8(0x05) + uint16(x) + uint16(y) + uint16(w) +
                        uint16(h) + uint16(len(bitmap_data)) + bitmap_data)
        return self

    def draw_line(self, p_start: tuple, p_end: tuple):
        self._bytes += uint8(0x9) + int16_arr(p_start + p_end)
        return self

    def draw_rect(self, point: tuple, width: int, height: int):
        self._bytes += uint8(0xa) + int16_arr(point + (width, height))
        return self

    def draw_circle(self, point: tuple, radius: int):
        self._bytes += uint8(0xb) + int16_arr(point + (radius,))
        return self

    def show_image(self, x: int, y: int, max_w: int, max_h: int, path: str):
        bitmap = Bitmap(path, max_w, max_h)
        self.show_bitmap(x, y, bitmap.w, bitmap.h, bitmap.data)
        return self

    def create_composite(self):
        self._bytes += uint8(0x6)
        return self

    def create_notification(self, timeout_ms: int = 500):
        self._bytes += uint8(0x7) + uint16(timeout_ms)

    def end_view(self):
        self._bytes += uint8(0x8)

    def finish(self) -> bytes:
        return self._bytes
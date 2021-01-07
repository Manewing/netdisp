#!/usr/bin/env python3

from PIL import Image


def get_scaled_black_white(path: str, max_w: int, max_h: int) -> Image:
    img = Image.open(path)
    img = img.convert('1')
    img.thumbnail((max_w, max_h), Image.ANTIALIAS)
    return img


class Bitmap(object):
    def __init__(self, path: str, max_w: int, max_h: int):
        self._bytes = bytes()
        self._last_byte = 0
        self._bits = 0
        self._max_w = max_w
        self._max_h = max_h

        img = get_scaled_black_white(path, max_w, max_h)
        self._w, self._h = img.size
        for y in range(int(self._h / 8)):
            for x in range(self._w):
                for l in range(8):
                    self.addBit(img.getpixel((x, y * 8 + l)) == 255)

    @property
    def data(self) -> bytes:
        return self._bytes

    @property
    def w(self) -> int:
        return self._w

    @property
    def h(self) -> int:
        return self._h

    def addBit(self, bit: bool):
        self._last_byte |= bit << self._bits
        self._bits += 1
        if self._bits >= 8:
            self._bytes += bytes([self._last_byte & 0xff])
            self._last_byte = 0
            self._bits = 0

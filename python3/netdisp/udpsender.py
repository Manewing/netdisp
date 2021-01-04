#!/usr/bin/env python3

import socket

class UdpSender(object):
    def __init__(self, ip: str, port: int):
      self._ip = ip
      self._port = port
      self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def send(self, data: bytes) -> int:
      return self._sock.sendto(data, (self._ip, self._port))

    def close(self):
      if self._sock is None:
        return
      self._sock.close()
      self._sock = None

    def __del__(self):
      self.close()
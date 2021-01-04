#!/usr/bin/env python3

from netdisp.udpsender import UdpSender
from netdisp.command import CommandBuilder

class NetDisp(object):
    def __init__(self, ip: str, port: int, view: int = None):
      self._sender = UdpSender(ip, port)
      self._current_cmd = None
      self._view = view

    def _getCmd(self) -> CommandBuilder:
      if self._current_cmd is None:
        self._current_cmd = CommandBuilder()
        if self._view is not None:
            self._current_cmd.select_view(self._view)
      return self._current_cmd

    def send(self):
      if self._current_cmd is None:
          raise ValueError("No command to send")
      self._sender.send(self._current_cmd.finish())
      self._current_cmd = None

    def clear(self):
      self._current_cmd = None

    def close(self):
      self._sender.close()

    def __getattribute__(self, name):
      if hasattr(CommandBuilder, name):
        cmd = self._getCmd()
        def wrapper(*args, **kwargs):
          getattr(cmd, name)(*args, **kwargs)
          return self
        return wrapper
      return object.__getattribute__(self, name)
#!/usr/bin/env python3

import sys

from netdisp.udpsender import UdpSender
from netdisp.command import CommandBuilder


class NetDisp(object):
    def __init__(self, ip: str, port: int, view: int = None):
        """
        ip: The IP address of the NetDisp
        port: The port of the NetDisp
        view: The NetDisp view to display on
        """
        self._sender = UdpSender(ip, port)
        self._current_cmd = None
        self._view = view

    def _getCmd(self) -> CommandBuilder:
        """
        Create a new command builder and set it as the current command. If a
        view is selected, also add a select view command.
        """
        if self._current_cmd is None:
            self._current_cmd = CommandBuilder()
            if self._view is not None:
                self._current_cmd.select_view(self._view)
        return self._current_cmd

    def send(self) -> bool:
        """
        Sends the current command to the NetDisp.

        return: True if command was send successfully
        raises: ValueError if there is no current command
        """
        if self._current_cmd is None:
            raise ValueError("No command to send")
        data = self._current_cmd.finish()
        self._current_cmd = None
        try:
            return self._sender.send(data) > 0
        except OSError as e:
            print(f"Failed to send commend: {e}", file=sys.stderr)
            return False
        return True

    def abort_cmd(self):
        """
        Aborts the current command
        """
        self._current_cmd = None

    def close(self):
        """
        Closes the connection to the NetDisp
        """
        self._sender.close()

    def __getattribute__(self, name):
        """
        Helper function to forward member functions of the command builder,
        this allows this class to behave as a command builder.
        """
        if hasattr(CommandBuilder, name):
            cmd = self._getCmd()

            def wrapper(*args, **kwargs):
                getattr(cmd, name)(*args, **kwargs)
                return self

            return wrapper
        return object.__getattribute__(self, name)

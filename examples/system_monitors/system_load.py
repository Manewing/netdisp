#!/usr/bin/env python3

import argparse
import psutil
import sys
import time

try:
    from netdisp import NetDisp
except ImportError as e:
    raise RuntimeError("Could not import netdisp, make sure its installed or "
                       "that the exports are sourced") from e


def convert_to_bps(value: float) -> str:
    if value >= 10**9:
        return f"{value/10**9:.1f} GB/s"
    if value >= 10**6:
        return f"{value/10**6:.1f} MB/s"
    if value >= 10**3:
        return f"{value/10**3:.1f} kB/s"
    return f"{value:.1f} B/s"


class DiskIO(object):
    def __init__(self):
        self._start = time.time()
        self._data = psutil.disk_io_counters()

    @property
    def bps(self) -> float:
        end = time.time()
        data = psutil.disk_io_counters()

        total_read = data.read_bytes - self._data.read_bytes
        total_write = data.write_bytes - self._data.write_bytes
        bps = (total_read + total_write) / (end - self._start)

        self._start = end
        self._data = data

        return bps

    def __str__(self) -> str:
        return convert_to_bps(self.bps)


class NetIO(object):
    def __init__(self):
        self._start = time.time()
        self._data = psutil.net_io_counters()

    @property
    def bps(self) -> float:
        end = time.time()
        data = psutil.net_io_counters()

        total_sent = data.bytes_sent - self._data.bytes_sent
        total_recv = data.bytes_recv - self._data.bytes_recv
        bps = (total_sent + total_recv) / (end - self._start)

        self._start = end
        self._data = data

        return bps

    def __str__(self) -> str:
        return convert_to_bps(self.bps)


def parse_args(args):
    parser = argparse.ArgumentParser(description="NetDisp System Load Monitor")
    parser.add_argument('-v',
                        '--view',
                        type=int,
                        default=0,
                        help="View index to write to")
    parser.add_argument('ip', type=str, help="The IP of the netdisp")
    parser.add_argument('port', type=int, help="The port of the netdisp")
    parser.add_argument("-p",
                        "--period",
                        type=float,
                        default=1.0,
                        help="Update period in seconds")
    parser.add_argument("--once",
                        action="store_true",
                        help="If to not run continously but only once")
    return parser.parse_args(args)


def main(args):
    args = parse_args(args)

    netdisp = NetDisp(args.ip, args.port)
    netdisp.show_view(args.view).send()

    netio = NetIO()
    diskio = DiskIO()

    while True:
        lines = []

        lines += [f"CPU: *{psutil.cpu_percent():.2f}* %"]
        lines += [f"RAM: *{psutil.virtual_memory().percent:.2f}* %"]
        lines += [f"Net: *{netio}*"]
        lines += [f"Disk: *{diskio}*"]

        netdisp.select_view(args.view).show_text('\n'.join(lines)).send()
        time.sleep(args.period)

        if args.once:
            break


if __name__ == "__main__":
    main(sys.argv[1:])

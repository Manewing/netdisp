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


def parse_args(args):
    parser = argparse.ArgumentParser(description="System Load Net-Monitor")
    parser.add_argument('-v',
                        '--view',
                        type=int,
                        default=0,
                        help="View index to write to")
    parser.add_argument('ip', type=str, help="The IP of the netdisp")
    parser.add_argument('port', type=int, help="The port of the netdisp")
    return parser.parse_args(args)


def main(args):
    args = parse_args(args)

    netdisp = NetDisp(args.ip, args.port)
    netdisp.show_view(args.view).send()

    while True:
        lines = []

        lines += [f"CPU: *{psutil.cpu_percent():.2f}* %"]
        lines += [f"RAM: *{psutil.virtual_memory().percent:.2f}* %"]
        lines += []  # TODO net
        lines += []  # TODO disk

        netdisp.select_view(args.view).show_text('\n'.join(lines)).send()
        time.sleep(1.0)


if __name__ == "__main__":
    main(sys.argv[1:])

#!/usr/bin/env python3

import argparse
import sys

try:
    from netdisp import NetDisp
except ImportError as e:
    raise RuntimeError("Could not import netdisp, make sure its installed or "
                       "that the exports are sourced") from e


def parse_args(args):
    parser = argparse.ArgumentParser(description="NetDisp Blink Led")
    parser.add_argument('ip', type=str, help="The IP of the netdisp")
    parser.add_argument('port', type=int, help="The port of the netdisp")
    parser.add_argument('led',
                        type=int,
                        help="Index of the LED to blink")
    parser.add_argument('times',
                        type=int,
                        help="The number of times to blink the LED")
    return parser.parse_args(args)


def main(args):
    args = parse_args(args)

    netdisp = NetDisp(args.ip, args.port)

    netdisp.blink_led(args.led, args.times).send()


if __name__ == "__main__":
    main(sys.argv[1:])

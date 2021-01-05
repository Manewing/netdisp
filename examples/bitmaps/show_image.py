#!/usr/bin/env python3

import sys
import argparse

try:
    from netdisp import NetDisp
except ImportError as e:
    raise RuntimeError("Could not import netdisp, make sure its installed or "
                       "that the exports are sourced") from e

def parse_args(args):
    parser = argparse.ArgumentParser(description="Show image on netdisp")
    parser.add_argument('-v',
                        '--view',
                        type=int,
                        default=0,
                        help="View index to write to")
    parser.add_argument('ip', type=str, help="The IP of the netdisp")
    parser.add_argument('port', type=int, help="The port of the netdisp")
    parser.add_argument('image', type=str, help="Path to the image to show")
    return parser.parse_args(args)


def main(args):
    args = parse_args(args)

    netdisp = NetDisp(args.ip, args.port, args.view)
    netdisp.show_view(args.view).send()
    netdisp.show_image(0, 0, 128, 64, args.image).send()

if __name__ == "__main__":
    main(sys.argv[1:])
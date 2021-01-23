#!/usr/bin/env python3

import os
import argparse
import sys

DROPLET_ICON_PATH = os.path.join(os.path.realpath(os.path.dirname(__file__)),
                                 "..", "..", "imgs", "droplet_icon.png")

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

    netdisp = NetDisp(args.ip, args.port, args.view)
    netdisp.show_view(args.view).send()
    netdisp.create_composite().show_text("\n~*Hello*").show_image(
        56, 40, 16, 16, DROPLET_ICON_PATH).end_view().send()


if __name__ == "__main__":
    main(sys.argv[1:])

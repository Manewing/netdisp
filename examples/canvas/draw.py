#!/usr/bin/env python3

import sys
import argparse
import random
import time

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
    return parser.parse_args(args)


class CanvasHelper(object):
    def __init__(self, w: int, h: int):
        self._w = w
        self._h = h

    def rand_size(self) -> int:
        return random.randint(0, min(self._w, self._h))

    def rand_point(self) -> tuple:
        return (random.randint(int(-self._w / 4), int(self._w * 5 / 5)),
                random.randint(int(-self._h / 4), int(self._h * 5 / 5)))


def main(args):
    args = parse_args(args)

    netdisp = NetDisp(args.ip, args.port, args.view)
    netdisp.show_view(args.view).send()

    cvh = CanvasHelper(128, 64)

    # Create composite view
    netdisp.create_composite()

    for _ in range(0, 20):

        # Randomly choose glyph to draw
        kind = random.randint(0, 2)

        # Draw the glyph, either line, rect or cricle
        if kind == 0:
            netdisp.draw_line(cvh.rand_point(), cvh.rand_point())
        elif kind == 1:
            netdisp.draw_rect(cvh.rand_point(), cvh.rand_size(),
                              cvh.rand_size())
        else:
            netdisp.draw_circle(cvh.rand_point(), cvh.rand_size())

        # Do not reset the current command we will append to it
        netdisp.send(reset=False)

        time.sleep(0.2)


if __name__ == "__main__":
    main(sys.argv[1:])
#!/usr/bin/env python3

import argparse
import sys

try:
    from netdisp import NetDisp
except ImportError as e:
    raise RuntimeError("Could not import netdisp, make sure its installed or "
                       "that the exports are sourced") from e


def parse_args(args):
    parser = argparse.ArgumentParser(description="NetDisp Notification")
    parser.add_argument('ip', type=str, help="The IP of the netdisp")
    parser.add_argument('port', type=int, help="The port of the netdisp")
    return parser.parse_args(args)


def main(args):
    args = parse_args(args)

    netdisp = NetDisp(args.ip, args.port)

    # Create notification this will override what is currently shown without
    # switching the view. Everything until the view is ended will be part of
    # the notification.
    netdisp.create_notification(timeout_ms=1000).draw_rect(
        (0, 0), 128-1, 64-1).show_text("\n~Notification").end_view().send()


if __name__ == "__main__":
    main(sys.argv[1:])

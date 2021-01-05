#!/usr/bin/env python3

import argparse
import os
import re
import sys

try:
    from netdisp.image import Bitmap
except ImportError as e:
    raise RuntimeError("Could not import netdisp, make sure its installed or "
                       "that the exports are sourced") from e


def parse_args(args):
    parser = argparse.ArgumentParser(description="Convert image to hex code")
    parser.add_argument('-c',
                        '--code',
                        action="store_true",
                        help="If to generate C code from image")
    parser.add_argument('-x',
                        type=int,
                        default=128,
                        help="The maximum width of the image")
    parser.add_argument('-y',
                        type=int,
                        default=64,
                        help="The maximum height of the image")
    parser.add_argument('image', type=str, help="Path to the image to convert")
    return parser.parse_args(args)


def main(args):
    args = parse_args(args)

    bmp = Bitmap(args.image, args.x, args.y)

    if args.code:
      image_name = os.path.splitext(os.path.basename(args.image))[0]
      image_name = re.sub('[^0-9a-zA-Z_]', '_', image_name)
      print(f"// Generated using {sys.argv[0]}")
      print(f"// Image size: {bmp.w}x{bmp.h} pixel")
      print(f"const uint8_t {image_name:s}_bitmap_data[] = {{")

    pred = ""
    output_str = ""
    for bt in bmp.data:
      output_str += pred + f"0x{int(bt):02x}"
      pred = ", "

    print(output_str)

    if args.code:
      print("};")


if __name__ == "__main__":
    main(sys.argv[1:])
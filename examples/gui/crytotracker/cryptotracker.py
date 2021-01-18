#!/usr/bin/env python3

import os
import sys
import time
import argparse
import requests

try:
    from netdisp import NetDisp
except ImportError as e:
    raise RuntimeError("Could not import netdisp, make sure its installed or "
                       "that the exports are sourced") from e

API_URL = 'https://api.coinbase.com/v2/prices'

IMGS_PATH = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                         "..", "..", "..", "imgs")


def get_icon(kind: str) -> str:
    return os.path.join(IMGS_PATH, f"{kind.lower():s}_icon.png")


def get_latest_price(currency: str, crypto: str, kind: str = "sell") -> float:
    currency = {
        "euro": "EUR",
        "us-dollar": "USD",
    }[currency.lower()]
    crypto = {"ethereum": "ETH", "bitcoin": "BTC"}[crypto.lower()]

    url = f"{API_URL:s}/{crypto:s}-{currency:s}/{kind:s}"
    response = requests.get(url).json()
    return float(response["data"]["amount"])


def parse_args(args):
    parser = argparse.ArgumentParser(description="System Load Net-Monitor")
    parser.add_argument('-v',
                        '--view',
                        type=int,
                        default=0,
                        help="View index to write to")
    parser.add_argument('ip', type=str, help="The IP of the netdisp")
    parser.add_argument('port', type=int, help="The port of the netdisp")
    parser.add_argument('currency',
                        type=str.lower,
                        choices=["euro", "us-dollar"],
                        help="The currency in which to track the price")
    parser.add_argument(
        'crypto',
        type=str.lower,
        choices=["bitcoin", "ethereum"],
        help="The crypto currency for which to track the price")
    parser.add_argument("-p",
                        "--period",
                        type=float,
                        default=10,
                        help="Update period in minutes")
    return parser.parse_args(args)


def main(args):
    args = parse_args(args)

    netdisp = NetDisp(args.ip, args.port, args.view)
    netdisp.show_view(args.view).send()

    while True:
        price = get_latest_price(args.currency, args.crypto)

        netdisp.create_composite()
        netdisp.show_text(f"~#{int(price):d}\n#~*/*")
        netdisp.show_image(16, 32, 32, 32, get_icon(args.currency))
        netdisp.show_image(80, 32, 32, 32, get_icon(args.crypto))
        netdisp.end_view()
        netdisp.send()

        time.sleep(args.period * 60)


if __name__ == "__main__":
    main(sys.argv[1:])

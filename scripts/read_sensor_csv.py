#!/usr/bin/env python3

from __future__ import annotations

import argparse
import csv
from pathlib import Path

try:
    import serial
    from serial.tools import list_ports
except ImportError as exc:
    raise SystemExit(
        "pyserial is required. Install it with `python3 -m pip install pyserial` "
        "or run this from the PlatformIO Python environment."
    ) from exc


CSV_LAYOUTS = {
    6: ("ms", "joy_x", "joy_y", "joy_x_step", "joy_y_step", "pot_data"),
    7: ("ms", "joy_x_value", "joy_y_value", "joy_x_step", "joy_y_step", "pot_value", "pot_pct"),
    9: (
        "ms",
        "joy_x_raw",
        "joy_y_raw",
        "joy_x_centered",
        "joy_y_centered",
        "joy_x_step",
        "joy_y_step",
        "pot_raw",
        "pot_pct",
    ),
    10: (
        "ms",
        "joy_x_raw",
        "joy_y_raw",
        "joy_x_value",
        "joy_y_value",
        "joy_x_step",
        "joy_y_step",
        "pot_raw",
        "pot_value",
        "pot_pct",
    ),
}


def find_serial_port() -> str:
    ports = list(list_ports.comports())
    for port in ports:
        description = f"{port.device} {port.description}".lower()
        if "usb" in description or "uart" in description or "cp210" in description or "ch340" in description:
            return port.device
    if ports:
        return ports[0].device
    raise SystemExit("No serial ports found. Connect the ESP32 or pass --port /dev/ttyUSB0.")


def parse_header(line: str) -> tuple[str, ...] | None:
    marker = "# ms,"
    if not line.startswith(marker):
        return None
    return tuple(part.strip() for part in line[2:].split(","))


def parse_sensor_row(line: str) -> list[int] | None:
    parts = next(csv.reader([line]))
    try:
        return [int(part.strip()) for part in parts]
    except ValueError:
        return None


def main() -> int:
    parser = argparse.ArgumentParser(description="Record Macroboard sensor debug serial output to CSV.")
    parser.add_argument(
        "-p",
        "--port",
        default=None,
        help="Serial port. Defaults to the first USB/UART-looking port.",
    )
    parser.add_argument("-b", "--baud", type=int, default=115200, help="Serial baud rate")
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        default=Path("logs/sensor_log.csv"),
        help="Output CSV path",
    )
    parser.add_argument(
        "--append",
        action="store_true",
        help="Append rows instead of replacing the CSV file",
    )
    args = parser.parse_args()

    port = args.port or find_serial_port()
    args.output.parent.mkdir(parents=True, exist_ok=True)

    mode = "a" if args.append else "w"
    row_count = 0
    columns: tuple[str, ...] | None = None

    print(f"Reading {port} at {args.baud} baud")
    print(f"Writing CSV to {args.output}")
    print("Press Ctrl+C to stop.")

    with serial.Serial(port, args.baud, timeout=1) as device, args.output.open(mode, newline="") as handle:
        writer = csv.writer(handle)

        try:
            while True:
                raw_line = device.readline()
                if not raw_line:
                    continue

                line = raw_line.decode("utf-8", errors="ignore").strip()
                if not line:
                    continue

                header = parse_header(line)
                if header is not None:
                    columns = header
                    if not args.append or handle.tell() == 0:
                        writer.writerow(columns)
                        handle.flush()
                    continue

                values = parse_sensor_row(line)
                if values is None:
                    continue

                if columns is None:
                    columns = CSV_LAYOUTS.get(len(values))
                    if columns is None:
                        continue
                    if not args.append or handle.tell() == 0:
                        writer.writerow(columns)

                if len(values) != len(columns):
                    continue

                writer.writerow(values)
                handle.flush()
                row_count += 1
                print(f"\rRows written: {row_count}", end="", flush=True)
        except KeyboardInterrupt:
            print(f"\nStopped. Rows written: {row_count}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3

from __future__ import annotations

import argparse
import csv
from pathlib import Path

import matplotlib.pyplot as plt


CSV_COLUMNS = (
    "ms",
    "joy_x_raw",
    "joy_y_raw",
    "joy_x_centered",
    "joy_y_centered",
    "joy_x_step",
    "joy_y_step",
    "pot_raw",
    "pot_pct",
)


def parse_log(path: Path) -> list[dict[str, int]]:
    rows: list[dict[str, int]] = []

    with path.open(newline="") as handle:
        for raw_line in handle:
            line = raw_line.strip()
            if not line:
                continue
            if line.startswith(("---", "#", "Macroboard")):
                continue

            parts = next(csv.reader([line]))
            if len(parts) != len(CSV_COLUMNS):
                continue

            try:
                values = [int(part) for part in parts]
            except ValueError:
                continue

            rows.append(dict(zip(CSV_COLUMNS, values)))

    return rows


def mark_dropouts(ax, time_s: list[float], pot_raw: list[int], threshold: int) -> int:
    dropout_runs = 0
    start_index: int | None = None

    for index, value in enumerate(pot_raw):
        is_low = value < threshold
        if is_low and start_index is None:
            start_index = index
        elif not is_low and start_index is not None:
            ax.axvspan(time_s[start_index], time_s[index - 1], color="red", alpha=0.08)
            dropout_runs += 1
            start_index = None

    if start_index is not None:
        ax.axvspan(time_s[start_index], time_s[-1], color="red", alpha=0.08)
        dropout_runs += 1

    return dropout_runs


def plot_file(path: Path, rows: list[dict[str, int]], output_path: Path, threshold: int) -> None:
    time_s = [(row["ms"] - rows[0]["ms"]) / 1000.0 for row in rows]
    pot_raw = [row["pot_raw"] for row in rows]
    pot_pct_scaled = [row["pot_pct"] * 40.95 for row in rows]
    joy_x_raw = [row["joy_x_raw"] for row in rows]
    joy_y_raw = [row["joy_y_raw"] for row in rows]

    fig, axes = plt.subplots(2, 1, figsize=(14, 8), sharex=True)

    axes[0].plot(time_s, pot_raw, label="pot_raw", color="#0d6efd", linewidth=1)
    axes[0].plot(time_s, pot_pct_scaled, label="pot_pct scaled", color="#fd7e14", linewidth=1, alpha=0.75)
    axes[0].set_ylabel("Pot ADC")
    axes[0].set_title(f"{path.name}: potentiometer and joystick capture")
    axes[0].grid(True, alpha=0.3)
    axes[0].legend(loc="upper right")

    axes[1].plot(time_s, joy_x_raw, label="joy_x_raw", color="#198754", linewidth=1)
    axes[1].plot(time_s, joy_y_raw, label="joy_y_raw", color="#dc3545", linewidth=1)
    axes[1].set_ylabel("Joystick ADC")
    axes[1].set_xlabel("Time (s)")
    axes[1].grid(True, alpha=0.3)
    axes[1].legend(loc="upper right")

    dropout_runs = mark_dropouts(axes[0], time_s, pot_raw, threshold)
    mark_dropouts(axes[1], time_s, pot_raw, threshold)

    fig.tight_layout()
    fig.savefig(output_path, dpi=170)
    plt.close(fig)

    zero_count = sum(1 for value in pot_raw if value == 0)
    print(f"Saved plot: {output_path}")
    print(f"Samples: {len(rows)}")
    print(f"Pot raw min/max: {min(pot_raw)} / {max(pot_raw)}")
    print(f"Pot zero samples: {zero_count}")
    print(f"Dropout runs below {threshold}: {dropout_runs}")


def build_output_path(input_path: Path, explicit_output: Path | None) -> Path:
    if explicit_output is not None:
        return explicit_output
    return input_path.with_suffix(".png")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Plot Macroboard sensor CSV logs generated from serial monitor output."
    )
    parser.add_argument("input", type=Path, help="Path to a sensor log CSV file")
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        help="Optional output PNG path. Defaults to the input filename with .png",
    )
    parser.add_argument(
        "--dropout-threshold",
        type=int,
        default=50,
        help="Pot raw values below this are highlighted as dropouts",
    )
    args = parser.parse_args()

    rows = parse_log(args.input)
    if not rows:
        raise SystemExit(f"No valid sensor rows found in {args.input}")

    output_path = build_output_path(args.input, args.output)
    plot_file(args.input, rows, output_path, args.dropout_threshold)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/bin/bash

set -e

python3 scripts/plot_sensor_logs.py logs/sensor_log.csv -o logs/my_plot.png
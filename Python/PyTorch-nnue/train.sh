#!/bin/bash

python3 train.py ./scripts/d5_100_3612.binpack ./scripts/d5_100_3612.binpack --lambda 1.0 --val_check_interval 1 --threads 2 --batch-size 10 --progress_bar_refresh_rate 20

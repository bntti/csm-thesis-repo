#!/usr/bin/env bash

set -e
export QT_QPA_PLATFORM=xcb
export CRYPTOGRAPHY_OPENSSL_NO_LEGACY=1
cd ./pygobnilp
source /opt/miniconda3/etc/profile.d/conda.sh
conda activate bayesian

time python3 rungobnilp.py \
    --noplot \
    --score BDeu \
    --palim 5 \
    --nopruning \
    --output_scores /tmp/data.jkl \
    /tmp/data.txt

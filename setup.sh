#!/usr/bin/env sh

python3 -m venv .venv
source .venv/bin/activate
which python

python3 -m pip install -r requirements.txt

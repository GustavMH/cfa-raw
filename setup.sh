#!/usr/bin/env sh

python3.11 -m venv .venv
source .venv/bin/activate
which python

python3.11 -m pip install -r requirements.txt

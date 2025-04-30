#!/bin/sh

git submodule init

python3 -m pip install --upgrade certifi
python3 -m pip install setuptools

cd esp-idf
./install.sh esp32
source ./export.sh
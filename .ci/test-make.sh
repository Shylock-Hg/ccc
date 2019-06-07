#! /usr/bin/env bash

# Testing by make
echo -ne '\n\n\n\n'

export LD_LIBRARY_PATH=/usr/local/lib
sudo make install
make test

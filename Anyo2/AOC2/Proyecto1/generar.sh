#!/bin/bash

ghdl -i --ieee=synopsys -fexplicit --workdir=WORK *.vhd
ghdl --gen-makefile --ieee=synopsys -fexplicit --workdir=WORK testbench > Makefile

#!/bin/bash
mkdir WORK
ghdl -i --ieee=synopsys -fexplicit --workdir=WORK *.vhd
ghdl --gen-makefile --ieee=synopsys --workdir=WORK testbench > Makefile

#!/bin/bash
if make ; then
    ./testbench --stop-time=10us --wave=resultado.ghw
    gtkwave resultado.ghw -a savefile.gtkw
fi
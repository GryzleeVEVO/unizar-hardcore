#!/bin/bash
if make ; then
    ./testbench --stop-time=1us --wave=resultado.ghw
    gtkwave resultado.ghw -a savefile.gtkw &
    disown
fi
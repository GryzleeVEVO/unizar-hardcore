#!/bin/bash

generar_pdf () {
    pandoc "$1.md" \
    -f markdown \
    --highlight-style haddock \
    -V geometry:a4paper \
    -V geometry:margin=2cm \
    -V documentclass=report \
    -V lang=es-ES \
    -o "$1.pdf"
} 

generar_pdf memoria
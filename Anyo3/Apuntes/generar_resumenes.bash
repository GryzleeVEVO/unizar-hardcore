#!/bin/bash

# Autor: Dorian Wozniak
# Genera a partir de ficheros Markdown apuntes en formato PDF
#
# Para generarlos se requiere:
#       - pandoc
#       - Una distribución de tex (texlive)

generar_pdf () {
    pandoc "$1.md" \
    -f markdown \
    --highlight-style haddock \
    -V geometry:a4paper \
    -V geometry:margin=2cm \
    -V documentclass=report \
    -V lang=es-ES \
    --toc \
    --top-level-division=chapter \
    -N \
    -H disable_float.tex \
    -o "$1.pdf"
}

generar_pdf $@
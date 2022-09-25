#!/bin/bash

# Autor: Dorian Wozniak
# Genera a partir de ficheros Markdown apuntes en formato PDF
# Los ficheros a generar son los siguientes:
#
#
#
#
#
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
    -o "$1.pdf"
} 

generar_pdf "./IArt/resumen"
generar_pdf "./IngSoft/resumen"
generar_pdf "./SistInf/resumen"


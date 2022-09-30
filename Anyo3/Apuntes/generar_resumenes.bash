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
    -H disable_float.tex \
    -o "$1.pdf"
} 

if [ $# -ne 0 ]
then
    case $1 in
        IA) generar_pdf "./IArt/resumen"
        ;;

        IS) generar_pdf "./IngSoft/resumen"
        ;;

        SI) generar_pdf "./SistInf/resumen"
        ;;
        
        SD) generar_pdf "./SistDist/resumen"
        ;;
    esac
else
    generar_pdf "./IArt/resumen"
    generar_pdf "./IngSoft/resumen"
    generar_pdf "./SistInf/resumen"
    generar_pdf "./SistDist/resumen"
fi


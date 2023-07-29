#!/bin/bash
# Genera los pdfs para los apuntes de Admsis
# Requiere pandoc y una distribución de Tex (p.e. texlive)

pandoc "resumen_admsis.md" \
	-f markdown \
	--highlight-style haddock \
	-V geometry:a4paper \
	-V geometry:margin=2cm \
	-V documentclass=report \
	--toc \
	--top-level-division=chapter \
	-N \
	-o "resumen_admsis.pdf"

pandoc "chuleta.md" \
	-f markdown \
	--highlight-style haddock \
	-V geometry:a4paper \
	-V geometry:margin=2cm \
	-o "chuleta.pdf"

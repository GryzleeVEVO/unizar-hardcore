---
mainfont: Liberation Serif
monofont: JetBrains Mono
lang: es-ES

documentclass: report

geometry:
  - a4paper
  - margin=2cm

title: |  
  \Huge Aprendizaje Automático
subtitle: |
  | \LARGE Práctica 8: Sistemas de recomendación
  | \LARGE Memoria
  |
  |
  |
  | ![](Image-0004.png){width=1.3cm}
author: Dorian Boleslaw Wozniak (817570@unizar.es)
header-includes: 
  - \usepackage{float}
  - \let\origfigure\figure
  - \let\endorigfigure\endfigure
  - \renewenvironment{figure}[1][2] {\expandafter\origfigure\expandafter[H]} { \endorigfigure }
  - \usepackage[center]{caption}
---

## Resultados: Calificaciones ejemplo

| Película                         | Calificación |
| -------------------------------- | ------------ |
| Toy Story (1995)                 | 4            |
| Twelve Monkeys (1995)            | 3            |
| Usual Suspects, The (1995)       | 5            |
| Outbreak (1995)                  | 4            |
| Shawshank Redemption, The (1994) | 5            |
| While You Were Sleeping (1995)   | 3            |
| Forrest Gump (1994)              | 5            |
| Silence of the Lambs, The (1991) | 2            |
| Alien (1979)                     | 4            |
| Die Hard 2 (1990)                | 5            |
| Sphere (1998)                    | 5            |

Tras añadirlos al conjunto de calificaciones y entrenar el recomendador:

| Película                         | Predicción |
| -------------------------------- | ---------- |
| Titanic (1997)                   | 4.1        |
| Star Wars (1977)                 | 4.0        |
| Raiders of the Lost Ark (1981)   | 3.9        |
| Shawshank Redemption, The (1994) | 3.9        |
| Braveheart (1995)                | 3.9        |
| Return of the Jedi (1983)        | 3.8        |
| Good Will Hunting (1997)         | 3.8        |
| Empire Strikes Back, The (1980)  | 3.8        |
| Usual Suspects, The (1995)       | 3.7        |
| Schindler's List (1993)          | 3.7        |

## Resultados: Califcaciones ejemplo + propias

| Película                          | Calificación |
| --------------------------------- | ------------ |
| Toy Story (1995)                  | 5            |
| Star Wars (1977)                  | 4            |
| Pulp Fiction (1994)               | 4            |
| Lion King, The (1994)             | 5            |
| Mask, The (1994)                  | 3            |
| Jurassic Park (1993)              | 4            |
| Terminator 2: Judgment Day (1991) | 5            |
| Apocalypse Now (1979)             | 5            |
| Return of the Jedi (1983)         | 4            |
| Air Bud (1997)                    | 2            |
| Titanic (1997)                    | 4            |
| Flubber (1997)                    | 1            |
| Big Lebowski, The (1998)          | 3            |

| Película                         | Calificación |
| -------------------------------- | ------------ |
| Star Wars (1977)                 | 4.1          |
| Titanic (1997)                   | 4.1          |
| Raiders of the Lost Ark (1981)   | 3.9          |
| Godfather, The (1972)            | 3.9          |
| Good Will Hunting (1997)         | 3.8          |
| Empire Strikes Back, The (1980)  | 3.8          |
| Shawshank Redemption, The (1994) | 3.8          |
| Schindler's List (1993)          | 3.8          |
| Return of the Jedi (1983)        | 3.7          |
| Braveheart (1995)                | 3.7          |

## Resultados: Califaciones combinando clasificaciones anteriores

| Película                         | Calificación |
| -------------------------------- | ------------ |
| Titanic (1997)                   | 4.5          |
| Star Wars (1977)                 | 4.4          |
| Raiders of the Lost Ark (1981)   | 4.4          |
| Shawshank Redemption, The (1994) | 4.4          |
| Godfather, The (1972)            | 4.4          |
| Braveheart (1995)                | 4.3          |
| Empire Strikes Back, The (1980)  | 4.2          |
| Good Will Hunting (1997)         | 4.2          |
| Silence of the Lambs, The (1991) | 4.2          |
| Die Hard (1988)                  | 4.2          |
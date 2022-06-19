{-
    Tecnología de Programación
    Práctica 5

    Lsystems.hs

    Autor: Dorian Boleslaw Wozniak  817570
    Descripción: Módulo que contiene una función para calcular los pasos
    necesarios para crear sistemas de Lindenmayer, junto a un conjunto de 
    reglas que generan sistemas varios
-}

module Lsystems where

{-
    Reglas: Si coincide el caracter con alguna de las definiciones, hace la
    sustitución adecuada
-}

{-
    Axioma: F
    Distancia: 1
    Ángulo: 60º
-}
curvaKoch :: Char -> String
curvaKoch 'F' = "F+F--F+F"
curvaKoch x = [x]

{-
    Axioma: F
    Distancia: 1
    Ángulo: 90º
-}
curvaKochCuad :: Char -> String
curvaKochCuad 'F' = "F+F-F-F+F"
curvaKochCuad x = [x]

{-
    Axioma: F++F++F
    Distancia: 1
    Ángulo: 60º
-}
copoKoch :: Char -> String
copoKoch 'F' = "F-F++F-F"
copoKoch x = [x]
{-
    Axioma: F++F--F+F
    Distancia: 1
    Ángulo: 60º
-}
anticopoKoch :: Char -> String
anticopoKoch 'F' = "F+F--F+F"
anticopoKoch x = [x]
 
{-
    Axioma: F+F+F+F
    Distancia: 1
    Ángulo: 90º
-}
islaMinkowski :: Char -> String
islaMinkowski 'F' = "F+F-F-FF+F+F-F"
islaMinkowski x = [x]

{-
    Axioma: F-G-G
    Distancia: 1
    Ángulo: 120º
-}
triangSierpinski :: Char -> String
triangSierpinski 'F' = "F-G+F+G-F"
triangSierpinski 'G' = "GG"
triangSierpinski x = [x]

{-
    Axioma: F
    Distancia: 1
    Ángulo: 60º
-}
flechaSierpinski :: Char -> String
flechaSierpinski 'F' = "G-F-G"
flechaSierpinski 'G' = "F+G+F"
flechaSierpinski x = [x]

{-
    Axioma: f
    Distancia: 1
    Ángulo: 90º
-}
curvaHilbert :: Char -> String
curvaHilbert 'f' = "-g>+f>f+>g-"
curvaHilbert 'g' = "+f>-g>g->f+"
curvaHilbert x = [x]

{-
    Axioma: F
    Distancia: 1
    Ángulo: 60º
-}
curvaGosper :: Char -> String
curvaGosper 'F' = "F-G--G+F++FF+G-"
curvaGosper 'G' = "+F-GG--G-F++F+G"
curvaGosper x = [x]

{-
    Devuelve una cadena de pasos para generar un L-system a partir de una
    regla de sustitución, un axioma y una profundidad de detalle
-}
lsystem :: (Char -> String) -> String -> Int -> String
-- Devuelve el propio axioma si se ha llegado a la profundidad deseada
lsystem regla axioma 0 = axioma
-- Devuelve una lista vacía si no hay axioma donde aplicar sustitución
lsystem regla [] prof = []
-- Caso complejo:
lsystem regla (x:xs) prof = 
    -- Llamada recursiva con la sustitucion de la cabeza, profundidad - 1
    (lsystem regla (regla x) (prof - 1))
    ++
    -- Llamada recutsiva con el resto de axioma, misma profundidad
    (lsystem regla xs prof)
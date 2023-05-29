{-
    Tecnología de Programación
    Práctica 5

    main.hs

    Autor: Dorian Boleslaw Wozniak  817570
    Descripción: Módulo que prueba la funcionalidad de los módulos Turtle y
    Lsystem, generando graficos vectoriales de varias figuras
-}

import Lsystems
import SVG
import Turtle

main = do
  savesvg "cuadrado" (tplot (50, 90, (0, 0), 0) ">+>+>+>")
  savesvg "triangulo" (tplot (50, 120, (0, 0), 0) ">+>+>")
  savesvg "curvaKoch" (tplot (1, 60, (0, 0), 0) (lsystem curvaKoch "F" 5))
  savesvg "curvaKochCuad" (tplot (1, 90, (0, 0), 0) (lsystem curvaKochCuad "F" 5))
  savesvg "copoKoch" (tplot (1, 60, (0, 0), 0) (lsystem copoKoch "F++F++F" 5))
  savesvg "anticopoKoch" (tplot (1, 60, (0, 0), 0) (lsystem anticopoKoch "F++F++F" 5))
  savesvg "islaMinkowski" (tplot (1, 90, (0, 0), 0) (lsystem islaMinkowski "F+F+F+F" 5))
  savesvg "triangSierpinski" (tplot (1, 120, (0, 0), 0) (lsystem triangSierpinski "F-G-G" 5))
  savesvg "flechaSierpinski" (tplot (1, 60, (0, 0), 0) (lsystem flechaSierpinski "F" 5))
  savesvg "curvaHilbert" (tplot (1, 90, (0, 0), 0) (lsystem curvaHilbert "f" 5))
  savesvg "curvaGosper" (tplot (1, 60, (0, 0), 0) (lsystem curvaGosper "F" 5))
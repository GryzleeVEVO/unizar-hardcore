{-
    Tecnología de Programación
    Práctica 5

    Turtle.hs

    Autor: Dorian Boleslaw Wozniak  817570
    Descripción: Módulo que implementa un sistema de gráficos de tortuga,
    que a partir de una serie de pasos mueve un cursor (la tortuga), devolviendo
    una lista de coordenadas para generar un gráfico vectorial
-}

module Turtle where

import Data.Char

-- Tipos de datos
type Position   = (Float,Float)
type Heading    = Float
type Turtle     = (Float, Float, Position, Heading)
data Move       = Forward | TurnLeft | TurnRight | Wait

{-
    A partir de una tortuga inicial y un movimiento, devuelve otra tortuga con
    el movimento introducido
-}
moveTurtle :: Turtle -> Move -> Turtle
moveTurtle (paso,giro,(x,y),orn) Forward = (paso,giro,(x+paso*c,y+paso*s),orn)
    where
        ornrad = (pi/180.0)*orn
        c = cos ornrad
        s = sin ornrad
moveTurtle (paso,giro,pos,orn) TurnLeft  = (paso,giro,pos,orn+giro)
moveTurtle (paso,giro,pos,orn) TurnRight = (paso,giro,pos,orn-giro)
-- Devuelve una tortuga igual si es minuscula o un símbolo no reconocido
moveTurtle turtle Wait = turtle 

{-
    Devuelve el tipo de movimiento adecuado según el primer caracter de una 
    cadena de movimientos
-}
nextMove :: String -> Move 
nextMove (x:_)
     | isUpper x = Forward
     | isLower x = Wait
     | x == '>' = Forward
     | x == '+' = TurnRight
     | x == '-' = TurnLeft
     | otherwise = Wait

{-
    A partir de una tortuga inicial y una serie de pasos no vacíos, devuelve
    la concatenación de la posición de la tortuga y la lista de posiciones
    tras realizar movimientos
-}
tplot :: Turtle -> String -> [Position]
-- Si no hay mas movimientos, devuelve su posición final
tplot (paso, giro, pos, orn) [] = [pos]
-- Caso recursivo
tplot (paso, giro, pos, orn) pasos@(_:xs) = 
    -- Posición actual
    [pos] 
    ++ 
    -- Llamada recursiva donde se avanza un paso la tortuga junto al resto de los pasos
    tplot (moveTurtle (paso, giro, pos, orn) (nextMove pasos)) xs 

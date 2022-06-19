{-
    BinaryTree.hs

    Tecnología de Programación
    Práctica 6

    Dorian Boleslaw Wozniak     817570

    Fichero de implementación de funciones para la construcción y utilización 
    de un árbol binario de búsqueda. 
-}

module BinaryTree where
import Data.List

data Tree t = Empty | Leaf t | Tree t (Tree t) (Tree t)

{-
    Árboles binarios genéricos
-}

-- Devuelve un árbol vacío
empty :: Tree t
empty = Empty

-- Devuelve un árbol de una hoja
leaf :: t -> Tree t 
leaf x = Leaf x

-- Devuelve un árbol con dos nodos de árbol
tree :: t -> Tree t -> Tree t -> Tree t
tree x lc rc = Tree x lc rc

-- Devuelve el tamaño del árbol
size :: Tree t -> Integer
size (Empty) = 0
size (Leaf x) = 1
size (Tree x lc rc) = 1 + size lc + size rc

-- Instanciación de la clase Show para imprimir árboles 
instance Show t => Show (Tree t) where
    show tree@(Tree x lc rc) = (printTree tree 0) ++ "\n" where
        printTree (Empty) n = "<>"
        printTree (Leaf x) n = show x
        printTree (Tree x lc rc) n = 
            (show x) ++ "\n" ++
            (tabs n) ++ "|-" ++ (printTree lc (n + 1)) ++ "\n" ++
            (tabs n) ++ "|-" ++ (printTree rc (n + 1))
        tabs 0 = []
        tabs n = "    " ++ (tabs (n - 1))

{-
    Árboles binarios de búsqueda
-}

-- Añade un elemento a la izda/dcha del árbol
add :: Ord t => t -> Tree t -> Tree t
add x (Empty) = leaf x
add x (Leaf y)
    | x < y = tree y (leaf x) (empty)
    | x >= y = tree y (empty) (leaf x)
add x (Tree y lc rc)
    | x < y = tree y (add x lc) rc
    | x >= y = tree y lc (add x rc)

-- Construye un árbol a partir de una lista
build :: Ord t => [t] -> Tree t
build x = addNext x Empty where
    addNext [] t = t
    addNext (x:xs) t = addNext xs (add x t)

{-
    Árboles de búsqueda equilibrados
-}

-- Construye un árbol equilibrado a partir de una lista
buildBalanced :: Ord t => [t] -> Tree t
buildBalanced [] = empty
buildBalanced [x] = leaf x
buildBalanced x = tree root (buildBalanced lc) (buildBalanced rc) where
    (lc, rcA) = splitAt (div (length x) 2) (sort x)
    root = head rcA
    rc = tail rcA

{-
    Recorridos
-}

-- Muestra el árbol en preorden (Raíz-Izda-Dcha)
preorder :: Tree t -> [t]
preorder Empty = []
preorder (Leaf x) = [x]
preorder (Tree x lc rc) = [x] ++ preorder lc ++ preorder rc

-- Muestra el árbol en postorden (Izda-Dcha-Raíz)
postorder :: Tree t -> [t]
postorder Empty = []
postorder (Leaf x) = [x]
postorder (Tree x lc rc) = postorder lc ++ postorder rc ++ [x]

-- Muestra el árbol en órden (Izda-Raíz-Dcha)
inorder :: Tree t -> [t]
inorder Empty = []
inorder (Leaf x) = [x]
inorder (Tree x lc rc) = inorder lc ++ [x] ++ postorder rc


{-
    Reequilibrado
-}

-- Reequilibra un árbol ya existente
balance :: Ord t => Tree t -> Tree t
balance tree = buildBalanced (inorder tree) 

{-
    Búsqueda
-}

-- Devuelve todos los elementos entre dos valores (incluidos) dentro de un árbol
between :: Ord t => Tree t -> t -> t -> [t]
between Empty _ _ = []
between (Leaf x) min max
    | x < min = []
    | x > max = []
    | otherwise = [x]
between (Tree x lc rc) min max
    | x < min = between rc min max
    | x > max = between lc min max
    | otherwise = (between lc min max) ++ (between rc min max) ++ [x]
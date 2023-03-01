# Esta sección ejecuta las pruebas de precondiciones del programa
ROJO='\033[0;31m'
VERDE='\033[0;32m'
NORMAL='\033[0m'

bien() {
    echo -e "[${VERDE}Superado${NORMAL}]"
}

mal() {
    echo -e "[${ROJO}Fallido${NORMAL}]"
}

# Batería de pruebas para comprobar que trata correctamente las entradas proporcionadas
# && -> Programa devuelve 0; || -> Programa devuelve 1 o mayor
test_precond() {
    echo -ne "Test: Mochila con números negativos "
    ./MHcypher -e -17 -2 1 4 8 10 12 14 1 2 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: Mochila no supercreciente "
    ./MHcypher -e 3 4 8 13 24 32 44 57 224 179 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: Mochila con números repetidos "
    ./MHcypher -e 3 8 13 28 28 124 256 493 1 2 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: N menor que suma "
    ./MHcypher -e 3 8 13 28 60 124 256 493 60 13 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: N igual que suma "
    ./MHcypher -e 3 8 13 28 60 124 256 493 985 13 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: w negativo "
    ./MHcypher -e 3 8 13 28 60 124 256 493 1001 -3 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: w no coprimo "
    ./MHcypher -e 3 8 13 28 60 124 256 493 1001 143 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: Todo correcto, pero mensaje vacío "
    ./MHcypher -e 3 8 13 28 60 124 256 493 1001 977 &>/dev/null </dev/null && bien || mal
}

test_encriptar() {
    echo -ne "Test 1: Texto genérico (lorem ipsum) "
    ./MHcypher -e -f test1 -o cifrado $@
    ./MHcypher -d -f cifrado -o descifrado $@
    cmp test1 descifrado &>/dev/null </dev/null && bien || mal
    echo -ne "Test 2: 20KB de datos aleatorios "
    ./MHcypher -e -f test2 -o cifrado $@
    ./MHcypher -d -f cifrado -o descifrado $@
    cmp test2 descifrado &>/dev/null </dev/null && bien || mal
    echo -ne "Test 3: MHcypher.cc, utilizando codificación ASCII "
    ./MHcypher -e -f test3 -o cifrado $@
    ./MHcypher -d -f cifrado -o descifrado $@
    cmp test3 descifrado &>/dev/null </dev/null && bien || mal
}

test_encriptar_ascii() {
    echo -ne "Test 4: Ejemplo del enunciado (codif. mayúsculas) "
    ./MHcypher -mayus -e -f test4 -o cifrado $@
    ./MHcypher -mayus -d -f cifrado -o descifrado $@
    cmp test4 descifrado &>/dev/null </dev/null && bien || mal
    echo -ne "Test 5: 20KB de mayusculas aleatorias (codif. mayúsculas) "
    ./MHcypher -mayus -e -f test5 -o cifrado $@
    ./MHcypher -mayus -d -f cifrado -o descifrado $@
    cmp test5 descifrado &>/dev/null </dev/null && bien || mal
}

# Compila, en caso de usar hendrix01 o 02 usa gmake
[ $(uname) -ne "SunOS" ] && gmake || make

test_precond

echo -e "\nClave privada 1: [3 8 13 28 60 124 256 493], N = 1001, w = 977"
test_encriptar 3 8 13 28 60 124 256 493 1001 977
test_encriptar_ascii 3 8 13 28 60 124 256 493 1001 977

echo -e " \nClave privada 2: [5 7 28 46 90 180 380 740 1480], N = 1820, w = 1431"
test_encriptar 5 7 28 46 90 180 380 740 1480 3410 2797
test_encriptar_ascii 5 7 28 46 90 180 380 740 1480 3410 2797

echo -e "\nClave privada 3: [2 3 7 15 31], N = 61, w = 17"
test_encriptar_ascii 2 3 7 15 31 61 17

echo -e "\nClave privada 4: [3, 4, 10, 18, 40, 83] N = 224, w = 179"
test_encriptar_ascii 3 4 10 18 40 83 224 179

rm cifrado descifrado

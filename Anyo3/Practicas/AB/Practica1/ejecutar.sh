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
    echo "Test 1: Texto genérico (lorem ipsum)"
    ./MHcypher -e -f test1 -o cifrado $@
    ./MHcypher -d -f cifrado -o descifrado $@
    cmp test1 descifrado &>/dev/null </dev/null && bien || mal
    echo "Test 2: 20KB de datos aleatorios"
    ./MHcypher -e -f test2 -o cifrado $@
    ./MHcypher -d -f cifrado -o descifrado $@
    cmp test2 descifrado &>/dev/null </dev/null && bien || mal
    ./MHcypher -e -f MHcypher.cc -o cifrado $@
    ./MHcypher -d -f cifrado -o descifrado $@
    cmp MHcypher.cc descifrado &>/dev/null </dev/null && bien || mal
    
    #rm cifrado descifrado
}

test_precond

test_encriptar 3 8 13 28 60 124 256 493 1001 977


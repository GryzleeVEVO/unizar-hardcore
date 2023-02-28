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
    echo -ne "Test: Mochila con números no ascendentes "
    ./MHcypher -e 1 2 4 3 5 7 10 14 1 2 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: Mochila con números repetidos "
    ./MHcypher -e 1 2 4 3 5 7 10 10 1 2 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: N menor que suma "
    ./MHcypher -e 4 8 10 12 16 24 48 60 13 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: N igual que suma "
    ./MHcypher -e 4 8 10 12 16 24 48 122 13 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: w negativo "
    ./MHcypher -e 3 4 8 13 24 32 44 57 224 -3 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: w no coprimo "
    ./MHcypher -e 3 4 8 13 24 32 44 57 224 58 &>/dev/null </dev/null && mal || bien
    echo -ne "Test: Todo correcto, pero mensaje vacío "
    ./MHcypher -e 3 4 8 13 24 32 44 57 224 179 &>/dev/null </dev/null && bien || mal
}

test_encriptar_desencriptar() {
    ./MHcypher -e -f   
}

test_precond

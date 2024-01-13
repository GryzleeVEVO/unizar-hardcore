#!/bin/bash

# Diseño y administración de redes
# Trabajo Tp6
#
# Autores:
#    Dorian Boleslaw Wozniak (817570@unizar.es)
#    Marcos Perez Guillén (820532@unizar.es)
#
# Descripción:
#   Script que permite configurar de forma automatizada una serie de hubs.
#
#   El script lee un fichero de texto que contiene la configuración de cada
#   hub. Cada entrada se compone de la IP de gestión del hub, seguido de una
#   serie de líneas que asocian cada segmento con una serie de puertos.
#
#   Si el fichero de configuración es correcto, configurará, de forma
#   concurrente, cada hub con la configuración especificada utiizando el
#   protocolo SNMP.
#
# Uso: ./tp6.sh <fichero_configuracion>
#
# Sintáxis de fichero de configuración:
#   IP
#   segmento: puerto1 puerto2 ... puerton
#
#   Ejemplo:
#
#   192.168.10.2
#   1: 3 6 7 8
#   2: 14 23 9
#   3: 17
#
#   192.168.20.2
#   1: 1 2
#   2: 17 21 24
#   4: 4 8 12 16

if [[ $# -ne 1 ]]; then
    echo "Uso: $0 <fichero_configuracion>"
    exit 1
fi

fichero="$1"
community="security"
snmp_segment_range=".1.3.6.1.4.1.43.10.26.1.1.1.5"

# Diccionario con asociaciones IP -> (Puerto->Segmento)
declare -A result

# Devuelve si un valor se encuentra en el rango [min, max]
is_within_range() {
    local value=$1
    local min=$2
    local max=$3
    ((value >= min && value <= max))
}

# Lee un fichero de texto con la configuración para una serie de hubs con
# los puertos los cuales se deben asignar en un segmento concreto.
#
# Si la sintáxis es incorrecta o los datos son incorrectos, se avisará y se
# detendrá la ejecución del script. Si en la configuración se declara un puerto
# en dos segmentos iguales se avisará por pantalla y se tomará el último valor
# leido como el actual
#
# Parámetros:
#   $1: Fichero a procesar
#
# Salida:
#   Se almacenan los resultados en result. result es un diccionario que contiene
#   parejas de IP con referencias a su diccionario de parejas puerto-segmento
#   correspondientes
leer_fichero() {
    local fichero=$1

    while IFS= read -r line || [[ -n "$line" ]]; do
        # Se encuentra con una IP nueva
        if [[ $line =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
            current_ip=$line
            # Diccionario Puerto->Segmento
            unset ip_segments
            declare -A ip_segments
        # Se encuentra con un segmento
        elif [[ $line =~ ^[0-9]+: ]]; then

            if [[ -z $current_ip ]]; then
                echo "[ERROR] Un segmento debe pertenecer a una IP"
                exit 1
            fi

            current_segment=${line%%:*}
            ports=${line#*:}

            if ! is_within_range "$current_segment" 1 4; then
                echo "[ERROR] $current_ip: segmento incorrecto $current_segment, debe estar entre 1 y 4"
                exit 1
            fi

            if ! [[ $ports =~ ^[0-9\ ]+$ ]]; then
                echo "[ERROR] $current_ip: formato de puerto incorrecto en el segmento $current_segment, debe ser una lista de numeros separados por espacios"
                exit 1
            fi

            # Añade puertos con su segmento correspondiente
            read -ra port_array <<<"$ports"
            for port in "${port_array[@]}"; do
                if ! is_within_range "$port" 1 24; then
                    echo "[ERROR]: $current_ip: puerto incorrecto $port en el segmento $current_segment, debe estar entre 1 y 24"
                    exit 1
                fi

                if [[ -n ${ip_segments["$port"]} ]]; then
                    echo "[AVISO] $current_ip: Redeclarado puerto $port al segmento $current_segment, perteneciía al puerto ${ip_segments["$port"]}"
                    echo "[AVISO] Se tomará el último valor proporcionado"
                fi

                ip_segments["$port"]=$current_segment
            done

            # Actualiza los datos para la IP actual
            result["$current_ip"]=$(declare -p ip_segments)
        # No reconoce formato de línea
        elif [[ -n $line ]]; then
            echo "[ERROR] $current_ip: formato incorrecto: $line"
            exit 1
        fi
    done <"$fichero"
}

# Dada una IP, obtiene su lista de asociaciones puerto-segmento y utiliza el
# protocolo SNMP para configurar el hub de forma autónoma
#
# Parámetros:
#   $1: IP
gestionar_hub() {
    local ip=$1
    eval "$(echo "${result[$ip]}")"

    segment1_id=$(snmpget -v1 -c "${community}" "${ip}" ${snmp_segment_range}.1.1001 | cut -d " " -f 4)
    segment2_id=$(snmpget -v1 -c "${community}" "${ip}" ${snmp_segment_range}.1.1002 | cut -d " " -f 4)
    segment3_id=$(snmpget -v1 -c "${community}" "${ip}" ${snmp_segment_range}.1.1003 | cut -d " " -f 4)
    segment4_id=$(snmpget -v1 -c "${community}" "${ip}" ${snmp_segment_range}.1.1004 | cut -d " " -f 4)

    for port in "${!ip_segments[@]}"; do
        segment="${ip_segments[$port]}"

        case "$segment" in
        1) segment_id="$segment1_id" ;;
        2) segment_id="$segment2_id" ;;
        3) segment_id="$segment3_id" ;;
        4) segment_id="$segment4_id" ;;
        esac

        snmpset -v1 -c "${community}" "${ip}" "${snmp_segment_range}".1."${port}" i "${segment_id}" &
    done

    while wait -n; do :; done
    echo "Configuración de $ip completada"
}

################################################################################

# Lee el fichero
leer_fichero "$fichero"

# Si la configuración es válida,
for ip in "${!result[@]}"; do
    gestionar_hub "$ip" &
done

while wait -n; do :; done
echo "Configuración de todos los hubs completada"

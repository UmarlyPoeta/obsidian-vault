#!/usr/bin/env bash
# Użycie:  ./sprawdz.sh 01          — testy twojej wersji (zadania/m01.cpp)
#          ./sprawdz.sh 04 --wzor   — testy wzorcowych rozwiązań
#          ./sprawdz.sh 04 --zaleznosci-wzor — twój m04, ale zależności (np. gauss z m02) z rozwiązań
#          ./sprawdz.sh wszystko    — wszystkie moduły, twoje wersje
set -euo pipefail
cd "$(dirname "$0")"
declare -A ZAL=( [04]="m02" [06]="m03" [07]="m02" [08]="m02" [09]="m03 m06" [10]="m02 m05" )
CXX=${CXX:-g++}
FLAGI="-std=c++17 -O2 -Wall -Wextra -Wno-unused-parameter"

uruchom() {
    local m=$1 src=$2 zsrc=$3 pliki=""
    for z in ${ZAL[$m]:-}; do pliki+=" $zsrc/$z.cpp"; done
    mkdir -p build
    $CXX $FLAGI "testy/test_m$m.cpp" "$src/m$m.cpp" $pliki -o "build/test_m$m" && "./build/test_m$m"
}

if [[ ${1:-} == "wszystko" ]]; then
    src=zadania; [[ ${2:-} == "--wzor" ]] && src=rozwiazania
    status=0
    for t in testy/test_m*.cpp; do m=${t#testy/test_m}; m=${m%.cpp}; uruchom "$m" $src $src || status=1; done
    exit $status
fi

m=${1:?podaj numer modułu, np. ./sprawdz.sh 01}
case ${2:-} in
    --wzor) uruchom "$m" rozwiazania rozwiazania ;;
    --zaleznosci-wzor) uruchom "$m" zadania rozwiazania ;;
    *) uruchom "$m" zadania zadania ;;
esac

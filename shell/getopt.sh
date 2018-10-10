#!/bin/bash
 
ARGS=`getopt -o "s:o:" -l "srcdir:,option:" -n "getopt.sh" -- "$@"`
 
eval set -- "${ARGS}"
 
while true; do
    case "${1}" in
        -s|--srcdir)
        echo -e "arg: specified ${1}"
        shift 2;
        ;;
        -o|--option)
        shift;
        if [[ -n "${1}" ]]; then
            echo -e "option: specified, value is ${1}"
            shift;
        fi
        ;;
        --)
        shift;
        break;
        ;;
    esac
done

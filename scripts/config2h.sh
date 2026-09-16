#!/bin/bash

echo "/* automatisch erzeugt */"

while read -r line; do
    case "$line" in
        CONFIG_*=y)
            key="${line%=y}"
            echo "#define ${key} 1"
            ;;
        CONFIG_*=*)
            key="${line%%=*}"
            value="${line#*=}"
            echo "#define ${key} ${value}"
            ;;
    esac
done < .config


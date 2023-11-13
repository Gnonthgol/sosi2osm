#!/bin/bash

set -euxo pipefail

for path in "$@"
do
    # Remove BOM (not needed for UTF-8)
    sed 's/\xef\xbb\xbf//' "$path" |
        iconv -f 'UTF-8' -t 'ISO-8859-10' |
        sed 's/TEGNSETT UTF-8/TEGNSETT ISO8859-10/' > "${path%.*}_iso8859-10.${path##*.}"
done

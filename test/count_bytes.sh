#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail
if [[ "${TRACE-0}" == "1" ]]; then
    set -o xtrace
fi

if [[ "${1-}" =~ ^-*h(elp)?$ ]]; then
    echo "Usage: ./count_bytes.sh

Tests whether \`randall\` outputs the input number of bytes.

"
    exit
fi

# Change to source files directory
cd "../$(dirname "$0")"

main() {
    make

    declare -r BOUND="1000"

    echo "Running $BOUND tests..."

    for ((i = 0; i < BOUND; i++)) do
        output_bytes=$(./randall "$i" | wc -c)

        if [[ "$output_bytes" != "$i" ]]; then
            echo "FAILED: Output $output_bytes bytes, expected $i bytes."
            exit 1
        fi
    done

    echo "PASSED: All $BOUND test cases passed."
    make clean
}

main "$@"

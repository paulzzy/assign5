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

cd "$(dirname "$0")"

main() {
    make

    declare -r NUM_TESTS="100"

    echo "Running $NUM_TESTS tests..."

    for ((i = 0; i < NUM_TESTS; i++)) do
        output_bytes=$(./randall "$i" | wc -c)

        if [[ "$output_bytes" != "$i" ]]; then
            echo "FAILED: Output $output_bytes bytes, expected $i bytes."
            exit 1
        fi
    done

    echo "PASSED: All $NUM_TESTS test cases passed."
    make clean
}

main "$@"

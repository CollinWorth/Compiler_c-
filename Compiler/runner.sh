#!/bin/bash

# Build compiler
make

TEST_DIR=${1:-.}
COMPILER=./c-

mkdir -p outputs

diff_count=0
total_count=0

for file in "$TEST_DIR"/*.c-; do
    [ -e "$file" ] || { echo "No .c- files found in $TEST_DIR"; exit 0; }

    base=$(basename "$file" .c-)
    output="outputs/$base.out"
    expected="$base.out"

    total_count=$((total_count + 1))

    echo "-----------------------------------"
    echo "Running test: $file"
    echo "-----------------------------------"

    # Run your compiler
    $COMPILER < "$file" > "$output" 2>&1

    # Check for expected output
    if [ -f "$expected" ]; then
        # Compare outputs silently first
        if ! diff -q "$expected" "$output" > /dev/null; then
            echo "⚠️  Differences found for $base:"
            echo "-----------------------------------"
            diff -y --suppress-common-lines "$expected" "$output"
            echo "-----------------------------------"
            diff_count=$((diff_count + 1))
        fi
    else
        echo "No expected output file ($expected) found."
    fi

    echo
done

echo "==================================="
echo "Tests completed: $total_count"
echo "Differences found: $diff_count"
echo "==================================="

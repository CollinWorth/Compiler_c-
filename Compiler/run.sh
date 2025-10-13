#!/bin/bash

count=0
TEST_DIR=${1:-testDataA3}
COMPILER=${2:-./c-}

# Create fresh output and diff directories
#rm -rf "$TEST_DIR/outputs" "$TEST_DIR/diffs"
#mkdir -p "$TEST_DIR/outputs" "$TEST_DIR/diffs"

for file in "$TEST_DIR"/*.c-; do
    base=$(basename "$file" .c-)
    test_output="$TEST_DIR/outputs/$base.test"
    diff_output="$TEST_DIR/diffs/$base.diff"
    expected_output="$TEST_DIR/$base.out"

    # Run your compiler
    $COMPILER -P "$file" > "$test_output" 2>&1

    # Only diff if expected output exists
    if [ -f "$expected_output" ]; then
        diff -y "$test_output" "$expected_output" > "$diff_output"
        if [ -s "$diff_output" ]; then
            echo "Difference found in $base.c-"
            ((count++))
        else
            # No differences → remove empty diff
            rm "$diff_output"
        fi
    else
        echo "Warning: Expected output $expected_output not found."
    fi
done

echo "Total differences: $count"

#!/bin/bash

# Colors for output formatting
GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
NC="\033[0m" # No color

# Cleanup before test
rm -rf test_files
mkdir -p test_files

# Create test files
echo -e "Hello World\n42 School\nPipex Project" > test_files/input1.txt
echo -e "apple\nbanana\ncherry\ndate" > test_files/input2.txt
echo -e "This is a test file for Pipex\nUNIX pipes are powerful" > test_files/input3.txt
touch test_files/empty.txt

# Special files for edge cases
echo -e "Hello\nHELLO\nhello\nhElLo" > test_files/mixed_case.txt
echo -e "42\n101\n2024\n999" > test_files/numbers.txt

# Set proper permissions
chmod -R 644 test_files/*

# Counters for test results
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run a basic test case
run_test() {
    INPUT=$1
    CMD1=$2
    CMD2=$3
    OUTPUT=$4

    EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    PIPEX_OUTPUT="test_files/pipex_$OUTPUT"

    # Run shell command
    timeout 3 bash -c "< \"$INPUT\" $CMD1 | $CMD2 > \"$EXPECTED_OUTPUT\""

    # Run pipex command
    timeout 3 ./pipex "$INPUT" "$CMD1" "$CMD2" "$PIPEX_OUTPUT"

    # Compare outputs
    if diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT" > /dev/null; then
        echo -e "${GREEN}✅ Test Passed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Test Failed${NC}"
        echo "Test: <$INPUT $CMD1 | $CMD2 >"
        echo "Expected Output:"
        cat "$EXPECTED_OUTPUT"
        echo "Actual Pipex Output:"
        cat "$PIPEX_OUTPUT"
        echo "Difference:"
        diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
}

# Function to test multiple pipes (Bonus)
run_bonus_test() {
    INPUT=$1
    OUTPUT=$2
    shift 2
    CMDS=("$@")

    EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    PIPEX_OUTPUT="test_files/pipex_$OUTPUT"

    # Construct shell command dynamically
    SHELL_CMD="< \"$INPUT\""
    for CMD in "${CMDS[@]}"; do
        SHELL_CMD+=" $CMD |"
    done
    SHELL_CMD="${SHELL_CMD%|} > \"$EXPECTED_OUTPUT\""

    # Run shell command
    timeout 3 bash -c "$SHELL_CMD"

    # Run pipex command
    timeout 3 ./pipex "$INPUT" "${CMDS[@]}" "$PIPEX_OUTPUT"

    # Compare outputs
    if diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT" > /dev/null; then
        echo -e "${GREEN}✅ Bonus Test Passed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Bonus Test Failed${NC}"
        echo "Test: <$INPUT ${CMDS[*]} >"
        echo "Expected Output:"
        cat "$EXPECTED_OUTPUT"
        echo "Actual Pipex Output:"
        cat "$PIPEX_OUTPUT"
        echo "Difference:"
        diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
}

# Run basic tests
echo -e "${BLUE}Running basic tests...${NC}"
run_test "test_files/input1.txt" "cat" "wc -l" "out1.txt"
run_test "test_files/input2.txt" "grep a" "wc -w" "out2.txt"
run_test "test_files/input3.txt" "ls -l" "grep .c" "out3.txt"

# Run edge case tests
echo -e "${BLUE}Running edge case tests...${NC}"
run_test "test_files/mixed_case.txt" "grep HELLO" "wc -l" "case_sensitive.txt"
run_test "test_files/numbers.txt" "sort -n" "tail -1" "max_number.txt"
run_test "test_files/empty.txt" "cat" "wc -l" "empty.txt"

# Run invalid command tests
echo -e "${BLUE}Running invalid command tests...${NC}"
run_test "test_files/input1.txt" "invalidcmd" "wc -l" "invalid_cmd.txt"

# Run missing file tests
echo -e "${BLUE}Running missing file tests...${NC}"
run_test "test_files/nonexistent.txt" "cat" "wc -l" "missing_file.txt"

# Run multiple pipes tests (Bonus)
echo -e "${BLUE}Running multiple pipes tests (Bonus)...${NC}"
run_bonus_test "test_files/input1.txt" "multi_pipes.txt" "grep Pipex" "sort" "wc -l"
run_bonus_test "test_files/input2.txt" "multi_pipes_2.txt" "grep a" "tr a-z A-Z" "sort"

# Summary
echo -e "${BLUE}Test Summary:${NC}"
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed Tests: $PASSED_TESTS${NC}"
echo -e "${RED}Failed Tests: $FAILED_TESTS${NC}"

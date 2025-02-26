#!/bin/bash
# Colors
GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
NC="\033[0m"

# Cleanup before tests
rm -rf test_files
mkdir -p test_files

# Create test files
echo -e "line one\nline two\nline three" > test_files/input1.txt
echo -e "apple\nbanana\ncherry\ndate" > test_files/input2.txt
echo -e "This is a test file for Pipex\nUNIX pipes are powerful" > test_files/input3.txt
touch test_files/empty.txt

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run basic two-command pipes and check output
run_test() {
    local INPUT="$1"
    local CMD1="$2"
    local CMD2="$3"
    local OUTPUT="$4"
    local EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    local PIPEX_OUTPUT="test_files/pipex_$OUTPUT"

    # Build shell pipeline for expected output
    timeout 3 bash -c "< \"$INPUT\" $CMD1 | $CMD2 > \"$EXPECTED_OUTPUT\""

    # Run pipex pipeline
    timeout 3 ./pipex "$INPUT" "$CMD1" "$CMD2" "$PIPEX_OUTPUT"

    # Compare results
    if diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT" > /dev/null; then
        echo -e "${GREEN}✅ Test Passed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Test Failed${NC}"
        echo "Test: <\"$INPUT\" $CMD1 | $CMD2 >"
        echo "Difference:"
        diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
}

# Function to run a test under Valgrind to check for leaks
run_valgrind_test() {
    local INPUT="$1"
    local CMD1="$2"
    local CMD2="$3"
    local OUTPUT="$4"
    echo -e "\n${BLUE}Valgrind check: <$INPUT $CMD1 $CMD2 $OUTPUT>${NC}"
    valgrind --leak-check=full --show-leak-kinds=all \
             --trace-children=yes --track-fds=yes \
             ./pipex "$INPUT" "$CMD1" "$CMD2" "/dev/null"
}

# Run Basic Tests
echo -e "${BLUE}Running basic tests...${NC}"
run_test "test_files/input1.txt" "cat" "wc -l" "out1.txt"
run_test "test_files/input2.txt" "grep a" "wc -w" "out2.txt"
run_test "test_files/input3.txt" "ls -l" "grep .c" "out3.txt"

echo -e "${BLUE}Running edge case tests...${NC}"
run_test "test_files/empty.txt" "cat" "wc -l" "empty.txt"
run_test "test_files/input1.txt" "invalidcmd" "wc -l" "invalid_cmd.txt"
run_test "test_files/nonexistent.txt" "cat" "wc -l" "missing_file.txt"

# Optional: Run Valgrind on a subset of tests to confirm no memory leaks
run_valgrind_test "test_files/input1.txt" "cat" "wc -l" "valgrind_out1.txt"
run_valgrind_test "test_files/empty.txt" "cat" "wc -l" "valgrind_out2.txt"

# Summary
echo -e "\n${BLUE}Test Summary:${NC}"
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed Tests: $PASSED_TESTS${NC}"
echo -e "${RED}Failed Tests: $FAILED_TESTS${NC}"

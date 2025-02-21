#!/bin/bash

# Colors
GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
NC="\033[0m"  

# Cleanup before test
rm -rf test_files
mkdir -p test_files

# Create test files
echo -e "line one\nline two\nline three" > test_files/input1.txt
echo -e "apple\nbanana\ncherry\ndate" > test_files/input2.txt
touch test_files/empty.txt

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to test multiple pipes (Bonus mode)
run_bonus_test() {
    INPUT=$1
    OUTPUT=$2
    shift 2
    CMDS=("$@")

    EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    PIPEX_OUTPUT="test_files/pipex_$OUTPUT"

    # Build a shell pipeline dynamically:
    SHELL_CMD="< \"$INPUT\""
    for CMD in "${CMDS[@]}"; do
        SHELL_CMD+=" $CMD |"
    done
    SHELL_CMD="${SHELL_CMD%|} > \"$EXPECTED_OUTPUT\""

    timeout 3 bash -c "$SHELL_CMD"
    timeout 3 ./pipex_bonus "$INPUT" "${CMDS[@]}" "$PIPEX_OUTPUT"

    if diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT" > /dev/null; then
        echo -e "${GREEN}✅ Bonus Test Passed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Bonus Test Failed${NC}"
        echo "Difference:"
        diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
}

# Function to test here_doc (Bonus mode)
run_here_doc_test() {
    LIMITER=$1
    CMD1=$2
    CMD2=$3
    OUTPUT=$4

    EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    PIPEX_OUTPUT="test_files/pipex_$OUTPUT"

    HERE_DOC_INPUT=$'line1\nline2\nTest line\nAnother test\nEOF'

    timeout 3 bash -c "echo -e '$HERE_DOC_INPUT' | $CMD1 | $CMD2 > \"$EXPECTED_OUTPUT\""
    timeout 3 ./pipex_bonus here_doc "$LIMITER" "$CMD1" "$CMD2" "$PIPEX_OUTPUT" <<< "$HERE_DOC_INPUT"

    if diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT" > /dev/null; then
        echo -e "${GREEN}✅ Here_doc Test Passed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Here_doc Test Failed${NC}"
        echo "Difference:"
        diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
}

# Run multiple pipes tests (Bonus)
echo -e "${BLUE}Running multiple pipes tests (Bonus)...${NC}"
run_bonus_test "test_files/input1.txt" "multi_pipes.txt" "grep Pipex" "sort" "wc -l"
run_bonus_test "test_files/input2.txt" "multi_pipes_2.txt" "grep a" "tr a-z A-Z" "sort"

# Run here_doc tests (Bonus)
echo -e "${BLUE}Running here_doc tests (Bonus)...${NC}"
run_here_doc_test "EOF" "grep Test" "cat" "here_doc_output.txt"
run_here_doc_test "EOF" "grep -i test" "sort" "here_doc_output2.txt"

# Summary
echo -e "${BLUE}Test Summary:${NC}"
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed Tests: $PASSED_TESTS${NC}"
echo -e "${RED}Failed Tests: $FAILED_TESTS${NC}"
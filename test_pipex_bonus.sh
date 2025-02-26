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
touch test_files/empty.txt

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to test multiple pipes (Bonus mode)
run_bonus_test() {
    local INPUT="$1"
    local OUTPUT="$2"
    shift 2
    local CMDS=("$@")
    local EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    local PIPEX_OUTPUT="test_files/pipex_$OUTPUT"
    local SHELL_CMD="< \"$INPUT\""

    for CMD in "${CMDS[@]}"; do
        SHELL_CMD+=" $CMD |"
    done
    SHELL_CMD="${SHELL_CMD%|} > \"$EXPECTED_OUTPUT\""

    # Run shell pipeline and pipex_bonus
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

# Function to run Valgrind check for multi-pipes tests
run_valgrind_bonus() {
    echo -e "\n${BLUE}Valgrind check (Multi-Pipes):${NC}"
    echo "Command: ./pipex_bonus $*"
    valgrind --leak-check=full --show-leak-kinds=all \
             --trace-children=yes --track-fds=yes \
             ./pipex_bonus "$@"
}

# Function to test here_doc (Bonus mode)
run_here_doc_test() {
    local LIMITER="$1"
    local CMD1="$2"
    local CMD2="$3"
    local OUTPUT="$4"
    local EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    local PIPEX_OUTPUT="test_files/pipex_$OUTPUT"
    local HERE_DOC_INPUT=$'line1\nline2\nTest line\nAnother test\nEOF'

    timeout 3 bash -c "echo -e '$HERE_DOC_INPUT' | $CMD1 | $CMD2 > \"$EXPECTED_OUTPUT\""
    # Run pipex_bonus in here_doc mode
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

# Function to run Valgrind check for here_doc tests
run_valgrind_heredoc() {
    local LIMITER="$1"
    local CMD1="$2"
    local CMD2="$3"
    echo -e "\n${BLUE}Valgrind check (Here_doc):${NC}"
    echo "Command: ./pipex_bonus here_doc $LIMITER $CMD1 $CMD2 /dev/null"
    echo $'line1\nline2\nEOF' | valgrind --leak-check=full --show-leak-kinds=all \
        --trace-children=yes --track-fds=yes \
        ./pipex_bonus here_doc "$LIMITER" "$CMD1" "$CMD2" /dev/null
}

# Run Bonus Tests
echo -e "${BLUE}Running multiple pipes tests (Bonus)...${NC}"
run_bonus_test "test_files/input1.txt" "multi_pipes.txt" "grep line" "sort" "wc -l"
run_bonus_test "test_files/input2.txt" "multi_pipes_2.txt" "grep a" "tr a-z A-Z" "sort"

echo -e "${BLUE}Running here_doc tests (Bonus)...${NC}"
run_here_doc_test "EOF" "grep Test" "cat" "here_doc_output.txt"
run_here_doc_test "EOF" "grep -i test" "sort" "here_doc_output2.txt"

# Optional Valgrind checks
run_valgrind_bonus "test_files/input1.txt" "cat" "wc -l" "test_files/devnull.txt"
run_valgrind_heredoc "EOF" "grep line" "wc -l"

# Summary
echo -e "\n${BLUE}Test Summary:${NC}"
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed Tests: $PASSED_TESTS${NC}"
echo -e "${RED}Failed Tests: $FAILED_TESTS${NC}"

#!/bin/bash

# Colors for output formatting
GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
NC="\033[0m" # No color

# Ensure cleanup before test
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

# Function to run a test case
run_test() {
    INPUT=$1
    CMD1=$2
    CMD2=$3
    OUTPUT=$4

    EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    PIPEX_OUTPUT="test_files/pipex_$OUTPUT"

    # Run shell command
    < "$INPUT" $CMD1 | $CMD2 > "$EXPECTED_OUTPUT"
    
    # Run pipex command
    ./pipex "$INPUT" "$CMD1" "$CMD2" "$PIPEX_OUTPUT"

    # Compare outputs
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "\n${BLUE}Test: <$INPUT $CMD1 | $CMD2 >${NC}"
    echo -e "${BLUE}Expected Output:${NC}"
    cat "$EXPECTED_OUTPUT"
    echo -e "\n${BLUE}Actual Pipex Output:${NC}"
    cat "$PIPEX_OUTPUT"
    echo ""

    if diff -q "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT" > /dev/null; then
        echo -e "${GREEN}✅ Test Passed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Test Failed${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# Function to test multiple pipes (Bonus)
run_bonus_test() {
    OUTPUT=$1
    EXPECTED_OUTPUT="test_files/shell_$OUTPUT"
    PIPEX_OUTPUT="test_files/pipex_$OUTPUT"

    # Run shell command
    bash -c "$2" > "$EXPECTED_OUTPUT"

    # Run pipex command
    eval "./pipex $3 \"$PIPEX_OUTPUT\""

    # Compare outputs
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "\n${BLUE}Bonus Test: $2${NC}"
    echo -e "${BLUE}Expected Output:${NC}"
    cat "$EXPECTED_OUTPUT"
    echo -e "\n${BLUE}Actual Pipex Output:${NC}"
    cat "$PIPEX_OUTPUT"
    echo ""

    if diff -q "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT" > /dev/null; then
        echo -e "${GREEN}✅ Test Passed${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ Test Failed${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$EXPECTED_OUTPUT" "$PIPEX_OUTPUT"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# Run basic tests
echo "Running basic tests..."
run_test "test_files/input1.txt" "cat" "wc -l" "out1.txt"
run_test "test_files/input2.txt" "grep a" "wc -w" "out2.txt"
run_test "test_files/input3.txt" "ls -l" "grep .c" "out3.txt"

# Run edge case tests
echo "Running edge case tests..."
run_test "test_files/mixed_case.txt" "grep HELLO" "wc -l" "case_sensitive.txt"
run_test "test_files/numbers.txt" "sort -n" "tail -1" "max_number.txt"
run_test "test_files/empty.txt" "cat" "wc -l" "empty.txt"

# Run invalid command tests
echo "Running invalid command tests..."
./pipex "test_files/input1.txt" "invalidcmd" "wc -l" "test_files/pipex_invalid.txt" &> test_files/pipex_error.txt
if grep -q "command not found" test_files/pipex_error.txt; then
    echo -e "${GREEN}✅ Test Passed: Invalid command${NC}"
else
    echo -e "${RED}❌ Test Failed: Invalid command${NC}"
fi

# Run missing file tests
echo "Running missing file tests..."
./pipex "test_files/missing.txt" "cat" "wc -l" "test_files/pipex_missing.txt" &> test_files/pipex_missing_error.txt
if grep -q "No such file" test_files/pipex_missing_error.txt; then
    echo -e "${GREEN}✅ Test Passed: Missing input file${NC}"
else
    echo -e "${RED}❌ Test Failed: Missing input file${NC}"
fi

# Run bonus tests (Multiple pipes)
echo "Running multiple pipes tests..."
run_bonus_test "multi_pipe1.txt" "< test_files/input1.txt cat | grep Pipex | wc -l" "test_files/input1.txt 'cat' 'grep Pipex' 'wc -l'"
run_bonus_test "multi_pipe2.txt" "< test_files/input3.txt cat | grep UNIX | sort | wc -w" "test_files/input3.txt 'cat' 'grep UNIX' 'sort' 'wc -w'"

# Run bonus test (Here Document)
echo "Running here_doc tests..."
echo -e "Hello\n42\nUNIX\nEND" | ./pipex here_doc END "cat" "wc -l" "test_files/pipex_here_doc.txt"
echo -e "Hello\n42\nUNIX" | wc -l > "test_files/shell_here_doc.txt"
if diff -q "test_files/shell_here_doc.txt" "test_files/pipex_here_doc.txt" > /dev/null; then
    echo -e "${GREEN}✅ Test Passed: Here Document${NC}"
else
    echo -e "${RED}❌ Test Failed: Here Document${NC}"
    diff "test_files/shell_here_doc.txt" "test_files/pipex_here_doc.txt"
fi

# Summary of results
echo -e "\n${BLUE}Test Summary:${NC}"
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "${GREEN}✅ Passed: $PASSED_TESTS${NC}"
echo -e "${RED}❌ Failed: $FAILED_TESTS${NC}"

if [ "$FAILED_TESTS" -eq 0 ]; then
    echo -e "${GREEN}🎉 All tests passed successfully! 🎉${NC}"
else
    echo -e "${RED}⚠️ Some tests failed! Check the output above. ⚠️${NC}"
fi

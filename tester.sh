#!/bin/bash

MINISHELL=./minishell   # path to your compiled minishell binary

run_parser_test() {
    # We use printf to interpret \n as a newline if needed, 
    # but strictly handle the input as a block.
    input="$1"
    desc="$2"

    echo
    echo "--- TEST: $desc ---"
    
    # Visualizing the newline for the log
    echo "Input:"
    echo "$input"

    # Feed input into minishell. 
    # We use 'printf' so we can pass multiple lines easily.
    output=$(printf "%b" "$input" | $MINISHELL 2>&1)
    # status=$?

    echo "--- OUTPUT ---"
    echo "$output"
    echo "----------------"
}

#############################################
#                TEST CASES                 #
#############################################

# 1. Standard test
run_parser_test "echo hello world" "Basic Echo"

# 2. IMPORTANT: Use single quotes '' so the TESTER doesn't expand $?
# We want Minishell to receive the characters '$' and '?'
run_parser_test 'echo $?' "Exit Status Check"

# 3. The Sequence Test you asked for
# We pass two lines in one string using \n
# Line 1: doesnot (fails)
# Line 2: $EMPTY (should likely do nothing or return 0 depending on implementation)
# Line 3: echo $? (checks the status of the $EMPTY command)
run_parser_test "doesnot\n\$EMPTY\necho \$?" "$EMPTY"

# 4. Additional test: heredoc with interrupt
run_parser_test "cat << EOF"
echo "All tests completed."

#!/bin/bash

# --- CONFIGURATION ---
# We use the system bash to generate reference outputs
SHELL_CMD="bash"
LOGFILE="bash_results.md"

# --- COLORS ---
RESET='\033[0m'
BOLD='\033[1m'
BLUE='\033[34m'
CYAN='\033[36m'
MAGENTA='\033[35m'
YELLOW='\033[33m'
RED='\033[31m'

# --- LOG SETUP ---
echo "# Bash Reference Results" > "$LOGFILE"
echo "Generated on: $(date)" >> "$LOGFILE"
echo "This file serves as the GOLDEN STANDARD for comparison." >> "$LOGFILE"
echo "" >> "$LOGFILE"

echo -e "${BLUE}${BOLD}### RUNNING BASH REFERENCE ###${RESET}"
echo "Results will be saved to: $LOGFILE"
echo "----------------------------------------"

# --- MAIN FUNCTION (BASH VERSION) ---
# This function mirrors the one in tester.sh but runs Bash without Valgrind
run_parser_test() {
    local raw_input="$1"
    local desc="$2"

    # 1. INPUT PREPARATION
    # We inject the exit code check exactly like in Minishell tester
    # to make the outputs 1:1 comparable using diff.
    local input_with_exit="${raw_input}\necho \"[EXIT: \$?]\""

    # 2. LOGGING (HEADER)
    {
        echo "## Test: $desc"
        echo "**Input:** \`$raw_input\`"
        echo "\`\`\`bash" 
    } >> "$LOGFILE"

    # 3. EXECUTION
    # Run system Bash, capture stdout and stderr
    # Note: We are not checking for leaks here, obviously.
    local current_output
    current_output=$(printf "%b" "$input_with_exit" | $SHELL_CMD 2>&1)

    # 4. LOGGING (OUTPUT)
    echo "$current_output" >> "$LOGFILE"
    echo "\`\`\`" >> "$LOGFILE"
    echo "---" >> "$LOGFILE"

    # Terminal Feedback (Just a progress indicator)
    echo -e "  ${MAGENTA}[BASH]${RESET} $desc"
}

# --- FILE LOADER ---
# Reusing the same test cases from the 'cases/' directory

if [ $# -eq 0 ]; then
    TEST_FILES=cases/*.sh
else
    TEST_FILES="$@"
fi

for file in $TEST_FILES; do
    if [ -f "$file" ]; then
        echo -e "\n${BOLD}>>> Generating reference for: $file${RESET}"
        echo "# Suite: $file" >> "$LOGFILE"
        source "$file"
    else
        echo -e "${RED}File $file does not exist!${RESET}"
    fi
done

echo -e "\n${CYAN}Done. Now you can compare the files:${RESET}"
echo -e "1. Minishell Output: ${YELLOW}minishell_tester.md${RESET}"
echo -e "2. Bash Reference:   ${YELLOW}bash_results.md${RESET}"

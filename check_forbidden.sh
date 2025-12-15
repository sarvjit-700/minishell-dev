#!/bin/bash

# ==============================================================================
#                               CONFIGURATION
# ==============================================================================

# Your Name Here
AUTHOR="ssukhija"

# Path to your executable
BINARY_NAME="./minishell"

# Allowed functions (Regex format)
ALLOWED="readline|rl_clear_history|rl_on_new_line|rl_replace_line|rl_redisplay|add_history|\
printf|malloc|free|write|access|open|read|close|fork|wait|waitpid|wait3|wait4|\
signal|sigaction|sigemptyset|sigaddset|kill|exit|getcwd|chdir|stat|lstat|fstat|\
unlink|execve|dup|dup2|pipe|opendir|readdir|closedir|strerror|perror|isatty|\
ttyname|ttyslot|ioctl|getenv|tcsetattr|tcgetattr|tgetent|tgetflag|tgetnum|\
tgetstr|tgoto|tputs"

# Suspicious functions to grep for
SUSPICIOUS="isdigit|isalpha|isalnum|isascii|isprint|tolower|toupper|\
fcntl|dprintf|fprintf|sprintf|snprintf|strtok|strchr|strrchr|strstr|strnstr|\
strdup|atoi|atol|calloc|memset|memcpy|memmove|bzero|memcmp|strcmp|strncmp|\
puts|putchar"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BLUE='\033[0;34m'
RESET='\033[0m'

# ==============================================================================
#                                  FUNCTIONS
# ==============================================================================

print_header() {
    clear
    echo -e "${BLUE}"
    echo "  __  __ _       _     _          _ _ "
    echo " |  \/  (_)_ __ (_)___| |__   ___| | |"
    echo " | |\/| | | '_ \| / __| '_ \ / _ \ | |"
    echo " | |  | | | | | | \__ \ | | |  __/ | |"
    echo " |_|  |_|_|_| |_|_|___/_| |_|\___|_|_|"
    echo -e "${RESET}"
    echo -e "      ${CYAN}FORBIDDEN FUNCTION CHECKER${RESET}"
    echo -e "      Created by: ${YELLOW}$AUTHOR${RESET}"
    echo "======================================================"
    echo ""
}

# ==============================================================================
#                               MAIN EXECUTION
# ==============================================================================

print_header

# --- STEP 1: SOURCE CODE SCAN ---
echo -e "${CYAN}>>> 1. SCANNING SOURCE CODE (GREP)${RESET}"
echo -e "Looking for: ${YELLOW}standard C lib string/ctype/print functions${RESET}..."

# We grep for suspicious words, but we pipe to another grep to EXCLUDE valid headers
# This prevents flagging '#include <fcntl.h>'
GREP_OUTPUT=$(grep -REn "\b($SUSPICIOUS)\b" . \
    --include=\*.{c,h} \
    --exclude-dir=libft \
    --exclude-dir=.git \
    | grep -vE "#\s*include\s+<fcntl.h>")

if [ -z "$GREP_OUTPUT" ]; then
    echo -e "${GREEN}✅  Clean. No suspicious text patterns found in .c/.h files.${RESET}"
else
    echo -e "${RED}⚠️  SUSPICIOUS PATTERNS DETECTED:${RESET}"
    echo "$GREP_OUTPUT"
fi
echo ""

# --- STEP 2: BINARY SCAN ---
echo -e "${CYAN}>>> 2. SCANNING BINARY SYMBOLS (NM)${RESET}"
echo -e "Checking compiled executable: ${YELLOW}$BINARY_NAME${RESET}..."

if [ -f "$BINARY_NAME" ]; then
    # 1. nm -u : Get undefined symbols
    # 2. awk : Cleanup formatting
    # 3. grep -vE : Remove ALLOWED functions (ignoring @GLIBC suffixes)
    # 4. grep -vE : Remove ft_ functions
    # 5. grep -vE : Remove system internal symbols (_)
    FORBIDDEN_CALLS=$(nm -u "$BINARY_NAME" | awk '{print $2}' | \
    grep -vE "^($ALLOWED)(@.*)?$" | \
    grep -vE "^ft_" | \
    grep -vE "^_")

    if [ -z "$FORBIDDEN_CALLS" ]; then
        echo -e "${GREEN}✅  PASS: No forbidden external functions found in binary.${RESET}"
    else
        echo -e "${RED}❌  FAIL: The following forbidden functions are linked:${RESET}"
        echo -e "${RED}$FORBIDDEN_CALLS${RESET}"
        echo -e "${YELLOW}(Tip: 'puts' or 'putchar', can be printf optimizations${RESET}"
        echo -e "${YELLOW}       which is ok!)${RESET}"
    fi
else
    echo -e "${RED}⚠️  Binary not found!${RESET}"
    echo "   Please compile with 'make' or 'make re' (add -fno-builtin to CFLAGS if needed)."
fi

echo ""
echo "======================================================"

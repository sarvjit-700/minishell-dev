#!/bin/bash

# --- KONFIGURACE ---
# PŘIDÁNO: --trace-children=yes (Nutné pro odhalení leaků v child procesech!)
MINISHELL="valgrind  --leak-check=full --log-file=valgrind.log  --track-origins=yes --show-leak-kinds=all --suppressions=valgrind.supp ./minishell"
LOGFILE="minishell_tester.md"

# --- BARVY ---
RESET='\033[0m'
BOLD='\033[1m'
GREEN='\033[32m'
RED='\033[31m'
CYAN='\033[36m'
YELLOW='\033[33m'

# --- STATISTIKA ---
TOTAL_TESTS=0
FAILED_TESTS=0

# --- SETUP LOGU ---
echo "# Minishell Tester Log" > "$LOGFILE"
echo "Generated on: $(date)" >> "$LOGFILE"
echo "" >> "$LOGFILE"

echo -e "${CYAN}${BOLD}### STARTING TESTER ###${RESET}"
echo "Detailed log file: $LOGFILE"
echo "----------------------------------------"

# --- HLAVNÍ FUNKCE ---
run_parser_test() {
    local raw_input="$1"
    local desc="$2"
    
    ((TOTAL_TESTS++))

    # 1. PŘÍPRAVA VSTUPU
    local input_with_exit="${raw_input}\necho \"[EXIT: \$?]\""

    # Vyčistíme log valgrindu
    > valgrind.log

    # 2. ZÁPIS DO LOGU
    {
        echo "## Test $TOTAL_TESTS: $desc"
        echo "**Input:** \`$raw_input\`"
        echo "\`\`\`bash" 
    } >> "$LOGFILE"

    # 3. SPUŠTĚNÍ
    local current_output
    current_output=$(printf "%b" "$input_with_exit" | $MINISHELL 2>&1)

    # 4. ZÁPIS VÝSTUPU
    echo "$current_output" >> "$LOGFILE"
    echo "\`\`\`" >> "$LOGFILE"

    # 5. KONTROLA VALGRINDU
    # Pokud grep nenajde "0 errors", je to chyba
    local valgrind_status="OK"
   if grep "ERROR SUMMARY" valgrind.log | grep -v "0 errors" > /dev/null; then
         valgrind_status="LEAKS"
         ((FAILED_TESTS++))
    fi

    # 6. VÝPIS
    if [ "$valgrind_status" == "OK" ]; then
        echo -e "  ${GREEN}[OK]${RESET} $desc"
        echo "**Valgrind:** ✅ OK" >> "$LOGFILE"
    else
        echo -e "  ${RED}[LEAKS]${RESET} $desc"
        # Zapíšeme detaily chyby do logu
        echo "**Valgrind:** ❌ ERRORS DETECTED" >> "$LOGFILE"
        echo "\`\`\`" >> "$LOGFILE"
        cat valgrind.log >> "$LOGFILE"
        echo "\`\`\`" >> "$LOGFILE"
        
        # VOLITELNÉ: Odkomentuj, pokud chceš zastavit testy hned při první chybě
        # echo -e "${RED}Stopping execution due to leaks...${RESET}"
        # exit 1
    fi

    echo "---" >> "$LOGFILE"
}

# --- NAČÍTÁNÍ SOUBORŮ ---

if [ $# -eq 0 ]; then
    TEST_FILES=cases/*.sh
else
    TEST_FILES="$@"
fi

for file in $TEST_FILES; do
    if [ -f "$file" ]; then
        echo -e "\n${BOLD}>>> Suite: $file${RESET}"
        echo "# Suite: $file" >> "$LOGFILE"
        source "$file"
    else
        echo -e "${RED}File $file does not exist!${RESET}"
    fi
done

# --- SOUHRN NA KONCI (TOHLE TI CHYBĚLO) ---
echo "----------------------------------------"
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}${BOLD}VÝSLEDEK: Všechny testy ($TOTAL_TESTS) prošly bez leaků! 🎉${RESET}"
else
    echo -e "${RED}${BOLD}VÝSLEDEK: Nalezeny chyby v $FAILED_TESTS z $TOTAL_TESTS testů! ❌${RESET}"
    echo -e "Detaily najdeš v souboru ${BOLD}$LOGFILE${RESET} (hledej 'ERRORS DETECTED')"
fi
echo "----------------------------------------"
echo -e "\n${CYAN}Done. Results saved in: $LOGFILE${RESET}"

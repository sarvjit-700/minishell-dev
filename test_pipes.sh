#!/bin/bash
MINISHELL=./minishell
TMP1=.minishell_out
TMP2=.bash_out

run_test() {
    local cmd="$1"
    echo "---- TEST: $cmd ----"

    # run minishell (feed command then exit)
    { echo "$cmd"; echo "exit"; } | $MINISHELL > $TMP1 2>&1
    # run bash
    echo "$cmd" | bash > $TMP2 2>&1

    # filter minishell prompt lines + exit echoes
    sed -i '/^minishell\$/d' $TMP1
    sed -i '/^exit$/d' $TMP1

    diff -u $TMP2 $TMP1
    if [ $? -eq 0 ]; then
        echo "✅ PASS"
    else
        echo "❌ FAIL"
    fi
    echo
}

# --- TEST CASES ---
run_test "ls | wc -l"
run_test "echo hi | cat"
# run_test "nosuchcmd | wc -l"
run_test "cat Makefile | grep minishell"
run_test "ls | grep src | wc -l"
run_test "echo hi | wc -c"
run_test "pwd | cat | wc -l"

# --- PIPELINE EDGE CASES ---
run_test "ls | cat | wc -l"
run_test "echo hello | grep h"
run_test "echo hello | grep z"
run_test "echo hello | grep -v h"
run_test "echo hi | wc -c"
run_test "pwd | wc -c"
run_test "whoami | cat | cat | wc -l"
run_test "ls | grep minishell | wc -l"

# --- COMMAND NOT FOUND VARIATIONS ---
# run_test "nosuchcmd | cat"
# run_test "nosuchcmd | nosuchcmd2"
# un_test "ls | nosuchcmd"
# run_test "nosuchcmd | wc -l"
# run_test "ls | grep xyz | nosuchcmd"
# run_test "echo hi | nosuchcmd"

# --- BUILTIN IN PIPES ---
run_test "echo hi | cat"
run_test "echo hi | wc -c"
run_test "pwd | wc -l"
run_test "cd . | wc -l"
run_test "export TEST=42 | cat"
run_test "unset TEST | wc -l"

# --- REDIRECTION WITH PIPELINES ---
run_test "echo hello > out.txt | cat"
run_test "cat < Makefile | grep minishell"
run_test "echo hi | grep h > out2.txt"
run_test "cat < Makefile | grep NAME | wc -l"

# --- EMPTY / WEIRD INPUTS ---
# run_test "| ls"
# run_test "ls |"
# run_test "ls || wc -l"
# run_test "ls | | wc -l"
run_test ""

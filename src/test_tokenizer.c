#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"   // <-- replace with your actual header

// Helper to print tokens
void print_tokens(t_token *head)
{
    int i = 0;
    for (t_token *cur = head; cur; cur = cur->next)
    {
        printf("[%d] type=%d, value='%s'\n", i, cur->type, cur->value);
        i++;
    }
}

// // Helper to free tokens
// void free_tokens(t_token *head)
// {
//     while (head)
//     {
//         t_token *tmp = head->next;
//         free(head->value);
//         free(head);
//         head = tmp;
//     }
// }

void run_test(const char *input, const char *label)
{
    printf("=== TEST: %s ===\n", label);

    const char *p = input;
    t_token *tokens = tokenize(&p);

    if (!tokens)
    {
        printf("(no tokens or error)\n");
    }
    else
    {
        print_tokens(tokens);
        free_tokens(tokens);
    }
    printf("\n");
}

int main(void)
{
    printf("\n *** TESTS (no quotes) ***\n");
    run_test("echo hello world", "Simple words");

    run_test("echo foo|grep bar", "Pipe without spaces");

    run_test("echo hi >file", "Redirection no space before");

    run_test("echo hi> file", "Redirection no space after");

    run_test("echo hi>file", "Redirection no spaces");

    run_test("echo    spaced     out", "Multiple (ignore) spaces");

    run_test("<< >>", "Operators only");

    run_test("", "Empty input");

    run_test("   ", "Whitespace only");   
    
// Simple quoted string with spaces
printf("\n *** DOUBLE QUOTE TESTS ***\n");
run_test("echo \"hello world\"",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='hello world'");

// Quoted string with literal $HOME (no expansion in quotes)
run_test("echo \"$HOME\"",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='$HOME'");

run_test("echo \"123\"\"456\"", 
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='123456'");

// Quoted string with escaped newline characters (\n)
run_test("echo \"line1\\nline2\"",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='line1\\nline2'");

// Quoted string with all letters
run_test("echo \"abcdefghi\"",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='abcdefghi'");

// Quoted string with leading and trailing spaces
run_test("echo \" spaced \"",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value=' spaced '");

// Adjacent quotes concatenated
run_test("echo \"foo\"\"bar\"\"baz\"",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='foobarbaz'");

// Escaped double quotes inside double quotes
run_test("echo \"he said \\\"hi\\\"\"",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='he said \"hi\"'");

// Unclosed double quotes (should fail / return NULL token)
run_test("echo \"unterminated",
    "Expected Result (unterminated) :[0] type=0, value='echo'");

// Empty double-quoted string
run_test("echo \"\"",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value=''");

printf("\n *** SINGLE QUOTE TESTS ***\n");
run_test("echo 'It''s ok'", "multi single quote check");

// Simple single-quoted string with spaces
run_test("echo 'hello world'",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='hello world'");

// Single quotes prevent variable expansion
run_test("echo '$HOME'",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='$HOME'");

// Single quotes prevent escape interpretation
run_test("echo 'line1\\nline2'",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='line1\\nline2'");

// Single-quoted string with letters
run_test("echo 'abcdefghi'",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value='abcdefghi'");

// Single-quoted string with leading and trailing spaces
run_test("echo ' spaced '",
    "Expected Result :[0] type=0, value='echo', [1] type=0, value=' spaced '");
// Single-quoted should fail
    run_test("echo 'unterminated",
    "Expected Result (unterminated) :[0] type=0, value='echo'");

    run_test("echo ''", 
    "Expected Result (empty) :[0] type=0, value='echo', [1] type=0, value=''");

    return 0;
}

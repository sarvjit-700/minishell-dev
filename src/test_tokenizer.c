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
    run_test("echo hello world", "Simple words");

    run_test("echo \"hello world\"", "Quoted with spaces");

    run_test("echo foo\"bar\"baz", "Word + quotes combined");

    run_test("echo \"123\"\"456\"", "Adjacent quotes concatenated");

    run_test("echo \"he said \\\"hi\\\"\"", "Escaped quotes inside quotes");

    run_test("echo \"unterminated", "Unclosed quotes (should fail)");

    run_test("echo \"\"", "Empty quoted string (double)");

    run_test("echo ''", "Empty quoted string (single)");

    run_test("echo foo|grep bar", "Pipe without spaces");

    run_test("echo hi >file", "Redirection no space before");

    run_test("echo hi> file", "Redirection no space after");

    run_test("echo hi>file", "Redirection no spaces");

    run_test("echo    spaced     out", "Multiple spaces");

    run_test("<< >>", "Operators only");

    run_test("", "Empty input");

    run_test("   ", "Whitespace only");

    return 0;
}

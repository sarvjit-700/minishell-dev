#include <stdio.h>
#include <stdlib.h>
#include "../includes/minishell.h"

// Helper to print token type as text
const char *token_type_str(t_token_type type)
{
    switch (type)
    {
        case TOKEN_WORD:       return "WORD";
        case TOKEN_PIPE:       return "PIPE";
        case TOKEN_REDIR_IN:   return "REDIR_IN";
        case TOKEN_REDIR_OUT:  return "REDIR_OUT";
        case TOKEN_APPEND:     return "APPEND";
        case TOKEN_HEREDOC:    return "HEREDOC";
        default:               return "UNKNOWN";
    }
}

int main(void)
{
    const char *input;
    t_token *tokens;
    t_token *tmp;

    // Example string — change this to test different cases
    input = "echo < <<< | \"hello world\" | grep world > out.txt";

    tokens = tokenize(&input);
    tmp = tokens;

    while (tmp)
    {
        printf("Type: %-10s | Value: '%s'\n", token_type_str(tmp->type), tmp->value);
        tmp = tmp->next;
    }

    // Cleanup (free tokens)
    tmp = tokens;
    while (tmp)
    {
        t_token *next = tmp->next;
        free(tmp->value);
        free(tmp);
        tmp = next;
    }

    return 0;
}

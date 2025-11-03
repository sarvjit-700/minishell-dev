#include "../includes/minishell.h"

int apply_redirs(t_cmd *cmd); // declare it manually for now

int main(void)
{
    // Example: simulate command "cat < infile > outfile"
    t_cmd cmd;
    t_redir in, out;

    // Setup redirections
    in.type = TOKEN_REDIR_IN;
    in.filename = "infile.txt";
    in.next = &out;

    out.type = TOKEN_REDIR_OUT;
    out.filename = "outfile.txt";
    out.next = NULL;

    cmd.redir = &in;
    cmd.argv = NULL;
    cmd.argc = 0;
    cmd.next = NULL;

    printf("Running redirection test...\n");

    // Step 1: create input file
    FILE *f = fopen("infile.txt", "w");
    if (f)
    {
        fprintf(f, "Hello from infile\n");
        fclose(f);
    }

    // Step 2: apply redirections
    if (apply_redirs(&cmd) == -1)
    {
        perror("apply_redirections");
        return (1);
    }

    // Step 3: confirm STDIN and STDOUT now point to these files
    // (read from stdin, write to stdout)
    char buffer[64];
    ssize_t n = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Read from stdin (infile.txt): %s\n", buffer);
    }

    printf("Writing to stdout (outfile.txt)...\n");
    printf("This text should go into outfile.txt\n");

    return (0);
}

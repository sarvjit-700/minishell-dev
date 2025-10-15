#include "../includes/minishell.h"

// Forward declarations from your lexer
t_token *tokenize(const char **input);
void free_tokens(t_token *tokens);

// Print command list for verification
void print_cmd_list(t_cmd *cmds)
{
    int i;
    int cmd_idx = 0;

    while (cmds)
    {
        printf("=== Command %d ===\n", cmd_idx++);
        for (i = 0; i < cmds->argc; i++)
            printf("argv[%d] = '%s'\n", i, cmds->argv[i]);

        t_redir *r = cmds->redir;
        while (r)
        {
            const char *type_str;
            switch (r->type)
            {
                case TOKEN_REDIR_IN:      type_str = "<"; break;
                case TOKEN_REDIR_OUT:     type_str = ">"; break;
                case TOKEN_APPEND:        type_str = ">>"; break;
                case TOKEN_HEREDOC:       type_str = "<<"; break;
                default:                  type_str = "(unknown)"; break;
            }
            printf("redir type=%s, file='%s'\n", type_str, r->filename);
            r = r->next;
        }

        cmds = cmds->next;
    }
}


void run_parser_test(const char *input, const char *desc)
{
    printf("\n--- TEST: %s ---\nInput: %s\n", desc, input);

    const char *p = input;
    t_token *tokens = tokenize(&p);
    if (!tokens)
    {
        printf("Tokenizer failed (NULL tokens)\n");
        return;
    }
    t_cmd *cmds = parse_tokens(tokens);

    // Detect expected failure by "❌" tag in description
    int should_fail = strstr(desc, "❌") != NULL;

    if (cmds == NULL)
    {
        if (should_fail)
            printf("✅ Parser correctly returned NULL (syntax error)\n");
        else
            printf("❌ Parser failed unexpectedly\n");
    }
    else
    {
        if (should_fail)
            printf("❌ Parser should have failed but did not\n");
        else
            print_cmd_list(cmds); // your existing pretty-printer
        free_cmd_list(cmds);
    }
    free_tokens(tokens);
}

int main(void)
{
    // --- 🧱 BASIC STRUCTURE & SPACING ---
    run_parser_test("echo hello world", "Simple command ✅");
    run_parser_test("   echo   spaced   words   ", "Extra whitespace ✅");
    run_parser_test("", "Empty input (should fail) ❌");
    run_parser_test("   ", "Only whitespace (should fail) ❌");

    // --- 🧩 QUOTES & ESCAPES ---
    run_parser_test("echo 'hello world'", "Quoted string ✅");
    run_parser_test("echo \"foo bar\"", "Double-quoted string ✅");
    run_parser_test("echo 'It\\'s fine'", "Escaped single quote inside single quotes (unterminated ❌)");
    run_parser_test("echo \"He said \\\"hello\\\"\"", "Escaped double quotes inside double quotes ✅");
    run_parser_test("echo 'one''two'", "Adjacent single-quoted strings ✅ (should merge to 'onetwo')");
    run_parser_test("echo \"foo\"\"bar\"", "Adjacent double-quoted strings ✅ (should merge to 'foobar')");
    run_parser_test("echo 'mixed'\"quotes\"", "Mixed single and double quotes ✅ (should merge to 'mixedquotes')");
    run_parser_test("echo 'line1\\nline2'", "Literal backslash-n inside single quotes ✅ (should keep \\n literal)");
    run_parser_test("echo \"line1\\nline2\"", "Escaped newline inside double quotes ✅ (should become line1 + newline + line2)");
    run_parser_test("echo ''", "Empty single quotes ✅ (empty arg)");
    run_parser_test("echo \"\"", "Empty double quotes ✅ (empty arg)");
    run_parser_test("echo ' '", "Single space inside quotes ✅");
    run_parser_test("echo \"  spaced  \"", "Multiple spaces inside double quotes ✅");

    // --- 🔄 PIPES ---
    run_parser_test("echo a b | grep b", "Pipeline test ✅");
    run_parser_test("echo a | grep b | wc -l", "Multiple pipes ✅");
    run_parser_test("| ls", "Pipe at start ❌");
    run_parser_test("echo |", "Pipe at end ❌");
    run_parser_test("echo | | ls", "Consecutive pipes ❌");
    run_parser_test("|| ls", "Double pipe ❌");
    run_parser_test("|", "Single pipe ❌");

    // --- 📁 REDIRECTIONS ---
    run_parser_test("cat < infile > outfile", "Redirections ✅");
    run_parser_test("cat << LIMITER", "Heredoc (no content) ✅");
    run_parser_test("echo > out", "Simple output redirection ✅");
    run_parser_test("cat < infile", "Single input redirection ✅");
    run_parser_test("cat > outfile", "Single output redirection ✅");
    run_parser_test("cat >> appendfile", "Append redirection ✅");
    run_parser_test("cat < in1 < in2", "Multiple input redirections ✅ (some shells allow, last one wins)");
    run_parser_test("cat > out1 > out2", "Multiple output redirections ✅ (last one wins)");
    run_parser_test("cat < infile | grep word > outfile", "Mixed redirections with pipe ✅");
    run_parser_test("echo > ", "Output redirection missing filename ❌");
    run_parser_test("cat <", "Input redirection missing filename ❌");
    run_parser_test(">", "Lonely redirection operator ❌");
    run_parser_test("echo >>", "Append operator without filename ❌");

    // --- ⚙️ COMMAND COMPOSITION ---
    run_parser_test("echo 'hello'world", "Quoted and unquoted combined ✅ (should merge to 'helloworld')");
    run_parser_test("ls -la /tmp", "Command with flags and path ✅");
    run_parser_test("grep 'foo bar' < input.txt > output.txt", "Full pipeline with redirects ✅");
    run_parser_test("cat <<EOF | grep something > out", "Heredoc plus pipe plus redirection ✅");

    // --- ❌ SYNTAX ERRORS ---
    run_parser_test("echo 'unterminated", "Unterminated single quote ❌");
    run_parser_test("echo \"unterminated", "Unterminated double quote ❌");
    run_parser_test("echo | |", "Multiple empty pipe segments ❌");
    run_parser_test("echo 'abc", "Missing closing single quote ❌");
    run_parser_test("echo \"abc", "Missing closing double quote ❌");

    return 0;
}


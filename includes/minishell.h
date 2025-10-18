/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 00:57:31 by ssukhija          #+#    #+#             */
/*   Updated: 2025/08/09 00:57:31 by ssukhija         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>


// -- For Lexer -- //
typedef enum e_token_type 
{
    TOKEN_WORD,
    TOKEN_STRING,
    TOKEN_PIPE,
    TOKEN_REDIR_IN, // <
    TOKEN_REDIR_OUT, // >
    TOKEN_APPEND,   // >>
    TOKEN_HEREDOC   // <<
}   t_token_type;

typedef struct s_token 
{
    t_token_type type;
    char *value;
    struct s_token *next;
}   t_token;

// -- For Parser -- //
typedef struct s_redir
{
    t_token_type type;            // e.g. TOKEN_REDIR_IN, TOKEN_REDIR_OUT, TOKEN_APPEND, TOKEN_HEREDOC
    char *filename;               // filename or heredoc delimiter
    struct s_redir *next;
} t_redir;

typedef struct s_cmd 
{
    char **argv;                  // NULL-terminated argv array
    int argc;
    t_redir *redir;         // linked list of redirections for this command
    struct s_cmd *next;       // next command in pipeline
} t_cmd;

typedef struct s_parser_state
{
    t_token *token;
    t_cmd   *head;
    t_cmd   *tail;
    t_cmd   *curr;
}   t_parser_state;

// -- For Execution --//
typedef struct s_env
{
	char    *key; // variable type
	char    *value; // content
    bool    exported;
	struct s_env	*next;
}	t_env;



// lexer-utils //
char *ft_strndup(const char *s, size_t n);
bool is_whitespace(char c);
bool is_op_start(char c);
t_token *create_token(t_token_type type, const char *start, size_t len);
void add_token(t_token **head, t_token *new);
void    free_tokens(t_token *tok); // move later

// -- Lexer functions -- //
t_token *tokenize(const char **input);
//t_token *lex_input(const char *input);
//void print_tokens(t_token *head);
//void free_tokens(t_token *head);

// -- for testing parser -- //
t_cmd   *parse_tokens(t_token *tokens);
void free_cmd_list(t_cmd *cmd);

// -- execution --//
char	*find_exec(char *cmd, char **envp);
int builtin_pwd(void); // move later

// -- builtins -- //
// -- main -- //
int is_builtin(const char *cmd);
int	exec_builtin(t_cmd *cmd, t_env **env_list);
char *get_env_value(t_env *env_list, const char *key);
void set_env(t_env **env_list, const char *key, const char *value);
int	is_valid_var(char *var);

// -- builtin echo, cd -- //
int builtin_echo(char **argv);
int builtin_cd(t_cmd *cmd, t_env **env_list);
int builtin_export(t_cmd *cmd, t_env **env_list);
int builtin_unset(t_cmd *cmd, t_env **env_list);
int builtin_env(t_env *env_list);
int builtin_exit(t_cmd *cmd);


#endif
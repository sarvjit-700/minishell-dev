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
# define COLOUR   "\001\033[33m\002"
# define RESET   "\001\033[0m\002"

# include "../libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

extern int g_exit_code;

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
    char *filename;
    int quoted;
    int fd;
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

typedef enum e_status 
{
    LOOP_OK,
    LOOP_CONTINUE,
    LOOP_BREAK,
    LOOP_ERROR
}   t_status;


typedef struct s_env
{
	char    *key; // variable type
	char    *value; // content
    bool    exported;
	struct s_env	*next;
}	t_env;

// -- For Pipeline -- //
typedef struct s_pipe_data 
{
    int     i;
    int     cmd_count;
    int     **pipes;
    t_env   **env_list;
} t_pipe_data;

typedef struct s_shell
{
	t_env    *env_list;
	t_token  *tokens;
	t_cmd    *cmd_list;
	char     *line;
	int       last_status;
	int       running;
}	t_shell;


// -- Lexer -- // -- SORTED
// -- tokenizer -- //
t_token *tokenize(const char **input);

// -- lexer main -- //
char	*append_str(char *dest, const char *src, size_t len);
t_token	*extract_operator(const char **input);
t_token	*extract_words(const char **input);

// -- lexer quoted -- //
char	*append_quoted(const char **input, char *words);

// lexer-utils //
char *ft_strndup(const char *s, size_t n);
bool is_whitespace(char c);
bool is_op_start(char c);
t_token *create_token(t_token_type type, const char *start, size_t len);
void add_token(t_token **head, t_token *new);


// -- PARSER -- //
// -- parser main -- //
int	handle_parser_err(const char *msg);
t_cmd   *parse_tokens(t_token *tokens);

// -- parse pipe redir -- //
int	process_token(t_parser_state *ps);

// -- parse word -- //
int	start_cmd(t_parser_state *ps);
int	handle_word(t_parser_state *ps);


// -- execution --//
char	*find_exec(char *cmd, char **envp);

int apply_redirs(t_cmd *cmd);
int process_heredocs(t_cmd *cmd_list); // move later

// -- BUILTINS -- // -- SORTED
// -- main -- //
int is_builtin(const char *cmd);
int	exec_builtin(t_cmd *cmd, t_env **env_list);
char *get_env_value(t_env *env_list, const char *key);
void set_env(t_env **env_list, const char *key, const char *value);

// -- builtins cd export -- //
int builtin_cd(t_cmd *cmd, t_env **env_list);
int builtin_export(t_cmd *cmd, t_env **env_list);

// -- builtin echo, unset -- //
int builtin_echo(char **argv);
int builtin_unset(t_cmd *cmd, t_env **env_list);

// -- builtin env, pwd, exit -- //
int builtin_env(t_env *env_list);
int builtin_pwd(void);
int builtin_exit(t_cmd *cmd);



// -- PIPEX -- //
//int execute_pipeline(t_cmd *cmd_list, char **envp, t_env **env_list);
int    init_pipe_data(t_cmd *cmd_list, char **envp, t_env **env_list);
//int    init_pipe_data(t_shell *shell, char **envp);

void    *handle_ptr_err(const char *msg, int code); //move 

// -- PIPE_UTILS --//
int	**create_pipes(t_pipe_data *data);
void	close_parent_pipes(t_pipe_data *data);
void	child_process(t_pipe_data *data, t_cmd *cmd, char **envp);

// -- CLEANUP -- //
void free_pipe_data(t_pipe_data *data);
void cleanup_simple(t_shell *shell);
void cleanup_shell(t_shell *shell);
void	free_cmd_list(t_cmd *cmd);
void    free_tokens(t_token *token); // move later
int extract_exit_code(int status); //move later


// -- Signal Handler -- //
// void sigint_handler(int sig);
// void sigquit_handler(int sig);
void setup_signal_handlers(int sig_type);

// -- Expand Vars -- //
void    expand_vars(t_shell *shell);

// -- Handle Error -- //
//void handle_exec_error(char *cmd);
void handle_exec_error(const char *path, int child);

#endif
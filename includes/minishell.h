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
# define COLOUR "\001\033[33m\002"
# define RESET "\001\033[0m\002"
# define S_QM_S '\x01'
# define S_QM_E '\x02'
# define D_QM_S '\x03'
# define D_QM_E '\x04'

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern int	g_exit_code;

// -- For Lexer -- //
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_STRING,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

// -- For Parser -- //
typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	int				fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	int				argc;
	int				redir_error;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_parser_state
{
	t_token			*token;
	t_cmd			*head;
	t_cmd			*tail;
	t_cmd			*curr;
}	t_parser_state;

// -- For Execution --//

typedef enum e_status
{
	LOOP_OK,
	LOOP_CONTINUE,
	LOOP_BREAK,
	LOOP_ERROR
}	t_status;

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			exported;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env			*env_list;
	t_token			*tokens;
	t_cmd			*cmd_list;
	char			*line;
	int				last_status;
	int				running;
	int				should_exit;
}	t_shell;

// -- For Pipeline -- //
typedef struct s_pipe_data
{
	int				i;
	int				cmd_count;
	int				**pipes;
	t_env			**env_list;
	t_shell			*shell;
}	t_pipe_data;

// -- For Expansion -- //
typedef enum e_mode
{
	MODE_COUNT,
	MODE_FILL
}	t_mode;

// -- Lexer -- // -- SORTED
// -- tokenizer -- //
t_token		*tokenize(const char **input);

// -- lexer main -- //
char		*append_str(char *dest, const char *src, size_t len);
t_token		*extract_operator(const char **input);
t_token		*extract_words(const char **input);

// -- lexer quoted -- //
char		*append_quoted(const char **input, char *words);

// -- lexer esc end -- //
char		*append_until_esc_end(const char **input, char *words);
char		*handle_esc_char(const char **input, char *words);

char		*append_char(char *dst, char c);
bool		is_whitespace(char c);
bool		is_op_start(char c);
t_token		*create_token(t_token_type type, const char *start, size_t len);
void		add_token(t_token **head, t_token *new);

// -- PARSER -- // -- SORTED
// -- parser main -- //
int			handle_parser_err(const char *msg);
t_cmd		*parse_tokens(t_token *tokens);

// -- parse pipe redir -- //
int			process_token(t_parser_state *ps);

// -- parse word -- //
int			start_cmd(t_parser_state *ps);
int			handle_word(t_parser_state *ps);

// -- OPERATORS -- //
int			init_pipe_data(t_cmd *cmd_list, char **envp,
				t_env **env_list, t_shell *shell);

// -- pipe_utils --//
int			**create_pipes(t_pipe_data *data);
void		child_process(t_pipe_data *data, t_cmd *cmd, char **envp);

// -- redirs -- //
int			apply_redirs(t_cmd *cmd);
int			process_heredocs(t_cmd *cmd_list, t_shell *shell);

// -- cleanup_shell -- //
void		free_cmd_list(t_cmd *cmd);
void		free_tokens(t_token *token);
void		cleanup_simple(t_shell *shell);
void		cleanup_shell(t_shell *shell);

// -- cleanup_general -- //
void		free_env_list(t_env *env);
void		free_env_array(char **envp);
void		free_pipe_data(t_pipe_data *data);
int			extract_exit_code(int status);

// -- EXPANSION -- //
// -- Expand Vars -- // NEED TO SORT
void		expand_variables(t_shell *shell);

// -- expand utils -- //
char		*expand_var(char *dst, char *src);
char		*remove_markers(const char *s);
void		free_words(char **w);
void		free_argv(char **argv);
int			process_expanded(char **words, char **dst, int idx, t_mode mode);

// -- expand string -- //
char		*expand_string(char *str, t_shell *shell);
int			expand_redirs(t_cmd *cmd, t_shell *shell);
char		*expand_dollar(char *str, int *i, t_shell *shell, char *res);

// -- EXECUTION -- //
// -- Signal Handler -- //
void		setup_signal_handlers(int sig_type);
void		heredoc_sigint(int sig);

// -- executor --//
int			execute(t_shell *shell, char **envp);

// -- exec_cmd -- //
int			execute_command(t_shell *shell, t_cmd *cmd, char **envp);

// -- shell level -- //
void		adjust_shlvl(t_env **env_list);
int			add_new_shell(t_shell *shell);
char		**env_to_array(t_env *env_list);

// -- run_shell -- //
void		run_shell(t_shell *shell, char **envp);

// -- paths -- //
char		*find_exec(char *cmd, t_env *env_list);

// -- Handle Error -- //
void		*handle_ptr_err(const char *msg, int code);
void		handle_exec_error(const char *path, int child);
int			export_ident_error(char *arg);

// -- BUILTINS -- //
// -- main -- //
int			is_builtin(const char *cmd);
int			exec_builtin(t_cmd *cmd, t_shell *shell, t_env **env_list);
char		*get_env_value(t_env *env_list, const char *key);
void		set_env(t_env **env_list, const char *key, const char *value);

// -- builtins cd export -- //
int			builtin_cd(t_cmd *cmd, t_env **env_list);
int			builtin_export(t_cmd *cmd, t_env **env_list);

// -- builtin echo, unset -- //
int			builtin_echo(char **argv);
int			builtin_unset(t_cmd *cmd, t_env **env_list);

// -- builtin env, pwd, exit -- //
int			builtin_env(t_env *env_list);
int			builtin_pwd(void);
int			builtin_exit(t_cmd *cmd, t_shell *shell);

// -- clean exit -- //
void		clean_pipe_exit(t_pipe_data *data, char **envp, int code);
void		clean_child_exit(t_shell *shell, char *path, int code);
void		hd_exit(t_shell *shell, int write_fd, int code);
void		close_parent_pipes(t_pipe_data *data);
#endif
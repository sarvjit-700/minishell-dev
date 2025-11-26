# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ssukhija <ssukhija@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/07 14:45:50 by ssukhija          #+#    #+#              #
#    Updated: 2025/08/07 14:45:50 by ssukhija         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

# colours
RESET = \033[0m
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m

NAME = minishell

# flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf

# paths
LIBFT_DIR	:= ./libft
LIBFT	:= $(LIBFT_DIR)/libft.a
PREP	:= ./src/prep
EXEC	:= ./src/execution
XPAND	:= ./src/expansion
BUILTINS := ./src/builtins

# files
SRC = $(PREP)/lexer_main.c $(PREP)/lexer_utils.c $(PREP)/tokenizer.c\
	$(PREP)/lexer_quoted.c $(PREP)/parser_main.c $(PREP)/parse_pipe_redir.c $(PREP)/parse_word.c\
 	$(EXEC)/executor.c $(EXEC)/exec_cmd.c $(EXEC)/run_shell.c $(EXEC)/paths.c $(EXEC)/redirs.c\
	$(EXEC)/pipeline.c $(EXEC)/pipe_utils.c $(EXEC)/cleanup_shell.c $(EXEC)/add_new_shell.c\
	$(EXEC)/handle_err.c $(EXEC)/signal_handler.c\
	$(XPAND)/expand_vars.c $(XPAND)/expand_dollar.c $(XPAND)/expand_string.c\
	$(XPAND)/expand_redir.c $(XPAND)/expand_utils.c\
	$(BUILTINS)/builtin_main.c $(BUILTINS)/builtin_echo_unset.c\
	$(BUILTINS)/builtin_cd.c $(BUILTINS)/builtin_export.c\
	$(BUILTINS)/builtin_env_pwd_exit.c
OBJ = $(SRC:.c=.o)

INCLUDES = -Iincludes -I$(LIBFT_DIR)
LDFLAGS = -lreadline

# rules
all: $(LIBFT) $(NAME)

src/%.o : src/%.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline
	printf "Linking executable	$(GREEN)[OK]$(RESET)\n"

$(LIBFT):
	printf "Making Libft		$(YELLOW)[OK]$(RESET)\n"
	$(MAKE) -sC $(LIBFT_DIR)

clean:
	$(RM) $(OBJ)
	printf "clean object files	$(YELLOW)[OK]$(RESET)\n"
	$(MAKE) fclean -sC $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	printf "cleaning all files	$(YELLOW)[OK]$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
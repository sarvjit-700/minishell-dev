<img width="1408" height="768" alt="image_51cba710" src="https://github.com/user-attachments/assets/a0b30117-7668-4415-9bd6-4b3f6cc8aac0" />

*This project has been created as part of the 42 curriculum by ssukhija, msimek.*

# DESCRIPTION
Minishell is a lightweight UNIX shell implementation written in C. The goal of this project is to demystify the inner workings of a shell by recreating the basic behavior of bash. It introduces core operating system concepts, including process creation, inter-process communication, file descriptor management, and signal handling. The shell interprets user input, manages environment variables, and executes complex command pipelines seamlessly.

## Features
Interactive Prompt: Displays a persistent prompt using the readline library, complete with working command history.

Execution: Locates and executes binaries using the PATH environment variable, or via absolute and relative paths.

Built-in Commands: Implements echo (with -n), cd, pwd, export, unset, env, and exit natively without invoking external binaries.

Pipes: Chains commands together using |, connecting the standard output of one process to the standard input of the next.

Redirections: Handles input <, output >, append >>, and heredoc << operations.

Environment Variables: Expands variables (e.g., $USER) and resolves the $? variable to the exit status of the most recently executed pipeline.

Signal Handling: Correctly intercepts and processes Ctrl-C (SIGINT), Ctrl-D (EOF), and Ctrl-\ (SIGQUIT) to match bash's default behavior.

# INSTRUCTIONS

## Compilation
Clone the repository and build the executable using the provided Makefile:

    Bash: make

This will compile the source files and generate the ./minishell executable. Additional rules include make clean (to remove object files), make fclean (to remove objects and the executable), and make re (to recompile from scratch).

## Execution
Launch the interactive shell by running:

    Bash: ./minishell

Once inside, you can execute standard UNIX commands. To exit the shell, type exit or press Ctrl+D.

## Usage Examples
Demonstrating standard pipelines, redirections, and environment variable expansion:

    Bash:
    minishell$ echo -n "Hello, Minishell!" | wc -c
    17
    minishell$ cat << EOF > output.txt
    > line 1
    > line 2
    > EOF
    minishell$ ls -la | grep "minishell" | wc -l
    minishell$ echo "Last command exited with: $?"

# RESOURCES
Advanced Programming in the UNIX Environment (APUE): Served as the primary technical reference for mastering UNIX system calls, specifically fork(), execve(), pipe(), dup2(), and signal management (sigaction).

GNU Bash Reference Manual: Consulted to accurately replicate bash's behavior regarding parsing rules, exit codes ($?), environment variable expansion, and edge cases.

GNU Readline Documentation: Used to implement the interactive prompt and command history, as well as to manage memory cleanly.

AI Usage: Artificial Intelligence was used during the planning and debugging phases to clarify complex documentation regarding file descriptor inheritance across multiple fork() calls, to troubleshoot edge cases in Abstract Syntax Tree (AST) parsing logic.

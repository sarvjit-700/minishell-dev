*This project has been created as part of the 42 curriculum by ssukhija, msimek.*

# DESCRIPTION
Minishell is a minimalist, custom implementation of a POSIX-compliant shell, heavily inspired by bash. The core goal of the project is to provide a deep understanding of UNIX architecture, specifically process creation, synchronization, file descriptor management, and signal handling.

The shell successfully interprets a command-line prompt, resolves executables via the PATH environment variable, and manages complex pipelines (|) and file redirections (<, >, <<, >>). It also includes custom implementations of standard built-in commands such as cd, echo, pwd, export, unset, env, and exit.

# INSTRUCTIONS
Prerequisites
To compile and run Minishell, you will need a C compiler (cc, gcc, or clang), make, and the GNU readline library installed on your system.

Compilation
Clone the repository and build the executable using the provided Makefile:

Bash
make
This will compile the source files and generate the ./minishell executable. Additional rules include make clean (to remove object files), make fclean (to remove objects and the executable), and make re (to recompile from scratch).

Execution
Launch the interactive shell by running:

Bash
./minishell
Once inside, you can execute standard UNIX commands. To exit the shell, type exit or press Ctrl+D.

# RESOURCES
Advanced Programming in the UNIX Environment (APUE): Served as the primary technical reference for mastering UNIX system calls, specifically fork(), execve(), pipe(), dup2(), and signal management (sigaction).

GNU Bash Reference Manual: Consulted to accurately replicate bash's behavior regarding parsing rules, exit codes ($?), environment variable expansion, and edge cases.

GNU Readline Documentation: Used to implement the interactive prompt and command history, as well as to manage memory cleanly.

AI Usage: Artificial Intelligence was used during the planning and debugging phases to clarify complex documentation regarding file descriptor inheritance across multiple fork() calls, to troubleshoot edge cases in Abstract Syntax Tree (AST) parsing logic.

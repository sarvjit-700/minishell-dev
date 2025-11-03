#include "../includes/minishell.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#define FD_MAX 20


// =========================== CLEANUP ===========================

static void cleanup_pipes_array(int **pipes, int cmd_count)
{
    if (!pipes)
        return;
    for (int i = 0; i < cmd_count - 1; i++)
    {
        if (pipes[i])
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
            free(pipes[i]);
        }
    }
    free(pipes);
}

// =========================== CREATE PIPES ===========================

static int **create_pipes(int cmd_count)
{
    int **pipes;
    if (cmd_count < 2)
        return NULL;

    pipes = malloc(sizeof(int *) * (cmd_count - 1));
    if (!pipes)
    {
        perror("malloc pipes");
        return NULL;
    }

    for (int i = 0; i < cmd_count - 1; i++)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i])
        {
            perror("malloc pipe pair");
            cleanup_pipes_array(pipes, i);
            return NULL;
        }
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe creation failed");
            cleanup_pipes_array(pipes, i + 1);
            return NULL;
        }
    }

    return pipes;
}

// =========================== CHILD PROCESS ===========================

static void child_process(t_cmd *cmd, int **pipes, int i, int cmd_count,
                          char **envp, t_env **env_list)
{
    char *path;

    // --- SETUP PIPE CONNECTIONS ---
    if (i > 0)
        dup2(pipes[i - 1][0], STDIN_FILENO);
    if (i < cmd_count - 1)
        dup2(pipes[i][1], STDOUT_FILENO);

    // --- CLOSE ALL PIPES ---
    for (int j = 0; j < cmd_count - 1; j++)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    // --- APPLY REDIRECTIONS ---
    if (apply_redirs(cmd) == -1)
        exit(1);

    // --- BUILTIN OR EXEC ---
    if (is_builtin(cmd->argv[0]))
        exit(exec_builtin(cmd, env_list));

    path = find_exec(cmd->argv[0], envp);
    if (!path)
    {
        printf("%s: command not found\n", cmd->argv[0]);
        exit(127);
    }

    execve(path, cmd->argv, envp);
    perror("execve failed");
    free(path);
    exit(127);
}

// =========================== COUNT CMDS ===========================

static int  count_cmds(t_cmd *cmd_list)
{
    int     count;
    t_cmd   *cmd;

    count = 0;
    cmd = cmd_list;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return (count);
}

static void close_parent_pipes(int **pipes, int cmd_count)
{
    int i;

    printf("close parent\n");
    i = 0;
    while (i < cmd_count - 1)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

static void    close_wait(int cmd_count, pid_t *pids)
{
    int i;
    int status;

    i = 0;
    status = 0;
    printf("close wait\n");
    while (i < cmd_count)
    {
        if (waitpid(pids[i], &status, 0) == -1)
            perror("waitpid");
        i++;
    }
}

// =========================== EXECUTE PIPELINE ===========================

int execute_pipeline(t_cmd *cmd_list, char **envp, t_env **env_list)
{
    int cmd_count;
    int **pipes;
    pid_t *pids;
    int status = 0;

    cmd_count = count_cmds(cmd_list);
    if (cmd_count == 0)
        return 1;
    pipes = create_pipes(cmd_count);
    if (!pipes && cmd_count > 1)
    {
        printf("minishell: failed to create pipes\n");
        return 1;
    }
    pids = malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
    {
        perror("malloc");
        cleanup_pipes_array(pipes, cmd_count);
        return 1;
    }

    t_cmd *cmd = cmd_list;
    for (int i = 0; i < cmd_count; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            cleanup_pipes_array(pipes, cmd_count);
            free(pids);
            return 1;
        }
        if (pids[i] == 0)
            child_process(cmd, pipes, i, cmd_count, envp, env_list);

        cmd = cmd->next;
    }
    // --- CLOSE ALL PIPE ENDS IN PARENT ---
    // for (int i = 0; i < cmd_count - 1; i++)
    // {
    //     close(pipes[i][0]);
    //     close(pipes[i][1]);
    // }
    close_parent_pipes(pipes, cmd_count);
    close_wait(cmd_count, pids);
    // --- WAIT FOR ALL CHILDREN ---
    // for (int i = 0; i < cmd_count; i++)
    // {
    //     if (waitpid(pids[i], &status, 0) == -1)
    //         perror("waitpid");
    // }
    free(pids);
    cleanup_pipes_array(pipes, cmd_count);
    return WEXITSTATUS(status);
}

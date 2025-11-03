#include "../includes/minishell.h"

static void cleanup_pipes(int **pipes, int cmd_count)
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

static void cleanup_pipes_array(t_pipe_data *data)
{
    int i;

    i = 0;
    if (!data->pipes)
        return;
    while (i < data->cmd_count - 1)
    {
        if (data->pipes[i])
        {
            close(data->pipes[i][0]);
            close(data->pipes[i][1]);
            free(data->pipes[i]);
        }
        i++;
    }
    free(data->pipes);
}


static int handle_error(const char *msg, pid_t *pids, t_pipe_data *data)
{
    if (pids)
        free(pids);
    if (data->pipes)
        cleanup_pipes_array(data);

    perror(msg);
    return (1);
}


// =========================== CREATE PIPES ===========================

//static int **create_pipes(int cmd_count)
static int **create_pipes(t_pipe_data *data)
{
    int **pipes;

    if (data->cmd_count < 2)
        return NULL;

    pipes = malloc(sizeof(int *) * (data->cmd_count - 1));
    if (!pipes)
    {
        perror("malloc pipes");
        return NULL;
    }

    for (int i = 0; i < data->cmd_count - 1; i++)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i])
        {
            perror("malloc pipe pair");
            cleanup_pipes(pipes, i);
            return NULL;
        }
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe creation failed");
            cleanup_pipes(pipes, i + 1);
            return NULL;
        }
    }

    return (pipes);
}

// =========================== CHILD PROCESS ===========================
// (cmd, pipes, i, cmd_count, envp, env_list);
//static void child_process(t_cmd *cmd, int **pipes, int i, int cmd_count, char **envp, t_env **env_list)
static void child_process(t_pipe_data *data, t_cmd *cmd, char **envp)
{
    char *path;
    int tty_fd;
    //int i;

    // --- SETUP PIPE CONNECTIONS ---
    //i = 0;
    if (data->i > 0)
        dup2(data->pipes[data->i - 1][0], STDIN_FILENO);
    if (data->i < data->cmd_count - 1)
        dup2(data->pipes[data->i][1], STDOUT_FILENO);

        //
    

    if (data->i < data->cmd_count - 1) // not last command
    {
        tty_fd = open("/dev/tty", O_WRONLY);
        if (tty_fd >= 0)
        {
            dup2(tty_fd, STDERR_FILENO);
            close(tty_fd);
        }
    }
//
    // --- CLOSE ALL PIPES ---
    for (int j = 0; j < data->cmd_count - 1; j++)
    {
        close(data->pipes[j][0]);
        close(data->pipes[j][1]);
    }

    // --- APPLY REDIRECTIONS ---
    if (apply_redirs(cmd) == -1)
        exit(1);

    // --- BUILTIN OR EXEC ---
    if (is_builtin(cmd->argv[0]))
        exit(exec_builtin(cmd, data->env_list));

    path = find_exec(cmd->argv[0], envp);
    if (!path && !is_builtin(cmd->argv[0]))
    {
        write(STDERR_FILENO, cmd->argv[0], ft_strlen(cmd->argv[0]));
        write(STDERR_FILENO, ": command not found\n", 20);
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

static void close_parent_pipes(t_pipe_data *data)
{
    int **pipes = data->pipes; 
    int cmd_count = data->cmd_count;
    int i;

    
    i = 0;
    while (i < cmd_count - 1)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

//static int    close_wait(int cmd_count, pid_t *pids)
static int    close_wait(t_pipe_data *data, pid_t *pids)
{
    int i;
    int status;
    int cmd_count = data->cmd_count;

    i = 0;
    status = 0;
    while (i < cmd_count)
    {
        if (waitpid(pids[i], &status, 0) == -1)
            perror("waitpid");
        i++;
    }
    return (status);
}

static int	*create_pids(t_cmd *cmd_list, t_pipe_data *data, char **envp)
{
	//int		i = data->i;
	t_cmd	*cmd;
	pid_t	*pids;
    //int cmd_count = data->cmd_count;
    //int **pipes = data->pipes;

	pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!pids)
	{	//return ((pid_t *)handle_error("malloc failed", NULL, pipes, cmd_count));
        perror("eRROR\n");
        exit(1);
    }
	//i = 0;
	cmd = cmd_list;
	while (data->i < data->cmd_count)
	{
		pids[data->i] = fork();
		if (pids[data->i] == -1)
		{
            perror("eRROR\n");
            exit(1);
        //return ((pid_t *)handle_error("fork failed", pids, pipes, cmd_count));
        }
		if (pids[data->i] == 0)
		{
			child_process(data, cmd, envp);
			return (NULL);
		}
		cmd = cmd->next;
		data->i++;
	}
	return (pids);
}


//int	execute_pipeline(t_cmd *cmd_list, char **envp, t_env **env_list)
static int	execute_pipeline(t_pipe_data *data, t_cmd *cmd_list, char **envp)
{
	pid_t	*pids;
	int		status;
    
	data->pipes = create_pipes(data);
	if (!data->pipes && data->cmd_count > 1)
		return (handle_error("minishell: failed to create pipes", NULL, NULL));
	pids = create_pids(cmd_list, data, envp);
	if (!pids)
		return (handle_error("Error: failed to create pids", pids, data));
    
	close_parent_pipes(data);
	status = close_wait(data, pids);
	free(pids);
    cleanup_pipes_array(data);
	return (status);
}

//static void    init_pipe_data(t_pipe_data *data, int cmd_count, int **pipes, t_env **env_list)
int    init_pipe_data(t_cmd *cmd_list, char **envp, t_env **env_list)
{
    t_pipe_data *data;
    int         ret;

    data = malloc(sizeof(t_pipe_data));
    data->cmd_count = count_cmds(cmd_list);
	if (data->cmd_count == 0)  // prob need to free data before return
		return (1);
    data->i = 0;
    data->pipes = NULL;
    data->env_list = env_list;
    ret = execute_pipeline(data, cmd_list, envp);
    return (WEXITSTATUS(ret));
}



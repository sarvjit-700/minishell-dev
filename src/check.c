static int	*create_pids(t_cmd *cmd_list, int cmd_count, int **pipes,
		char **envp, t_env **env_list)
{
	int		i;
	t_cmd	*cmd;
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (handle_error("malloc failed", NULL, pipes, cmd_count));
	i = 0;
	cmd = cmd_list;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (handle_error("fork failed", pids, pipes, cmd_count));
		if (pids[i] == 0)
		{
			child_process(cmd, pipes, i, cmd_count, envp, env_list);
			return (NULL);
		}
		cmd = cmd->next;
		i++;
	}
	return (pids);
}
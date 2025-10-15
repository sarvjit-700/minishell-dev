char	*find_executable(char *cmd, char **envp)
{
	char	*env_path;
	char	**paths;
	char	*full_path;

	if (!cmd || !*cmd)
		return (NULL);

	// if cmd contains '/', it’s a direct path
	if (ft_strchr(cmd, '/'))
		return (try_direct_path(cmd));

	env_path = get_env_path(envp);
	if (!env_path)
		return (NULL);

	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);

	full_path = search_in_paths(paths, cmd);
	return (full_path);
}
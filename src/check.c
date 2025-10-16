void	set_new_env(t_env **env_list, char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	new->key = ft_strdup(key);
	if (value != NULL)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->exported = true;
	new->next = *env_list;
	*env_list = new;
}

void	set_env(t_env **env_list, const char *key, const char *value)
{
	t_env	*curr;

	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(key)) == 0)
		{
			free(curr->value);
			if (value != NULL)
				curr->value = ft_strdup(value);
			else
				curr->value = NULL;
			curr->exported = true;
			return ;
		}
		curr = curr->next;
	}
	set_new_env(env_list, key, value);
}

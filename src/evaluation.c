#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	print_result(int fd)
{
	char	*gnl;
	char	*str;

	gnl = get_next_line(fd);
	str = ft_calloc(1, 1);
	if (!str)
	{
		if (gnl)
			free(gnl);
		return ;
	}
	while (gnl)
	{
		str = ft_strjoin(str, gnl, O_BOTH);
		gnl = get_next_line(fd);
	}
	close(fd);
	printf("%s", str);
	free(str);
}

int	execute_command(t_shell *shell, t_node *current)
{
	char	**paths;
	char	*path;
	int		count;

	paths = ft_split(get_env_node(shell->env, "PATH")->value, ':');
	count = 0;
	while (paths[count])
	{
		path = ft_strjoin(paths[count], "/", O_NONE);
		path = ft_strjoin(path, current->command, O_ONE);
		execv(path, current->args);
		free(path);
		count++;
	}
	free_split(paths);
	exit(EXIT_FAILURE);
}

int	*evaluate_command(t_node *current, int *old_yield, t_shell *shell)
{
	int	pid;
	int	status;
	int	*new_yield;

	status = -1;
	new_yield = (int *)malloc(sizeof(int) * 2);
	if (!new_yield)
		return (NULL);
	if (pipe(new_yield) < 0)
		return (free(new_yield), NULL);
	pid = fork();
	if (pid == 0)
	{
		dup2(old_yield[0], 0);
		dup2(new_yield[1], 1);
		close(old_yield[0]);
		close(old_yield[1]);
		close(new_yield[1]);
		close(new_yield[0]);
		execute_command(shell, current);
	}
	else
	{
		close(old_yield[0]);
		close(old_yield[1]);
		free(old_yield);
		waitpid(pid, &status, 0);
		if (status > 0)
			printf("%s: command not found\n", current->command);
	}
	return (new_yield);
}

int	is_pipe(char *token)
{
	if (!ft_strncmp(token, "|", 2))
		return (TRUE);
	return (FALSE);
}

int	is_redirect_output(char *token)
{
	if (!ft_strncmp(token, ">", 2) || !(ft_strncmp(token, ">>", 3)))
		return (TRUE);
	return (FALSE);
}

int	is_redirect_input(char *token)
{
	if (!ft_strncmp(token, "<", 2))
		return (TRUE);
	return (FALSE);
}

int	is_heredoc(char *token)
{
	if (!ft_strncmp(token, "<<", 3))
		return (TRUE);
	return (FALSE);
}

// file = open(current->next->command, O_RDWR | O_APPEND | O_CREAT, 0777);
void	redirect_output(t_node *current, int *old_yield)
{
	int		file;
	int		pid;
	char	*path;

	if (!current->next || !current->next->command
		|| !ft_strlen(current->next->command))
		printf("minishell: Syntax error\n");
	if (!ft_strncmp(current->token, ">", 2))
		file = open(current->next->command, O_RDWR | O_TRUNC | O_CREAT, 0777);
	else
		file = open(current->next->command, O_RDWR | O_APPEND | O_CREAT, 0777);
	if (file < 0)
		return ;
	pid = fork();
	if (pid == 0)
	{
		dup2(old_yield[0], 0);
		dup2(file, 1);
		close(old_yield[0]);
		close(old_yield[1]);
		close(file);
		path = ft_strjoin("/bin/", current->command, O_NONE);
		execv(path, current->args);
			// NOTE: execve retorna 1 mesmo quando o path é um ponteiro NULL, então não precisamos de check pro malloc aqui.
	}
	else
	{
		close(old_yield[0]);
		close(old_yield[1]);
		close(file);
		waitpid(pid, NULL, 0);
	}
}

int	evaluate_prompt(char *prompt, t_shell *shell)
{
	t_list	*prompt_list;
	t_node	*current;
	int		*yield;

	prompt_list = generate_list(prompt);
	if (!prompt_list)
		return (FALSE);
	current = prompt_list->head;
	yield = (int *)malloc(sizeof(int) * 2);
	if (!yield)
		return (free_list(prompt_list), FALSE);
	if (pipe(yield) < 0)
		return (free_list(prompt_list), free(yield), FALSE);
	while (current)
	{
		if (!current->token || is_pipe(current->token))
		{
			yield = evaluate_command(current, yield, shell);
		}
		else if (is_redirect_output(current->token))
		{
			redirect_output(current, yield);
			current = current->next;
		}
		current = current->next;
	}
	close(yield[1]);
	print_result(yield[0]);
	free_list(prompt_list);
	(void)shell;
	return (TRUE);
}

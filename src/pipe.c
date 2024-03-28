/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdrago <pdrago@student.42.rio>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:59:12 by pdrago            #+#    #+#             */
/*   Updated: 2024/03/21 18:51:11 by pdrago           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_pipe(char *token)
{
	if (!ft_strncmp(token, "|", 2))
		return (TRUE);
	return (FALSE);
}

void	pipe_output(t_node *node, t_shell *shell)
{
	int	pipe_fd[2];

	pipe(pipe_fd);
	if (is_builtin(node->command))
	{
		exec_builtin(node, shell, pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[1]);
		shell->pids->p_array[shell->pids->index] = g_pid;
		shell->pids->c_array[shell->pids->index++] = node->command;
		return ;
	}
	g_pid = fork();
	if (g_pid == 0)
	{
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		execute_command(shell, node);
		exit(1);
	}
	else
	{
		dup2(pipe_fd[0], 0);
		close(pipe_fd[1]);
		shell->pids->p_array[shell->pids->index] = g_pid;
		shell->pids->c_array[shell->pids->index++] = node->command;
	}
}

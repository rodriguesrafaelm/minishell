/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelro <rafaelro@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 16:48:38 by rafaelro          #+#    #+#             */
/*   Updated: 2024/03/11 20:08:02 by pdrago           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	int	last_status;
	char	*shell_path;
	char	*path;
	char	*prompt_string;
	t_env	*env;
	

}	t_shell;

typedef struct s_node
{
	char *command;
	char **args;
	char *token;
	struct s_node *next;
}	t_node;

typedef struct s_list
{
	t_node *head;
	t_node *tail;
}	t_list;


#endif

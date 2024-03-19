#include "../includes/minishell.h"
#include <stdio.h>

void	free_split(char **splited)
{
	int	count;

	count = -1;
	if (!splited)
		return ;
	while (splited[++count])
		free(splited[count]);
	free(splited);
}

void	print_split(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
		printf("!%s!", argv[i++]);
	// printf("%s", argv[i]);
	printf("\n");
	printf("------------------------\n");
}

void	free_list(t_list *list)
{
	t_node	*tmp;

	while(list->head)
	{
		if (list->head->command)
			free(list->head->command);
		if (list->head->token)
			free(list->head->token);
		if (list->head->args) 
			free_split(list->head->args);
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}
	free(list);
}

void	print_list(t_list *arg)
{
	t_node *tmp;

	tmp = arg->head;
	while (tmp)
	{
		printf("command: %s\n", tmp->command);
		printf("args: ");
		print_split(tmp->args);
		printf("token: %s\n", tmp->token);
		tmp = tmp->next;
	}
}

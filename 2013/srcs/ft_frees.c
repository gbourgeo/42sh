/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_frees.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:56:22 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 04:19:42 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void		ft_free_history(t_hist *hist)
{
	if (hist->next != NULL)
		ft_free_history(hist->next);
	if (hist->command != NULL)
		ft_freestr(&hist->command);
	if (hist->save != NULL)
		ft_freestr(&hist->save);
	hist->prev = NULL;
	hist->next = NULL;
	free(hist);
}

void		ft_free_list(t_args **list)
{
	if (*list)
	{
		if ((*list)->next != NULL)
			ft_free_list(&(*list)->next);
		(*list)->type = 0;
		(*list)->pipe = 0;
		if ((*list)->args)
			ft_free(&(*list)->args);
		(*list)->next = NULL;
		free(*list);
		*list = NULL;
	}
}

void		ft_free(char ***str)
{
	char	**table;
	int	i;

	i = 0;
	if (str && *str != NULL)
	{
		table = (char **)*str;
		while (table[i] != NULL)
		{
			free(table[i]);
			table[i] = NULL;
			++i;
		}
		free(table);
		*str = NULL;
	}
}

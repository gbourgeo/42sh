/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_tables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/01 18:42:56 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/14 17:29:55 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_tables(char **tab)
{
	int		i;

	i = 0;
	if (tab != NULL)
	{
		while (tab[i])
		{
			ft_putendl(tab[i]);
			i++;
		}
	}
}

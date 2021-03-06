/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/05 19:31:47 by rfontain          #+#    #+#             */
/*   Updated: 2019/04/06 10:19:02 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memset(void *s, int c, size_t n)
{
	char	*swp;
	size_t	i;

	swp = (char*)s;
	i = 0;
	while (i < n)
	{
		swp[i] = c;
		i++;
	}
	return (s);
}

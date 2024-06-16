/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft2.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/06 17:11:14 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:11:30 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT2_H
# define LIBFT2_H

void        ft_deletechar(char *str, int pos);
int         ft_dup2(int oldd, int newd);
char        *ft_realloc(char *str, int size);
char        *ft_strcharset(char *s1, char *s2);
char        *ft_strndup(const char *s1, int size);

#endif

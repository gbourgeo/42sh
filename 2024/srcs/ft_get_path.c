/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/16 06:17:44 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:48:20 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char *ft_generate_path_from_table(char **table)
{
    char *path;
    char *tmp;

    path = ft_strdup("/");
    while (table && *table != NULL)
    {
        tmp = ft_strjoin(path, *table);
        free(path);
        path = ft_strjoin(tmp, "/");
        free(tmp);
        table++;
    }
    return (path);
}

static int ft_remove_field_from_table(char **table, int i, int times)
{
    int tablen;
    int t;

    tablen = ft_tablen(table);
    t      = times;
    while (t-- && i >= 0)
    {
        free(table[i]);
        table[i] = NULL;
        i--;
    }
    t = i + 1;
    while (t + times < tablen)
    {
        table[t] = table[t + times];
        t++;
    }
    while (t < tablen)
    {
        table[t] = NULL;
        t++;
    }
    return (i + 1);
}

/**
 * @brief Cette fonction reçoit en entrée un chaîne de caractères représentant
 * un chemin absolu pouvant comporter des '..' et des '.' (ex: "/home/user/work/../libft/./src").
 * Elle parse cette chaîne et traduit les '..' et '.' pour en sortir une chaine sans (ex:
 * "/home/user/libft/src").
 * @param[in] pwd Chemin absolu commençant par un '/'
 * @return Chemin absolu sans '..' ni '.'
 */
char *ft_get_path(char *pwd)
{
    char **table;
    char  *ret;
    int    i;

    i   = 0;
    ret = NULL;
    if ((table = ft_strsplit(pwd, '/')) == NULL)
        return (ret);
    while (table[i] != NULL)
        if (ft_strcmp(table[i], "..") == 0)
            i = ft_remove_field_from_table(table, i, 2);
        else if (ft_strcmp(table[i], ".") == 0)
            i = ft_remove_field_from_table(table, i, 1);
        else
            i++;
    ret = ft_generate_path_from_table(table);
    ft_freetab(&table);
    free(pwd);
    return (ret);
}

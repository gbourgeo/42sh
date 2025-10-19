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
#include <stdlib.h>
#include <sys/syslimits.h>

static char *ft_generate_path_from_table(char **table)
{
    char path[PATH_MAX] = { 0 };

    path[0] = '/';
    if (table != NULL)
    {
        while (*table != NULL)
        {
            ft_strncat(path, *table, sizeof(path));
            ft_strncat(path, "/", sizeof(path));
            table++;
        }
    }
    return ft_strdup(path);
}

static size_t ft_remove_field_from_table(char **table, size_t pos, size_t times)
{
    size_t tablen = 0;
    size_t repeat = 0;

    tablen = ft_tablen((const char **) table);
    repeat = times;
    while (repeat-- && pos >= 0)
    {
        free(table[pos]);
        table[pos] = NULL;
        pos--;
    }
    repeat = pos + 1;
    while (repeat + times < tablen)
    {
        table[repeat] = table[repeat + times];
        repeat++;
    }
    while (repeat < tablen)
    {
        table[repeat] = NULL;
        repeat++;
    }
    return (pos + 1);
}

/**
 * @brief Cette fonction reçoit en entrée une chaîne de caractères représentant
 * un chemin absolu pouvant comporter des '..' et des '.' (ex: "/home/user/work/../libft/./src").
 * Elle parse cette chaîne et traduit les '..' et '.' pour en sortir une chaine sans (ex:
 * "/home/user/libft/src").
 * @param[in] pwd Chemin absolu commençant par un '/'
 * @return Chemin absolu sans '..' ni '.'
 */
char *ft_get_path(char *pwd)
{
    char **table = NULL;
    char  *ret   = NULL;
    size_t iter  = 0;

    table = ft_strsplit(pwd, '/');
    if (table == NULL)
    {
        return (ret);
    }
    while (table[iter] != NULL)
    {
        if (ft_strcmp(table[iter], "..") == 0)
        {
            iter = ft_remove_field_from_table(table, iter, 2);
        }
        else if (ft_strcmp(table[iter], ".") == 0)
        {
            iter = ft_remove_field_from_table(table, iter, 1);
        }
        else
        {
            iter++;
        }
    }
    ret = ft_generate_path_from_table(table);
    ft_freetab(&table);
    free(pwd);
    return (ret);
}

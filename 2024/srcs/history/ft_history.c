/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:32:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:50:55 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_history.h"
#include "get_next_line.h"
#include "libft.h"
#include <stdlib.h>
#include <sys/fcntl.h>

/**
 * Initialise un contexte d'historique de commandes suivant un fichier.
 * @param[in] history_file Fichier d'historique de commandes à ouvrir.
 * @return Le début de l'historique de commande
 */
t_hist *ft_init_shell_history(char *history_file)
{
    t_hist *history = NULL;
    char   *line    = NULL;
    int     filed   = 0;

    if (history_file == NULL)
    {
        return (NULL);
    }
    /* Ouverture du fichier */
    filed = open(history_file, O_RDONLY);
    if (filed < 0)
    {
        return (NULL);
    }
    /* Parsing du fichier: une ligne == une commande */
    while (get_next_line(filed, &line) > 0)
    {
        history = ft_history_new(line, history);
    }
    return (history);
}

/**
 * Alloue un nouveau contexte d'historique à partir d'un nouveau buffer
 * et l'insère dans la liste d'historique.
 * @param[in] buffer Buffer à insérer dans l'historique
 * @param[in] hist_list Liste d'historique
 * @return Le début de l'historique de commande
 */
t_hist *ft_history_new(char *buffer, t_hist *hist_list)
{
    t_hist *history = NULL;

    history = (t_hist *) malloc(sizeof(*history));
    if (history == NULL)
    {
        return (hist_list);
    }
    history->command     = ft_strdup(buffer);
    history->command_len = ft_strlen(buffer);
    history->next        = NULL;
    history->prev        = NULL;
    /* Insère le nouvel historique au début de la liste */
    hist_list            = ft_history_rewind(hist_list);
    if (hist_list)
    {
        history->next   = hist_list;
        hist_list->prev = history;
    }
    return (history);
}

/**
 * Supprime la liste d'historique
 * @param[in] hist_list Liste d'historique
 */
void ft_history_remove_all(t_hist *hist_list)
{
    if (hist_list == NULL)
    {
        return;
    }
    ft_history_remove_all(hist_list->next);
    free(hist_list->command);
}

/**
 * Retourne le début de l'historique
 * @param[in] hist_list Liste d'historique
 * @return Le début de l'historique de commande
 */
t_hist *ft_history_rewind(t_hist *hist_list)
{
    if (hist_list)
    {
        while (hist_list->prev != NULL)
        {
            hist_list = hist_list->prev;
        }
    }
    return (hist_list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_history.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:32:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:50:55 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell_history.h"
#include "ft_shell_builtins.h"
#include "ft_shell_command.h"
#include "ft_shell_log.h"
#include "ft_snprintf.h"
#include "get_next_line.h"
#include "libft.h"
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

void ft_shell_history_init(t_hist *history)
{
    history->filepath = NULL;
    history->max_size = 0;
}

size_t ft_shell_history_parse_file(t_cmd      **command,
                                   t_hist      *history,
                                   const char  *progname,
                                   const char **environ)
{
    const char *ptr        = NULL;
    char        path[1024] = { 0 };
    uint8_t    *line       = NULL;
    size_t      hist_size  = 0;
    int         filed      = 0;

    ptr = ft_real_getenv("HOME", environ);
    if (ptr == NULL)
    {
        ptr = ".";
    }
    ft_snprintf(path, sizeof(path), "%s/.%s_history", ptr, progname);
    history->filepath = ft_strdup(path);
    ptr               = ft_real_getenv("42SH_HISTORY_SIZE", environ);
    history->max_size = (ptr != NULL) ? ft_atoi(ptr) : ft_atoi(SHELL_HISTORY_MAX_ELEMS);
    filed             = open(path, O_CREAT | O_RDONLY | O_CLOEXEC, 0644);
    if (filed < 0)
    {
        ft_shell_log(SH_LOG_LEVEL_WARN,
               "Error opening historic file %s: %s",
               path,
               strerror(errno));
        return (0);
    }
    ft_shell_log(SH_LOG_LEVEL_DBG, "Parsing history file %s", path);
    /* Parsing du fichier: une ligne == une commande */
    *command = NULL;
    while (get_next_line(filed, (char **) &line) > 0)
    {
        *command  = ft_shell_command_new(line,
                                        *command,
                                        SHELL_COMMAND_NEW_STEAL_LINE);
        hist_size = ft_shell_command_save(command,
                                          hist_size,
                                          history->max_size,
                                          SHELL_COMMAND_SAVE_FROM_HISTORIC);
    }
    ft_shell_log(SH_LOG_LEVEL_DBG,
           "Added %ld line%c to historic (max %ld)",
           hist_size,
           (hist_size < 2) ? '\0' : 's',
           history->max_size);
    close(filed);
    return (hist_size);
}

void ft_shell_history_save_to_file(const t_hist *history, const t_cmd *command)
{
    int    filed  = 0;
    size_t cmd_nb = 0;

    if (command == NULL)
    {
        return;
    }
    if (history->filepath == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_WARN, "Command historic cannot be saved, no historic file specified.");
        return;
    }
    /* Ouverture du fichier d'historique */
    filed = open(history->filepath, O_CREAT | O_TRUNC | O_WRONLY | O_CLOEXEC, 0644);
    if (filed < 0)
    {
        ft_shell_log(SH_LOG_LEVEL_ERR,
               "Error opening historic file %s: %s",
               history->filepath,
               strerror(errno));
        return;
    }
    /* Départ de la fin de l'historique */
    command = ft_shell_command_get_last(command);
    while (command->prev != NULL)
    {
        write(filed, command->buffer, command->len);
        write(filed, "\n", 1);
        cmd_nb++;
        command = command->prev;
    }
    /* Debug */
    ft_shell_log(SH_LOG_LEVEL_DBG,
           "Saved %ld line%c to historic file %s",
           cmd_nb,
           (cmd_nb < 2) ? '\0' : 's',
           history->filepath);
}

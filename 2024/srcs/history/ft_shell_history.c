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
#include "ft_shell_command.h"
#include "ft_shell_environ.h"
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

static size_t ft_shell_history_save_line(uint8_t *line,
                                         t_cmd  **command,
                                         size_t   history_size,
                                         long     history_max_size);

void ft_shell_history_init(t_hist *history)
{
    history->filepath = NULL;
    history->max_size = 0;
}

size_t ft_shell_history_parse_file(t_cmd     **command,
                                   t_hist     *history,
                                   t_env      *environ,
                                   const char *progname)
{
    const char *ptr        = NULL;
    char        path[1024] = { 0 };
    uint8_t    *line       = NULL;
    size_t      hist_size  = 0;
    int         filed      = 0;

    ptr = ft_shell_env_get_value("HOME", environ);
    if (ptr == NULL)
    {
        ptr = ".";
    }
    ft_snprintf(path, sizeof(path), "%s/.%s_history", ptr, progname);
    history->filepath = ft_strdup(path);
    ptr               = ft_shell_env_get_value("42SH_HISTORY_SIZE", environ);
    history->max_size = (ptr != NULL) ? ft_atoi(ptr) : ft_atoi(SHELL_HISTORY_MAX_ELEMS);
    filed             = open(path, O_CREAT | O_RDONLY | O_CLOEXEC, 0644);
    if (filed < 0)
    {
        ft_shell_log(SH_LOG_LEVEL_WARN,
                     "Error opening historic file %s: %s",
                     path,
                     strerror(errno));
        errno = 0;
        return (0);
    }
    ft_shell_log(SH_LOG_LEVEL_DBG, "Parsing history file at %s", path);
    /* Parsing du fichier: une ligne == une commande */
    while (get_next_line(filed, (char **) &line) > 0)
    {
        hist_size = ft_shell_history_save_line(line,
                                               command,
                                               hist_size,
                                               history->max_size);
        line      = NULL;
    }
    ft_shell_log(SH_LOG_LEVEL_DBG,
                 "Added %ld command%c from history (max %ld)",
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
        ft_shell_log(SH_LOG_LEVEL_DBG, "Historic not saved, no file specified.");
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
        errno = 0;
        return;
    }
    /* Départ de la fin de l'historique */
    command = ft_shell_command_get_last(command);
    while (command->next != NULL)
    {
        write(filed, command->buffer, command->len);
        write(filed, "\n", 1);
        cmd_nb++;
        command = command->next;
    }
    /* Debug */
    ft_shell_log(SH_LOG_LEVEL_DBG,
                 "Saved %ld line%c to historic file %s",
                 cmd_nb,
                 (cmd_nb < 2) ? '\0' : 's',
                 history->filepath);
}

static size_t ft_shell_history_save_line(uint8_t *line,
                                         t_cmd  **command,
                                         size_t   history_size,
                                         long     history_max_size)
{
    t_cmd *cmd = *command;
    /* Pas de sauvegarde
     * Si la ligne est vide ou commence par un espace blanc */
    if (line[0] == '\0' || ft_iswhitespace((char) line[0]))
    {
        free(line);
        return (history_size);
    }
    /* Recherche de doublon dans l'historique */
    while (cmd != NULL)
    {
        /* Ligne déjà existante, on place la commande au début de la liste */
        if (ft_ustrcmp(cmd->origin, line) == 0)
        {
            /* Si c'est le premier élément de la liste, rien à faire */
            if (cmd->next == NULL)
            {
                free(line);
                return (history_size);
            }
            /* Sinon on insère la commande au début de la liste */
            cmd->next->prev = cmd->prev;        /* ... <-> C <-> CMD --> A <-> *COMMAND -> NULL */
            if (cmd->prev != NULL)              /*           <----------/                       */
            {                                   /* ******************************************** */
                cmd->prev->next = cmd->next;    /* ... <-> C <-- CMD --> A <-> *COMMAND -> NULL */
            }                                   /*           <--------->                        */
            cmd->next = NULL;                   /*           <-- CMD -------------------->      */
                                                /* ... <-> C <---------> A <-> *COMMAND -> NULL */
            cmd->prev = *command;               /*                              <-- CMD ->      */
                                                /* ... <-> C <-> A <-> *COMMAND ---------> NULL */
            (*command)->next = cmd;             /* ... <-> C <-> A <-> *COMMAND <-> CMD -> NULL */
            free(line);                         /* ******************************************** */
            return (history_size);
        }
        cmd = cmd->prev;
    }
    /* Pas de doublon, on créé une nouvelle commande */
    *command = ft_shell_command_new(line, *command, NULL, SHELL_COMMAND_NEW_STEAL_LINE);
    history_size++;
    /* Suppression des éléments en trop */
    while ((long) history_size > history_max_size)
    {
        ft_shell_command_remove_last(command);
        history_size--;
    }
    return (history_size);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:32:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:50:55 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell_command.h"
#include "ft_defines.h"
#include "ft_shell_constants.h"
#include "ft_shell_terminal.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

/******************************************************************************
 * FONCTIONS UTILES POUR LA COMMANDE
 ******************************************************************************/
static size_t ft_shell_command_save_form_historic(t_cmd **command,
                                                  size_t  historic_size,
                                                  long    historic_max_size);
static size_t ft_shell_command_save_form_shell(t_cmd **command,
                                               size_t  historic_size,
                                               long    historic_max_size);
static void   ft_shell_command_delete(t_cmd *command);

t_cmd *ft_shell_command_new(const uint8_t *line,
                            t_cmd         *command_list,
                            e_newcmdopt    option)
{
    t_cmd *cmd = (t_cmd *) malloc(sizeof(*cmd));

    switch (option)
    {
        case SHELL_COMMAND_NEW_STEAL_LINE:
        {
            cmd->origin = line;
            break;
        }
        case SHELL_COMMAND_NEW_DUPLICATE_LINE:
        {
            cmd->origin = ft_ustrdup(line);
            break;
        }
    }
    cmd->len  = ft_ustrlen(line);
    cmd->size = 0;
    while (cmd->size <= cmd->len)
    {
        cmd->size += SHELL_COMMAND_BUFFER_HOP;
    }
    cmd->buffer = (uint8_t *) ft_memalloc(cmd->size);
    ft_memcpy(cmd->buffer, line, cmd->len);
    cmd->pos    = cmd->len;
    cmd->option = 0;
    cmd->harea  = NULL;
    cmd->prev   = NULL;
    cmd->next   = command_list;
    if (command_list != NULL)
    {
        command_list->prev = cmd;
    }

    return (cmd);
}

size_t ft_shell_command_save(t_cmd      **command,
                             size_t       historic_size,
                             long         historic_max_size,
                             e_savecmdopt option)
{
    switch (option)
    {
        case SHELL_COMMAND_SAVE_FROM_HISTORIC:
        {
            historic_size = ft_shell_command_save_form_historic(command,
                                                                historic_size,
                                                                historic_max_size);
            break;
        }
        case SHELL_COMMAND_SAVE_FROM_INTERACTIVE_MODE:
        {
            historic_size = ft_shell_command_save_form_shell(command,
                                                             historic_size,
                                                             historic_max_size);
            break;
        }
    }
    return (historic_size);
}

t_cmd *ft_shell_command_get_first(const t_cmd *command_list)
{
    if (command_list != NULL)
    {
        while (command_list->prev != NULL)
        {
            command_list = command_list->prev;
        }
    }
    return ((t_cmd *) command_list);
}

t_cmd *ft_shell_command_get_last(const t_cmd *command_list)
{
    while (command_list != NULL)
    {
        if (command_list->next == NULL)
        {
            return ((t_cmd *) command_list);
        }
        command_list = command_list->next;
    }
    return ((t_cmd *) command_list);
}

void ft_command_change_pos(
    t_cmd  *command,
    size_t  newval,
    e_posop operation,
    t_term *terminal)
{
    switch (operation)
    {
        case COMMAND_POS_ADD_VALUE:
        {
            command->pos += newval;
            break;
        }
        case COMMAND_POS_SUBSTRACT_VALUE:
        {
            command->pos -= newval;
            break;
        }
    }
    ft_shell_terminal_calc_current_command_position(terminal, command->pos);
    ft_term_move_cursor(terminal, MOVE_CURSOR_CURRENT);
}

void ft_shell_command_insert_character(t_cmd *command, uint8_t charac)
{
    size_t pos = 0;

    /* Réallocation de la taille du buffer de la commande avant insertion si nécessaire */
    if (command->len + 1 >= command->size)
    {
        ft_shell_command_realloc(command);
    }
    /* Copie des caractères d'un cran vers la droite à partir de la position courante */
    pos = command->len;
    while (pos > command->pos)
    {
        command->buffer[pos] = command->buffer[pos - 1];
        pos--;
    }
    /* Insertion du caractère dans le buffer */
    command->buffer[pos] = charac;
    command->pos += 1;
    command->len += 1;
    if (command->origin != NULL
        && !TEST_BIT(command->option, COMMAND_HISTORIC_MODIFIED))
    {
        ASSIGN_BIT(command->option, COMMAND_HISTORIC_MODIFIED);
    }
}

void ft_shell_command_delete_character(t_cmd *command, e_cmdop operation, size_t size)
{
    uint8_t *buffer = command->buffer;
    size_t   len    = command->len;
    size_t   pos    = 0;

    if (operation == COMMAND_REMOVE_CHAR_LEFT)
    {
        command->pos -= size;
    }
    pos = command->pos;
    command->len -= size;
    ft_memcpy(buffer + pos, buffer + pos + size, len - pos);
    buffer[len] = '\0';
    /* Indique que le buffer d'origine a changé */
    if (command->origin != NULL
        && !TEST_BIT(command->option, COMMAND_HISTORIC_MODIFIED))
    {
        ASSIGN_BIT(command->option, COMMAND_HISTORIC_MODIFIED);
    }
}

static t_higharea *find_closest_highlighted_area(t_higharea *harea, size_t pos)
{
    while (harea != NULL)
    {
        if (pos < harea->head)
        {
            break;
        }
        harea = harea->next;
    }
    return (harea);
}

void ft_shell_command_print(const t_cmd  *command,
                            const t_term *terminal,
                            e_printopt    option)
{
    const uint8_t *buffer = command->buffer;
    size_t         pos    = 0;
    t_higharea    *harea  = find_closest_highlighted_area(command->harea, pos);

    switch (option)
    {
        /* Imprime la commande depuis le début */
        case COMMAND_PRINT_FROM_START:
        {
            ft_term_move_cursor(terminal, MOVE_CURSOR_START);
            break;
        }
        /* Imprime la commande depuis la position */
        case COMMAND_PRINT_FROM_POS:
        {
            pos = command->pos;
            break;
        }
        /* Imprime la commande depuis la position - 1 */
        case COMMAND_PRINT_FROM_POS_LESS:
        {
            pos = command->pos - 1;
            break;
        }
    }
    /* Désactive le mode de surlignage */
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE))
    {
        ft_term_clear_modes(terminal);
    }
    /* Efface les caractères du terminal à partir du curseur */
    ft_term_clear_cursor_and_under(terminal);
    /* Réécriture de la commande */
    while (harea != NULL)
    {
        if (pos < harea->tail)
        {
            ft_term_clear_modes(terminal);
            write(STDOUT_FILENO, buffer + pos, harea->tail - pos);
            pos = harea->tail;
        }
        ft_term_highlight_mode_on(terminal);
        write(STDOUT_FILENO, buffer + pos, harea->head - pos);
        pos   = harea->head;
        harea = harea->next;
    }
    ft_term_clear_modes(terminal);
    write(STDOUT_FILENO, buffer + pos, command->len - pos);
    /* Positionne le curseur à la fin de la commande et
       efface les caractères du terminal à partir là */
    if (terminal->current.line != terminal->end.line)
    {
        ft_term_move_cursor(terminal, MOVE_CURSOR_END);
        ft_term_clear_cursor_and_under(terminal);
    }
    /* Repositionne le curseur à sa position d'origine */
    ft_term_move_cursor(terminal, MOVE_CURSOR_CURRENT);
    /* Réactive le mode de surlignage */
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE))
    {
        ft_term_highlight_mode_on(terminal);
    }
}

t_cmd *ft_shell_command_reinit(t_cmd *command)
{
    t_cmd *first = ft_shell_command_get_first(command);

    command = first;
    if (command->size != SHELL_COMMAND_BUFFER_HOP)
    {
        free((void *) command->buffer);
        command->buffer = (uint8_t *) malloc(SHELL_COMMAND_BUFFER_HOP);
        command->size   = SHELL_COMMAND_BUFFER_HOP;
    }
    ft_memset(command->buffer, 0, command->size);
    command->len    = 0;
    command->pos    = 0;
    command->option = 0;
    command->harea  = ft_highlight_remove_all(command->harea);
    /* Réinitialisation de l'historique */
    while (command->next != NULL)
    {
        command = command->next;
        if (TEST_BIT(command->option, COMMAND_HISTORIC_MODIFIED))
        {
            command->len = ft_ustrlen(command->origin);
            ft_memcpy(command->buffer, command->origin, command->len + 1);
        }
        command->option = 0;
        command->pos    = command->len;
        command->harea  = ft_highlight_remove_all(command->harea);
    }
    return (first);
}

void ft_shell_command_realloc(t_cmd *command)
{
    uint8_t *old = command->buffer;

    command->buffer = (uint8_t *) ft_memalloc(command->size + SHELL_COMMAND_BUFFER_HOP);
    ft_memcpy(command->buffer, old, command->size);
    command->size += SHELL_COMMAND_BUFFER_HOP;
    free(old);
}

void ft_shell_command_delete_list(t_cmd *command_list)
{
    t_cmd *next = NULL;

    command_list = ft_shell_command_get_first(command_list);
    while (command_list != NULL)
    {
        next = command_list->next;
        ft_shell_command_delete(command_list);
        command_list = next;
    }
}

/**
 * @brief Supprime une commande.
 * @param command Commande à supprimer
 */
static void ft_shell_command_delete(t_cmd *command)
{
    free((void *) command->origin);
    free((void *) command->buffer);
    ft_highlight_remove_all(command->harea);
    free(command);
}

/**
 * @brief Supprime la dernière commande de la liste
 * @param history Contexte de l'historique du Shell
 */
static void ft_shell_command_remove_last(t_cmd **command)
{
    t_cmd *last = ft_shell_command_get_last(*command);

    if (last == NULL)
    {
        return;
    }
    if (last->prev == NULL) /* Dernier élément de la liste */
    {
        *command = NULL;
    }
    else
    {
        *command         = last->prev;
        last->prev->next = NULL;
    }
    ft_shell_command_delete(last);
}

static size_t ft_shell_command_save_form_historic(t_cmd **command,
                                                  size_t  historic_size,
                                                  long    historic_max_size)
{
    t_cmd   *cmd    = *command;
    uint8_t *buffer = cmd->buffer;

    /* Si la ligne est vide ou commence par un espace blanc */
    if (buffer[0] == '\0' || ft_iswhitespace((char) buffer[0]))
    {                                   /* ********************************* */
                                        /* NULL <- COMMAND <-> A <-----> ... */
                                        /*          (CMD)                    */
                                        /* ********************************* */
        *command = (*command)->next;    /* NULL <--- CMD <-> COMMAND <-> ... */
                                        /* ********************************* */
        if (*command != NULL)           /*                                   */
        {                               /* NULL <--- CMD -->                 */
            (*command)->prev = NULL;    /*      <----------- COMMAND <-> ... */
        }                               /* ********************************* */
        ft_shell_command_delete(cmd);   /*       *\ C\M\D \*                 */
                                        /* ********************************* */
        return (historic_size);
    }
    /* Recherche de doublon dans l'historique */
    while (cmd->next != NULL)
    {
        cmd = cmd->next;
        /**
         * Doublon trouvé, on place le doublon au début de la liste et
         * on supprime la première commande.
         */
        if (ft_ustrcmp(buffer, cmd->origin) == 0)
        {
            /* Si c'est le premier élément de la liste */
            if (cmd->prev == *command)          /* ******************************* */
            {                                   /* NULL <- COMMAND <-> CMD <-> ... */
                                                /* ******************************* */
                cmd      = *command;            /* NULL <-   CMD   <-> ...         */
                                                /*      <- COMMAND <-> ...         */
                                                /* ******************************* */
                *command = (*command)->next;    /* NULL <- CMD <-> COMMAND <-> ... */
                                                /* ******************************* */
                if (*command != NULL)           /*                                 */
                {                               /*      <--- CMD ->                */
                    (*command)->prev = NULL;    /* NULL <--------- COMMAND <-> ... */
                }                               /* ******************************* */
                ft_shell_command_delete(cmd);   /*       *\ C\M\D \*               */
                                                /* ******************************* */
                return (historic_size);
            }
            /* Sinon on insert la commande au début */
                                                /* ****************************************** */
            cmd->prev->next = cmd->next;        /* NULL <- COMMAND <-> A <-- CMD <-> C -> ... */
                                                /*                       ---------->          */
                                                /* ****************************************** */
            cmd->prev->prev = cmd;              /* NULL <- COMMAND --> A <-- CMD <-> C -> ... */
                                                /*                       --------<->          */
                                                /* ****************************************** */
            if (cmd->next != NULL)              /*                       <----------          */
            {                                   /* NULL <- COMMAND --> A <-- CMD --> C -> ... */
                cmd->next->prev = cmd->prev;    /*                       --------<->          */
            }                                   /* ****************************************** */
            cmd->prev = NULL;                   /* NULL <- COMMAND ----------> A <-> C -> ... */
                                                /*                        <---                */
                                                /*      <------------ CMD --------->          */
                                                /* ****************************************** */
            cmd->next = (*command)->next;       /* NULL <- COMMAND ----------> A <-> C -> ... */
                                                /*      <------------ CMD <-->                */
                                                /* ****************************************** */
            ft_shell_command_delete(*command);  /*            *\ C\O\M\M\A\N\D \*             */
                                                /* ****************************************** */
            *command = cmd;                     /* NULL <--- CMD <-----------> A <-> C -> ... */
                                                /* ****************************************** */
            return (historic_size);
        }
    }
    /* Pas de doublon, on garde la commande. */
    historic_size++;
    /* Suppression des éléments en trop */
    while ((long) historic_size > historic_max_size)
    {
        ft_shell_command_remove_last(command);
        historic_size--;
    }
    return (historic_size);
}

static size_t ft_shell_command_save_form_shell(t_cmd **command,
                                               size_t  historic_size,
                                               long    historic_max_size)
{
    t_cmd   *cmd    = *command;
    uint8_t *buffer = cmd->buffer;

    /* Si la ligne est vide ou commence par un espace blanc */
    if (buffer[0] == '\0' || ft_iswhitespace((char) buffer[0]))
    {
        /* Rien de plus à faire ici */
        return (historic_size);
    }
    /* Recherche de doublon dans l'historique */
    t_cmd *first = ft_shell_command_get_first(cmd);
    cmd          = first;
    while (cmd->next != NULL)
    {
        cmd = cmd->next;
        /* Doublon trouvé, on le place au début */
        if (ft_ustrcmp(buffer, cmd->origin) == 0)
        {
            /* Si c'est le premier élément de la liste */
            if (cmd == first->next)
            {
                /* Rien de plus à faire ici */
                return (historic_size);
            }
            /* Sinon on insère la commande au début de "l'historique" (après la première commande) */
            if (cmd->next != NULL)           /* ***************************************** */
            {                                /*                    <------------          */
                cmd->next->prev = cmd->prev; /* NULL <- FIRST <-> A <-> CMD --> C -> NULL */
            }                                /*                                           */
                                             /* ***************************************** */
            cmd->prev->next   = cmd->next;   /* NULL <- FIRST <-> A <---------> C -> NULL */
                                             /*                     <-- CMD -->           */
                                             /* ***************************************** */
            cmd->prev         = first;       /* NULL <- FIRST <-> A <---------> C -> NULL */
                                             /*               <-------- CMD -->           */
                                             /* ***************************************** */
            cmd->next         = first->next; /* NULL <- FIRST <---------> A <-> C -> NULL */
                                             /*               <-- CMD -->                 */
                                             /* ***************************************** */
            first->next->prev = cmd;         /* NULL <- FIRST ----------> A <-> C -> NULL */
                                             /*               <-- CMD <->                 */
                                             /* ***************************************** */
            first->next       = cmd;         /* NULL <- FIRST <-> CMD <-> A <-> C -> NULL */
                                             /* ***************************************** */
            return (historic_size);
        }
    }
    /* Pas de doublon, on sauvegarde la commande au début de l'historique */
                                                                  /* ****************************************** */
                                                                  /* NULL <- FIRST <-> A <-> B -> NULL          */
                                                                  /* ****************************************** */
    cmd         = ft_shell_command_new(buffer,                    /* NULL <--- CMD <-> A                        */
                               first->next,                       /* NULL <- FIRST --> A <-> B -> NULL          */
                               SHELL_COMMAND_NEW_DUPLICATE_LINE); /* ****************************************** */
    cmd->prev   = first;                                          /* NULL <- FIRST <-- CMD <-> A <-> B -> NULL  */
                                                                  /*               ---------->                  */
                                                                  /* ****************************************** */
    first->next = cmd;                                            /* NULL <- FIRST <-> CMD <-> A <-> B -> NULL  */
                                                                  /* ****************************************** */
    historic_size++;
    /* Suppression des éléments en trop */
    while ((long) historic_size > historic_max_size)
    {
        ft_shell_command_remove_last(command);
        historic_size--;
    }
    return (historic_size);
}

/******************************************************************************
 * FONCTIONS UTILES POUR LES ZONES DE TEXTE SURLIGNEES
 ******************************************************************************/

static t_higharea *ft_command_highlight_remove_area(t_cmd *command, t_higharea *harea);

void ft_command_highlight_move_areas(t_cmd *command, e_highop operation, size_t value)
{
    t_higharea *harea = ft_highlight_first(command->harea);
    size_t      pos   = command->pos;
    int         hmode = TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE);

    while (harea != NULL)
    {
        switch (operation)
        {                                           /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_ADD_CHAR:   /* Ajout d'un caractère                                 */
            {                                       /*                                                      */
                if (pos < harea->tail)              /* La position ne se trouve pas dans une zone de texte  */
                {                                   /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                    harea->tail += value;           /* Result:   | - |     *   | - - - - - - - |   | - - |  */
                    harea->head += value;           /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                }                                   /*                                                      */
                else if (pos == harea->tail)        /* La position se trouve au début d'une zone de texte   */
                {                                   /* Highlight Mode : OFF                                 */
                    if (hmode == 0)                 /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    {                               /* Result:   | - |           * - - - - - - - |   | - -  */
                        harea->tail += value;       /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    }                               /* Highlight Mode : ON                                  */
                    harea->head += value;           /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                }                                   /* Result:   | - |       | - * - - - - - - - |   | - -  */
                                                    /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos < harea->head)         /* La position se trouve dans une zone de texte         */
                {                                   /* Hareas:   | - |       | - - * - - - - |   | - - |    */
                    harea->head += value;           /* Result:   | - |       | - - - * - - - - |   | - - |  */
                }                                   /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos == harea->head)        /* La position se trouve à la fin d'une zone de texte   */
                {                                   /* Highlight Mode : OFF                                 */
                    if (hmode != 0)                 /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                    {                               /* Result:   | - |       | - - - - - - - | *   | - - |  */
                        harea->head += value;       /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    }                               /* Highlight Mode : ON                                  */
                }                                   /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                break;                              /* Result:   | - |       | - - - - - - - - *   | - - |  */
            }                                       /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                                                    /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_LEFT: /* Suppression d'un caractère à gauche            */
            {                                       /*                                                      */
                if (pos <= harea->tail)             /* La position ne se trouve pas dans une zone de texte  */
                {                                   /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                    harea->tail -= value;           /* Result:   | - | *   | - - - - - - - |     | - - |    */
                    harea->head -= value;           /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                }                                   /* La position se trouve au début d'une zone de texte   */
                                                    /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                                                    /* Result:   | - |     * - - - - - - - |     | - - |    */
                                                    /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos <= harea->head)        /* La position se trouve dans une zone de texte         */
                {                                   /* Hareas:   | - |       | - - * - - - - |   | - - |    */
                    harea->head -= value;           /* Result:   | - |       | - * - - - - |     | - - |    */
                }                                   /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                                                    /* La position se trouve à la fin d'une zone de texte   */
                                                    /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                                                    /* Result:   | - |       | - - - - - - *     | - - |    */
                                                    /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos > harea->head)         /* La position se trouve après une zone de texte        */
                {                                   /*                                                      */
                    if (pos - value <= harea->tail) /* et la value se trouve avant cette zone de texte      */
                    {                               /* Hareas:   | - |       | - - - - - - - | *   | - - |  */
                        harea = ft_command_highlight_remove_area(command, harea);
                                                    /* Result:   | - |       *                     | - - |  */
                        continue;                   /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    }                               /*                                                      */
                    if (pos - value < harea->head)  /* et la value se trouve dans cette zone de texte       */
                    {                               /* Hareas:   | - |       | - - - - - - - | *   | - - |  */
                        harea->head = pos - value;  /* Result:   | - |       | - - - *             | - - |  */
                    }                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                }                                   /*                                                      */
                break;                              /*                                                      */
            }                                       /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_RIGHT: /* Suppression d'un caractère à droite           */
            {                                       /*                                                      */
                if (pos < harea->tail)              /* La position ne se trouve pas dans une zone de texte  */
                {                                   /*                                                      */
                    if (pos + value >= harea->head) /* et la value se trouve après cette zone de texte      */
                    {                               /* on supprime la zone et on passe à la suivante...     */
                        harea = ft_command_highlight_remove_area(command, harea);
                        continue;                   /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                    }                               /* Result:   | - |   *                       | - - |    */
                                                    /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    if (pos + value >= harea->tail) /* et la value se trouve dans cette zone de texte       */
                    {                               /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                        harea->tail = pos;          /* Result:   | - |   * - - - - |             | - - |    */
                    }                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    else                            /*                                                      */
                    {                               /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                        harea->tail -= value;       /* Result:   | - |   * | - - - - - - - |     | - - |    */
                    }                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    harea->head -= value;           /*                                                      */
                }                                   /*                                                      */
                else if (pos == harea->tail)        /* La position se trouve au début d'une zone de texte   */
                {                                   /*                                                      */
                    if (pos + value >= harea->head) /* et la value se trouve après cette zone de texte      */
                    {                               /* Highlight Mode : OFF                                 */
                        if (hmode == 0)             /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                        {                           /* Result:   | - |       *                   | - - |    */
                            harea = ft_command_highlight_remove_area(command, harea); /* on supprime la zone et         */
                            continue;                                                 /* on passe à la zone suivante... */
                        }                           /*                                                      */
                        harea->head = harea->tail;  /* Highlight Mode : ON                                  */
                    }                               /* on conserve la zone de texte surlignée...            */
                    else                            /*                                                      */
                    {                               /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                        harea->head -= value;       /* Result:   | - |       * - - - |           | - - |    */
                    }                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                }                                   /*                                                      */
                else if (pos < harea->head)         /* La position se trouve dans une zone de texte         */
                {                                   /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                    if (pos + value >= harea->head) /* et la value se trouve après cette zone de texte      */
                    {                               /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                        harea->head = pos;          /* Result:   | - |       | - - - *           | - - |    */
                    }                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    else                            /* et la value se trouve dans cette zone de texte       */
                    {                               /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                        harea->head -= value;       /* Result:   | - |       | - - - * - |       | - - |    */
                    }                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                }                                   /*                                                      */
                                                    /* La position se trouve à la fin d'une zone de texte   */
                break;                              /* -> rien à faire.                                     */
            }                                       /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_MOVE_LEFT:  /* Déplacement de la position vers la gauche            */
            {                                       /*                                                      */
                if (hmode == 0)                     /* Highlight Mode : OFF                                 */
                {                                   /*                                                      */
                    break;                          /* -> rien à faire                                      */
                }                                   /*                                                      */
                if (pos == harea->tail)             /* La position se trouve au début d'une zone de texte   */
                {                                   /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    harea->tail -= value;           /* Result:   | - |   * - - - - - - - - - |   | - - |    */
                }                                   /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos < harea->head)         /* La position se trouve dans une zone de texte         */
                {                                   /*                                                      */
                    if (pos - value < harea->tail)  /* et la value est avant la tail                        */
                    {                               /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                        harea->tail = pos - value;  /* Result:   | - |   * - - - - - - - - - |   | - - |    */
                    }                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                }                                   /*                                                      */
                else if (pos == harea->head)        /* La position se trouve à la fin d'une zone de texte   */
                {                                   /*                                                      */
                    if (pos - value <= harea->tail) /* et la value est avant la tail                        */
                    {                               /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                        harea->head = harea->tail;  /* Result:   | - |   * - |                   | - - |    */
                        harea->tail = pos - value;  /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    }                               /*                                                      */
                    else                            /* sinon                                                */
                    {                               /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                        harea->head -= value;       /* Result:   | - |       | - - - *           | - - |    */
                    }                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                }                                   /*                                                      */
                else if (pos > harea->head)         /* La position se trouve après une zone de texte        */
                {                                   /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                    if (pos - value <= harea->tail) /* Result:               | - - - - - - - *   | - - |    */
                    {                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                        harea = ft_command_highlight_remove_area(command, harea); /* on supprime la zone et     */
                        continue;                                                 /* on passe à la suivante...  */
                    }                               /*                                                      */
                }                                   /* Sinon rien à faire.                                  */
                break;                              /*                                                      */
            }                                       /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT: /* Déplacement de la position vers la droite            */
            {                                       /*                                                      */
                if (hmode == 0)                     /* Highlight Mode : OFF                                 */
                {                                   /*                                                      */
                    break;                          /* -> Rien à faire                                      */
                }                                   /*                                                      */
                if (pos < harea->tail)              /* La position se trouve avant d'une zone de texte      */
                {                                   /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    if (pos + value >= harea->head) /* Result:   | - |                       | - - - - *    */
                    {                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                        harea = ft_command_highlight_remove_area(command, harea); /* on supprime la zone et     */
                        continue;                                                 /* on passe à la suivante...  */
                    }                               /*                                                      */
                }                                   /*                                                      */
                if (pos < harea->head)              /* La position se trouve au début d'une zone de texte   */
                {                                   /* et la value est supérieure à la head                 */
                    if (pos + value > harea->head)  /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    {                               /* Result:   | - |                       | - - - - *    */
                        harea->tail = harea->head;  /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                        harea->head = pos + value;  /* Sinon                                                */
                    }                               /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    else                            /* Result:   | - |               * - - - |   | - - |    */
                    {                               /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                        harea->tail += value;       /* La position se trouve dans une zone de texte         */
                    }                               /* et la value est supérieure à la head                 */
                }                                   /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                                                    /* Result:   | - |                       | - - - - *    */
                                                    /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                                                    /* Sinon                                                */
                                                    /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                                                    /* Result:   | - |               * - - - |   | - - |    */
                                                    /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos == harea->head)        /* La position se trouve à la fin d'une zone de texte   */
                {                                   /* Hareas:   | - |       | - - - - *         | - - |    */
                    harea->head += value;           /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                }                                   /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                break;                              /*                                                      */
            }
        }
        harea = harea->next;
    }
}

/**
 * @brief Fonction de suppression d'une zone de texte surlignée d'une commande.
 * @param[in] command Structure d'une commande
 * @param[in] harea Structure d'une zone de texte surlignée
 * @return La zone de texte surlignée suivante.
 */
static t_higharea *ft_command_highlight_remove_area(t_cmd *command, t_higharea *harea)
{
    t_higharea *next = NULL;

    if (harea != NULL)
    {
        next = harea->next;
        if (harea->prev != NULL)
        {
            harea->prev->next = next;
        }
        else
        {
            command->harea = next;
        }
        if (next != NULL)
        {
            next->prev = harea->prev;
        }
        free(harea);
    }
    return (next);
}

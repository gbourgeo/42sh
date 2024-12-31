/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell.h"
#include "ft_termkeys.h"

#include <unistd.h>

/**
 * @brief Fonction d'insertion d'un caractère dans la commande
 * en cours à la position courante.
 * @param[in] c Caractère à insérer dans la commande
 * @param[in] shell_ctx Environnement du shell
 */
void ft_insert_character(char c, t_shell *shell)
{
    t_htext *htext;
    t_cmd   *cmd;
    size_t   i;

    cmd = &shell->command;
    /* Vérification de la taille du buffer de commande */
    if (cmd->buffer_len + 1 >= cmd->buffer_size)
        ft_command_realloc(cmd, cmd->buffer_size + COMMAND_BUFFER_SIZE);
    /* Copie des caractères d'un cran vers la droite à partir de la position courante */
    i = cmd->buffer_len;
    if (shell->options & SHELL_INTERACTIVE_MODE)
    {
        while (i > cmd->pos)
        {
            cmd->buffer[i] = cmd->buffer[i - 1];
            i--;
        }
    }
    /* Insertion du caractère */
    cmd->buffer[i] = c;
    cmd->buffer_len += 1;

    if (shell->options & SHELL_INTERACTIVE_MODE)
    {
        /* Le caractère est-il dans une zone surligné ? */
        htext = ft_highlight_get_area(cmd->pos, shell->highlighted.texts);
        if (shell->highlighted.on != 0 || htext != NULL)
            htext->head += 1;
        /* Décalage des zones de texte surligné suivantes */
        if (htext != NULL)
        {
            while ((htext = htext->next) != NULL)
            {
                htext->tail += 1;
                htext->head += 1;
            }
        }
    }
}

/**
 * @brief Fonction d'affichage d'un caractère sur la sortie standard.
 * @param[in] shell_ctx Environnement du shell
 */
void ft_print_character(t_shell *shell)
{
    t_htext *htext;
    size_t   pos;

    pos   = shell->command.pos;

    /* Récupération de la zone de texte surlignée par la position du curseur */
    htext = ft_highlight_get_area(pos, shell->highlighted.texts);
    /* Si le mode surlignage est activé ou la position du curseur est dans une zone surlignée */
    if (shell->highlighted.on != 0 || htext != NULL)
        ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
    /* Affichage du caractère */
    write(STDOUT_FILENO, shell->command.buffer + pos, 1);
    /* Déplacement du curseur */
    ft_move_cursor_right(shell);
    /* Si le mode surlignage est désactivé ou la position du curseur est dans une zone surlignée */
    if (shell->highlighted.on == 0)
        ft_term_clear_modes(&shell->terminal); /* Désactive le mode surlignage */
}

/**
 * @brief Fonction d'affichage de la commande sur la sortie standard.
 * @param shell Structure interne du shell
 * @param restore_pos Repositionne le curseur à sa position
 */
void ft_print_command(t_shell *shell, char restore_pos)
{
    t_htext *htext;
    size_t   pos, col, line;
    int      highlight;

    /* La position du curseur est-elle dans une zone de texte surlignée ? */
    htext = ft_highlight_get_area(shell->command.pos, shell->highlighted.texts);
    if (restore_pos != 0)                           /* Restauration de la position du curseur */
    {
        pos       = shell->command.pos;             /* Sauvegarde la position dans la commande */
        col       = shell->terminal.current_column; /* Sauvegarde colonne */
        line      = shell->terminal.current_line;   /* Sauvegarde ligne */
        highlight = shell->highlighted.on;          /* Sauvegarde du mode surlignage */
    }
    ft_term_clear_modes(&shell->terminal);          /* Désactive tous les modes actifs */
    shell->highlighted.on = 0;                      /* Désactive le surlignage */
    /* Réécriture de la ligne de commande */
    while (shell->command.buffer[shell->command.pos] != '\0')
    {
        if (htext
            && shell->command.pos >= htext->tail
            && shell->command.pos < htext->head)         /* Le curseur est dans une zone de texte surlignée */
            ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
        write(STDOUT_FILENO, shell->command.buffer + shell->command.pos, 1);
        if (htext
            && shell->command.pos >= htext->tail
            && shell->command.pos < htext->head)   /* Le curseur est dans une zone de texte surlignée */
            ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
        ft_move_cursor_right(shell);               /* Déplace le curseur d'un cran sur la droite */
        if (htext
            && shell->command.pos > htext->head)
            htext = htext->next; /* Change de zone de texte surlignée */
    }
    if (restore_pos != 0)
    {
        if (highlight != 0)
            ft_term_highlight_mode_on(&shell->terminal);  /* Active le mode surlignage */
        shell->highlighted.on          = highlight;       /* Réaffecte le mode surlignage */
        shell->terminal.current_line   = line;            /* Restaure position ligne */
        shell->terminal.current_column = col;             /* Restaure position colonne */
        shell->command.pos             = pos;             /* Restaure la position dans la commande */
        ft_term_move_cursor(&shell->terminal, col, line); /* Restaure la position du curseur */
    }
}

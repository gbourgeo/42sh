/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_highlight.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_termkeys.h"
#include "ft_minishell.h"
#include "ft_highlight.h"

/**
 * @brief Fonction de récupération d'une zone de texte surlignée.
 * @param[in] pos Position actuelle du curseur
 * @param[in] htexts Liste des zones surlignées
 * @return Si la position actuelle du curseur se trouve sur une zone surlignée
 * celle-ci est retournée, NULL sinon.
*/
t_high_t        *ft_highlight_get_area(size_t pos, t_high_t *htexts)
{
    while (htexts)
    {
        if (pos >= htexts->tail && pos < htexts->head)
            break ;
        htexts = htexts->next;
    }
    return (htexts);
}

/**
 * @brief Fonction de création d'une nouvelle zone de surlignage.
 * @param[in] pos Position actuelle du curseur
 * @param[in] htexts Liste des zones surlignées
 * @return Zone surlignée de la position ou une nouvelle zone de surlignage
*/
t_high_t        *ft_highlight_new_area(size_t pos, t_high_t *htexts)
{
    t_high_t    *ptr;

    ptr = ft_highlight_get_area(pos, htexts);
    if (ptr == NULL)
    {
        ptr = (t_high_t *)malloc(sizeof(*ptr));
        ptr->head = pos;
        ptr->tail = pos;
        ptr->next = NULL;
        ptr->prev = NULL;
        if (htexts && htexts->prev)
        {
            htexts->prev->next = ptr;
            ptr->prev = htexts->prev;
        }
        if (htexts && htexts->next)
        {
            htexts->next->prev = ptr;
            ptr->next = htexts;
        }
    }
    return (ptr);
}
/*
           *
    HT -x> HT -> HT -> NULL
     |_>  PTR
*/

/**
 * @brief Fonction de suppression de zone de surlignage.
 * @param[in] htext Zone de surlignage à supprimer
 * @return La zone précédente si elle existe, la suivante sinon
*/
t_high_t        *ft_highlight_remove_area(t_high_t *htext)
{
    t_high_t    *ret;

    ret = NULL;
    if (htext != NULL)
    {
        if (htext->prev)
        {
            ret = htext->prev;
            htext->prev->next = htext->next;
        }
        else
            ret = htext->next;
        if (htext->next)
            htext->next->prev = htext->prev;
        /* Désallocation de la zone de surlignage */
        free(htext);
    }
    return (ret);
}

/**
 * @brief Fonction de suppression de toutes les zones surlignées dans la liste.
 * Elle supprime également le surlignage des zones.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void            ft_highlight_remove_all(t_high_t **htexts)
{
    while (*htexts != NULL)
        *htexts = ft_highlight_remove_area(*htexts);
}

/**
 * @brief Fonction de réarrangement des zones surlignées.
 * La tete (head) et la queue (tail) sont réorganisées de telle sorte que la
 * tete soit toujours supérieure à la queue.
 * Les zones sont ensuite triées par ordre croissant, la première zone
 * surlignée du texte sera placée au début de la liste.
 * @param[in] text Liste des zones surlignées
*/
t_high_t        *ft_highlight_sort_area(t_high_t *text)
{
    t_high_t    *ptr;
    size_t      pos;

    if (text == NULL)
        return NULL;
    /* Réarrangement de tail et head */
    if (text->head < text->tail)
    {
        pos = text->head;
        text->head = text->tail;
        text->tail = pos;
        ft_highlight_sort_area(text->next);
    }
    ptr = text->next;
    if (ptr == NULL)
        return (text);
    if (ptr->head < text->head)
    {
        pos = ptr->head;
        ptr->head = text->head;
        text->head = pos;
        pos = ptr->tail;
        ptr->tail = text->tail;
        text->tail = pos;
    }
    return (text);
}

/**
 * @brief Fonction de démarrage/arret du mode de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void            ft_highlight_mode(void *shell_ctx)
{
    t_shell     *shell;
    t_high_t    *text;

    shell = (t_shell *)shell_ctx;
    shell->highlighted.on = !shell->highlighted.on;
    if (shell->highlighted.on == 0)
    {
        text = shell->highlighted.texts;
        if (text->head >= text->tail)
            ft_tputs("me", shell); /* Désactive tous les modes actifs */
        if (shell->command.buffer[shell->command.pos] != '\0')
        {
            ft_tputs("im", shell); /* Active le mode insertion */
            ft_tputs("dc", shell); /* Efface 1 caractère sous la position du curseur */
            ft_putchar(shell->command.buffer[shell->command.pos]);
            ft_tputs("le", shell); /* Bouge le curseur à gauche d'1 colonne */
            ft_tputs("ei", shell); /* Désactive le mode insertion */
        }
        if (text->head == text->tail)
            shell->highlighted.texts = ft_highlight_remove_area(shell->highlighted.texts);
        else
            shell->highlighted.texts = ft_highlight_sort_area(shell->highlighted.texts);
        ft_tputs("me", shell); /* Désactive tous les modes actifs */
    }
    else
    {
        ft_tputs("mr", shell); /* Active le mode surlignage */
        text = ft_highlight_get_area(shell->command.pos, shell->highlighted.texts);
        if (text == NULL)
            shell->highlighted.texts = ft_highlight_new_area(shell->command.pos, shell->highlighted.texts);
        else
            shell->highlighted.texts = text;
    }
}

/**
 * @brief Fonction de copie du/des textes surlignés
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void            ft_highlight_paste(void *shell_ctx)
{
    t_shell     *shell;

    shell = (t_shell *)shell_ctx;
    (void)shell;
}

/**
 * @brief Fonction de sauvegarde du/des textes surlignés.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void            ft_highlight_yank(void *shell_ctx)
{
    t_shell     *shell;
    t_high_t    *text;
    char        *yank;
    size_t      len;
    int         col;
    int         line;

    shell = (t_shell *)shell_ctx;
    text = NULL;
    yank = NULL;
    len = 0;
    ft_highlight_sort_area(shell->highlighted.texts);
    ft_freestr(&shell->highlighted.yank);
    while (shell->highlighted.texts)
    {
        text = shell->highlighted.texts;
        len = ft_strlen(shell->highlighted.yank) + (text->head - text->tail) + 2;
        yank = (char *)malloc(len);
        ft_strcpy(yank, shell->highlighted.yank);
        ft_strcat(yank , " ");
        ft_strncat(yank , shell->command.buffer + text->tail, text->head - text->tail);
        yank[len - 1] = '\0';
        ft_freestr(&shell->highlighted.yank);
        shell->highlighted.yank = yank;
        shell->highlighted.texts = ft_highlight_remove_area(shell->highlighted.texts);
    }
    shell->highlighted.on = 0;

    /* On se déplace au début de la commande */
    col = shell->terminal.current_column - (shell->command.pos % shell->terminal.max_column);
    if (col < 0)
        col = shell->terminal.max_column + col + 1;
    line = shell->terminal.current_line - ((shell->command.pos + shell->prompt.len) / (shell->terminal.max_column + 1));
    ft_tgoto(col, line, shell);
    ft_tputs("me", shell); /* Désactive tous les modes actifs */
    // ft_tputs("cd", shell); /* Efface depuis le curseur jusqu'à la fin du terminal */
    ft_putstr(shell->command.buffer);

    /* On se replace à la position de base */
    ft_tgoto(shell->terminal.current_column, shell->terminal.current_line, shell);
}

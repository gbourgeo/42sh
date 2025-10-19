/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_prompt.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_PROMPT_H_
#define _FT_SHELL_PROMPT_H_

#include "ft_defines.h"
#include "ft_shell_environ.h"
#include "ft_shell_terminal.h"
#include <stddef.h>

/*****************************************************************************
 * PROMPT STRUCTURE
 ******************************************************************************/

typedef struct _align(32) s_prompt
{
    char  *str;         /* Prompt string */
    size_t real_len;    /* Longueur réel du prompt (avec les codes couleurs) */
    size_t printed_len; /* Longueur d'affichage du prompt (sans les codes couleurs) */
    char   print;       /* Afficher le prompt ? */
} t_prompt;

/*****************************************************************************
 * PROMPT FUNCTIONS
 ******************************************************************************/

/**
 * @brief Initialise la structure du prompt.
 * @param propmt Structure interne du prompt
 */
void ft_shell_prompt_init(t_prompt *prompt);

/**
 * @brief Créé le prompt du shell (mode interactif).
 * @param prompt Prompt du Shell
 * @param environ Environement du Shell
 */
void ft_shell_prompt_create(t_prompt *prompt, t_env *environ);

/**
 * @brief Affiche le prompt du shell (mode interactif).
 * @param shell Structure interne du shell
 */
void ft_shell_prompt_print(t_prompt *prompt, t_term *terminal);

/**
 * @brief Nettoie la structure du prompt
 * @param prompt
 */
void ft_shell_prompt_clear(t_prompt *prompt);

#endif /* _FT_SHELL_PROMPT_H_ */

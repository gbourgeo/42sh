/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_PROMPT_H_
#define _FT_PROMPT_H_

#include <stddef.h>

typedef struct __attribute__((aligned(SOFT_ALIGNMENT_CONSTANT))) s_prompt
{
    char  *str;   /* Prompt string */
    size_t len;   /* Longueur du prompt */
    char   print; /* Afficher le prompt ? */
} t_prompt;

/**
 * @brief Initialise la structure du prompte.
 * @param propmt Structure interne du prompte
 */
void ft_init_shell_prompt(t_prompt *prompt);

#endif /* _FT_PROMPT_H_ */

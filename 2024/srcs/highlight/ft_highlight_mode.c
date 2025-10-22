/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_highlight_mode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_command.h"
#include "ft_shell_terminal.h"
#include <stdlib.h>

void ft_highlight_mode(t_shell *shell)
{
    /* Highlight mode : actif */
    if (_test_bit(shell->command->option, COMMAND_HIGHLIGHT_MODE))
    {
        _remove_bit(shell->command->option, COMMAND_HIGHLIGHT_MODE); /* Désactive l'option */
        ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
        shell->command->harea = ft_command_highlight_sort_area(shell->command->harea);
    }
    /* Highlight mode : inactif */
    else
    {
        _set_bit(shell->command->option, COMMAND_HIGHLIGHT_MODE); /* Active l'option */
        ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
        shell->command->harea = ft_command_highlight_get_area(shell->command->harea,
                                                              shell->command->pos);
    }
}

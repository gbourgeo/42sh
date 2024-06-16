/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_termkeys.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TERMKEYS_H
# define FT_TERMKEYS_H

# define MAX_KEY_SIZE   16


typedef struct  s_special_key
{
    char        code[16];
    int         code_len;
    void        (*handler)(void *);
}               t_key;

void        ft_key_read(char *buf, int buf_len, void *shell_ctx);

/*
** KEYBOARD functions handlers
*/
void        ft_control_c(void *shell_ctx);
void        ft_control_d(void *shell_ctx);
void        ft_delete_character_left(void *shell_ctx);
void        ft_delete_character_right(void *shell_ctx);
void        ft_delete_word_left(void *shell_ctx);
void        ft_delete_word_right(void *shell_ctx);
void        ft_move_cursor_end_of_command(void *shell_ctx);
void        ft_move_cursor_start_of_commmand(void *shell_ctx);
void        ft_move_cursor_right(void *shell_ctx);
void        ft_move_cursor_left(void *shell_ctx);
void        ft_move_cursor_up(void *shell_ctx);
void        ft_move_cursor_down(void *shell_ctx);
void        ft_move_word_right(void *shell_ctx);
void        ft_move_word_left(void *shell_ctx);
void        ft_move_shift_right(void *shell_ctx);
void        ft_move_shift_left(void *shell_ctx);
void        ft_insert_character(char c, void *shell_ctx);
void        ft_print_character(void *shell_ctx);
void        ft_print_prev_command(void *shell_ctx);
void        ft_print_next_command(void *shell_ctx);

void        ft_print_command(void *shell_ctx, char restore_pos);

#endif /* FT_TERMKEYS_H */

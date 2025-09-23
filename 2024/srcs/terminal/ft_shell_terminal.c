/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_terminal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 12:00:56 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:09:42 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell_terminal.h"
#include "ft_defines.h"
#include "ft_shell_constants.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/ttycom.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>

void ft_shell_terminal_init(t_term *terminal)
{
    ft_memset(terminal, 0, sizeof(*terminal));
    terminal->fd = STDIN_FILENO;
    ft_shell_terminal_get_size(terminal);
}

void ft_shell_terminal_clear(const t_term *terminal, uint32_t options)
{
    if (TEST_BIT(options, SHELL_TERMATTR_LOADED))
    {
        ft_shell_terminal_restore_attributes(terminal);
    }
    if (terminal->fd > 0 && terminal->fd != STDIN_FILENO)
    {
        close(terminal->fd);
    }
}

void ft_shell_terminal_get_size(t_term *terminal)
{
    struct winsize winsize;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
    terminal->max_column = winsize.ws_col;
    terminal->max_line   = winsize.ws_row - 1;
}

void ft_shell_terminal_get_cursor_position(t_term *terminal, e_cursorset option)
{
    char buff[16] = { 0 };
    int  line     = 0;
    int  column   = 0;

    write(STDOUT_FILENO, "\033[6n", 4);
    read(STDOUT_FILENO, buff, sizeof(buff));
    if (buff[0] != '\033' || buff[1] != '[' || ft_strchr(buff, ';') == NULL)
    {
        return;
    }
    line   = ft_atoi(buff + 2) - 1;
    column = ft_atoi(ft_strchr(buff + 2, ';') + 1) - 1;
    switch (option)
    {
        case SET_CURSOR_CURRENT:
        {
            terminal->current.line   = line;
            terminal->current.column = column;
            break;
        }
        case SET_CURSOR_ALL:
        {
            terminal->start.line     = line;
            terminal->start.column   = column;
            terminal->current.line   = line;
            terminal->current.column = column;
            terminal->end.line       = line;
            terminal->end.column     = column;
            break;
        }
    }
}

void ft_shell_terminal_calc_current_command_position(t_term *terminal,
                                                     size_t  command_pos)
{
    size_t column = (size_t) terminal->start.column + command_pos;
    size_t line   = column / (size_t) terminal->max_column;

    terminal->current.column = (int) column % terminal->max_column;
    terminal->current.line   = terminal->start.line + (int) line;

    if (terminal->current.line > terminal->max_line)
    {
        terminal->start.line -= (terminal->current.line - terminal->max_line);
        terminal->current.line = terminal->max_line;
    }
}

void ft_shell_terminal_calc_end_command_position(t_term *terminal,
                                                 size_t  command_len)
{
    size_t column = (size_t) terminal->start.column + command_len;
    size_t line   = column / (size_t) terminal->max_column;

    terminal->end.column = (int) column % terminal->max_column;
    terminal->end.line   = terminal->start.line + (int) line;

    if (terminal->end.line > terminal->max_line)
    {
        terminal->start.line -= (terminal->end.line - terminal->max_line);
        terminal->end.line = terminal->max_line;
    }
}

/**
 * @brief Récupère les valeurs de certain Termcaps.
 * @param terminal Structure interne Terminal
 * @return 0 si tous les termcaps ont été chargé, 1 autrement.
 */
static int ft_get_termcaps(const char ** const capabilities)
{
    static char *caps[] = {
        "cd", // Efface la ligne et celles sous le curseur.
        "ce", // Efface du curseur jusqu'à la fin de la ligne et celles en dessous.
        "cm", // Déplace le curseur (sur la colonne #1 et la ligne #2).
        "dc", // Efface 1 caractère sous la position du curseur.
        "ei", // Désactive le mode insertion.
        "im", // Active le mode insertion.
        "le", // Bouge le curseur à gauche d'1 colonne.
        "me", // Désactive tous les modes actifs.
        "mr", // Active le mode surlignage.
        "nw"  // Déplace le curseur au début de la ligne suivante.
    };
    unsigned long iter  = 0;
    int           error = 0;

#pragma unroll(LENGTH_OF(caps))
    while (iter < LENGTH_OF(caps))
    {
        capabilities[iter] = NULL;
        capabilities[iter] = tgetstr(caps[iter], NULL);
        if (capabilities[iter] == NULL && iter != TERMCAP_MOVE_CURSOR_DOWN)
        {
            ft_shell_log(SH_LOG_LEVEL_FATAL, "termcaps: capability %s not available.", caps[iter]);
            error = 1;
        }
        iter++;
    }
    return (error);
}

int ft_shell_terminal_load_termcaps(t_term *terminal, const char *term)
{
    char *tty_name = NULL;
    int   ret      = 0;

    if (isatty(STDIN_FILENO) == 0)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "termcaps: Terminal not valid.");
        return (1);
    }
    tty_name = ttyname(STDIN_FILENO);
    if (tty_name == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "termcaps: Not connected to a terminal");
        return (1);
    }
    terminal->fd = open(tty_name, O_RDWR | O_CLOEXEC);
    if (terminal->fd == -1)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "termcaps: Failed to open tty \"%s\".", tty_name);
        return (1);
    }
    if (tcgetattr(terminal->fd, &terminal->ios) != 0)
    {
        (ft_shell_log(SH_LOG_LEVEL_FATAL, "termcaps: Can't get terminal attributes."));
        return (1);
    }
    if (term == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "termcaps: Specify a terminal type in your environment.");
        return (1);
    }
    ret = tgetent(terminal->desc, term);
    if (ret <= 0)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL,
               "termcaps: %s.",
               (ret == 0) ? "Terminal not defined in database" : "Termcap's data base files unavailable");
        return (1);
    }
    return ft_get_termcaps((const char ** const) terminal->capabilities);
}

int ft_shell_terminal_change_attributes(const t_term *terminal)
{
    struct termios termios;

    termios = terminal->ios;
    REMOVE_BIT(termios.c_lflag, ICANON | ECHO | ISIG);
    termios.c_cc[VMIN]  = 1;
    termios.c_cc[VTIME] = 0;
    if ((tcsetattr(terminal->fd, TCSANOW, &termios)) == -1)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "failed to load terminal attributes");
        return (1);
    }
    return (0);
}

void ft_shell_terminal_restore_attributes(const t_term *terminal)
{
    if (tcsetattr(terminal->fd, TCSANOW, &terminal->ios) == -1)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "failed to restore terminal attributes");
    }
}

void ft_term_clear_line_and_under(const t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_CLEAR_LINE_AND_UNDER];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_clear_cursor_and_under(const t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_CLEAR_FROM_CURSOR_AND_UNDER];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_move_cursor(const t_term *terminal, e_cursormove move)
{
    int   column = 0;
    int   line   = 0;
    char *cap    = NULL;

    switch (move)
    {
        case MOVE_CURSOR_START:
        {
            column = terminal->start.column;
            line   = terminal->start.line;
            break;
        }
        case MOVE_CURSOR_CURRENT:
        {
            column = terminal->current.column;
            line   = terminal->current.line;
            break;
        }
        case MOVE_CURSOR_END:
        {
            column = terminal->end.column;
            line   = terminal->end.line;
            break;
        }
    }
    cap = tgoto(terminal->capabilities[TERMCAP_MOVE_CURSOR], column, line);
    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_remove_char(const t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_REMOVE_CHARACTER];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_insert_mode_off(const t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_INSERT_MODE_OFF];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_insert_mode_on(const t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_INSERT_MODE_ON];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_clear_modes(const t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_ALL_MODE_OFF];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_highlight_mode_on(const t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_HIGHLIGHT_MODE_ON];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_move_cursor_down(const t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_MOVE_CURSOR_DOWN];

    if (cap != NULL)
    {
        write(STDOUT_FILENO, cap, ft_strlen(cap));
    }
    else
    {
        write(STDOUT_FILENO, "\r\n", 2);
    }
}

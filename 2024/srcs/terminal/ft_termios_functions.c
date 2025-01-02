/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_termios_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 12:00:56 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:09:42 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_builtins.h"
#include "ft_log.h"
#include "ft_termios.h"
#include "libft.h"
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/ttycom.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>

void ft_init_shell_terminal(t_term *terminal)
{
    terminal->fd      = STDIN_FILENO;
    terminal->desc[0] = '\0';
    ft_get_terminal_size(terminal);
    terminal->current_column = 0;
    terminal->current_line   = 0;
    terminal->start_column   = 0;
    terminal->start_line     = 0;
}

void ft_clear_shell_terminal(t_term *terminal, uint8_t restore_attr)
{
    if (restore_attr != 0)
    {
        ft_restore_terminal_attributes(terminal);
    }
    if (terminal->fd > 0)
    {
        close(terminal->fd);
    }
}

void ft_get_terminal_size(t_term *terminal)
{
    struct winsize winsize;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
    terminal->max_column = winsize.ws_col - 1;
    terminal->max_line   = winsize.ws_row - 1;
}

void ft_get_cursor_position(t_term *terminal)
{
    char buff[16] = { 0 };

    write(STDOUT_FILENO, "\033[6n", 4);
    read(STDOUT_FILENO, buff, sizeof(buff));
    if (buff[0] != '\033' || buff[1] != '[' || ft_strchr(buff, ';') == NULL)
    {
        return;
    }
    terminal->current_line   = ft_atoi(buff + 2) - 1;
    terminal->current_column = ft_atoi(ft_strchr(buff + 2, ';') + 1) - 1;
    terminal->start_line     = terminal->current_line;
    terminal->start_column   = terminal->current_column;
}

/**
 * @brief Récupère les valeurs de certain Termcaps.
 * @param terminal Structure interne Terminal
 */
static void ft_get_termcaps(const char ** const capabilities)
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
    unsigned long iter = 0;

#pragma unroll(sizeof(caps) / sizeof(caps[0]))
    while (iter < sizeof(caps) / sizeof(caps[0]))
    {
        capabilities[iter] = NULL;
        capabilities[iter] = tgetstr(caps[iter], NULL);
        if (capabilities[iter] == NULL && iter != TERMCAP_MOVE_CURSOR_DOWN)
        {
            ft_log(SH_LOG_LEVEL_FATAL, "termcaps: capability %s not available.", caps[iter]);
        }
        iter++;
    }
}

void ft_load_termcaps(t_term *terminal, void *shell_ctx)
{
    char       *tty_name = NULL;
    const char *term     = NULL;
    int         ret      = 0;

    if (isatty(STDIN_FILENO) == 0)
    {
        return ft_log(SH_LOG_LEVEL_FATAL, "termcaps: Terminal not valid.");
    }
    tty_name = ttyname(STDIN_FILENO);
    if (tty_name == NULL)
    {
        return ft_log(SH_LOG_LEVEL_FATAL, "termcaps: Not connected to a terminal->");
    }
    terminal->fd = open(tty_name, O_RDWR | O_CLOEXEC);
    if (terminal->fd == -1)
    {
        return ft_log(SH_LOG_LEVEL_FATAL, "termcaps: Failed to open tty \"%s\".", tty_name);
    }
    if (tcgetattr(terminal->fd, &terminal->ios) != 0)
    {
        return (ft_log(SH_LOG_LEVEL_FATAL, "termcaps: Can't get terminal attributes."));
    }
    term = ft_getenv("TERM", shell_ctx);
    if (term == NULL)
    {
        return ft_log(SH_LOG_LEVEL_FATAL, "termcaps: Specify a terminal type in your environment.");
    }
    ret = tgetent(terminal->desc, term);
    if (ret <= 0)
    {
        return ft_log(SH_LOG_LEVEL_FATAL,
                      "termcaps: %s.",
                      (ret == 0) ? "Terminal not defined in database" : "Termcap's data base files unavailable");
    }
    ft_get_termcaps((const char ** const) terminal->capabilities);
}

int ft_change_terminal_attributes(t_term *terminal)
{
    struct termios termios;

    termios = terminal->ios;
    termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    termios.c_cc[VMIN]  = 1;
    termios.c_cc[VTIME] = 0;
    if ((tcsetattr(terminal->fd, TCSANOW, &termios)) == -1)
    {
        ft_log(SH_LOG_LEVEL_FATAL, "failed to load terminal attributes");
        return (1);
    }
    return (0);
}

void ft_restore_terminal_attributes(t_term *terminal)
{
    if (tcsetattr(terminal->fd, TCSANOW, &terminal->ios) == -1)
    {
        ft_log(SH_LOG_LEVEL_FATAL, "failed to restore terminal attributes");
    }
}

void ft_term_clear_line_and_under(t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_CLEAR_LINE_AND_UNDER];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_clear_cursor_and_under(t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_CLEAR_FROM_CURSOR_AND_UNDER];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_move_cursor(t_term *terminal, int column, int line)
{
    char *cap = tgoto(terminal->capabilities[TERMCAP_MOVE_CURSOR], column, line);

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_remove_char(t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_REMOVE_CHARACTER];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_insert_mode_off(t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_INSERT_MODE_OFF];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_insert_mode_on(t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_INSERT_MODE_ON];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_move_cursor_left(t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_MOVE_CURSOR_LEFT];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_clear_modes(t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_ALL_MODE_OFF];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_highlight_mode_on(t_term *terminal)
{
    char *cap = terminal->capabilities[TERMCAP_HIGHLIGHT_MODE_ON];

    write(STDOUT_FILENO, cap, ft_strlen(cap));
}

void ft_term_move_cursor_down(t_term *terminal)
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

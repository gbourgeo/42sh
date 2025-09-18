/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_prompt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell_prompt.h"
#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_constants.h"
#include "ft_shell_terminal.h"
#include "libft.h"
#include <stddef.h>

typedef int (*promt_option_handler)(char *, size_t *, const char *);

typedef struct _align(16) s_prompt_options
{
    const char           value;
    promt_option_handler handler;

} t_poptions;

typedef struct _align(16) s_prompt_formats

{
    const char *name;
    const char *format;
} t_pformats;

static int ft_shell_prompt_handle_format(char *prompt, size_t *promptlen, const char *psone, size_t *psonepos)
{
    size_t prompt_len = *promptlen;
    size_t pos        = *psonepos;
    int    ret        = 0;

    pos++;
    if (psone[pos] == '{')
    {
        /* Formats supportés */
        static t_pformats formats[] = {
            // Modes
            { .name = "reset",             .format = "0"   }, // Désactive tous les attributs
            { .name = "normal",            .format = "0"   }, // Désactive tous les attributs
            { .name = "bold",              .format = "1"   }, // Augmentte l'intensité de l'affichage
            { .name = "faint",             .format = "2"   }, // Réduit l'intensité de l'affichage
            { .name = "dim",               .format = "2"   }, // Réduit l'intensité de l'affichage
            { .name = "italic",            .format = "3"   }, // Peu supporté. Parfois traitécomme 'inverse' ou 'blink'
            { .name = "underon",           .format = "4"   }, // Active le soulignement
            { .name = "sblink",            .format = "5"   }, // Slow blink. Moins de 150 fois par minute
            { .name = "rblink",            .format = "6"   }, // Rapid blink. Plus de 150 fois par minute
            { .name = "invert",            .format = "7"   }, // Interverti l'avant-plan et l'arrière-plan
            { .name = "underoff",          .format = "24"  }, // Désactive le soulignement
            { .name = "blinkoff",          .format = "25"  }, // Désactive le blink
            // Couleurs du texte
            { .name = "black",             .format = "30"  }, // Couleur : noir
            { .name = "red",               .format = "31"  }, // Couleur : rouge
            { .name = "green",             .format = "32"  }, // Couleur : vert
            { .name = "yellow",            .format = "33"  }, // Couleur : jaune
            { .name = "blue",              .format = "34"  }, // Couleur : bleu
            { .name = "magenta",           .format = "35"  }, // Couleur : magenta
            { .name = "cyan",              .format = "36"  }, // Couleur : cyan
            { .name = "white",             .format = "37"  }, // Couleur : blanc
            { .name = "default",           .format = "39"  }, // Couleur : par defaut
            { .name = "grey",              .format = "90"  }, // Couleur : noir éclairé (gris)
            { .name = "brigth-red",        .format = "91"  }, // Couleur : rouge éclairé
            { .name = "bright-green",      .format = "92"  }, // Couleur : vert éclairé
            { .name = "bright-yellow",     .format = "93"  }, // Couleur : jaune éclairé
            { .name = "bright-blue",       .format = "94"  }, // Couleur : bleu éclairé
            { .name = "bright-magenta",    .format = "95"  }, // Couleur : magenta éclairé
            { .name = "bright-cyan",       .format = "96"  }, // Couleur : cyan éclairé
            { .name = "bright-white",      .format = "97"  }, // Couleur : blanc éclairé
            // Couleurs de l'arrière-plan
            { .name = "bg-black",          .format = "40"  }, // Couleur : noir
            { .name = "bg-red",            .format = "41"  }, // Couleur : rouge
            { .name = "bg-green",          .format = "42"  }, // Couleur : vert
            { .name = "bg-yellow",         .format = "43"  }, // Couleur : jaune
            { .name = "bg-blue",           .format = "44"  }, // Couleur : bleu
            { .name = "bg-magenta",        .format = "45"  }, // Couleur : magenta
            { .name = "bg-cyan",           .format = "46"  }, // Couleur : cyan
            { .name = "bg-white",          .format = "47"  }, // Couleur : blanc
            { .name = "bg-default",        .format = "49"  }, // Couleur : par défaut
            { .name = "bg-grey",           .format = "100" }, // Couleur : noir éclairé (gris)
            { .name = "bg-brigth-red",     .format = "101" }, // Couleur : rouge éclairé
            { .name = "bg-bright-green",   .format = "102" }, // Couleur : vert éclairé
            { .name = "bg-bright-yellow",  .format = "103" }, // Couleur : jaune éclairé
            { .name = "bg-bright-blue",    .format = "104" }, // Couleur : bleu éclairé
            { .name = "bg-bright-magenta", .format = "105" }, // Couleur : magenta éclairé
            { .name = "bg-bright-cyan",    .format = "106" }, // Couleur : cyan éclairé
            { .name = "bg-bright-white",   .format = "107" }, // Couleur : blanc éclairé
        };
        size_t start = pos + 1;
        size_t iter  = 0;
        int    first = 0;

        ft_strncpy(prompt + prompt_len, "\033[", ft_strlen("\033["));
        prompt_len += ft_strlen("\033[");

        while (psone[pos] != '\0'
               && ret == 0)
        {
            pos++;
            if (psone[pos] == ',' || psone[pos] == '}')
            {
                iter = 0;
                while (iter < LENGTH_OF(formats))
                {
                    if (ft_strncmp(formats[iter].name, psone + start, start - pos) == 0)
                    {
                        if (first != 0)
                        {
                            if (prompt_len + 1 >= SHELL_PROMPT_MAX_LENGTH - 1)
                            {
                                ret = 1;
                                break;
                            }
                            ft_strcpy(prompt + prompt_len, ";");
                            prompt_len += 1;
                        }
                        first = 1;
                        if (prompt_len + ft_strlen(formats[iter].format) >= SHELL_PROMPT_MAX_LENGTH - 1)
                        {
                            ret = 1;
                            break;
                        }
                        ft_strcpy(prompt + prompt_len, formats[iter].format);
                        prompt_len += ft_strlen(formats[iter].format);
                        break;
                    }
                    iter++;
                }
                if (psone[pos] == '}')
                {
                    break;
                }
                start = pos + 1;
            }
        }
    }
    ft_strncpy(prompt + prompt_len, "m", 1);
    prompt_len += 1;
    *promptlen = prompt_len;
    *psonepos  = pos;

    return (ret);
}

static int ft_shell_prompt_handle_str(char *prompt, size_t *prompt_len, const char *str)
{
    if (*prompt_len + ft_strlen(str) > SHELL_PROMPT_MAX_LENGTH)
    {
        // Message d'erreur ?
        return (1);
    }
    ft_strcpy(prompt + *prompt_len, str);
    *prompt_len += ft_strlen(str);
    return (0);
}

void ft_shell_prompt_create(t_prompt *prompt,
                            void     *shell_ctx)
{
    t_shell    *shell                               = NULL;
    const char *psone                               = NULL;
    char        prompt_str[SHELL_PROMPT_MAX_LENGTH] = { 0 };
    size_t      prompt_len                          = 0;
    size_t      prompt_printed_len                  = 0;
    size_t      pos                                 = 0;
    size_t      last_pos                            = 0;

    shell = (t_shell *) shell_ctx;
    ft_freestr(&prompt->str);
    prompt->real_len    = 0;
    prompt->printed_len = 0;
    prompt->print       = 1;

    psone = ft_getenv("PS1", shell);
    if (psone == NULL)
    {
        return;
    }

    while (psone[pos] != '\0')
    {
        if (psone[pos] == '%')
        {
            if (pos > last_pos)
            {
                ft_strncpy(prompt_str + prompt_len, psone + last_pos, pos - last_pos);
                prompt_len += (pos - last_pos);
                prompt_printed_len += (pos - last_pos);
            }
            pos++;
            if (psone[pos] == 'F') // Format
            {
                if (ft_shell_prompt_handle_format(prompt_str, &prompt_len, psone, &pos) != 0)
                {
                    // Message d'erreur ?
                    break;
                }
            }
            else if (psone[pos] == 'p') // Nom du programme
            {
                if (ft_shell_prompt_handle_str(prompt_str, &prompt_len, shell->progname) != 0)
                {
                    // Message d'erreur ?
                    break;
                }
                prompt_printed_len += ft_strlen(shell->progname);
            }
            else if (psone[pos] == 'u') // Nom d'utilisateur
            {
                const char *user = ft_getenv("USER", shell);

                if (ft_shell_prompt_handle_str(prompt_str, &prompt_len, user) != 0)
                {
                    // Message d'erreur ?
                    break;
                }
                prompt_printed_len += ft_strlen(user);
            }
            else if (psone[pos] == 'w') // Répertoire courant
            {
                const char *pwd  = ft_getenv("PWD", shell);
                const char *home = ft_getenv("HOME", shell);

                if (ft_strncmp(pwd, home, ft_strlen(home)) == 0)
                {
                    if (ft_shell_prompt_handle_str(prompt_str, &prompt_len, "~") != 0)
                    {
                        // Message d'erreur ?
                        break;
                    }
                    prompt_printed_len += 1;
                    pwd += ft_strlen(home);
                }
                if (ft_shell_prompt_handle_str(prompt_str, &prompt_len, pwd) != 0)
                {
                    // Message d'erreur ?
                    break;
                }
                prompt_printed_len += ft_strlen(pwd);
            }
            last_pos = pos + 1;
        }
        pos++;
    }
    if (pos > last_pos)
    {
        ft_strncpy(prompt_str + prompt_len, psone + last_pos, pos - last_pos);
        prompt_len += (pos - last_pos);
    }
    prompt->str         = ft_strdup(prompt_str);
    prompt->real_len    = prompt_len;
    prompt->printed_len = prompt_printed_len;
    prompt->print       = 1;
}

void ft_shell_prompt_print(t_prompt *prompt, t_term *terminal)
{
    if (prompt->print == 1)
    {
        prompt->print = 0;
        /* Efface depuis le curseur jusqu'à la fin du terminal */
        ft_term_clear_line_and_under(terminal);
        ft_putstr(prompt->str);
        /* Récupère la position du curseur */
        ft_shell_terminal_get_cursor_position(terminal,
                                              UINT32(MOVE_CURSOR_START)
                                                  | UINT32(MOVE_CURSOR_CURRENT)
                                                  | UINT32(MOVE_CURSOR_END));
    }
}

void ft_shell_prompt_init(t_prompt *prompt)
{
    prompt->str         = NULL;
    prompt->real_len    = 0;
    prompt->printed_len = 0;
    prompt->print       = 0;
}

void ft_shell_prompt_clear(t_prompt *prompt)
{
    ft_freestr(&prompt->str);
}

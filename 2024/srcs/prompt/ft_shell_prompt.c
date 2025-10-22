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

#include "ft_defines.h"
#include "ft_shell_environ.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include "libft.h"
#include <stddef.h>

typedef int (*promt_option_handler)(t_prompt    *,
                                    const char  *,
                                    size_t      *,
                                    const t_env *);

typedef struct _align(16) s_prompt_options
{
    const char           value;
    promt_option_handler handler;
} t_poptions;

/**
* @brief Formats supportés par l'option '%F{...}'
 */
typedef struct _align(16) s_prompt_formats
{
    const char *name;   /* Nom du format */
    const char *format; /* Valeur d'échappement */
} t_pformats;

/************************************/
/* FONCTIONS PRIVEES                */
/************************************/

static int internal_shell_prompt_handle_format(t_prompt *prompt, const char *psone, size_t *pos, const t_env *environ);
static int internal_shell_prompt_handle_progname(t_prompt *prompt, const char *psone, size_t *pos, const t_env *environ);
static int internal_shell_prompt_handle_username(t_prompt *prompt, const char *psone, size_t *pos, const t_env *environ);
static int internal_shell_prompt_handle_curdir(t_prompt *prompt, const char *psone, size_t *pos, const t_env *environ);
static int internal_shell_prompt_handle_str(t_prompt *prompt, const char *str);

/************************************/
/* FONCTIONS PUBLIQUES              */
/************************************/

void ft_shell_prompt_create(t_prompt *prompt, const t_env *environ)
{
    t_poptions options[] = {
        { .value = 'F', .handler = internal_shell_prompt_handle_format },   // Format
        { .value = 'p', .handler = internal_shell_prompt_handle_progname }, // Nom du programme
        { .value = 'u', .handler = internal_shell_prompt_handle_username }, // Nom d'utilisateur
        { .value = 'w', .handler = internal_shell_prompt_handle_curdir },   // Répertoire courant
    };
    const char *psone    = ft_shell_env_get_value("PS1", environ);
    size_t      pos      = 0;
    size_t      last_pos = 0;
    int         ret      = 0;

    ft_shell_prompt_init(prompt);
    prompt->doprint = 1;
    if (psone == NULL)
    {
        psone = SHELL_PROMPT_DEFAULT_STR;
    }
    while (psone[pos] != '\0' && ret == 0)
    {
        if (psone[pos] == '%')
        {
            if (pos > last_pos)
            {
                ft_strncpy(prompt->str + prompt->real_len,
                           psone + last_pos,
                           pos - last_pos);
                prompt->real_len    += (pos - last_pos);
                prompt->printed_len += (pos - last_pos);
            }
            pos++;
            size_t iter = 0;

            while (iter < _length_of(options))
            {
                if (psone[pos] == options[iter].value)
                {
                    ret = options[iter].handler(prompt, psone, &pos, environ);
                    break;
                }
                iter++;
            }
            last_pos = pos + 1;
        }
        pos++;
    }
    if (pos > last_pos)
    {
        ft_strncpy(prompt->str + prompt->real_len, psone + last_pos, pos - last_pos);
        prompt->real_len += (pos - last_pos);
    }
}

void ft_shell_prompt_init(t_prompt *prompt)
{
    prompt->str[0]      = '\0';
    prompt->real_len    = 0;
    prompt->printed_len = 0;
    prompt->doprint     = 0;
}

void ft_shell_prompt_print(t_prompt *prompt, t_term *terminal)
{
    if (prompt->doprint == 1)
    {
        prompt->doprint = 0;
        /* Efface depuis le curseur jusqu'à la fin du terminal */
        ft_term_clear_line_and_under(terminal);
        ft_putstr(prompt->str);
        /* Récupère la position du curseur */
        ft_shell_terminal_get_cursor_position(terminal, SET_CURSOR_ALL);
    }
}


static int internal_shell_prompt_handle_format(
    t_prompt    *prompt,
    const char  *psone,
    size_t      *pos,
    const t_env *environ _unused)
{
    int ret = 0;

    (*pos)++;
    if (psone[*pos] == '{')
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
        size_t start = *pos + 1;
        size_t iter  = 0;
        int    first = 0;

        ft_strncpy(prompt->str + prompt->real_len, "\033[", ft_strlen("\033["));
        prompt->real_len += ft_strlen("\033[");

        while (psone[*pos] != '\0'
               && ret == 0)
        {
            (*pos)++;
            if (psone[*pos] == ',' || psone[*pos] == '}')
            {
                iter = 0;
                while (iter < _length_of(formats))
                {
                    if (ft_strncmp(formats[iter].name, psone + start, start - *pos) == 0)
                    {
                        if (first != 0)
                        {
                            if (prompt->real_len + 1 >= SHELL_PROMPT_MAX_LENGTH - 1)
                            {
                                ret = 1;
                                break;
                            }
                            ft_strcpy(prompt->str + prompt->real_len, ";");
                            prompt->real_len += 1;
                        }
                        first = 1;
                        if (prompt->real_len + ft_strlen(formats[iter].format) >= SHELL_PROMPT_MAX_LENGTH - 1)
                        {
                            ret = 1;
                            break;
                        }
                        ft_strcpy(prompt->str + prompt->real_len, formats[iter].format);
                        prompt->real_len += ft_strlen(formats[iter].format);
                        break;
                    }
                    iter++;
                }
                if (psone[*pos] == '}')
                {
                    break;
                }
                start = *pos + 1;
            }
        }
    }
    ft_strncpy(prompt->str + prompt->real_len, "m", 1);
    prompt->real_len += 1;
    return (ret);
}

static int internal_shell_prompt_handle_progname(
    t_prompt    *prompt,
    const char  *psone _unused,
    size_t      *pos _unused,
    const t_env *environ)
{
    const char *progname = ft_shell_env_get_value("SHELL", environ);
    int         ret      = 0;

    ret = internal_shell_prompt_handle_str(prompt, progname);
    return (ret);
}

static int internal_shell_prompt_handle_username(
    t_prompt    *prompt,
    const char  *psone _unused,
    size_t      *pos _unused,
    const t_env *environ)
{
    const char *user = ft_shell_env_get_value("USER", environ);
    int         ret  = 0;

    ret = internal_shell_prompt_handle_str(prompt, user);
    return (ret);
}

static int internal_shell_prompt_handle_curdir(
    t_prompt    *prompt,
    const char  *psone _unused,
    size_t      *pos _unused,
    const t_env *environ)
{
    const char *pwd  = ft_shell_env_get_value("PWD", environ);
    const char *home = ft_shell_env_get_value("HOME", environ);
    int         ret  = 0;

    if (ft_strncmp(pwd, home, ft_strlen(home)) == 0)
    {
        ret = internal_shell_prompt_handle_str(prompt, "~");
        pwd += ft_strlen(home);
    }
    ret += internal_shell_prompt_handle_str(prompt, pwd);
    return (ret);
}

static int internal_shell_prompt_handle_str(t_prompt *prompt, const char *str)
{
    if (prompt->real_len + ft_strlen(str) > SHELL_PROMPT_MAX_LENGTH)
    {
        // Message d'erreur ?
        return (1);
    }
    ft_strcpy(prompt->str + prompt->real_len, str);
    prompt->real_len += ft_strlen(str);
    prompt->printed_len += ft_strlen(str);
    return (0);
}

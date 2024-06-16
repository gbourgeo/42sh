/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <term.h>
# include <unistd.h>

# include "libft.h"

# include "ft_builtins.h"
# include "ft_command.h"
# include "ft_highlight.h"
# include "ft_history.h"
# include "ft_token.h"
# include "libft2.h"
# include "redirections.h"

# define SEPARATORS                 "&|;"
# define PIPES                      "<>|"
# define DOUBLE_OR                  1
# define DOUBLE_AND                 2
# define PIPE_LEFT                  3
# define PIPE_RIGHT                 4
# define PIPE_RIGHT_2               5
# define PIPE_PIPE                  6
# define PRINTABLE_KEYS(i, len)     ( buf[i] >= 32 && buf[i] <= 126 )
# define CMD_SIZE                   10

enum
{
    SHELL_TERMCAPS_LOADED = 1,
    SHELL_DEBUG_MODE      = 2,
};

typedef struct      s_termcap
{
    struct termios  ios;            /* Caractéristiques du terminal à restorer en fin de programme */
    int             max_column;     /* Nombre de colonne max du terminal */
    int             max_line;       /* Nombre de ligne max du terminal */
    int             current_column; /* Colonne de la position courante curseur */
    int             current_line;   /* Ligne de la position courante curseur */
    int             start_column;   /* Colonne du début de la commande */
    int             start_line;     /* Ligne du début de la commande */
}                   t_term;

typedef struct      s_prompt
{
    char            *str;           /* Prompt string */
    size_t          len;            /* Longueur du prompt */
    char            print;          /* Afficher le prompt ? */
}                   t_pmpt;

typedef struct      s_shell
{
    int             options;        /* Options du shell */
    int             fd;             /* Entrée de lecture du programme */
    int             quit;           /* Valeur définissant si le programme doit se terminer */
    int             status;         /* Valeur de sortie de la dernière commande exécutée */
    const char      *progname;      /* Nom du programme */
    const char      *bin_path;      /* Chemins absolu des binaires */
    char * const    *env;           /* Variables d'environement */
    t_pmpt          prompt;         /* Structure du prompt */
    t_cmd           command;        /* Commande actuelle */
    t_hist          *history;       /* Historique des commandes */
    t_high          highlighted;    /* Texte surligné (pour la copie) */
    t_term          terminal;       /* Données du terminal */
    __sighandler_t  sigs[_NSIG];    /* Tableau de sauvegarde des handlers de signal */
}                   t_shell;

int                 main(int ac, const char **av);
void                debug(char *buf, t_shell *shell);
void                ft_init_shell(const char **av, char * const *environ, t_shell *e);
const char          **ft_get_opt(const char **av, char * const *environ, t_shell *shell);
void                ft_free_shell(t_shell *e);
void                ft_signals(t_shell *e);
void                ft_prompt(t_shell *e);
void                ft_get_terminal_size(t_shell *e);
void                ft_get_cursor_position(t_shell *shell);
int                 check_and_exec(const char **args, t_shell *e);
int                 fork_function(const char **args, t_shell *e);
void                ft_error_fatal(t_shell *e, const char *err_str, ...);
void                ft_error(t_shell *e, const char *err_str, ...);
char                *ft_get_path(char *pwd);

/*
** TERMIOS
*/

void                ft_load_termcaps(t_shell *e);
void                ft_restore_termcaps(t_shell *e);
void                ft_tputs(const char *t, t_shell *e);
void                ft_tgoto(int column, int line, t_shell *e);

/*
** FREE
*/

void                ft_free_list(t_args **list);

/*
** PARSER
*/

void                ft_parse_command(t_shell *e);
char                *ft_lexical_analysis(char *command);
int                 ft_syntax_analysis(char **args, t_shell *e);
t_args              *ft_argsnew(char **args, int type);

/*
** PIPEX
*/

void                ft_pipex_or_exec(t_args *list, t_shell *e);
void                ft_pipe_pipe(t_args *pipes, t_shell *e);
void                ft_pipe_right(t_args *pipes, t_shell *e);
void                ft_pipe_right_2(t_args *pipes, t_shell *e);
void                ft_pipe_left(t_args *pipes, t_shell *e);

#endif

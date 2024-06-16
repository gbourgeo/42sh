#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/12/28 02:12:40 by gbourgeo          #+#    #+#              #
#    Updated: 2018/04/06 19:02:35 by gbourgeo         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME    = 42sh

SRC_DIR = srcs/
SRC     = debug.c               \
        ft_command.c            \
        ft_error.c              \
        ft_fork.c               \
        ft_free_shell.c         \
        ft_frees.c              \
        ft_get_path.c           \
        ft_history.c            \
        ft_init_shell.c         \
        ft_minishell.c          \
        ft_prompt.c             \
        ft_signals.c            \
        ft_terminal_size.c      \
        check_and_exec.c        \
        termios_functions.c

BUILTINS_DIR    = builtins/
BUILTINS    = ft_cd.c           \
            ft_cd_check.c       \
            ft_cd_change_pwds.c \
            ft_echo.c           \
            ft_env.c            \
            ft_exit.c           \
            ft_getcwd.c         \
            ft_getenv.c         \
            ft_setenv.c         \
            ft_unsetenv.c

LIB2_DIR    = libft2/
LIB2    = ft_deletechar.c       \
        ft_dup2.c               \
        ft_realloc.c

REDIREC_DIR = redirections/
REDIREC = ft_argsnew.c          \
        ft_lexical_analysis.c   \
        ft_parser.c             \
        ft_tokeniser.c          \
        ft_pipes_errors.c       \
        ft_separators_errors.c  \
        ft_syntax_analysis.c

PIPEX_DIR   = pipex/
PIPEX   = ft_pipe_left.c        \
        ft_pipe_pipe.c          \
        ft_pipe_rights.c        \
        ft_pipex_or_exec.c

KEY_DIR = keyboard/
KEYS    = ft_key_control.c      \
        ft_key_highlight.c      \
        ft_key_history.c        \
        ft_key_move.c           \
        ft_key_print.c          \
        ft_key_read.c           \
        ft_key_suppr.c
# ft_key_shift.c        \

OBJ_DIR = obj/
OBJ     = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
OBJ_BUI = $(addprefix $(OBJ_DIR), $(BUILTINS:.c=.o))
OBJ_LIB = $(addprefix $(OBJ_DIR), $(LIB2:.c=.o))
OBJ_RED = $(addprefix $(OBJ_DIR), $(REDIREC:.c=.o))
OBJ_PIP = $(addprefix $(OBJ_DIR), $(PIPEX:.c=.o))
OBJ_KEY = $(addprefix $(OBJ_DIR), $(KEYS:.c=.o))

FLAGS    = -Wall -Werror -Wextra

LIBFT    = libft/

INCLUDES = -I includes -I $(LIBFT)/includes -I /usr/include

LIBRAIRIES = -L$(LIBFT) -lft -ltermcap

.PHONY: debugging profiling coverage libft shell clean fclean re

all: debugging profiling coverage libft shell $(NAME)

debugging:
ifdef debug
	@echo "Debugging mode enabled !"
FLAGS += -g3
endif

profiling:
ifdef prof
	@echo "Profiling mode enabled !"
FLAGS += -g -pg
LIBRAIRIES += -pg
endif

coverage:
ifdef cov
	@echo "Coverage mode enabled !" $(GCOV_CFLAGS) " - " $(GCOV_LIBS)
FLAGS += -O0 -g --coverage
LIBRAIRIES += --coverage
endif

libft:
	@echo '[Compilation de la libFT]'
	@make -sC $(LIBFT)

shell:
	@echo '[Compilation du 42Sh]'
	@echo -n "Compiling "
	@mkdir -p obj

$(NAME): $(OBJ) $(OBJ_BUI) $(OBJ_LIB) $(OBJ_RED) $(OBJ_PIP) $(OBJ_KEY)
	@gcc -o $@ $^ $(LIBRAIRIES)
	@echo " $@"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo -n .
	@gcc $(FLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(BUILTINS_DIR)%.c
	@echo -n .
	@gcc $(FLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(LIB2_DIR)%.c
	@echo -n .
	@gcc $(FLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(REDIREC_DIR)%.c
	@echo -n .
	@gcc $(FLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(PIPEX_DIR)%.c
	@echo -n .
	@gcc $(FLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(KEY_DIR)%.c
	@echo -n .
	@gcc $(FLAGS) -o $@ -c $< $(INCLUDES)

clean:
	@make -sC $(LIBFT) clean
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	@make -sC $(LIBFT) fclean
	/bin/rm -rf $(NAME)

re: fclean all

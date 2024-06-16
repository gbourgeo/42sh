/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TOKEN_H
# define FT_TOKEN_H

typedef enum    e_token_type
{
    TOKEN = 0,
    IO_NUMBER,
}               e_token;

typedef struct  s_token
{
    e_token         type;
    char            *ptr;
    size_t          len;
    struct s_token  *next;
}               t_token;

typedef struct      s_args
{
    int             type;
    int             pipe;
    char            **args;
    struct s_args   *next;
}                   t_args;

t_token     *ft_tokenise_command(const char *command, char * const * env);
void        ft_free_token_list(t_token *token_list);

#endif /* FT_TOKEN_H */

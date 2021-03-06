/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/14 23:38:28 by gbourgeo          #+#    #+#             */
/*   Updated: 2019/04/29 21:34:54 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "token_error.h"
#include "shell.h"

static char		*get_new_prompt(t_quote *quote)
{
	static char	*prompt[] = {
		DEFAULT_PROMPT, BACKSLASH_PROMPT, DQUOTE_PROMPT, SQUOTE_PROMPT,
		BRACE_PROMPT, D_BRACE_PROMPT, PARENTHESE_PROMPT, D_PARENTHESE_PROMPT,
		BACKQUOTE_PROMPT, HERE_DOC_PROMPT,
	};
	char		*ret;
	int			i;

	ret = NULL;
	while (quote)
	{
		if (ret && quote->type != BACKSLASH
		&& !(ret = ft_strjoinfree(ret, " ", 1)))
			return (NULL);
		i = quote->type;
		if (!(ret = (ret)
			? ft_strjoinfree(ret, prompt[i], 1) : ft_strdup(prompt[i])))
			return (NULL);
		quote = quote->next;
	}
	ret = (ret) ? ft_strjoinfree(ret, prompt[0], 1) : ft_strdup(prompt[0]);
	return (ret);
}

static int		get_new_line(void *quote, int type, t_line *line)
{
	char		*promptsave;

	promptsave = line->prompt;
	init_new_buff(line);
	if (!(line->prompt = get_new_prompt((t_quote *)quote)))
		return (ERR_MALLOC);
	line->lprompt = ft_strlen(line->prompt);
	line->curr->quoted = 1;
	line->tmp[0] = 0;
	put_prompt(line->prompt, *line->ret);
	while (line->shell_loop && line->tmp[0] == 0)
		deal_typing(line);
	if (line->tmp[0] != 4)
		write(STDIN_FILENO, "\n", 1);
	line->shell_loop = 1;
	line->curr->quoted = 0;
	ft_strdel(&line->prompt);
	line->prompt = promptsave;
	line->lprompt = ft_strlen(line->prompt);
	if (line->tmp[0] == -1)
		return (ERR_FREE_ALL);
	if (line->tmp[0] == 4 && quote_type(quote) != BACKSLASH)
		return ((type == HEREDOCUMENT) ? ERR_HEREDOC_EOF : ERR_MATCHING_EOF);
	return (ERR_NONE);
}

static int		tokenise_quote(t_param *param, void *quote, t_line *line)
{
	static int	(*handler[])(t_param *, t_line *) = {
		aliased_line, bslashed_line, dquoted_line, squoted_line, braced_line,
		dbraced_line, parenthed_line, dparenthed_line, backquoted_line,
		heredoc_line,
	};
	char		*old;
	int			type;
	int			error;

	old = (char *)param->line;
	type = quote_type((t_quote *)quote);
	if ((error = handler[type](param, line)))
		return (error);
	if (type != HEREDOCUMENT && (param->token = param->head))
		while (param->token)
		{
			if (param->token->oldhd)
				param->token->oldhd = param->line + (param->token->oldhd - old);
			else
				param->token->head = param->line + (param->token->head - old);
			if (!param->token->next)
				break ;
			param->token = param->token->next;
		}
	return (ERR_NONE);
}

t_token			*quote_line(t_param *param)
{
	t_line		*line;
	int			type;
	int			error;

	line = get_struct();
	type = quote_type(param->token->quote);
	if (type != NO_QUOTE)
	{
		if ((error = get_new_line(param->token->quote, type, line)) != ERR_NONE
		|| (error = tokenise_quote(param, param->token->quote, line)))
			return (token_error(error, param));
	}
	else if (param->hdoc)
	{
		if ((error = get_new_line(param->hdoc, HEREDOCUMENT, line)) != ERR_NONE
		|| (error = tokenise_quote(param, param->hdoc, line)) != ERR_NONE)
			return (token_error(error, param));
	}
	return (param->token);
}

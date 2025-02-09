/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:35:00 by ldedier           #+#    #+#             */
/*   Updated: 2019/09/30 17:10:56 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_arg_option g_opts[] =
{
	{"no-sort", 'p', nm_opt_p, "Don't sort; display in symbol-table order"},
	{"numeric-sort", 'n', nm_opt_n,
		"Sort numerically rather than alphabetically"},
	{"reverse-sort", 'r', nm_opt_r, "Sort in reverse order"},
	{"undefined-only", 'u', nm_opt_u, "Display only undefined symbols"}
};

/*
** dump obj filename by creating a list of the parsed architectures
** in fill_browser, then printing it one by one in nm_print
*/

int		process_nm(char *filename, t_browser *browser)
{
	t_header_parser	parser;

	if (init_browser(browser, filename))
		return (1);
	init_parser(&parser, browser->ptr, 0, filename);
	if (fill_browser(&parser, browser, 1))
	{
		if (free_browser(browser))
		{
			ft_tree_del(&browser->parsers, free_parser_tree);
			return (1);
		}
		ft_tree_del(&browser->parsers, free_parser_tree);
		return (1);
	}
	nm_print(browser, 0);
	ft_tree_del(&browser->parsers, free_parser_tree);
	if (free_browser(browser))
		return (1);
	return (0);
}

int		process_args(t_arg_parser *parser, t_browser *browser)
{
	t_list			*lst;
	t_arg_parsed	*test;
	int				ret;

	ret = 0;
	browser->nb_args = parser->nb_args;
	lst = parser->parsed;
	while (lst)
	{
		test = (t_arg_parsed*)lst->content;
		if (test->type == E_ARG)
		{
			if (process_nm(test->long_name, browser))
				ret = 1;
		}
		lst = lst->next;
	}
	return (ret);
}

void	init_wrapper(t_nm_wrapper *wrapper, t_nm_flags *flags,
			t_browser *browser)
{
	ft_bzero(flags, sizeof(flags));
	wrapper->flags = flags;
	wrapper->browser = browser;
}

int		main(int ac, char **av)
{
	t_nm_wrapper	wrapper;
	t_browser		browser;
	t_nm_flags		flags;
	t_arg_parser	parser;

	(void)ac;
	init_wrapper(&wrapper, &flags, &browser);
	init_browser_general(&browser, av[0], E_BIN_NM);
	opt_init_parser(&parser, flag_invalid, av[0]);
	opt_add_to_parser(&parser, g_opts, sizeof(g_opts));
	if (opt_parse_args(&parser, av + 1))
		return (opt_free(&parser, EXIT_FAILURE));
	process_opt(&parser, &wrapper);
	browser.reserved = &flags;
	if (browser.ret)
		return (opt_free(&parser, browser.ret));
	browser.ret |= process_args(&parser, &browser);
	if (parser.nb_args == 0)
	{
		browser.nb_args = 1;
		if (process_nm(DEFAULT_NM_FILE, &browser))
			return (opt_free(&parser, EXIT_FAILURE));
		return (opt_free(&parser, browser.ret));
	}
	return (opt_free(&parser, browser.ret));
}

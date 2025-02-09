/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:36:52 by ldedier           #+#    #+#             */
/*   Updated: 2019/09/30 18:12:08 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

t_arg_option g_opts[] =
{
	{"mach-header", 'h', flag_h, "print the mach headers"},
	{"text", 't', flag_t, "print the text section"},
	{"data", 'd', flag_d, "print the data section"}
};

/*
** dump obj filename by creating a list of the parsed architectures
** in fill_browser, then printing it one by one in nm_print
*/

int		process_otool(char *filename, t_browser *browser, t_otool_flags *flags)
{
	t_header_parser	parser;
	int				ret;

	if (init_browser(browser, filename))
		return (0);
	init_parser(&parser, browser->ptr, 0, filename);
	if ((ret = fill_browser(&parser, browser, 1)))
	{
		if (ret == ERROR_CORRUPTED)
			otool_print(browser, flags);
		ft_tree_del(&browser->parsers, free_parser_tree);
		free_browser(browser);
		return (1);
	}
	otool_print(browser, flags);
	ft_tree_del(&browser->parsers, free_parser_tree);
	if (free_browser(browser))
		return (1);
	return (0);
}

int		check_valid(t_arg_parser *parser, t_otool_flags *flags)
{
	char *shorts;

	if (parser->nb_args == 0)
	{
		print_usage(parser);
		return (1);
	}
	if (!flags->valid)
	{
		if (!(shorts = get_shorts(parser)))
			return (1);
		ft_dprintf(2, "error: %s: one of -%s must be specified\n",
				parser->prog_name, shorts);
		free(shorts);
		print_usage(parser);
		return (1);
	}
	return (0);
}

int		process_args(t_arg_parser *parser, t_otool_flags *flags,
		t_browser *browser)
{
	t_list			*lst;
	t_arg_parsed	*test;

	lst = parser->parsed;
	if (check_valid(parser, flags))
		return (1);
	while (lst)
	{
		test = (t_arg_parsed*)lst->content;
		if (test->type == E_ARG)
		{
			if (process_otool(test->long_name, browser, flags))
				return (1);
		}
		lst = lst->next;
	}
	return (0);
}

int		main(int ac, char **av)
{
	t_browser		browser;
	t_otool_flags	flags;
	t_arg_parser	parser;

	(void)ac;
	ft_bzero(&flags, sizeof(flags));
	opt_init_parser(&parser, flag_invalid, av[0]);
	opt_add_to_parser(&parser, g_opts, sizeof(g_opts));
	opt_parse_args(&parser, av + 1);
	if (process_opt(&parser, &flags))
		return (opt_free(&parser, 1));
	init_browser_general(&browser, av[0], E_BIN_OTOOL);
	if (process_args(&parser, &flags, &browser))
		return (opt_free(&parser, EXIT_FAILURE));
	return (opt_free(&parser, browser.ret));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:36:45 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/19 14:59:10 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void	flag_h(t_arg_parser *parser, void *flags)
{
	t_otool_flags *ptr;

	(void)parser;
	ptr = (t_otool_flags*)flags;
	ptr->flag_h = 1;
	ptr->valid = 1;
}

void	flag_d(t_arg_parser *parser, void *flags)
{
	t_otool_flags *ptr;

	(void)parser;
	ptr = (t_otool_flags*)flags;
	ptr->flag_d = 1;
	ptr->valid = 1;
}

void	flag_t(t_arg_parser *parser, void *flags)
{
	t_otool_flags *ptr;

	(void)parser;
	ptr = (t_otool_flags*)flags;
	ptr->flag_t = 1;
	ptr->valid = 1;
}

void	flag_invalid(t_arg_parser *parser, void *flags)
{
	t_otool_flags	*ptr;
	t_arg_parsed	*opt;

	opt = parser->current;
	ptr = (t_otool_flags*)flags;
	if (opt->type == E_OPT_LONG)
		ft_dprintf(2, "error: %s: unknown option %s\n",
		parser->prog_name, opt->long_name);
	else if (opt->type == E_OPT_SHORT)
		ft_dprintf(2, "error: %s: unknown char `%c' in flag %s\n",
		parser->prog_name, opt->short_name, opt->long_name);
	else
		ft_dprintf(2, "error: %s: unexpected error handling %s\n",
		parser->prog_name, opt->long_name);
	print_usage(parser);
}

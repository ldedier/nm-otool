/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:35:54 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/19 14:35:55 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	flag_invalid(t_arg_parser *parser, void *flags)
{
	t_nm_wrapper *ptr;

	(void)parser;
	ptr = (t_nm_wrapper*)flags;
	if (ptr->browser->ret)
		return ;
	ptr->browser->ret = 1;
	ft_dprintf(2, "%s: Unknown command line argument '%s'.\n",
		parser->prog_name, parser->current->long_name);
}

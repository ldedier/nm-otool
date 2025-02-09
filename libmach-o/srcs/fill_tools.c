/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:34:34 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/19 14:34:36 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

void		fill_bad_index(t_browser *browser, int index)
{
	browser->has_bad_index = 1;
	browser->bad_string_index = index;
	browser->bad_symbol_index = 1;
}

uint32_t	max_uint32(uint32_t a, uint32_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

int			nm_perror(char *error_message, t_browser *browser)
{
	ft_dprintf(2, "%s: %s\n", browser->filename, error_message);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_value.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 16:40:18 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/19 17:27:56 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		should_print_value64(t_symbol64 symbol64, t_browser *browser)
{
	if ((!((symbol64.nlist->n_type & N_TYPE) == N_UNDF) ||
		browser->has_bad_index) || symbol64.nlist->n_value > 0)
		return (1);
	else
		return (0);
}

int		should_print_value32(t_symbol32 symbol32, t_browser *browser)
{
	if (((!((symbol32.nlist->n_type & N_TYPE) == N_UNDF)
		|| browser->has_bad_index)))
		return (1);
	else
		return (0);
}

int		should_print_value(t_symbol *symbol, t_browser *browser)
{
	return (symbol->symbol_enum == E_SYMBOL_32
			? should_print_value32(symbol->symbol_union.symbol32, browser)
				: should_print_value64(symbol->symbol_union.symbol64, browser));
}

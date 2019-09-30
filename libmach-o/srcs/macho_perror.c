/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_perror.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:32:46 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/19 14:32:51 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		macho_perror(int error, t_browser *browser)
{
	if (error == ERROR_CORRUPTED)
		ft_dprintf(2, "%s: %s: truncated or malformed object (load commands"
					" extend past the end of the file)\n", browser->progname,
					browser->filename);
	return (error);
}

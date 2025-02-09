/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:33:28 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/19 14:33:30 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

char		*get_section_sectname(t_section *section)
{
	return (section->section_enum == E_SECTION_32
		? section->section_union.section32->sectname
			: section->section_union.section64->sectname);
}

char		*get_section_segname(t_section *section)
{
	return (section->section_enum == E_SECTION_32
		? section->section_union.section32->segname
			: section->section_union.section64->segname);
}

uint64_t	get_section_addr(t_section *section)
{
	return (section->section_enum == E_SECTION_32
		? section->section_union.section32->addr
			: section->section_union.section64->addr);
}

uint64_t	get_section_size(t_section *section)
{
	return (section->section_enum == E_SECTION_32
		? section->section_union.section32->size
			: section->section_union.section64->size);
}

uint64_t	get_section_offset(t_section *section)
{
	return (section->section_enum == E_SECTION_32
		? section->section_union.section32->offset
			: section->section_union.section64->offset);
}

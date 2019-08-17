/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:59:56 by niragne           #+#    #+#             */
/*   Updated: 2019/08/17 12:43:58 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

void	debug_mach_header(struct mach_header *ptr)
{
	ft_dprintf(2, "mach magic number identifier: %x\n", ptr->magic);
	ft_dprintf(2, "cpu specifier: %p\n", ptr->cputype);
	ft_dprintf(2, "machine specifier: %p\n", ptr->cpusubtype);
	ft_dprintf(2, "type of file: %d\n", ptr->filetype);
	ft_dprintf(2, "number of load commands: %d\n", ptr->ncmds);
	ft_dprintf(2, "the size of all the load commands: %d\n", ptr->sizeofcmds);
	ft_dprintf(2, "flags: %d\n", ptr->flags);
}

void	nm_print_header(t_header_parser *parser)
{
	struct mach_header_64	*ptr;
	struct fat_header		*ptr2;

	ft_printf("Mach header\n");
	if (parser->type == E_64)
	{
		ptr = (struct mach_header_64*)parser->header_union.header64;
		swap_uint32(&(ptr->magic), parser->should_swap);
		ft_printf("      magic cputype cpusubtype  caps    filetype ncmds sizeofcmds      flags\n");
		ft_printf("%#11x %8u %10hu  %#04p %11d %5d %10d %#010x\n", ptr->magic, ptr->cputype, ptr->cpusubtype, (ptr->cpusubtype & 0xff000000) >> 24, ptr->filetype, ptr->ncmds, ptr->sizeofcmds, ptr->flags);
	}
	else if (parser->type == E_32)
	{
		ptr = (struct mach_header_64*)parser->header_union.header32;
		swap_uint32(&(ptr->magic), parser->should_swap);
		ft_printf("      magic cputype cpusubtype  caps    filetype ncmds sizeofcmds      flags\n");
		ft_printf("%#11x %7u %10hu  %#04p %11d %5d %10d %#010x\n", ptr->magic, ptr->cputype, ptr->cpusubtype, (ptr->cpusubtype & 0xff000000) >> 24, ptr->filetype, ptr->ncmds, ptr->sizeofcmds, ptr->flags);
	}
	else if (parser->type == E_FAT32 || parser->type == E_FAT64)
	{
		ptr2 = parser->header_union.fat_header;
		ft_dprintf(2, "mach magic number identifier: %x\n", ptr2->magic);
		ft_dprintf(2, "number of arch : %u\n", ptr2->nfat_arch);
	}
	else if (parser->type == E_UNKNOWN)
		ft_dprintf(2, "Unknown file type\n");
}

void	print_cpu(struct fat_arch *fat_arch)
{
	ft_printf("%s\n", get_cpu_name(fat_arch->cputype, fat_arch->cpusubtype));
}

void	print_arch32(struct fat_arch *fat_arch)
{
	ft_printf("fat_arch offset: %u\n", fat_arch->offset);
	ft_printf("fat_arch size: %u\n", fat_arch->size);
	ft_printf("fat_arch align: %u\n", fat_arch->align);
	ft_printf("fat_arch cpu: ");
	print_cpu(fat_arch);
	ft_printf("\n");
	ft_printf("\n");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_o.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 14:30:29 by ldedier           #+#    #+#             */
/*   Updated: 2019/09/30 18:11:13 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACH_O_H
# define MACH_O_H

# include "libft.h"
# include <stdio.h>
# include <sys/mman.h>
# include <stdint.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <mach/vm_prot.h>
# include <mach-o/nlist.h>
# include <mach-o/ranlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "symbols.h"
# include "sections.h"

# define DEFAULT_NM_FILE	"a.out"
# define BAD_INDEX_STR		"bad string index"

# define ERROR_CORRUPTED			3
# define ARCHIVE_IDENTIFIER	"!<arch>\n"

# ifndef ARCH
#  define ARCH "unknown"
# endif

typedef enum				e_bin_type
{
	E_UNKNOWN,
	E_32,
	E_64,
	E_FAT32,
	E_FAT64,
	E_ARCHIVE
}							t_bin_type;

typedef enum				e_bin
{
	E_BIN_OTOOL,
	E_BIN_NM
}							t_bin;

typedef union				u_header_union
{
	struct mach_header		*header32;
	struct mach_header_64	*header64;
	struct fat_header		*fat_header;
}							t_header_union;

typedef enum				e_parser_enum
{
	PARSER_ENUM_OBJECT,
	PARSER_ENUM_ARCHI,
	PARSER_ENUM_NONE
}							t_parser_enum;

typedef struct				s_arch
{
	cpu_type_t				cputype;
	cpu_subtype_t			cpusubtype;
	int						relevant;
}							t_arch;

typedef struct				s_object
{
	char					*name;
	uint32_t				ran_off;
}							t_object;

typedef struct				s_archive
{
	uint32_t				ranlib_array_len;
	uint32_t				ranlib_array_size;
	uint32_t				string_array_size;
	struct ranlib			*ranlib_array;
	char					*stringtable;
}							t_archive;

typedef struct				s_parser_union
{
	t_arch					arch;
	t_object				object;
	t_archive				ar;
}							t_parser_union;

typedef struct				s_header_parser
{
	char					*filename;
	uint32_t				magic;
	void					*ptr;
	uint64_t				offset;
	t_bin_type				type;
	t_header_union			header_union;
	int						should_swap;
	t_section_arr			section_arr;
	t_section				*text_section;
	t_section				*data_section;
	t_tree					*symbols;
	t_parser_enum			parser_enum;
	t_parser_union			parser_union;
}							t_header_parser;

struct						s_browser
{
	void					*ptr;
	t_bin_type				from;
	struct stat				st;
	int						ret;
	long					(*sort_func)(void *, void *, void *);
	char					sort_mult;
	char					*filename;
	int						has_bad_index : 1;
	int						bad_string_index;
	int						bad_symbol_index;
	int						nb_args;
	char					*progname;
	int						force;
	int						strsize;
	char					*last_member_name;
	t_tree					*parsers;
	t_bin					bin;
	t_list					**segments;
	int						returned;
	int						cmp_ret;
	void					*reserved;
};

typedef struct s_browser	t_browser;

/*
** debug.c
*/
void						nm_print_header(t_header_parser *parser);
void						debug_mach_header(struct mach_header *ptr);
void						print_arch32(struct fat_arch *fat_arch);

/*
** init.c
*/
void						init_browser_general(t_browser *browser,
								char *progname, t_bin bin);
int							init_browser(t_browser *browser, char *filename);
void						init_parser(t_header_parser *parser,
								void *ptr, uint64_t offset, char *filename);

/*
** fill_tools.c
*/
int							add_symbol_to_browser(t_header_parser *parser,
								t_browser *browser, t_symbol *new_symbol);
int							nm_perror(char *error_message,
								t_browser *browser);
t_symbol					*nm_new_symbol32(struct nlist *nlist,
								char *symbol_name, int index,
									t_browser *browser);
t_symbol					*nm_new_symbol64(struct nlist_64 *nlist,
								char *symbol_name, int index,
									t_browser *browser);
int							should_add_symbol(uint8_t n_type, uint16_t n_stab,
								char *name, t_browser *browser);
uint32_t					max_uint32(uint32_t a, uint32_t b);
void						fill_bad_index(t_browser *browser, int index);
/*
** fill.c
*/
int							fill_browser(t_header_parser *parser,
								t_browser *browser, int first);

/*
** fill32.c
*/
int							fill_browser32(t_header_parser *parser,
									t_browser *browser);
/*
** fill64.c
*/
int							fill_browser64(t_header_parser *parser,
									t_browser *browser);
/*
** fill_fat32.c
*/
int							fill_browser_fat32(
								t_header_parser *parser,
									t_browser *browser);
/*
** fill_fat32_tools.c
*/
char						*get_cpu_name(cpu_type_t cpu, cpu_subtype_t sub);
int							is_corrupted_fat_arch(struct fat_arch *fat_arch,
								t_header_parser *parser, t_browser *browser);
int							fat_corrupted_print_error_alignment(
								struct fat_arch *fat_arch, t_browser *browser);
/*
** print.c
*/
void						nm_print(t_browser *browser, int force);

/*
** fill_debug.c
*/
int							fill_debug64(t_symbol *symbol,
								t_section_arr section_arr,
									t_browser *browser);
int							fill_debug32(t_symbol *symbol,
								t_section_arr section_arr,
									t_browser *browser);

/*
** free.c
*/
void						free_parser(t_header_parser *parser);
void						free_parser_tree(void *content);
int							free_browser(t_browser *browser);
void						free_symbol(t_symbol *symbol);

/*
** swap.c
*/
void						swap_mach_header64(struct mach_header_64 *header64);
void						swap_mach_header(struct mach_header *header32,
								int should_swap);
void						swap_fat_header(struct fat_header *fat_header);
void						swap_fat_arch(struct fat_arch *fat_arch,
								int should_swap);
void						swap_load_command(struct load_command *lc,
								int should_swap);
void						swap_segment_command(struct segment_command *seg,
								int should_swap);
void						swap_segment_command_64(
								struct segment_command_64 *seg,
									int should_swap);
void						swap_symtab_command(struct symtab_command *sym,
								int should_swap);
void						swap_nlist(struct nlist *nlist, int should_swap);
void						swap_nlist(struct nlist *nlist, int should_swap);
void						swap_nlist64(struct nlist_64 *nlist_64,
								int should_swap);
void						swap_section(struct section *section,
								int should_swap);
void						swap_section_64(struct section_64 *section,
								int should_swap);
void						swap_bytes(void *to_swap, size_t size,
								int should_swap);
void						swap_uint32(uint32_t *value, int should_swap);
/*
** corrupted.c
*/
int							is_corrupted_string(char *str,
								t_browser *browser, int *len);
int							is_corrupted_data(void *address,
								size_t size, t_browser *browser);
int							is_corrupted_offset(uint64_t offset, uint64_t size,
								t_browser *browser);

/*
** fill_archive.c
*/
int							fill_browser_archive(t_header_parser *parser,
								t_browser *browser);

/*
** fill_archive_member.c
*/
int							fill_archive_member(t_browser *browser,
								t_header_parser *parser,
									uint32_t offset, char *str);

/*
** tools.c
*/
size_t						align(size_t value, size_t alignment);
long						cmp_parser_ran_off(void *p1, void *p2);
int							add_parser(t_browser *browser,
								t_header_parser *parser);

/*
** macho_perror
*/
int							macho_perror(int error, t_browser *browser);

/*
** corrupted_fill64.c
*/
int							overlaps_symtab_command64(
								struct symtab_command *sym);
int							is_corrupted_fill_symbol64(
								struct symtab_command *sym,
									struct mach_header_64 *header,
										t_browser *browser);
int							corrupted_load_command(struct load_command *lc,
								t_browser *browser,
									struct mach_header_64 *header, int index);
int							is_corrupted_segment_command_64(
								struct segment_command_64 *seg,
									t_header_parser *parser,
										t_browser *browser, int index);
/*
** corrupted_fill32.c
*/
int							overlaps_symtab_command32(
								struct symtab_command *sym);
int							is_corrupted_fill_symbol32(
								struct symtab_command *sym,
									struct mach_header *header,
										t_browser *browser);
int							corrupted_load_command32(struct load_command *lc,
								t_browser *browser,
									struct mach_header *header, int index);
int							is_corrupted_segment_command_32(
								struct segment_command *seg,
									t_header_parser *parser,
										t_browser *browser, int index);

int							get_total_sections64(t_list *segments64);
int							get_sections64(t_header_parser *parser,
								t_browser *browser);
int							fill_symbol_table64(t_header_parser *parser,
								t_browser *browser);
int							get_total_sections32(t_list *segments);
int							get_sections32(t_header_parser *parser,
								t_browser *browser);
int							fill_symbol_table32(t_header_parser *parser,
								t_browser *browser);
/*
** should_print_value.c
*/
int							should_print_value32(t_symbol32 symbol32,
								t_browser *browser);
int							should_print_value64(t_symbol64 symbol64,
								t_browser *browser);
int							should_print_value(t_symbol *symbol,
								t_browser *browser);
#endif

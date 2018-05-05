/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_vm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iosypenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 16:34:17 by iosypenk          #+#    #+#             */
/*   Updated: 2018/05/01 16:34:21 by iosypenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header_vm.h"

void			init(t_core *core)
{
	core->champ = NULL;
	core->champ_count = 0;
}

void			err(void)
{
	ft_printf(RED"ERROR\n"RC);
	exit (0);
}

t_champ 		*get_last_list(t_champ *head)
{
	t_champ		*list;

	list = NULL;
	if (head)
	{
		list = head;
		while (list->next)
			list = list->next;
	}
	return (list);
}

int				get_champ(t_core *core)
{
	t_champ		*new;
	t_champ		*last;

	// list = core->champ;
	if (!(new = malloc(sizeof(t_champ))))
		return (0);
	new->next = NULL;
	new->last_live = 0;
	new->name = NULL;
	new->id = 0;
	// if (list)
	// {
	// 	while (list->next)
	// 		list = list->next;
	// 	list->next = new;
	// }
	if ((last = get_last_list(core->champ)))
		last->next = new;
	else
		core->champ = new;
	return (1);
}

unsigned int	get_magic_size(unsigned int b, int slide)
{
	unsigned int size;

	size = 0;
	if (slide == 24)
		(size = b << slide);
	else if (slide == 16)
		(size = b << slide & 0x00ffffff);
	else if (slide == 8)
		(size = b << slide & 0x0000ffff);
	else if (slide == 0)
		(size = b & 0x000000ff);
	return (size);
}

int				check_magic(int fd, t_champ *champ, t_core *core)
{
	int 				i;
	int 				slide;
	unsigned char		buff[4];
	unsigned int		b[4];

	if (read(fd, buff, 4) < 0) // <-- fix error management
		return (0);
	slide = 24;
	i = 0;
	while (i < 4)
	{
		b[i] = get_magic_size(buff[i], slide);
		slide -= 8;
		i++;
	}
	champ->header.magic = b[0] | b[1] | b[2] | b[3];
	if (champ->header.magic == COREWAR_EXEC_MAGIC)
	{
		if (core->champ_count <= MAX_PLAYERS)
			// champ->id = ((core->champ_count++) + 1);
			champ->id -= ((core->champ_count++) + 1);
		else
		{
			ft_printf(RED"ERROR: too many players\n"RC);
			exit (0);
		}
		return (1); // <--- OK
	}
	return (0);
}

int 			get_prog_size(int fd, t_champ *champ)
{
	int 				i;
	int 				slide;
	unsigned char		buff[4];
	unsigned int		b[4];

	if (lseek(fd, CODE_SIZE_POS, SEEK_SET) < 0) 
		return (0);
	if (read(fd, buff, 4) < 0) // <-- fix error management
		return (0);
	slide = 24;
	i = 0;
	while (i < 4)
	{
		b[i] = get_magic_size(buff[i], slide);
		slide -= 8;
		i++;
	}
	champ->header.prog_size = b[0] | b[1] | b[2] | b[3];
	if (champ->header.prog_size <= CHAMP_MAX_SIZE)
		return (1);
	return (0);
}

int				get_place(t_champ *champ, unsigned int champ_count)
{
	int		res;
	int		shift;

	res = 0;
	shift = 0;
	if (champ_count == 1 || champ->id == 1)
		return (res);
	else
	{
		shift = (MEM_SIZE / champ_count);
		res = shift * (-champ->id - 1); 
	}
	return (res);
}

void			place_champs(t_core *core)
{
	t_champ 	*lst;

	lst = core->champ;
	core->i = 0;
	while (core->i < core->champ_count)
	{
		lseek(lst->fd, CODE_POS, SEEK_SET);
		read(lst->fd, (core->map + get_place(lst, core->champ_count)), CHAMP_MAX_SIZE);
		// ft_printf(GRN"id = %d\n"RC, lst->id);
		lst = lst->next;
		core->i++;
	}
}

void			parsing_champ(t_core *core, char *argv)
{
	int			fd;
	t_champ		*champ;

	fd = open(argv, O_RDONLY);
	champ = get_last_list(core->champ);
	champ->fd = fd;
	((check_magic(fd, champ, core)) != 1) ? err() : 0;
	(read(fd, champ->header.prog_name, PROG_NAME_LENGTH + 1) < 0) ? err() : 0;
	(get_prog_size(fd, champ) != 1) ? err() : 0;
	// place_champ(fd, core, champ);
	// (read(fd, champ->header.prog_size, 4) < 0) ? err() : 0;
}


/*
**************************************************************************
** 							TEST functions 								**
**								 Start									**
**							Del them before push						**
**************************************************************************
*/

void			test_print_list(t_champ *list) // <---- del it
{
	while (list)
	{
		ft_printf(GRN"id = %d => "RED"%s %d_Mb\n"RC,
			list->id, list->header.prog_name, list->header.prog_size);
		list = list->next;
	}
}

void			test_print_map(t_core core)
{
	int		i;

	i = 0;
	while (i < MEM_SIZE)
	{
		// write(1, &core->map[i], 1);
		if (i % 64 == 0 && i != 0)
		{
			ft_printf(GRN"\n"RC);
		}
		if (core.map[i] < 16)
			ft_printf("0"RC);
		ft_printf("%x "RC, core.map[i]);
		i++;
	}
	ft_printf("\n");
}

/*
**************************************************************************
**								 END									**
**************************************************************************
*/

void			create_map(t_core *core)
{
	core->i = 0;
	while (core->i < MEM_SIZE)
	{
		// core->mapid[core->i] = 0;
		core->map[core->i] = 0;
		core->i++;
	}
	// ft_printf("\n"RC);
}

int				main(int argc, char **argv)
{
	t_core			core;
	int				i;

	if (argc == 1)
	{
		ft_printf(YEL"usage:\n"RC);
	}
	else if (argc > 1)
	{
		init(&core);
		create_map(&core);
		i = 1;
		while (i < argc)
		{
			get_champ(&core);
			parsing_champ(&core, argv[i++]);
		}
		place_champs(&core);
		test_print_list(core.champ);//  <---- del it
		test_print_map(core);//  <---- del it
 	}
	else
	{
		ft_printf(RED"ERROR: too many arguments\n"RC);
	}
	// system("leaks war");
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_vm.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iosypenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 16:37:58 by iosypenk          #+#    #+#             */
/*   Updated: 2018/05/01 16:38:02 by iosypenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_VM_H

# define HEADER_VM_H

# include <stdint.h> // <--this shit for uint32_t
# include <fcntl.h>
# include "../../libft/libft.h"
# include "../../op.h"

# define ALIGNMENT		((PROG_NAME_LENGTH + 1) % 4)
# define CODE_SIZE_POS	((9 + PROG_NAME_LENGTH) - ALIGNMENT)
# define COMMENT_POS	((13 + PROG_NAME_LENGTH) - ALIGNMENT)
# define CODE_POS		(COMMENT_POS + COMMENT_LENGTH + 4)


typedef struct		s_champ
{
	char			*name;
	int				id;
	int				fd;
	int				last_live;
	t_header		header;
	struct s_champ	*next;
}					t_champ;

typedef struct		s_proc
{
	struct s_proc	*next;
}					t_proc;

typedef struct		s_core
{
	t_champ			*champ;
	t_proc			*proc;
	unsigned int	cycles_left;
	unsigned char	map[MEM_SIZE];
	// unsigned char	mapid[MEM_SIZE];
	unsigned int 	i;
	unsigned int	champ_count;	
}					t_core;

#endif

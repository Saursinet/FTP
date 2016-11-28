/*
** socket_macro.h for my_ftp in /home/saurs_f/PSU_2015_myftp/inc/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 18:52:26 2016 Florian Saurs
** Last update May May 5 22:22:56 2016
*/

#ifndef SOCKET_MACRO_H_
# define SOCKET_MACRO_H_

# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include "struct_socket.h"

# define WRITESOCKET(x, y)	write(x, y, strlen(y))
# define UNUSED			__attribute__((__unused__))
# define BUFF_SIZE		2

#endif /* !SOCKET_MACRO_H_ */

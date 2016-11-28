/*
** client_recv_send.c for my_ftp in /home/saurs_f/PSU_2015_myftp
**
** Made by Saursinet
** Login   <saurs_f@epitech.net>
**
** Started on  Fri May  6 11:00:08 2016 Saursinet
** Last update Fri May 13 16:54:18 2016 Saursinet
*/

#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "socket_prototype.h"
#include "socket_macro.h"

int	create_serveur_ip_port(char *answer, t_struct_socket *sock)
{
  char		ip[256];
  char		*tmp;
  int		i;

  bzero(ip, 256);
  tmp = strtok(answer, ",");
  i = -1;
  while (++i < 4)
    {
      strcat(ip, tmp == NULL ? "" : tmp);
      i != 3 ? strcat(ip, ".") : 0;
      tmp = strtok(NULL, ",");
    }
  i = atoi(tmp == NULL ? "0" : tmp) * 256;
  i += atoi((tmp = strtok(NULL, ")")) == NULL ? "0" : tmp);
  sock->port = i;
  sock->s_in.sin_addr.s_addr = inet_addr(ip);
  if (init_socket(sock) != 0)
    sock->fd = -1;
  return (sock->fd);
}

int				take_fd_from_client(int *fd)
{
  t_struct_client_socket	client;
  int				fd_tmp;

  client.s_in_size = sizeof(client.s_in_client);
  client.client_fd = accept(*fd,
			    (struct sockaddr *)&client.s_in_client,
			    &client.s_in_size);
  fd_tmp = *fd;
  *fd = client.client_fd;
  return (fd_tmp);
}

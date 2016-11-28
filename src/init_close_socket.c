/*
** init_socket.c for my_ftp in /home/saurs_f/PSU_2015_myftp/src/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 12:08:30 2016 Florian Saurs
** Last update Thu May 12 22:06:10 2016 Saursinet
*/

#include <stdio.h>
#include <unistd.h>
#include "struct_socket.h"
#include "socket_macro.h"
#include "socket_prototype.h"

int	bind_and_listen_server(t_struct_socket *sock, int nb_co)
{
  int	one_value;

  one_value = 1;
  if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR,
		 &one_value, sizeof(int)) == -1)
    {
      if (close_socket(sock->fd) != 0)
	return (-1);
      return (fprintf(stderr, "Setsockopt failed.\n"));
    }
  if (bind(sock->fd, (struct sockaddr *)&sock->s_in, sizeof(sock->s_in)) == -1)
    {
      if (close_socket(sock->fd) != 0)
	return (-1);
      return (fprintf(stderr, "Bind failed.\n"));
    }
  if (listen(sock->fd, nb_co) == -1)
    {
      if (close_socket(sock->fd) != 0)
	return (-1);
      return (fprintf(stderr, "Listen failed.\n"));
    }
  return (0);
}

int	init_socket(t_struct_socket *sock)
{
  sock->s_in.sin_family = AF_INET;
  sock->s_in.sin_port = htons(sock->port);
  sock->protocol_struct = getprotobyname("TCP");
  if (!sock->protocol_struct)
    return (fprintf(stderr, "Can't access to protocol ID.\n"));
  sock->fd = socket(AF_INET, SOCK_STREAM, sock->protocol_struct->p_proto);
  if (sock->fd == -1)
    return (fprintf(stderr, "Can't create the socket.\n"));
  return (0);
}

int	init_client_socket(t_struct_socket *sock, char *ip)
{
  sock->s_in.sin_addr.s_addr = inet_addr(ip);
  if (init_socket(sock) != 0)
    return (-1);
  if (connect(sock->fd, (struct sockaddr *)&sock->s_in,
	      sizeof(sock->s_in)) == -1)
    {
      if (close(sock->fd) == -1)
	return (fprintf(stderr, "Can't close the socket.\n"));
      return (fprintf(stderr, "Can't connect on the socket.\n"));
    }
  return (0);
}

int	close_socket(int fd)
{
  if (close(fd) == -1)
    return (fprintf(stderr, "Can't close the socket.\n"));
  return (0);
}

int	close_both_connections(int sock_fd, int client_fd, int opt)
{
  if (opt == 0)
    printf("Client deconnection...\n");
  if (close(client_fd) == -1)
    {
      if (close_socket(sock_fd) != 0)
	return (-1);
      return (fprintf(stderr, "Can't close the cliend fd.\n"));
    }
  return (0);
}

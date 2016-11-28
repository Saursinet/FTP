/*
** main.c for myftp in /home/saurs_f/PSU_2015_myftp/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 09:35:52 2016 Florian Saurs
** Last update Thu May 12 19:35:13 2016 Saursinet
*/

#include <stdlib.h>
#include "socket_macro.h"
#include "socket_prototype.h"

int	search_command(char buffer[4096], char *cmd, t_dico *dico,
		       t_struct_client_socket *client)
{
  int	i;

  i = 0;
  while (cmd && dico->word[i] && strcasecmp(dico->word[i], cmd) != 0)
    ++i;
  if (dico->word[i] && cmd)
    dico->ptr[i](client->client_fd, strtok(NULL, "\r\n"), &dico->current_user);
  else
    {
      i = 0;
      cmd = strtok(buffer, "\r\n");
      while (cmd && dico->word[i] && strcasecmp(dico->word[i], cmd) != 0)
	++i;
      if (dico->word[i] && cmd)
	dico->ptr[i](client->client_fd, "", &dico->current_user);
      else
	{
	  if (dico->current_user.is_logged)
	    WRITESOCKET(client->client_fd, "500 Unknown command.\r\n");
	  else
	    WRITESOCKET(client->client_fd,
			"530 Please login with USER and PASS.\r\n");
	}
    }
  return (0);
}

void	read_input(int fd, char buffer[4096])
{
  char	buffer_save[4096];
  int	nb;

  bzero(buffer, 4096);
  while ((nb = read(fd, buffer_save, 4096)) > 0)
    {
      buffer_save[nb] = 0;
      strcat(buffer, buffer_save);
      if (buffer_save[nb -1] == '\n')
	return ;
    }
}

int		deal_with_clients(t_struct_socket *sock,
				  t_struct_client_socket *client,
				  t_dico *dico)
{
  char		buffer[4096];
  char		*cmd;

  client->client_ip = inet_ntoa(client->s_in_client.sin_addr);
  printf("%s at port :%d\n", client->client_ip, client->s_in_client.sin_port);
  WRITESOCKET(client->client_fd, "220 Service ready for new user.\r\n");
  while (bzero(buffer, 4096), 1)
    {
      read_input(client->client_fd, buffer);
      printf("Command is [%s]\n", buffer);
      cmd = strtok(buffer, " ");
      if (strstr(buffer, "QUIT") != NULL || strstr(buffer, "quit") != NULL)
	{
	  WRITESOCKET(client->client_fd, "221 Goodbye and see you soon.\r\n");
	  return (close_both_connections(sock->fd, client->client_fd, 0));
	}
      search_command(buffer, cmd, dico, client);
    }
  return (0);
}

int				handle_clients(t_struct_socket *sock)
{
  int				pid;
  t_struct_client_socket	client;
  t_dico			dico;

  client.s_in_size = sizeof(client.s_in_client);
  if (init_dico(&dico) == 1)
    return (1);
  while (1)
    {
      client.client_fd =
	accept(sock->fd, (struct sockaddr *)&client.s_in_client,
	       &client.s_in_size);
      if (client.client_fd == -1)
	{
	  if (close_socket(sock->fd) != 0)
	    return (-1);
	  return (fprintf(stderr, "Client fd cannot be created.\n"));
	}
      if (pid = fork(), pid == 0)
	deal_with_clients(sock, &client, &dico);
      else
	close_both_connections(sock->fd, client.client_fd, 1);
    }
  return (clear_dico(&dico), 0);
}

int			main(int ac, char **av)
{
  t_struct_socket	sock;

  if (ac != 3 || (sock.port = atoi(av[1])) <= 0 || !is_directory(av[2]))
    return (fprintf(stderr,
		    "Usage: ./server [PORT] [PATH]\n\
With port positive non-NULL and path a valid path name.\n"));
  sock.s_in.sin_addr.s_addr = INADDR_ANY;
  if (init_socket(&sock) != 0)
    return (-1);
  if (chdir(av[2]) == -1)
    return (fprintf(stderr, "Cannot go in the directory path given\n"));
  if (bind_and_listen_server(&sock, 42) != 0)
    return (close_socket(sock.fd));
  printf("Server listen on %s at %d\n", take_addr(NULL), sock.port);
  if (handle_clients(&sock) != 0)
    return (-1);
  if (close_socket(sock.fd) != 0)
    return (-1);
  return (0);
}

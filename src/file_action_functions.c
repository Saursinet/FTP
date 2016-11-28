/*
** file_action_functions.c for my_ftp in /home/saurs_f/PSU_2015_myftp/src/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 17:03:52 2016 Florian Saurs
** Last update Fri May 13 16:51:32 2016 Saursinet
*/

#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include "socket_macro.h"
#include "socket_prototype.h"

int		send_answer_to_download_client(int fd, t_user *user,
					       int fd_file)
{
  struct stat	stat_file;

  if (fstat(fd_file, &stat_file) == -1 ||
      sendfile(user->is_active ? user->sock.fd : user->client.client_fd,
	       fd_file, NULL, stat_file.st_size) == -1 ||
      close_socket(user->is_active ? user->sock.fd :
		   user->client.client_fd) != 0 || close(fd_file) == -1)
    {
      if (user->is_passive)
	close_socket(user->sock.fd);
      user->is_active = 0;
      user->is_passive = 0;
      return (WRITESOCKET(fd, "226 Closing data connection: abort.\r\n"));
    }
  user->is_active = 0;
  user->is_passive = 0;
  WRITESOCKET(fd, "226 Requested file action okay, completed.\r\n");
  return (0);
}

int	accept_co_if_is_passive(t_user *user, int fd)
{
  if (user->is_logged && user->is_passive)
    {
      user->client.s_in_size = sizeof(user->client.s_in_client);
      user->client.client_fd = accept(user->sock.fd,
				      (struct sockaddr *)&user->client.s_in_client,
				      &user->client.s_in_size);
      if (user->client.client_fd == -1)
	{
	  user->is_passive = 0;
	  return (WRITESOCKET(fd, "421 Closing data connection: abort.\r\n"));
	}
    }
  else if (user->is_logged && user->is_active)
    {
      if (connect(user->sock.fd, (struct sockaddr *)&user->sock.s_in,
		  sizeof(user->sock.s_in)) == -1)
	{
	  user->is_passive = 0;
	  return (WRITESOCKET(fd, "421 Closing data connection: abort.\r\n"));
	}
    }
  return (0);
}

int	upload_file(int fd, char *file, t_user *user)
{
  int	fd_file;
  int	r;
  char	str[4096];

  if (accept_co_if_is_passive(user, fd) != 0)
    return (0);
  WRITESOCKET(fd, "150 File status okay; about to open data connection.\r\n");
  if (user->is_logged && user->sock.fd != -1)
    {
      if ((fd_file = open(file, O_WRONLY | O_CREAT, 0666)) == -1)
        return (user->is_active = 0, user->is_passive = 0,
		WRITESOCKET(fd, "550 Closing data connection: abort.\r\n"));
      while ((r = read(user->is_passive ? user->client.client_fd :
		       user->sock.fd, str, 4096)) > 0)
	write(fd_file, str, r);
      user->is_passive ?
	close_both_connections(user->sock.fd, user->client.client_fd, 1) :
	close_socket(user->sock.fd);
      WRITESOCKET(fd, "226 Requested file action okay, completed.\r\n");
    }
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (user->is_active = 0, user->is_passive = 0, 0);
}

int	download_file(int fd, char *file, t_user *user)
{
  int	fd_file;

  if (accept_co_if_is_passive(user, fd) != 0)
    return (0);
  WRITESOCKET(fd, "150 File status okay; about to open data connection.\r\n");
  if (user->is_logged && user->sock.fd != -1)
    {
      if ((fd_file = open(file, O_RDONLY)) == -1)
	{
	  user->is_active = 0;
	  user->is_passive = 0;
	  return (WRITESOCKET(fd, "550 Closing data connection: abort.\r\n"));
	}
      return (send_answer_to_download_client(fd, user, fd_file));
    }
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (0);
}

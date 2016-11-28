/*
** list_and_print_dir.c for myftp in /home/saurs_f/PSU_2015_myftp
**
** Made by Saursinet
** Login   <saurs_f@epitech.net>
**
** Started on  Sun May  8 18:48:02 2016 Saursinet
** Last update Sat May 14 17:44:56 2016 Saursinet
*/

#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include "socket_macro.h"
#include "socket_prototype.h"

void		list_directory(t_user *user, int fd, char *list)
{
  char		str[256];
  int		tmp;

  tmp = dup(1);
  dup2(user->is_passive ? user->client.client_fd : user->sock.fd, 1);
  bzero(str, 256);
  strcat(str, "ls -l ");
  strcat(str, list == NULL ? "." : list);
  strcat(str, "| tail -n +2");
  system(str);
  dup2(tmp, 1);
  if (close_socket(user->is_active ? user->sock.fd :
		   user->client.client_fd) != 0)
    WRITESOCKET(fd, "421 Closing data connection: abort.\r\n");
  else
    WRITESOCKET(fd, "226 Requested file action okay, completed.\r\n");
}

int		list_file(int fd, char *list, t_user *user)
{
  if (accept_co_if_is_passive(user, fd) != 0)
    return (0);
  WRITESOCKET(fd, "150 File status okay; about to open data connection.\r\n");
  if (user->is_logged && user->sock.fd != -1)
    list_directory(user, fd, list);
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (user->is_active = 0, user->is_passive = 0, 0);
}

int	delete_file(int fd, char *file, t_user *user)
{
  int	value;
  char	str[256];

  if (user->is_logged)
    {
      if ((value = is_deletable(user->pwd, strdup(file), user->home)) == -1)
	sprintf(str, "550 \"%s\" cannot be deleted: permission denied.\r\n", file);
      else if (value == -2)
	sprintf(str, "550 \"%s\" cannot be deleted: no such file or directory.\r\n",
		file);
      else if ((value = remove(file)) == -1 && errno == ENOTEMPTY)
	sprintf(str, "550 \"%s\" cannot be deleted: directory not empty.\r\n", file);
      else if (value == -1)
	sprintf(str, "550 \"%s\" cannot be deleted.\r\n", file);
      else
        sprintf(str, "250 \"%s\" deleted.\r\n", file);
      WRITESOCKET(fd, str);
    }
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (0);
}

int	print_working_directory(int fd, UNUSED char *pwd, t_user *user)
{
  char	str[256];

  bzero(str, 256);
  if (user->is_logged)
    WRITESOCKET(fd, strcat(strcat(strcat(str, "257 \""),
				  &user->pwd[strlen(user->home)]), "/\"\r\n"));
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (0);
}

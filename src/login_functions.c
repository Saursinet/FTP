/*
** LoginFunctions.c for my_ftp in /home/saurs_f/PSU_2015_myftp/src/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 16:54:42 2016 Florian Saurs
** Last update Sat May 14 18:01:15 2016 Saursinet
*/

#include <stdlib.h>
#include "socket_macro.h"
#include "socket_prototype.h"

int	login(int fd, char *log, t_user *user)
{
  if (log || user->is_logged)
    {
      if (user->is_logged >= 1)
	WRITESOCKET(fd, "230 User logged in, proceed.\r\n");
      else
	{
	  user->login = strdup(log);
	  WRITESOCKET(fd, "331 user name okay, need password.\r\n");
	}
      user->is_good_name = 1;
      printf("user okay = %d\n", user_okay(log, NULL, 0) == 1);
      if (user->home[0] == 0 && user_okay(log, NULL, 0) == 1)
	put_path_to_user(user->home, log, user->pwd);
    }
  else
    {
      WRITESOCKET(fd, "530 Permission denied.\r\n");
      free(user->login);
      user->is_good_name = 0;
      bzero(user->home, 256);
      bzero(user->pwd, 256);
    }
  return (0);
}

int	password(int fd, char *password, t_user *user)
{
  printf("user okay = %d\n", user_okay(user->login, password, 1) == 1);
  if (user_okay(user->login, password, 1) == 2)
    {
      WRITESOCKET(fd, "230 user logged in, proceed.\r\n");
      user->is_logged = 1;
    }
  else
    {
      WRITESOCKET(fd, "530 Login incorrect.\r\n");
      user->is_logged = 0;
      bzero(user->home, 256);
      bzero(user->pwd, 256);
    }
  return (0);
}

void	fill_the_path(char str[256], char *cwd, char *home)
{
  bzero(str, 256);
  if (cwd[0] == '/')
    {
      strcat(str, home);
      strcat(str, "/");
      strcat(str, &cwd[1]);
    }
  else
    strcat(str, cwd);
}

int	change_working_directory(int fd, char *cwd, t_user *user)
{
  int	value;
  char	str[256];

  if (user->is_logged)
    {
      if (!cwd || strcmp(cwd, "") == 0)
	WRITESOCKET(fd, "501 Failed to change directory.\r\n");
      else if (fill_the_path(str, cwd, user->home),
	       (value = change_path(user->pwd, str, user->home)) == -1)
	WRITESOCKET(fd, "550 Permission denied.\r\n");
      else if (value == -2)
	WRITESOCKET(fd, "550 Failed to change directory.\r\n");
      else
	WRITESOCKET(fd, "250 Directory successfully changed.\r\n");
    }
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (0);
}

int	change_working_directory_to_parent_directory(UNUSED int fd,
						     UNUSED char *cwdtp,
						     t_user *user)
{
  if (user->is_logged)
    {
      if (change_path(user->pwd, "..", user->home) == -1)
	WRITESOCKET(fd, "200 Directory successfully changed.\r\n");
    }
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (0);
}

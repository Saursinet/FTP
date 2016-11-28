/*
** utils.c for my_ftp in /home/saurs_f/PSU_2015_myftp/src/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Tue May  3 13:32:23 2016 Florian Saurs
** Last update Fri May  6 17:46:14 2016 Saursinet
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>

int	is_directory(char *path)
{
  DIR	*directory;

  directory = opendir(path);
  if (directory != NULL)
    {
      closedir(directory);
      return (1);
    }
  return (0);
}

void	put_path_to_user(char *path, char *login, char *pwd)
{
  bzero(path, 256);
  if (!is_directory(login))
    mkdir(login, 0700);
  chdir(login);
  getcwd(path, 256);
  getcwd(pwd, 256);
}

int	change_path(char *pwd, char *cwd, char *home)
{
  char	*tmp;

  if (!is_directory(cwd))
    return (-2);
  tmp = strdup(pwd);
  if (chdir(cwd) != -1)
    getcwd(pwd, 256);
  if (strstr(pwd, home) == NULL &&
      strcmp(pwd, home) != 0)
    {
      if (chdir(tmp) != -1)
	getcwd(pwd, 256);
      free(tmp);
      return (-1);
    }
  free(tmp);
  return (0);
}

int	is_deletable(char *pwd, char *cwd, char *home)
{
  char	str[256];

  if (access(cwd, F_OK) == -1)
    return (free(cwd), -2);
  if (!is_directory(cwd))
    {
      if (chdir(dirname(cwd)) == -1 ||
	  getcwd(str, 256) == NULL ||
	  (strstr(str, home) == NULL &&
	   strcmp(str, home) != 0))
	{
	  chdir(pwd);
	  return (free(cwd), -1);
	}
      return (free(cwd), 0);
    }
  chdir(cwd);
  if (getcwd(str, 256) == NULL ||
      (strstr(pwd, home) == NULL && strcmp(pwd, home) != 0))
    {
      chdir(pwd);
      return (free(cwd), -1);
    }
  chdir(pwd);
  return (free(cwd), 0);
}

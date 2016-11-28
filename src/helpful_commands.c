/*
** helpful_commands.c for my_ftp in /home/saurs_f/PSU_2015_myftp/src/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 17:11:26 2016 Florian Saurs
** Last update Sat May 14 18:01:09 2016 Saursinet
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "socket_macro.h"

int	help(int fd, UNUSED char *help, t_user *user)
{
  char	str[1024];
  int	i;

  if (user->is_logged)
    {
      sprintf(str, "214 -The following commands are recognized.\n ");
      i = 0;
      while (user->word[i])
	{
	  strcat(str, user->word[i]);
	  if (i != 0 && i % 14 == 0 && user->word[i + 1])
	    strcat(str, "\n");
	  strcat(str, " ");
	  ++i;
	}
      strcat(str, "\n214 Help OK.\r\n");
      WRITESOCKET(fd, str);
    }
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (0);
}

int	noop(int fd, UNUSED char *noop, t_user *user)
{
  if (user->is_logged)
    WRITESOCKET(fd, "200 NOOP ok.\r\n");
  else
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  return (0);
}

char	**fill_tab(char **tab)
{
  char	str[4096];
  int	nb;
  int	fd;
  int	count;
  char	*tmp;

  if (fd = open(".users", O_RDONLY), bzero(str, 4096), count = 2, fd == -1 ||
      (nb = read(fd, str, 4096)) == -1)
    return (NULL);
  close(fd);
  str[nb] = 0;
  nb = -1;
  while (str[++nb])
    count += str[nb] == '\n' ? 1 : 0;
  if ((tab = malloc(count * sizeof(char *))) == NULL)
    return (NULL);
  count = 0;
  tab[count] = strdup(strtok(str, "\n"));
  while (1)
    {
      if (tab[count++] == NULL)
	return (tab);
      tab[count] = (tmp = strtok(NULL, "\n")) == NULL ? NULL : strdup(tmp);
    }
  return (tab);
}

int		user_okay(char *log, char *pass, int opt)
{
  static char	**tab = NULL;
  int		count;

  if ((tab == NULL && (tab = fill_tab(tab)) == NULL) || log == NULL)
    return ((strcmp(log == NULL ? "" : log, "Anonymous") == 0 &&
	     (pass == NULL || strcmp(pass, "") == 0)) ? 2: 0);
  count = -1;
  while (tab[++count])
    {
      if ((strstr(tab[count], log) != NULL && tab[count][0] == log[0] &&
	   (tab[count][strlen(log)] == 0 || tab[count][strlen(log)] == ' ')))
	{
	  if (opt == 1 && strstr(tab[count], " ") == NULL &&
	      (pass == NULL || strcmp(pass, "") == 0))
	    return (2);
	  if (pass == NULL)
	    return (1);
	  if (strstr(tab[count], pass) != NULL)
	    return (2);
	  return (-1);
	}
    }
  return (0);
}

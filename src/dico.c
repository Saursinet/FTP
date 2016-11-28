/*
** dico.c for my_ftp in /home/saurs_f/PSU_2015_myftp/src/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 16:50:23 2016 Florian Saurs
** Last update Sat May 14 16:55:04 2016 Saursinet
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_prototype.h"
#include "socket_macro.h"

int	type(int fd, UNUSED char *login, UNUSED t_user *user)
{
  return (WRITESOCKET(fd, "200 Command okay.\r\n"));
}

int		init_fptr(t_dico *dico)
{
  dico->ptr[0] = &login;
  dico->ptr[1] = &password;
  dico->ptr[2] = &change_working_directory;
  dico->ptr[3] = &change_working_directory_to_parent_directory;
  dico->ptr[4] = &transfer_active;
  dico->ptr[5] = &transfer_passive;
  dico->ptr[6] = &upload_file;
  dico->ptr[7] = &download_file;
  dico->ptr[8] = &list_file;
  dico->ptr[9] = &delete_file;
  dico->ptr[10] = &print_working_directory;
  dico->ptr[11] = &help;
  dico->ptr[12] = &noop;
  dico->ptr[13] = &type;
  return (0);
}

int		init_dico(t_dico *dico)
{
  dico->word[0] = strdup("USER");
  dico->word[1] = strdup("PASS");
  dico->word[2] = strdup("CWD");
  dico->word[3] = strdup("CDUP");
  dico->word[4] = strdup("PORT");
  dico->word[5] = strdup("PASV");
  dico->word[6] = strdup("STOR");
  dico->word[7] = strdup("RETR");
  dico->word[8] = strdup("LIST");
  dico->word[9] = strdup("DELE");
  dico->word[10] = strdup("PWD");
  dico->word[11] = strdup("HELP");
  dico->word[12] = strdup("NOOP");
  dico->word[13] = strdup("TYPE");
  dico->word[14] = NULL;
  dico->current_user.is_good_name = 0;
  dico->current_user.is_logged = 0;
  bzero(dico->current_user.home, 256);
  bzero(dico->current_user.pwd, 256);
  dico->current_user.is_active = 0;
  dico->current_user.is_passive = 0;
  dico->current_user.word = dico->word;
  dico->current_user.sock.fd = -1;
  dico->current_user.login = NULL;
  return (init_fptr(dico));
}

void		clear_dico(t_dico *dico)
{
  int		i;

  i = 0;
  while (i < 14)
    free(dico->word[i++]);
}

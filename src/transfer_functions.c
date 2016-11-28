/*
** TransferFunctions.c for my_ftp in /home/saurs_f/PSU_2015_myftp/src/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 16:59:37 2016 Florian Saurs
** Last update Fri May 13 16:37:38 2016 Saursinet
*/

#include <sys/types.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include "socket_macro.h"
#include "socket_prototype.h"

int	transfer_active(int fd, char *ta, t_user *user)
{
  if (user->is_logged && !user->is_active && !user->is_passive)
    {
      if (create_serveur_ip_port(ta, &user->sock) == -1)
	WRITESOCKET(fd, "550 Closing data connection.\r\n");
      else
	{
	  WRITESOCKET(fd, "200 File status okay;\
 about to open data connection.\r\n");
	  user->is_active = 1;
	}
    }
  else
    {
      if (!user->is_logged)
	WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
      else
	WRITESOCKET(fd,
		    "200 Data connection open; no transfer in progress.\r\n");
    }
  return (0);
}

char			*take_addr(char *save)
{
  struct ifaddrs	*ipstruct;
  struct ifaddrs	*tmp;
  char			host[NI_MAXHOST];

  if (getifaddrs(&ipstruct) == -1)
    return (save);
  tmp = ipstruct;
  while (tmp)
    {
      if (tmp->ifa_addr->sa_family == AF_INET)
	{
	  if (getnameinfo(tmp->ifa_addr, sizeof(struct sockaddr_in), host,
			  NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
	    return (save[0] = -1, save);
	  if (!save || strcmp("127.0.0.1", save) == 0)
	    {
	      if (save)
		free(save);
	      save = strdup(host);
	    }
	}
      tmp = tmp->ifa_next;
    }
  return (freeifaddrs(ipstruct), save);
}

void		take_ip(char *ipstr, char *save, t_user *user)
{
  char		ip[256];
  char		*tmp;
  socklen_t	len;

  if (len = sizeof(user->sock.s_in),
      getsockname(user->sock.fd,
		  (struct sockaddr *)&user->sock.s_in, &len) == -1)
    sprintf(ipstr, "421 Cannot open data connection.\r\n");
  else
    {
      bzero(ip, 256);
      tmp = strtok(save, ".");
      while (tmp)
	{
	  strcat(ip, tmp);
	  strcat(ip, ",");
	  tmp = strtok(NULL, ".");
	}
      sprintf(ipstr, "227 Entering Passive Mode (%s%d,%d).\r\n",
	      ip, ntohs(user->sock.s_in.sin_port) / 256,
	      ntohs(user->sock.s_in.sin_port) % 256);
      user->is_passive = 1;
    }
}

int		transfer_passive(int fd, UNUSED char *tp, t_user *user)
{
  char		*save;
  char		ipstr[256];

  if (user->is_logged && !user->is_passive && !user->is_active)
    {
      if (user->sock.s_in.sin_port = 0, user->is_passive = 1,
	  user->sock.port = 0, (save = take_addr(NULL)))
	{
	  user->sock.s_in.sin_addr.s_addr = inet_addr(save);
	  init_socket(&user->sock);
	  bind_and_listen_server(&user->sock, 1);
	  take_ip(ipstr, save, user);
	}
      else
	sprintf(ipstr, "421 Cannot open data connection.\r\n");
      //////here
      WRITESOCKET(fd, ipstr);
    }
  else if (!user->is_logged)
    WRITESOCKET(fd, "530 Please login with USER and PASS.\r\n");
  else
    WRITESOCKET(fd, "225 Data connection open; no transfer in progress.\r\n");
  return (0);
}

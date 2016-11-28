/*
** StructSocket.h for my_ftp in /home/saurs_f/PSU_2015_myftp/inc/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 11:45:15 2016 Florian Saurs
** Last update Thu May 12 19:56:09 2016 Saursinet
*/

#ifndef STRUCTSOCKET_H_
# define STRUCTSOCKET_H_

# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>

typedef struct			s_struct_socket
{
  struct protoent		*protocol_struct;
  int				fd;
  struct sockaddr_in		s_in;
  int				port;
}				t_struct_socket;

typedef struct			s_struct_client_socket
{
  struct sockaddr_in		s_in_client;
  socklen_t			s_in_size;
  int				client_fd;
  char				*client_ip;
}				t_struct_client_socket;

typedef struct			s_user
{
  int				is_good_name;
  int				is_logged;
  char				*login;
  char				pwd[256];
  char				home[256];
  int				is_passive;
  int				is_active;
  char				**word;
  t_struct_socket		sock;
  t_struct_client_socket	client;
}				t_user;

typedef struct			s_dico
{
  char				*word[15];
  int				(*ptr[14])(int, char *, t_user *);
  t_user			current_user;
}				t_dico;

typedef struct			s_client
{
  int				is_passive;
  int				is_active;
  t_struct_socket		sock;
  t_struct_client_socket	client;
  int				fd_file;
  int				fd_flux;
  int				fd_data;
}				t_client;

#endif /* !STRUCTSOCKET_H_ */

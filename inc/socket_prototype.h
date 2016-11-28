/*
** socket_prototype.h for my_ftp in /home/saurs_f/PSU_2015_myftp/inc/
**
** Made by Florian Saurs
** Login   <saurs_f@epitech.eu>
**
** Started on  Mon May  2 12:17:16 2016 Florian Saurs
** Last update Sat May 14 18:00:34 2016 Saursinet
*/

#ifndef SOCKET_PROTOTYPE_H_
# define SOCKET_PROTOTYPE_H_

# include "struct_socket.h"

/*
** main_server_read_and_write.c
*/

int	check_to_create_server(int *, t_client *, char *);
char	*read_and_write(char *, char [4096], int, t_client *);
int	end_communication(char [4096], int, int);

/*
** client_recv_send.c
*/

int	take_ip_addr_from_answer(char *, t_struct_socket *);
int	create_serveur_ip_port(char *, t_struct_socket *);
int	read_on_socket_data(int *, int, t_client *);
int	send_file_to_serv(int *, t_client *, int);
int	take_fd_from_client(int *);
/*
** get_next_line.c
*/

char	*get_next_line(const int);

/*
** init_close_socket.c
*/

int	bind_and_listen_server(t_struct_socket *, int);
int	init_socket(t_struct_socket *);
int	init_client_socket(t_struct_socket *, char *);
int	close_both_connections(int, int, int);
int	close_socket(int);

/*
** dico.c
*/

int	init_dico(t_dico *);
void	clear_dico(t_dico *);

/*
** login_functions.c
*/

int	login(int, char *, t_user *);
int	password(int, char *, t_user *);
int	change_working_directory(int, char *, t_user *);
int	change_working_directory_to_parent_directory(int, char *, t_user *);

/*
** transfer_functions.c
*/

int	transfer_active(int, char *, t_user *);
int	transfer_passive(int, char *, t_user *);
char	*take_addr(char *);
void	take_ip(char *, char *, t_user *);

/*
** file_action_functions.c
*/

int	upload_file(int, char *, t_user *);
int	download_file(int, char *, t_user *);
int	list_file(int, char *, t_user *);
int	delete_file(int, char *, t_user *);
int	print_working_directory(int, char *, t_user *);
int	accept_co_if_is_passive(t_user *, int);

/*
** helpful_functions.c
*/

int	help(int, char *, t_user *);
int	noop(int, char *, t_user *);
char	**fill_tab(char **);
int	user_okay(char *, char *, int);

/*
** utils.c
*/

void	put_path_to_user(char *, char *, char *);
int	change_path(char *, char *, char *);
int	is_directory(char *);
int	is_deletable(char *, char *, char *);

#endif /* !SOCKET_PROTOTYPE_H_ */

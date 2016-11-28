##
## Makefile for myftp in /home/saurs_f/PSU_2015_myftp
##
## Made by Saursinet
## Login   <saurs_f@epitech.net>
##
## Started on  Mon May  2 09:34:15 2016 Saursinet
## Last update Fri May 13 16:41:08 2016 Saursinet
##

CC		= gcc -g

RM		= rm -f

MAKEOBJ		= obj

CFLAGS		= -Wall -Werror -W

# DIRECTORIES PART

SRCDIR		= src/

OBJDIR		= obj/

INCDIR		= -I inc/


# SOURCES PART

# CLIENTSRCS	= main_client.c			\
# 		  get_next_line.c		\
# 		  init_close_socket.c		\
# 		  transfer_functions.c		\
# 		  main_client_read_and_write.c	\
# 		  client_recv_send.c

SERVERSRCS	= main_server.c			\
		  dico.c			\
		  file_action_functions.c	\
		  helpful_commands.c		\
		  login_functions.c		\
		  transfer_functions.c		\
		  utils.c			\
		  client_recv_send.c		\
		  list_and_print_dir.c		\
		  init_close_socket.c


# OBJS PART

# CLIENTOBJS	= $(addprefix $(OBJDIR), $(CLIENTSRCS:.c=.o))

SERVEROBJS	= $(addprefix $(OBJDIR), $(SERVERSRCS:.c=.o))

# BINARIES PART

SERVERNAME	= server

# CLIENTNAME	= client


# RULES

all:
		@make --no-print-directory $(SERVERNAME)

$(OBJDIR)%.o:	$(SRCDIR)%.c
		@mkdir -p $(MAKEOBJ)
		$(CC) $(CFLAGS) -o $@ -c $< $(INCDIR)

# $(CLIENTNAME): 	$(CLIENTOBJS)
# 		$(CC) $(CFLAGS) -o $(CLIENTNAME) $(CLIENTOBJS)

$(SERVERNAME): 	$(SERVEROBJS)
		$(CC) $(CFLAGS) -o $(SERVERNAME) $(SERVEROBJS)
clean:
	$(RM) $(SERVEROBJS)

fclean: clean
	$(RM) $(SERVERNAME)

re:	fclean all

.PHONY:	all clean fclean re

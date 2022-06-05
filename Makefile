NAME = ircserv

CMD_P = ./Commands/

CMD_SRCS = Command.cpp \
			Privmsg.cpp \
			Notice.cpp\
			Pass.cpp \
			Nick.cpp \
			User.cpp \
			Join.cpp \
			Names.cpp \
			Mode.cpp \
			Invite.cpp \
			Kick.cpp \
			Topic.cpp

OTHER_SRCS = Channel.cpp \
			Client.cpp \
			Server.cpp \
			ServerError.cpp \
			Socket.cpp \
			main.cpp \

CC = clang++

CFLAGS = -g -Wall -Wextra -Werror 

OBJS = $(SRCS:.cpp=.o)

SRCS = $(OTHER_SRCS) $(P_CMD_SRCS)

P_CMD_SRCS =	$(addprefix $(CMD_P), $(CMD_SRCS))

all :$(NAME)

$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 

%.o: %.cpp 
	$(CC) $(CFLAGS) -c $< -o $@ 

clean : 
		@rm -f $(OBJS)

fclean : clean
		@rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
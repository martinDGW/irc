NAME	= ircserv

CC		= c++ -g3 -Wall -Wextra -Werror -std=c++98

HEADER	= \
	./srcs/channel/Channel.hpp \
	./srcs/server/Server.hpp \
	./srcs/user/User.hpp \
	./srcs/includes/Basics.hpp \
	./srcs/includes/Typedef.hpp \
	./srcs/includes/Replies.hpp \

RM = rm -rf

PATH_TO_OBJS = ./objs/

PATH_TO_SRCS	= ./srcs/
PATH_TO_SERVER	= ./srcs/server/
PATH_TO_CMD		= ./srcs/commands/
PATH_TO_CHANNEL	= ./srcs/channel/
PATH_TO_USER	= ./srcs/user/

SRCS_WITHOUT_PATH = \
			main.cpp \
			strtolower.cpp \
			dateStr.cpp

CMD_WITHOUT_PATH =		\
			Nick.cpp	\
			Who.cpp		\
			Mode.cpp	\
			Pass.cpp	\
			Join.cpp	\
			Invite.cpp	\
			Kick.cpp	\
			Privmsg.cpp	\
			User_cmd.cpp\
			Quit.cpp 	\
			Part.cpp	\
			List.cpp	\
			Topic.cpp	\
			Whois.cpp

SERVER_WITHOUT_PATH = \
			Server.cpp \
			Parsing.cpp

CHANNEL_WITHOUT_PATH = \
			Channel.cpp

USER_WITHOUT_PATH = \
			User.cpp

SRCS	=	$(addprefix $(PATH_TO_SRCS), $(SRCS_WITHOUT_PATH))
CMD = $(addprefix $(PATH_TO_CMD), $(CMD_WITHOUT_PATH))
SERVER = $(addprefix $(PATH_TO_SERVER), $(SERVER_WITHOUT_PATH))
CHANNEL = $(addprefix $(PATH_TO_CHANNEL), $(CHANNEL_WITHOUT_PATH))
USER = $(addprefix $(PATH_TO_USER), $(USER_WITHOUT_PATH))

OBJS_SRCS_WITHOUT_PATH = $(SRCS_WITHOUT_PATH:.cpp=.o)
OBJS_CMD_WITHOUT_PATH = $(CMD_WITHOUT_PATH:.cpp=.o)
OBJS_SERVER_WITHOUT_PATH = $(SERVER_WITHOUT_PATH:.cpp=.o)
OBJS_CHANNEL_WITHOUT_PATH = $(CHANNEL_WITHOUT_PATH:.cpp=.o)
OBJS_USER_WITHOUT_PATH = $(USER_WITHOUT_PATH:.cpp=.o)

OBJS_SRCS = $(addprefix $(PATH_TO_OBJS), $(OBJS_SRCS_WITHOUT_PATH))
OBJS_CMD = $(addprefix $(PATH_TO_OBJS), $(OBJS_CMD_WITHOUT_PATH))
OBJS_SERVER = $(addprefix $(PATH_TO_OBJS), $(OBJS_SERVER_WITHOUT_PATH))
OBJS_CHANNEL = $(addprefix $(PATH_TO_OBJS), $(OBJS_CHANNEL_WITHOUT_PATH))
OBJS_USER = $(addprefix $(PATH_TO_OBJS), $(OBJS_USER_WITHOUT_PATH))


OBJS = \
		$(OBJS_SRCS) \
		$(OBJS_CMD) \
		$(OBJS_SERVER) \
		$(OBJS_CHANNEL) \
		$(OBJS_USER) \

	###### COLORS FOR PRINTF ######
GREEN 		= \033[0;92m
RED			= \033[0;91m
BLUE		= \033[0;94m
YELLOW      = \033[0;93m
WHITE       = \033[1;49;97m
RESET		= \033[0m
	###############################

all : $(PATH_TO_OBJS) $(NAME)
	printf "\n$(GREEN)✅ $(NAME) is up to date$(WHITE) 👏\n$(RESET)"

$(PATH_TO_OBJS):
	mkdir -p ./objs/

$(OBJS_SRCS):$(PATH_TO_OBJS)%.o	: $(PATH_TO_SRCS)%.cpp Makefile $(HEADER)
	printf "\033[2K\r$(YELLOW)⏳ Compiling:$(WHITE) $< 🤞$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJS_CMD):$(PATH_TO_OBJS)%.o	: $(PATH_TO_CMD)%.cpp Makefile $(HEADER)
	printf "\033[2K\r$(YELLOW)⏳ Compiling:$(WHITE) $< 🤞$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJS_SERVER):$(PATH_TO_OBJS)%.o	: $(PATH_TO_SERVER)%.cpp Makefile $(HEADER)
	printf "\033[2K\r$(YELLOW)⏳ Compiling:$(WHITE) $< 🤞$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJS_CHANNEL):$(PATH_TO_OBJS)%.o	: $(PATH_TO_CHANNEL)%.cpp Makefile $(HEADER)
	printf "\033[2K\r$(YELLOW)⏳ Compiling:$(WHITE) $< 🤞$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJS_USER):$(PATH_TO_OBJS)%.o	: $(PATH_TO_USER)%.cpp Makefile $(HEADER)
	printf "\033[2K\r$(YELLOW)⏳ Compiling:$(WHITE) $< 🤞$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@ 

$(NAME) : $(OBJS)
		$(CC) $(OBJS) -o $@

%.o : %.cpp Makefile $(HEADER)
		$(CC) -c -o $@ $<

clean :
		$(RM) $(PATH_TO_OBJS)
		printf "$(RED)👌 Files has been deleted in $(PATH_TO_OBJS) ($(NAME))$(WHITE)\n$(RESET)"  
		printf "$(GREEN)✅ Clean 👍$(WHITE)\n$(RESET)"

fclean : clean
		printf "$(RED)👌 $(NAME) has been deleted$(WHITE)\n$(RESET)"
		$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re
.SILENT:

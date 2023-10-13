/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 19:07:19 by tde-vlee          #+#    #+#             */
/*   Updated: 2023/06/06 17:03:04 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* ****************************************************************************
-----------------------------------INCLUDES-----------------------------------
* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <algorithm>

#include "../includes/Basics.hpp"
#include "../channel/Channel.hpp"
#include "../user/User.hpp"
#include "../includes/Typedef.hpp"

/* ****************************************************************************
------------------------------------COLORS------------------------------------
* ************************************************************************** */

#define RESCOL			"\x1b[0m"

#define WHITE			"\x1B[37m"

#define STORM_BLUE		"\x1b[38;5;12m"
#define CYAN			"\x1B[36m"

#define GREEN			"\x1B[32m"

#define POOL_GREEN		"\x1b[38;5;50m"

#define BLOOD_RED		"\x1b[38;5;125m"

#define PASTEL_YELLOW	"\x1b[38;5;229m"

#define MAGENTA			"\x1b[35m"
#define PASTEL_PINK		"\x1b[38;5;213m"
#define PASTEL_PURPLE	"\x1b[38;5;147m"
#define PARMA			"\x1b[38;5;105m"

#define SERVERNAME "MyServer"
#define MAX_QUEUE 10
#define MAX_CHANNEL 10

/* ****************************************************************************
-------------------------------------CLASS-------------------------------------
* ************************************************************************** */

class Server
{
	private:
		std::string			_servername;
		std::string			_serverpass;
		int					_pollfd_idx;
		struct sockaddr_in	_address;
		int					_addrlen;
		usrmap 				_user_map;
		usrmap 				_queued_user_map;
		chmap				_channel_map;
		funmap				_functions_map;
		pollarray			_all_pollfds;

		short	stop(std::string);
		void	handle_user_message(int user_fd, char buf[512], ssize_t recv_ret, size_t i);
		static void	sighandler(int signal);

	public:
		static bool	loop;

		Server(std::string port, std::string password );
		Server(const Server& ref);
		~Server();
		Server& operator=(const Server& ref);

		const std::string	&getServerName(void) const;
		const struct pollfd	&getPollfd(void) const;
		struct sockaddr_in	getAdress(void) const;
		int					getAddrlen(void) const;
		const usrmap& 		getUser_map(void) const;
		const chmap&		getChannel_map(void) const;
		const pollarray&	getAll_pollfds(void) const;
		int					getTargetSock(const std::string &) const;
		Channel 			*getChannel(std::string &name) const;
		User				*getUser(std::string &) const;

/* ********************************FUNCTIONS********************************** */
		void	run(void);
		void	servaccept(void);

		void	addConnectedUser(User *user);
		void	addQueuedUser(User *user);

		void		removeUser(int fd, size_t pollfd_idx);
		Channel*	addChannel(std::string name);
		void		removeChannel(std::string name);

		void	senddm(User& user, std::string msg) const;
		void	send_to_channel(User&, Channel&, std::string) const;
		void	send_msg(int sock, std::string msg) const;
		
		void	chanMode(User &, Channel &, std::vector<std::string>);
		std::vector<std::string>	setVec(std::string &)const ;
		std::vector<std::vector<std::string> >	setVecVec(std::string &) const;
		std::vector<t_pres>	parsing(User &, std::vector<std::vector<std::string> >);

		//===========BOOLEANS===========//
		bool	userExist(const std::string &) const;
		bool	userQeued(const int) const;
		bool	chanExist(const std::string &) const;
		bool	isInChan(std::string, std::string) const;

		//==========FUNCTIONS==========//
		void	pass(User &, std::string &);
		void	nick(User &, std::string &);
		void	user(User &, std::string &);
		void	join(User &, std::string &);
		void	quit(User &, std::string &);
		void	part(User &, std::string &);
		void	privmsg(User &, std::string &);
		void	invite(User &, std::string &);
		void	list(User &, std::string &);
		void	who(User &, std::string &);
		void	mode(User &, std::string &);
		void	kick(User &, std::string &);
		void	ban(User &user, std::string &str);
		void	topic(User &, std::string &);
		void	whois(User &, std::string &);

/* ********************************EXCEPTIONS********************************* */

		class CreatingSocketFailure : public std::exception
		{
			public:
				virtual const char* what() const throw() {return (PARMA "Error while creating socket." RESCOL);}
		};
		class BindingSocketFailure : public std::exception
		{
			public:
				virtual const char* what() const throw() {return (PARMA "Error while binding socket." RESCOL);}
		};
		class ListenFailure : public std::exception
		{
			public:
				virtual const char* what() const throw() {return (PARMA "Error while trying to listen on the server socket." RESCOL);}
		};
		class WrongPortFormat : public std::exception
		{
			public:
				virtual const char* what() const throw() {return (PARMA "Wrong port format." RESCOL);}
		};
		class ConnectionFailure : public std::exception
		{
			public:
				virtual const char* what() const throw() {return (PARMA "Accept connection failed." RESCOL);}
		};
};

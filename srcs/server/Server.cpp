/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 19:03:15 by tde-vlee          #+#    #+#             */
/*   Updated: 2023/06/06 17:04:01 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <csignal>
#include "Server.hpp"

bool	Server::loop = true;


Server::Server(std::string strport, std::string password)
	: _servername(SERVERNAME), _serverpass(password)
{
	struct pollfd pollfd;

	pollfd.events = POLLIN;
	pollfd.fd = socket(AF_INET, SOCK_STREAM, 0);
	int reuse = 1;
	setsockopt(pollfd.fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	if (pollfd.fd == -1)
		throw CreatingSocketFailure();
	this->_address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_address.sin_family = AF_INET;
	this->_address.sin_port = htons(this->stop(strport));
	this->_addrlen = sizeof(this->_address);
	if (bind(pollfd.fd, (struct sockaddr *)&this->_address, this->_addrlen))
		throw BindingSocketFailure();
	if (listen(pollfd.fd, MAX_QUEUE))
		throw ListenFailure();
	signal(SIGINT, Server::sighandler);
	this->_all_pollfds.clear();
	this->_all_pollfds.push_back(pollfd);
	this->_pollfd_idx = 0;
	this->_functions_map.insert(std::pair<std::string , server_method_t>("PASS", &Server::pass));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("NICK", &Server::nick));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("USER", &Server::user));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("JOIN", &Server::join));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("WHO", &Server::who));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("MODE", &Server::mode));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("QUIT", &Server::quit));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("PART", &Server::part));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("PRIVMSG", &Server::privmsg));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("INVITE", &Server::invite));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("KICK", &Server::kick));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("TOPIC", &Server::topic));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("LIST", &Server::list));
	this->_functions_map.insert(std::pair<std::string , server_method_t>("WHOIS", &Server::whois));
	std::cout << "Server is listening on port : " << strport << std::endl;
}

Server::Server(const Server& ref)
	: _servername(ref.getServerName()), _address(ref.getAdress()), _addrlen(ref.getAddrlen())
{
	this->_user_map = ref.getUser_map();
	this->_channel_map = ref.getChannel_map();
	this->_all_pollfds = ref.getAll_pollfds();
	this->_pollfd_idx = 0;
}

Server& Server::operator=(const Server& ref)
{
	if (this == &ref)
		return (*this);
	_address = ref.getAdress();
	_addrlen = ref.getAddrlen();
	this->_user_map = ref.getUser_map();
	this->_channel_map = ref.getChannel_map();
	this->_all_pollfds = ref.getAll_pollfds();
	this->_pollfd_idx = 0;
	return (*this);
}

/* ****************************************************************************
-----------------------------------DESTRUCTOR---------------------------------
* ************************************************************************** */

Server::~Server()
{
	while (this->_all_pollfds.size() > 1)
	{
		this->removeUser(this->_all_pollfds[1].fd, 1);
	}
	this->_user_map.clear();
	this->_channel_map.clear();
	close(this->_all_pollfds[0].fd);
}

//====================GETTER====================

const std::string	&Server::getServerName(void) const
{
	return (this->_servername);
}

const struct pollfd	&Server::getPollfd(void) const
{
	return (this->_all_pollfds.at(this->_pollfd_idx));
}

struct sockaddr_in	Server::getAdress(void) const
{
	return (this->_address);
}

int	Server::getAddrlen(void) const
{
	return (this->_addrlen);
}

const usrmap&	Server::getUser_map(void) const
{
	return (this->_user_map);
}

const chmap&	Server::getChannel_map(void) const
{
	return (this->_channel_map);
}

const pollarray&	Server::getAll_pollfds(void) const
{
	return (this->_all_pollfds);
}


Channel *Server::getChannel(std::string& name) const
{
	chmap::const_iterator it = _channel_map.find(name);
	if (it != _channel_map.end())
		return it->second;
	return NULL;
}

User	*Server::getUser(std::string &name) const {
	if (this->userExist(name)) {
		usrmap::const_iterator it = this->_user_map.find(this->getTargetSock(name));
		return (it->second);
	}
	return (NULL);
}

/* ****************************************************************************
---------------------------------PRIVATES METHODS------------------------------
* ************************************************************************** */


void	Server::sighandler(int signal)
{
	switch (signal)
	{
		case SIGINT :
			Server::loop = false;
			break;
	}
}


short Server::stop(std::string strport)
{
	short	port;
	size_t	idx;

	port = 0;
	idx = 0;
	if (strport.length() > 4 || strport.find_first_not_of("0123456789") != std::string::npos)
		throw WrongPortFormat();
	while (strport.length() != idx)
	{
		port = port * 10;
		port += strport.at(idx) - '0';
		idx++;
	}
	return (port);
}

/* ****************************************************************************
---------------------------------PUBLIC METHODS--------------------------------
* ************************************************************************** */

void	Server::addQueuedUser(User *user)
{
	this->_queued_user_map.insert(std::pair<int, User*>(user->getSocket_fd(), user));
}

void	Server::addConnectedUser(User *user)
{
	this->_user_map.insert(std::pair<int, User*>(user->getSocket_fd(), user));
}

Channel*	Server::addChannel(std::string channel) {
	Channel	*chan;

	chan = new Channel(channel);
	this->_channel_map.insert(std::pair<std::string, Channel *>(channel, chan));
	return (chan);
}

void Server::removeChannel(std::string name)
{
	if (this->chanExist(name))
	{
		chmap::iterator it = this->_channel_map.find(name);
		it->second->clearChan();
		delete it->second;
		this->_channel_map.erase(it);
	}
}

void	Server::removeUser(int fd, size_t pollfd_idx)
{
	usrmap::const_iterator usr_it = this->_user_map.find(fd);

	if (usr_it != this->_user_map.end()) {
		usr_it = this->_user_map.find(fd);
		while (usr_it->second->chan_joined) {
			for (chmap::iterator ch_it = this->_channel_map.begin(); ch_it != this->_channel_map.end(); ch_it++) {
				if (ch_it->second->userExist(usr_it->second->getNickname()))
				{
					ch_it->second->removeUser(usr_it->second->getNickname());
					if (ch_it->second->getUserCount() == 0) {
						this->removeChannel(ch_it->first);
						break ;
					}
				}
			}
		}
	}
	else
		usr_it = this->_queued_user_map.find(fd);
	close(fd);
	delete usr_it->second;
	this->_user_map.erase(fd);
	this->_all_pollfds.erase(this->_all_pollfds.begin() + pollfd_idx);
}

void	Server::servaccept(void)
{
	struct pollfd	pollfd;
	struct sockaddr	addr;

	pollfd.fd = accept(this->_all_pollfds[0].fd, &addr, reinterpret_cast<socklen_t*>(&this->_addrlen));
	if (pollfd.fd == -1)
		throw ConnectionFailure();
	pollfd.events = POLLIN;
	this->_all_pollfds.push_back(pollfd);
	this->addQueuedUser(new User("", pollfd.fd, addr));
}

void	Server::handle_user_message(int user_fd, char buf[512], ssize_t recv_ret, size_t i)
{
	std::vector<t_pres>	pr;

	if (recv_ret > 0)
	{
		usrmap::const_iterator it = this->_user_map.find(user_fd);
		std::string msg(buf, recv_ret);
		if (it != this->_user_map.end())
		{
			pr = this->parsing(*it->second, this->setVecVec(msg));
			for (std::vector<t_pres>::iterator parse  = pr.begin(); parse != pr.end(); parse ++)
				(this->*(parse->func))(*it->second, parse->params);
			return;
		}
		it = this->_queued_user_map.find(user_fd);
		if (it != this->_queued_user_map.end())
		{
			pr = this->parsing(*it->second, this->setVecVec(msg));
			for (std::vector<t_pres>::iterator parse  = pr.begin(); parse != pr.end(); parse ++)
			{
				if (it->second->getPassStatus() == 0 && parse->mname != "PASS")
				{
					this->send_msg(it->second->getSocket_fd(), ERR_NOTREGISTERED(this->_servername, "PASS"));
					return ;
				}
				else if (it->second->getPassStatus() == 1 && it->second->getNickname().empty() && parse->mname != "NICK")
				{
					this->send_msg(it->second->getSocket_fd(), ERR_NOTREGISTERED(this->_servername, "NICK"));
					return ;
				}
				else if (it->second->getPassStatus() == 1 && !it->second->getNickname().empty() && parse->mname != "USER")
				{
					this->send_msg(it->second->getSocket_fd(), ERR_NOTREGISTERED(this->_servername, "USER"));
					return ;
				}
				(this->*(parse->func))(*it->second, parse->params);
			}
			return;
		}
	}
	else
	{
		this->removeUser(user_fd, i);
	}
}

void	Server::run()
{
	char	buf[512];
	ssize_t	recv_ret;
	int		user_fd;

	while (this->loop)
	{
		if (poll(this->_all_pollfds.data(), this->_all_pollfds.size(), 100) == -1)
		{
			std::cerr << "Poll stop" << std::endl;
			break;
		}
		if (this->_all_pollfds.at(0).revents & POLLIN)
		{
			this->servaccept();
		}
		for (size_t i = 1; i < this->_all_pollfds.size(); i++)
		{
			if (this->_all_pollfds[i].revents & POLLIN)
			{
				user_fd = this->_all_pollfds[i].fd;
				recv_ret = recv(user_fd, buf, 512, 0);
				this->handle_user_message(user_fd, buf, recv_ret, i);
			}
		}
	}
}

void	Server::send_msg(int sock, std::string msg) const
{
	msg = msg + "\r\n";
	send(sock, msg.data(), msg.length(), 0);
}

void	Server::send_to_channel(User& user, Channel& channel, std::string msg) const {
	if (channel.userExist(user.getNickname())) {
		channel.send_msg_except_one(user.getNickname(), msg);
	}
	else
		this->send_msg(user.getSocket_fd(), ERR_NOTONCHANNEL(user.getRealname(), user.getNickname(), channel.getName()));
}

int	Server::getTargetSock(const std::string &target) const {
	for (std::map<int, User *>::const_iterator it = this->_user_map.begin(); it != this->_user_map.end(); it++) {
		if (it->second->getNickname() == target)
			return (it->second->getSocket_fd());
	}
	return (-1);
}

//============BOOLEANS=============//

bool	Server::userExist(const std::string &target) const {
	for(usrmap::const_iterator it = this->_user_map.begin(); it != this->_user_map.end(); it++) {
		if (it->second->getNickname() == target)
			return (true);
	}
	return (false);
}

bool	Server::userQeued(const int fd) const
{
	usrmap::const_iterator it;

	it = this->_queued_user_map.find(fd);
	if (it != this->_queued_user_map.end())
		return (true);
	return (false);
}

bool	Server::chanExist(const std::string &target) const {
	if (this->_channel_map.find(target) == this->_channel_map.end())
		return (false);
	return (true);
}

bool	Server::isInChan(std::string user, std::string channel) const {
	if (this->chanExist(channel)) {
		if (this->userExist(user)) {
			std::map<std::string, Channel *>::const_iterator it = this->_channel_map.find(channel);
			return (it->second->userExist(user));
		}
		return (false);
	}
	return (false);
}

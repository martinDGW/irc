/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cben-bar <cben-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 10:10:52 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/05/30 13:43:36 by cben-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/* ****************************************************************************
-----------------------------------CONSTRUCTORS--------------------------------
* ************************************************************************** */

User::User() {
	return ;
}

User::User(const std::string name, int socket_fd, struct sockaddr addr_in) : chan_joined(0), _nickname(name), _socket_fd(socket_fd), _addr_user(addr_in), _passcheck(0)
{
	this->setRealname(_nickname);
	return ;
}

User::User(const User &rhs) {
	(void) rhs;
	return ;
}

User	&User::operator=(const User &rhs) {
	(void) rhs;
	return *this;
}

User::~User()
{
	return ;
}

//=============GUETTER==============//

const std::string	&User::getNickname() const {
	return (this->_nickname);
}

const std::string	&User::getUserName() const {
	return (this->_username);
}

const std::string		&User::getRealname() const {
	return (this->_realname);
}

const int	&User::getSocket_fd() const
{
	return (this->_socket_fd);
}

const struct sockaddr	&User::getAddr(void) const
{
	return (this->_addr_user);
}

short	User::getPassStatus() const
{
	return (this->_passcheck);
}

//=============SETTER==============//

void	User::setNickname(const std::string &name) {
	this->_nickname = name;
	this->setRealname(name);
	return ;
}

void	User::setUsername(const std::string &username)
{
	this->_username = username;
}

void	User::setRealname(const std::string &name) {
	this->_realname = name + "!" + this->_username + "@127.0.0.1";
	return ;
}

void	User::setPassStatus(short status)
{
	this->_passcheck = status;
}

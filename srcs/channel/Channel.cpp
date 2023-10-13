/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 17:42:53 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/01 22:41:37 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#include "Channel.hpp"
#include "../includes/Basics.hpp"

/* ****************************************************************************
-----------------------------------CONSTRUCTORS--------------------------------
* ************************************************************************** */

Channel::Channel() {
	return ;
}

Channel::Channel(std::string &name) : _name(name), _mod("+"), _topic(""), _user_count(0), _max_users(-1) {
	this->_c_time = dateStr();
	return ;
}

Channel::Channel(const Channel &rhs) {
	*this = rhs;
	return ;
}

Channel	&Channel::operator=(const Channel &rhs) {
	(void)rhs;
	return (*this);
}

/* ****************************************************************************
-----------------------------------DESTRUCTOR---------------------------------
* ************************************************************************** */

Channel::~Channel() {
	return ;
}

/* ****************************************************************************
-------------------------------------GETTERS-----------------------------------
* ************************************************************************** */

const std::string	&Channel::getName() const
{
	return this->_name;
}

const std::string	&Channel::getMode() const {
	return (this->_mod);
}

const std::string	&Channel::getTopic() const
{
	return (this->_topic);
}

const std::string	&Channel::getCreationTime() const
{
	return (this->_c_time);
}

const int	&Channel::getUserCount() const
{
	return (this->_user_count);
}

const std::string	&Channel::getKey() const
{
	return (this->_key);
}

const int	&Channel::getMaxUsers() const
{
	return (this->_max_users);
}

const std::string	&Channel::getTopicDate() const {
	return (this->_topic_date);
}

User	&Channel::getUser(const std::string &target) const {
	return (*this->_user_map.at(target));
}

const std::map<std::string, User *>	&Channel::getUser_map(void) const
{
	return (this->_user_map);
}

const std::map<std::string, User *>	&Channel::getAdmin_map(void) const
{
	return (this->_admin_map);
}


/* ****************************************************************************
-------------------------------------SETTERS-----------------------------------
* ************************************************************************** */

void	Channel::setName(const std::string &str) {
	this->_name = str;
	return ;
}

void	Channel::setMod(const std::string &modChar) {
		this->_mod += modChar;
	return ;
}

void	Channel::setTopicDate(const std::string topicDate) {
	this->_topic_date = topicDate;
	return ;
}

void	Channel::setTopic(const std::string &topic)
{
	if (!this->_topic.empty())
		this->clearTopic();
	this->_topic = topic;
	return ;
}

void	Channel::setKey(const std::string &key)
{
	this->_key = key;
}

void	Channel::setMaxUsers(const int max)
{
	this->_max_users = max;
}

/* ****************************************************************************
---------------------------------PUBLIC METHODS--------------------------------
* ************************************************************************** */

std::string Channel::getString_list() const {
	std::string	list;

	for (std::map<std::string, User *>::const_iterator it = this->_user_map.begin(); it != this->_user_map.end(); it++) {
		if (this->isAdmin(it->first))
			list += "@";
		list += it->first + " ";
	}
	return (list);
}

void	Channel::addUser(User &newUser) {
	if (!this->userExist(newUser.getNickname())) {
		if (this->_user_map.size() < 1)
			this->_admin_map.insert(std::pair<std::string, User *>(newUser.getNickname(), &newUser));
		this->_user_map.insert(std::pair<std::string, User *>(newUser.getNickname(), &newUser));
		this->_user_count++;
		newUser.chan_joined += 1;
	}
}

void	Channel::addAdmin(User &newAdmin) {
	if (this->userExist(newAdmin.getNickname()) && !this->isAdmin(newAdmin.getNickname())) 
			this->_admin_map.insert(std::pair<std::string, User *>(newAdmin.getNickname(), &newAdmin));
}

void	Channel::removeAdmin(User &user, std::string &target) {
	if (this->isAdmin(user.getNickname())) {
		std::map<std::string, User *>::iterator it = this->_admin_map.find(target);
		if (it != this->_admin_map.end()) {
			this->_admin_map.erase(it);
		}
	}
}

void	Channel::removeUser(const std::string &uname) {
	if (this->userExist(uname)) {
		if (this->_admin_map.find(uname) != this->_admin_map.end())
			this->_admin_map.erase(this->_admin_map.find(uname));
		std::map<std::string, User *>::iterator it = this->_user_map.find(uname);
		it->second->chan_joined -= 1;
		this->_user_map.erase(it);
		this->_user_count--;
	}
}

void	Channel::addRemoveMode(char mode, char pm) {
	if (pm == '+' && !this->isMode(mode)) {
		this->addMode(mode);
	}
	else if(pm == '-' && this->isMode(mode)) {
		this->removeMode(mode);
	}
}

void	Channel::addInviteList(const std::string &invited) {
	if (!this->isInvited(invited))
		this->_invite_list.push_back(invited);
}

void	Channel::removeInviteList(const std::string &removed) {
	if (this->isInvited(removed)) {
		std::vector<std::string>::iterator it = this->_invite_list.begin();
		while (*it != removed) {
			it++;
		}
		this->_invite_list.erase(it);
	}
}

void	Channel::addMode(char c) {
	this->_mod += c;
}

void	Channel::removeMode(char c) {
	for (size_t i = 0; i < this->_mod.length(); i++) {
		if (this->_mod[i] == c)
			this->_mod.erase(i);
	}
}

void	Channel::clearTopic() {
	this->_topic.clear();
	this->_topic_date.clear();
	return ;
}

void	Channel::clearChan() {
	this->_name.clear();
	this->_c_time.clear();
	this->_mod.clear();
	this->_topic.clear();
	this->_topic_date.clear();
	this->_key.clear();
	
	this->_user_map.clear();
	this->_admin_map.clear();
	this->_ban_map.clear();
	this->_invite_list.clear();
}

void	Channel::setKey(User &user, std::string &key) {
	if (this->isAdmin(user.getNickname()))
		this->_key = key;
}

void	Channel::removeKey(User &user) {
	if (this->isAdmin(user.getNickname()))
		this->_key.clear();
}

void	Channel::addRemoveMaxUsers(int val) {
	if (val > this->_user_count || val < 0)
		this->_max_users = -1;
	else
		this->_max_users = val;
}

// =====================BOOLEANS=======================//

bool	Channel::userExist(const std::string &name) const {
	if (this->_user_map.find(name) == this->_user_map.end())
		return (false);
	return (true);
}

bool	Channel::isAdmin(const std::string &target) const {
	if (this->_admin_map.find(target) != this->_admin_map.end())
		return (true);
	return (false);
}

bool	Channel::isBan(const std::string &target) const {
	for (std::map<std::string, User *>::const_iterator it = this->_ban_map.begin(); it != this->_ban_map.end(); it++) {
		if (it->first == target)
			return (true);
	}
	return (false);
}

bool	Channel::isMode(char c) const {
	if (this->_mod.find(c) == this->_mod.npos)
		return (false);
	return (true);
}

bool	Channel::isInvited(const std::string &name) const
{
	for (size_t idx = 0; idx < this->_invite_list.size(); idx++)
	{
		if (name == _invite_list[idx])
			return (true);
	}
	return (false);
}

bool	Channel::isTopic() const {
	if (this->_topic.empty())
		return (false);
	return (true);
}

// ========================================================//

void	Channel::rename(const std::string &actnick, std::string &newnick) {
	std::map<std::string, User *>::iterator it;

	if (this->userExist(actnick)) {
		it = this->_user_map.find(actnick);
		this->_user_map.insert(std::pair<std::string , User *>(newnick, it->second));
		this->_user_map.erase(it);
		this->send_msg_except_one(newnick, RPL_NICK(this->getUser(newnick).getRealname(), newnick));
	}
	if (this->isAdmin(actnick)) {
		it = this->_admin_map.find(actnick);
		this->_admin_map.erase(it);
		this->_admin_map.insert(std::pair<std::string , User *>(newnick, it->second));
	}
	if (this->isBan(actnick)) {
		this->_ban_map.insert(std::pair<std::string, User *>(newnick, this->_ban_map.at(actnick)));
		this->_ban_map.erase(actnick);
	}
	if (this->isInvited(actnick)) {
		for (std::vector<std::string>::iterator str_it = this->_invite_list.begin(); str_it != this->_invite_list.end(); str_it++)
		{
			if (*str_it == actnick)
			{
				*str_it = newnick;
				break;
			}
		}
	}
}

void	Channel::ban(User &user, const std::string &target) {
	if (!this->isAdmin(target)) {
		this->send_msg(RPL_BAN(user.getRealname(), target, this->getName()));
		this->send_msg(RPL_KICK(user.getRealname(), target, this->getName()));
		this->_ban_map.insert(std::pair<std::string, User *>(target, &this->getUser(target)));
		this->removeUser(target);
	}
}

void	Channel::unban(User &user, const std::string &target) {
	if (this->isBan(target)) {
		this->send_msg(RPL_UNBANTOCHAN(user.getRealname(), target, this->getName()));
		this->send_to_one(this->_ban_map.at(target)->getSocket_fd(), RPL_UNBANTOCHAN(user.getRealname(), target, this->getName()));
		this->_ban_map.erase(target);
	}
}

/* ****************************************************************************
---------------------------------PUBLIC SENDERS--------------------------------
* ************************************************************************** */

void	Channel::send_to_one(int socket, std::string str) const
{
	str = str + "\r\n";
	send(socket, str.data(), str.length(), 0);
}

void	Channel::send_msg(const std::string &str) const
{
	std::string s = str + "\r\n";

	for (std::map<std::string, User *>::const_iterator it = this->_user_map.begin(); it != this->_user_map.end(); it++) {
		send(it->second->getSocket_fd(), s.data(), s.length(), 0);
	}
}

void	Channel::send_msg_except_one(std::string sender, std::string msg) const
{
	msg = msg + "\r\n";
	for (std::map<std::string, User *>::const_iterator it = this->_user_map.begin(); it != this->_user_map.end(); it++)
	{
		if(it->second->getNickname() != sender)
			send(it->second->getSocket_fd(), msg.data(), msg.length(), 0);
	}
}

void	Channel::send_list(const int &userSock) const
{
	for (std::map<std::string, User *>::const_iterator it = this->_user_map.begin(); it != this->_user_map.end(); it++) {
		std::string name = it->second->getNickname();
		if (this->isAdmin(name))
			name = "@" + name;
		send(userSock, name.data(), name.length(), 0);
	}
}


void	Channel::printUserMap() const {
	for(std::map<std::string, User *>::const_iterator	it = this->_user_map.begin(); it != this->_user_map.end(); it++) {
		std::cout << it->first << std::endl;
	}
}

void	Channel::printAdminMap() const {
	for(std::map<std::string, User *>::const_iterator	it = this->_admin_map.begin(); it != this->_admin_map.end(); it++) {
		std::cout << it->first << std::endl;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 17:43:01 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/01 22:41:10 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

/* ****************************************************************************
-----------------------------------INCLUDES-----------------------------------
* ************************************************************************** */

# include "../includes/Basics.hpp"
# include "../includes/Typedef.hpp"
# include "../user/User.hpp"

# define USERCHANNELSLIMIT 5

/* ****************************************************************************
-------------------------------------CLASS-------------------------------------
* ************************************************************************** */

class Channel {
	public:
		Channel();
		Channel(std::string &);
		~Channel();

/* ********************************GETERS********************************** */

		const std::string	&getName() const;
		const std::string	&getMode() const;
		const std::string	&getTopicDate() const;
		const std::string	&getTopic() const;
		const std::string	&getCreationTime() const;
		const int			&getUserCount() const;
		const std::string	&getKey() const;
		const int			&getMaxUsers() const;
		User				&getUser(const std::string &target) const;

		const std::map<std::string, User *>	&getUser_map(void) const;
		const std::map<std::string, User *>	&getAdmin_map(void) const;


/* ********************************SETERS********************************** */

		void	setName(const std::string &);
		void	setMod(const std::string &);
		void	setTopic(const std::string &);
		void	setTopicDate(const std::string);
		void	setKey(const std::string &);
		void	setMaxUsers(const int);


/* *******************************BOOLEANS********************************* */

		bool	userExist(const std::string &) const;
		bool	isAdmin(const std::string &) const;
		bool	isBan(const std::string &) const;
		bool	isMode(char) const;
		bool	isTopic() const;
		bool	isInvited(const std::string &) const;

/* ******************************SENDERS********************************* */

		void	send_to_one(int socket, std::string str) const;
		void	send_msg(const std::string &str) const;
		void	send_msg_except_one(std::string sender, std::string msg) const;
		void	send_list(const int &) const;

/* ******************************FUNCTIONS********************************* */

		std::string getString_list() const;

		void	addUser(User &);
		void	removeUser(const std::string &);
		void	addAdmin(User &);
		void	removeAdmin(User &, std::string &);
		void	addMode(char);
		void	removeMode(char);
		void	addRemoveMode(char, char);
		void	addRemoveMaxUsers(int val);
		void	addInviteList(const std::string &);
		void	removeInviteList(const std::string &);


		void	ban(User &, const std::string &);
		void	unban(User &, const std::string &);

		void	rename(const std::string &, std::string &);

		void	clearTopic();
		void	setKey(User &, std::string &);
		void	removeKey(User &);

		void	clearChan();

		void	printUserMap() const;
		void	printAdminMap() const;

	private:
		std::string	_name;
		std::string	_c_time;
		std::string	_mod;

		std::string	_topic;
		std::string _topic_date;

		std::string _key;
		int			_user_count;
		int			_max_users;

		std::map<std::string, User *>	_user_map;
		std::map<std::string, User *>	_admin_map;
		std::map<std::string, User *>	_ban_map;
		std::vector<std::string>		_invite_list;

		Channel(const Channel &);
		Channel	&operator=(const Channel &);
};

#endif

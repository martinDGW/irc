/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 10:10:03 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/05/31 18:03:27 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

/* ****************************************************************************
-----------------------------------INCLUDES-----------------------------------
* ************************************************************************** */

# include "../includes/Basics.hpp"

# define CHANLIMIT 5

/* ****************************************************************************
-------------------------------------CLASS-------------------------------------
* ************************************************************************** */

class User {
	public:
		User(const std::string, int, struct sockaddr);
		~User();

		const std::string		&getNickname(void) const;
		const std::string		&getUserName() const;
		const std::string		&getRealname() const;
		const int				&getSocket_fd(void) const;
		const struct sockaddr	&getAddr(void) const;
		short 					getPassStatus() const;

/* ********************************SETERS********************************** */

		void				setNickname(const std::string &);
		void				setUsername(const std::string &);
		void				setRealname(const std::string &name);

		void				setPassStatus(short);

/* ********************************BOOLEANS********************************** */

		int	chan_joined;

	private:
		std::string		_nickname;
		std::string		_username;
		std::string		_realname;
		int				_socket_fd;
		struct sockaddr	_addr_user;
		short			_passcheck;

		User();
		User(const User &);
		User	&operator=(const User &);
};

#endif

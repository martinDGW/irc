/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 03:30:52 by tde-vlee          #+#    #+#             */
/*   Updated: 2023/05/22 21:29:53 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

class Server;
class Channel;
class User;

typedef void (Server::*server_method_t)(User &, std::string &);
typedef std::map<int , User *> usrmap;
typedef std::map<std::string , Channel *> chmap;
typedef std::vector<struct pollfd> pollarray;
typedef std::map<std::string , server_method_t> funmap;

typedef struct s_pres
{
	std::string		mname;
	server_method_t	func;
	std::string		params;
}	t_pres;

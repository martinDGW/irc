/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 13:34:59 by tde-vlee          #+#    #+#             */
/*   Updated: 2023/05/30 14:41:04 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <limits>
#include <cstdlib>
#include "./server/Server.hpp"
#include "./user/User.hpp"

int checkparams(int argc, char **argv)
{
	std::string	port;

	if (argc != 3)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return (1);
	}
	port = argv[1];
	if (port.length() > 4 || port.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cerr << "Wrong port format" << std::endl;
		return (1);
	}
	if (std::atoi(port.data()) < 1025)
	{
		std::cerr << "Port number is too low, port number should be superior at 1024" << std::endl;
		return (1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (checkparams(argc, argv))
		return (1);
	Server server(argv[1], argv[2]);
	server.run();
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:25:51 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/01 16:32:00 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"

std::string	&stringTraiter(std::string &str) {

	if (str.length()) {
		size_t i = str.length() - 1;
		while (i > 0 && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
			i--;
		str.erase(i + 1);
		i = 0;
		while (i < str.length() && (str[i] == ' ' || (str[i] <= 9 && str[i] >= 13)))
			i++;
	}
	return (str);
}

std::vector<std::string>	Server::setVec(std::string &str) const {

	std::vector<std::string>	vec;

	str = stringTraiter(str);
	if (str.length() == 0)
		vec.push_back("null");
	else {
		std::string	cmd;

		cmd = str.substr(0, str.find(' '));
		try {
			if (cmd[0] != '#')
				this->_functions_map.at(cmd);
			vec.push_back(cmd);
			if (cmd != str)
				vec.push_back(str.substr(str.find(' ') + 1));
		}
		catch (std::exception &e) {
			vec.push_back("null");
			vec.push_back(str);
		}
	}
	if (vec[0] == "LIST" && vec.size() == 1)
		vec.push_back("");
	return (vec);
}

std::vector<std::vector<std::string> >	Server::setVecVec(std::string &str) const {
	
	std::vector<std::vector<std::string> >	vecVec;

	while (str.find("\r\n") != str.npos) {
		std::string cmd(str.substr(0, str.find("\r\n")));
		str = str.substr(str.find("\r\n") + 2 , str.npos);
		vecVec.push_back(this->setVec(cmd));
	}
	return (vecVec);
}

std::vector<t_pres>	Server::parsing(User &user, std::vector<std::vector<std::string> > vecVec)
{
	std::vector<t_pres>	pr;
	t_pres pr_temp;

	pr_temp.func = 0;
	for (size_t i = 0; i < vecVec.size(); i++) {
		std::vector<std::string> cmd(vecVec[i]);
		if (cmd[0] == "null") {
			if (cmd.size() <= 1)
				this->send_msg(user.getSocket_fd(), ERR_UNKNOWNERROR(user.getRealname(), user.getNickname(), "UNKNOWN"));
			else
				this->send_msg(user.getSocket_fd(), ERR_UNKNOWNCOMMAND(user.getRealname(), user.getNickname(), cmd[1].substr(0, cmd[1].find(' '))));
		}
		else {
			funmap::iterator it = this->_functions_map.find(cmd[0]);
			if (it != this->_functions_map.end()) {
				if (cmd.size() <= 1 || cmd[1] == "null") {
					this->send_msg(user.getSocket_fd(), ERR_NEEDMOREPARAMS(user.getRealname(), user.getNickname(), cmd[0]));
					continue;
				}
				pr_temp.mname = cmd[0];
				pr_temp.func = it->second;
				pr_temp.params = cmd[1];
				pr.push_back(pr_temp);
			}
		}
	}
	return (pr);
}

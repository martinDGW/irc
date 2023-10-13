/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dateStr.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:03:01 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/05/26 16:46:33 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/Basics.hpp"

std::string	dateStr() {

	time_t now = time(0);
	tm *timestamp = localtime(&now);

	char date[100];
	struct tm tm_time;
	strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", timestamp);

	strptime(date, "%Y-%m-%d %H:%M:%S" , &tm_time);
	time_t	seconds = mktime(&tm_time);

	std::stringstream ss;
    ss << seconds;
    return (ss.str());
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Init.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessabir <aessabir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 10:17:07 by aessabir          #+#    #+#             */
/*   Updated: 2023/07/08 16:38:20 by aessabir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_HPP_GARD
# define INIT_HPP_GARD
# include "Server.hpp"

/*

	init is a class to initial all servers 

	add_server() : adding all the severs exist in the config file
				to our vector (except form duplicate host and port)
	start_listening() : 

*/

class Init{
public:
	std::vector<Server> server;
	fd_set				masterRead;
	fd_set				masterWrite;
	int					max_Rsocket;
	int					max_Wsocket;

	Init(){
		FD_ZERO(&masterRead);
		FD_ZERO(&masterWrite);
		max_Rsocket = 0;
		max_Wsocket = 0;
	}
	void	add_server(Server& s){
		for (int i = 0; i < server.size(); ++i){
			if (server[i]._host == s._host && server[i]._port == s._port)
				return ;
		}
		server.push_back(s);
		FD_SET(s._socket, &masterRead);
		(s._socket > max_Rsocket) ? max_Rsocket = s._socket : max_Rsocket;
	}
	void	start_listening();
	void	read_socket(Server& s);
	void	write_socket(Server& s);
};

#endif
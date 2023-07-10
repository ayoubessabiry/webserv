/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Init.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessabir <aessabir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 10:17:07 by aessabir          #+#    #+#             */
/*   Updated: 2023/07/09 14:29:15 by aessabir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_HPP_GARD
# define INIT_HPP_GARD
# include "Server.hpp"
# include "parse_request.hpp"

/*

	init is a class to initial all servers 

	add_server() : adding all the severs exist in the config file
				to our vector (except form duplicate host and port)
	start_listening() : 

*/

class Init{
public:
	std::vector<Server>	server;
	std::vector<Client>	clients;
	std::string			response;
	fd_set				masterRead;
	fd_set				masterWrite;
	fd_set				read;
	fd_set				write;
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
		s.create_socket();
		server.push_back(s);
		FD_SET(s._socket, &masterRead);
		(s._socket > max_Rsocket) ? max_Rsocket = s._socket : max_Rsocket;
	}
	void	send_response(Client&);
	void	start_listening();
	void	read_socket(int ready_client);
	void	write_socket(int ready_client);
	int		wait_clients(fd_set ready, int max_sock);
	void	add_client(int new_client);
	void	get_rqst(int ready_client);
	void	send_rqst(int ready_client);

};

#endif
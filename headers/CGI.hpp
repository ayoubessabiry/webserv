/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessabir <aessabir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:33:54 by aessabir          #+#    #+#             */
/*   Updated: 2023/07/18 14:13:54 by aessabir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP_GARD
# define CGI_HPP_GARD
# include "Client.hpp"
# include "header.hpp"


class CGI{
public:
	std::map<std::string, std::string>	ENV;
	std::string							header;
	std::string							cgi_file_name;
	int									status;
	char								**env;

	bool		check_cgi(Client& client);
	void		set_env(request& request);
	void		exec_cgi(std::string);
	void		get_uri_info(std::string& uri);
	void		convert_map_to_char();
	bool		send_cgi_response(Client& clinet);
	std::string	get_file_name(std::string &uri);
};

#endif
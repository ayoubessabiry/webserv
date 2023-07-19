/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smounir <smounir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:28:49 by smounir           #+#    #+#             */
/*   Updated: 2023/06/12 14:28:49 by smounir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP
# include "webserv.hpp"

enum parsing_state
{
	SERVER = 1,
	LOCATION
};

struct location_block
{
	std::string					prefix;
	std::string					root;
	std::string					upload;
	std::string					cgi_exec;
	std::string					cgi_path;
	std::string					client_max_body_size;
	std::string					auto_index;
	std::vector<std::string>	redirect;
	std::vector<std::string>	indexes;
	std::vector<std::string>	methods;
};

struct server_block
{
	std::string					host;
	std::string					server_name;
	std::string					port;
	std::map<int, std::string>	error_path;
	std::vector<location_block>	locations;
};

struct webserver
{
	std::vector<server_block>	server_blocks;
	std::vector<std::string>	config_tokens;
	void						parse_server_block(std::string config_file_data);
	void						print_config_file();	// For debugging
	bool						parse_state;
};

#endif

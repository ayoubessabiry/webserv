/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smounir <smounir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 21:55:14 by smounir           #+#    #+#             */
/*   Updated: 2023/06/10 21:55:14 by smounir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parse_request.hpp"

void request::print_request()
{
	std::map<std::string, std::string>::iterator	it;

	std::cout << "\e[1;33m----------Request Line----------\n";
	if (!method.empty())
		std::cout << "\e[1;33m Method: \e[0;33m" << method << "\n";
	if (!uri.empty())
	{
		std::cout << "\e[1;33m Request Uri: \e[0;33m" << uri << "\n";
		std::cout << "\n\n";
	}

	for	(it = headers.begin(); it != headers.end() ; ++it)
	{
		std::cout << "\e[1;32m----------Header value----------\e[0;32m\n";
		std::cout << "\e[1;32mHeader: \e[0;32m|" << it->first << "|\n";
		std::cout << "\e[1;32mValue: \e[0;32m|" << it->second << "|\n";
	}

	if (!body.empty())
	{
		std::cout << "\e[1;35m---------Body------------\e[0;35m\n";
		std::cout << "\e[1;35m" << body;
	}
}

std::string	random_file_name()
{
	std::string alphas = "abcdefghijklmnopqrstuvwxyz123456789";
	std::string	file_name = "";

	srand(time(0));

	file_name += alphas[rand() % alphas.size()];
	file_name += alphas[rand() % alphas.size()];
	file_name += alphas[rand() % alphas.size()];
	file_name += alphas[rand() % alphas.size()];
	file_name += alphas[rand() % alphas.size()];

	return file_name;
}

size_t	convert_hex_to_decimal(std::string hex_number)
{
	int					num;
	std::stringstream	ss;

	ss << std::hex << hex_number;
	ss >> num;

	return num;
}

bool request::body_chunked_encoding(std::string &req)
{
	bool				chunking_done = false;

	std::size_t find = req.find("\r\n");
	std::string	hex_num = req.substr(0, find);

	size_t	chunk_size = convert_hex_to_decimal(hex_num);

	while (chunk_size)
	{
		
	}

	return chunking_done;
}

bool request::parse_request_data(std::string &appended_string, bool &is_reading_body)
{
	if (!strstr(appended_string.c_str(), "\r\n\r\n") && !is_reading_body)
		return false;

	if (!is_reading_body)
	{
		std::size_t find = appended_string.find("\r\n\r\n");
		std::string header_half = appended_string.substr(0, find);

		std::stringstream header_data(header_half);
		std::string	line;
		std::getline(header_data, line, '\r');

		std::stringstream first_line_data(line);

		std::getline(first_line_data, method, ' ');
		std::getline(first_line_data, uri, ' ');

		while (std::getline(header_data, line))
		{
			std::stringstream	field_data(line);
			std::string			value;
			std::string 		field;
			std::getline(field_data, field, ':');
			std::stringstream	field_space(field);
			std::getline(field_space, field, ' ');
			std::getline(field_data, value, ' ');
			std::getline(field_data, value, '\r');
			if (field != "")
				headers.insert(std::make_pair<std::string, std::string>(std::string(field), value));
		}
		appended_string = appended_string.substr(find, appended_string.size());
	}

	if (method != "GET" && method != "POST" && method != "DELETE" && !is_reading_body)
	{
		std::cout << "Here error\n";
		return true;
		status = "400";
	}
	if (method == "GET")
		return true;

	if (method == "POST")
	{
		is_reading_body = true;
	}

	return false;
}

bool	send_request(Client& client)
{
	bool	ended = false;

	if (client.is_reading_body)
		client.request_collector = "";
	client.request_collector += client.buff;

	ended = client.rqst.parse_request_data(client.request_collector, client.is_reading_body);

	if (!client.body_file_opened)
	{
		client.rqst.file_name = random_file_name();
		client.body_file_opened = true;
	}
	std::fstream	body_file;

	body_file.open(client.rqst.file_name, std::ios_base::binary|std::ios_base::out|std::ios_base::app);
	
	body_file << client.buff;
	if (client.is_reading_body)
	{
		std::fstream	body_file;

		body_file.open(random_file_name().c_str(), std::ios_base::binary|std::ios_base::out);
	
		body_file << client.buff;
		
		if (client.rqst.headers.count("Content-Length"))
		{
			client.rqst.body += client.request_collector;
			int	content_length;
			std::istringstream(client.rqst.headers["Content-Length"]) >> content_length;
			if (client.rqst.body.size() >= content_length)
			{
				ended = true;
			}
		}
		if (client.rqst.headers.count("Transfer-Encoding"))
		{
			ended = client.rqst.body_chunked_encoding(client.request_collector);
		}
	}

	if (ended)
	{
		client.rqst.print_request();
		client.is_reading_body = false;
		std::cout << "\nRequest Ended\n";

		client.request_collector = "";
	}

	return ended;
}

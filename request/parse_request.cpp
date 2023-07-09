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
	// if (!method.empty())
		std::cout << "\e[1;33m Method: \e[0;33m" << method << "\n";
	// if (!method.uri())
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
		std::cout << "\e[1;35m" << body.size();
	}
}

std::string request::random_file_name_generate()
{
	std::string name = "/tmp/";

	std::string char_list = "abcdefghijklmnopqrstuvzxyzABCDEFGHIJKLMNQRSTUVWXYZ";

	srand(time(0));

	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];
	name += char_list[rand() % char_list.size()];

	return name;
}

size_t	convert_hex_to_decimal(std::string hex_number)
{
	int					num;
	std::stringstream	ss;

	ss << std::hex << hex_number;
	ss >> num;

	return num;
}

bool request::body_chunked_encoding(std::string &body)
{
	bool				chunking_done = false;

	std::string body_line;
	std::stringstream	body_data(body);
	std::getline(body_data, body_line, '\r');
	size_t chunk_size = convert_hex_to_decimal(body_line);

	std::string	chunk_data = "";
	while (std::getline(body_data, body_line, '\r')) 
	{
		chunk_data += body_line;
		if (chunk_data.size() - 1 >= chunk_size)
		{
			body += chunk_data;
			chunk_data = "";
			std::getline(body_data, body_line, '\r');
			chunk_size = convert_hex_to_decimal(body_line);
		}
		chunking_done = (chunk_size == 0);
	}

	return chunking_done;
}

bool request::parse_request_data(std::string &req)
{
	bool	request_ended = false;

	if (!strstr(req.c_str(), "\r\n\r\n") && !is_reading_body)
		return false;

	std::size_t find = req.find("\r\n\r\n");

	std::string header_half = req.substr(0, find);

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

	if (method != "GET" || method != "POST" || method != "DELETE")
	{
		return true;
		status = "400";
	}
	if (method == "GET")
		return true;

	if (method == "POST")
	{
		is_reading_body = true;
		if (headers.count("Content-Length") >= 1)
		{
			std::cout << "Here" << std::endl;
			body += req;
			body_file << req;
			int	content_length;
			std::istringstream(headers["Content-Length"]) >> content_length;
			if (body.size() == content_length)
				return true;
		}
		if (headers.count("Transfer-Encoding"))
			return body_chunked_encoding(req) ;
	}

	return false;
}

request _request;

bool	send_request(Client& client)
{ 
	request _request;

	_request.body_file.open(_request.random_file_name_generate());
	_request.is_reading_body = false;
	_request.body = "";
	std::string		temp_string;
	std::string buffer = std::string(client.buff);

	if (_request.method == "POST")
		r_http = "";

	r_http += std::string(client.buff);

	std::cout << r_http << std::endl;

	bool ended = _request.parse_request_data(r_http);
	// if (ended)
	// 	std::cout << "Request Ended !!" << std::endl;
	// _request.print_request();

	return false;
}

// int main()
// {
// 	std::ofstream file;

// 	request rqst;

// 	// file.open(rqst.random_file_name_generate());
// 	file.open("ttt");

// 	file << "eret00000fghdtffgh";
// }

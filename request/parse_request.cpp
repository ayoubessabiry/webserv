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

#include "parse_request.hpp"

void request::print_request()
{
	std::map<std::string, std::string>::iterator	it;

	std::cout << "\e[1;33m----------Request Line----------\n";
	std::cout << "\e[1;33m Method: \e[0;33m" << method << "\n";
	std::cout << "\e[1;33m Request Uri: \e[0;33m" << uri << "\n";
	std::cout << "\n\n";

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

size_t	convert_hex_to_decimal(std::string hex_number)
{
	int					num;
	std::stringstream	ss;

	ss << std::hex << hex_number;
	ss >> num;

	return num;
}

bool	valid_method(std::string method)
{
	return method == "GET" | method == "POST" | method == "DELETE";
}

bool body_chunked_encoding(std::string body)
{
	bool				chunking_done = false;

	size_t find = body.find("\r\n\r\n");
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
			body_data << chunk_data.substr(0, 2 * chunk_data.size() - chunk_size);
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

	std::size_t find = req.find("\r\n\r\n");
	std::size_t find_sec = req.find("\r\n\r\n", find + 1);
	std::string header_half = req.substr(0, find);

	if (find >= req.size())
		body = req.substr(find, req.size() - 1);

	std::stringstream header_data(header_half);
	std::string	line;
	std::getline(header_data, line, '\r');

	std::stringstream first_line_data(line);

	std::getline(first_line_data, method, ' ');
	std::getline(first_line_data, uri, ' ');

	if (!valid_method(method))
	{
		status = "400";
		return false;
	}

	while (std::getline(header_data, line))
	{
		if (line.size() > 8192)
		{
			status = "400";
			return false;
		}
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

	if (body.empty() && method == "GET")
	{
		request_ended = true;
	}
	if (method == "POST")
	{
		if (headers.count("Content-Length") >= 1)
		{
			int	content_length;
			std::istringstream(headers["Content-Length"]) >> content_length;
			request_ended = (body.size() == content_length);
		}
		if (headers["Transfer-Encoding"] == "chunked")
			request_ended = body_chunked_encoding(body);
	}

	return request_ended;
}

bool	send_request(char *buff)
{ 
	std::string		r_http = "";
	// static std::ofstream request_file;
	std::string buffer = std::string(buff);

	// request_file.open("test", std::ios_base::app);

	r_http += std::string(buff);

	std::cout << r_http;

	// request_file << buff;
	// std::cout << request_file.rdbuf();
	// request _request;
	// bool ended = _request.parse_request_data(buffer);
	// _request.print_request();

	return false;
}

// int main(int ac, char **av)
// {

// }

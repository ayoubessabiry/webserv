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

// test if the parsing is working correctly
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

bool request::parse_request_data(std::string &req)
{
	bool	request_ended = false;

	std::size_t find = req.find("\r\n\r\n");
	std::size_t find_sec = req.find("\r\n\r\n", find + 1);
	std::string header_half = req.substr(0, find);

	if (find >= req.size())
		body = req.substr(find, req.size() - 1);

	// Retrieve method and uri
	std::stringstream header_data(header_half);
	std::string	line;
	std::getline(header_data, line, '\r');

	std::stringstream first_line_data(line);

	std::getline(first_line_data, method, ' ');
	std::getline(first_line_data, uri, ' ');

	// Retrieve Headers with there values
	while (std::getline(header_data, line))
	{
		std::stringstream	field_data(line);
		std::string			value;
		std::cout << "|" << line << "|\n";
		std::string field;
		std::getline(field_data, field, ':');
		std::stringstream	field_space(field);
		std::getline(field_space, field, ' ');
		std::getline(field_data, value, ' ');
		std::getline(field_data, value, '\r');
		if (field != "")
		headers.insert(std::make_pair<std::string, std::string>(std::string(field), value));
	}

	// Chunked transfer encoding
	if (body.empty() && method == "GET")
	{
		request_ended = true;
		std::cout << "Ended\n";
	}

	return request_ended;
}

// Check if request is done
bool	send_request(char *buff)
{
	std::fstream 	post_file;
	std::string buffer = std::string(buff);

	post_file.open("yekh", std::ios::in | std::ios::out);
	std::cout << buff << std::endl;
	// post_file << buff;

	request _request;
	bool ended = _request.parse_request_data(buffer);
	_request.print_request();

	return true;
}

// int main()
// {
// 	std::string POST_HTTP  = 
// 	"POST /echo/post/json HTTP/1.1\r\n"
// 	"Authorization: Bearer mt0dgHmLJMVQhvjpNXDyA83vA_Pxh33Y\r\n"
// 	"Accept: application/json\r\n"
// 	"Content-Type: application/json\r\n"
// 	"Content-Length: 85\r\n"
// 	"Host: reqbin.com\r\n"
// 	"\r\n\r\n"
// 	"{\r\n"
// 	"Id: 12345,\r\n"
// 	"Customer: John Smit,\r\n"
// 	"Quantity: 1,\r\n"
// 	"Price: 10.00\r\n"
// 	"}\r\n"
// 	"\r\n\r\n";

// 	bool	ended = false;

// 	ended = send_request((char*)POST_HTTP.c_str());

// }

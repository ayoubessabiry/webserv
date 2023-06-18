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
	std::cout << "\e[1;33m Method: \e[0;33m" << first_line.method << "\n";
	std::cout << "\e[1;33m Request Uri: \e[0;33m" << first_line.request_uri << "\n";
	std::cout << "\n\n";

	for	(it = headers.request_headers.begin(); it != headers.request_headers.end() ; ++it)
	{
		std::cout << "\e[1;32m----------Header value----------\e[0;32m\n";
		std::cout << "\e[1;32mHeader: \e[0;32m" << it->first << "\n";
		std::cout << "\e[1;32mValue: \e[0;32m" << it->second << "\n";
	}

	if (!body.empty())
	{
	}
}

bool request::parse_request_data(std::string &req)
{
	bool	request_ended = false;

	// Retrieve method and uri
	std::size_t find = req.find("\r\n");
	std::string line_one = req.substr(0, find);

	first_line.method = line_one.substr(0, req.find(" "));
	find = req.find(" ");
	std::size_t find_sec = req.find(" ", find + 1);
	first_line.request_uri = line_one.substr(find + 1, find_sec - find - 1);

	// Retrieve Headers with there values
	find = req.find("\r\n") + 2;
	find_sec = req.find("\r\n", find + 1, 2) - find;
	std::string line = req.substr(find, find_sec);
	line = req.substr(find, find_sec);
	std::string header;
	std::string header_value;
	if (line.size() >= 1)
	{
		header = line.substr(0, line.find(": "));
		header_value = line.substr(line.find(": ") + 2, line.size() - 1);
		headers.request_headers.insert(std::pair<std::string, std::string>
		(header, header_value));
	}
	while (1)
	{
		find = find_sec + find + 2;
		if (find == 1)
			break ;
		find_sec = req.find("\r\n", find + 1, 2) - find;
		line = req.substr(find, find_sec);
		if (line == "")
			request_ended = true;
		if (line.size() >= 1)
		{
			header = line.substr(0, line.find(": "));
			header_value = line.substr(line.find(": ") + 2, line.size() - 1);
			headers.request_headers.insert(std::pair<std::string, std::string>
			(header, header_value));
		}
	}

	// Retrieve the body if it's a POST method

	return request_ended;
}

// Check if request is done
bool	send_request(char *buff)
{
	std::fstream 	post_file;
	std::string buffer = std::string(buff);

	post_file.open("yekh", std::ios::in | std::ios::out);
	std::cout << "Request" << std::endl;
	std::cout << buff << std::endl;
	post_file << buff;
	post_file.flush();

	// request _request;
	// _request.print_request();

	// bool ended = _request.parse_request_data(buffer);

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
// 	"\r\n"
// 	"{\r\n"
// 	"Id: 12345,\r\n"
// 	"Customer: John Smit,\r\n"
// 	"Quantity: 1,\r\n"
// 	"Price: 10.00\r\n"
// 	"}\r\n"
// 	"\r\n\r\n";

// 	bool	ended = false;

// 	std::cout << std::boolalpha << ended;

// 	ended = send_request((char*)POST_HTTP.c_str());

// 	if (!ended)
// 		std::cout << "hehe";
// }

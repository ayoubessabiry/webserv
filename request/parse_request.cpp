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

size_t	convert_hex_to_decimal(std::string hex_number)
{
	int					num;
	std::stringstream	ss;

	ss << std::hex << hex_number;
	ss >> num;

	return num;
}

bool body_chunked_encoding(std::string body)
{
	std::ofstream		chunk_file;
	std::stringstream	chunk_stream(body);
	std::string			chunk_size;

	return false;
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
		std::cout << "Ended\n";
	}
	if (method == "POST")
	{
		if (headers.count("Content-Length") >= 1)
		{
			int	content_length;
			std::istringstream(headers["Content-Length"]) >> content_length;
			request_ended = (body.size() == content_length);
		}
		// Chunked transfer encoding : not done yet
		if (headers["Transfer-Encoding"] == "chunked")
			request_ended = body_chunked_encoding(body);
	}

	return request_ended;
}

// Check if request is done
bool	send_request(char *buff)
{
	std::string		request_buffer 
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

int main()
{
	std::string POST_HTTP  =
	"c\r\n"
    "<h1>go!</h1>\r\n"
    "1b\r\n"
    "<h1>first chunk loaded</h1>\r\n"
    "2a\r\n"
    "<h1>second chunk loaded and displayed</h1>\r\n"
	"29\r\n"
    "<h1>third chunk loaded and displayed</h1>\r\n"
    "0\r\n"
	"\r\n"
	;
	std::cout << convert_hex_to_decimal("c") << "\n";
	//body_chunked_encoding(POST_HTTP);
	// send_request((char*)POST_HTTP.c_str());

}
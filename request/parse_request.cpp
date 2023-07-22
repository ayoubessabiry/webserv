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
	std::string	file_name = "/tmp/";

	srand(time(0));

	for	(int i = 0 ; i < 13; i++)
		file_name += alphas[rand() % alphas.size()];

	return file_name;
}

int	convert_hex_to_decimal(std::string hex_number)
{
	int					num;
	std::stringstream	ss;

	ss << std::hex << hex_number;
	ss >> num;

	return num;
}

std::string url_encoding(std::string uri)
{
    std::string url = "";
    for (int i = 0 ; i < (int)uri.size(); i++)
    {
        
        if (uri[i] == ' ' || uri[i] == '!' 
			|| uri[i] == '\\' || uri[i] == '#' 
			|| uri[i] == '$' || uri[i] == '%' 
			|| uri[i] == '&' || uri[i] == '\"'
			|| uri[i] == '+'|| uri[i] == '*'
			|| uri[i] == ','|| uri[i] == ';'
			|| uri[i] == ':'|| uri[i] == '_'
			|| uri[i] == '('|| uri[i] == ')'
			|| uri[i] == '{'|| uri[i] == '}'
			|| uri[i] == '~'|| uri[i] == '^'
			|| uri[i] == '<'|| uri[i] == '>'
			|| uri[i] == '=')
        {
            std::stringstream    stream;
            stream << std::hex << (int)uri[i];
            url += "%" + std::string(stream.str());
        }
		else
        	url += uri[i];
    }
    return url;
}

bool request::body_chunked_encoding(std::string &req)
{
	is_reading_new_chunk_part = true;
	b_size += req.size();
	std::fstream	body_file;

	chunk_part += req;
	while (b_size)
	{
		if (!found_next_hexa)
		{
			if (strstr(chunk_part.c_str(), "\r\n"))
			{
				std::size_t find = chunk_part.find("\r\n");
				next_hex_saver = chunk_part.substr(0, find);
				chunk_saver = chunk_part.substr(find + 2, chunk_part.size());
				chunk_size = convert_hex_to_decimal(next_hex_saver);
				if (chunk_size == 0)
				{
					found_next_hexa = false;
					return true ;
				}
				found_next_hexa = true;
				is_reading_new_chunk_part = false;
				b_size -= (next_hex_saver.size() + 2);
			}
			else
				return false;
		}
		if (found_next_hexa)
		{
			body_file.open(file_name.c_str(), 
				std::ios_base::binary|std::ios_base::out|std::ios_base::app);

			if (is_reading_new_chunk_part)
			{
				b_size += chunk_saver.size();
				chunk_saver += req;
			}
			if ((int)chunk_saver.size() >= chunk_size)
			{
				std::string chunk = chunk_saver;
				chunk_part = chunk.substr(chunk_size + 2, chunk_saver.size());

				chunk_saver = chunk.substr(0, chunk_size);

				hole_chunk_size += chunk_saver.size();

				found_next_hexa = false;
				body_file.write(chunk_saver.c_str(), chunk_size);
			}
			b_size -= chunk_saver.size();
			if (b_size <= 0)
				b_size = 0;
			is_reading_new_chunk_part = false;
			body_file.close();
		}
	}
	return false;
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
		appended_string = appended_string.substr(find + 4, appended_string.size());
	}

	if (method != "GET" && method != "POST" && method != "DELETE" && !is_reading_body)
	{
		return true;
		status = "400";
	}
	if (method == "GET" || method == "DELETE")
		return true;

	if (method == "POST")
		is_reading_body = true;

	return false;
}


std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

bool	send_request(Client& client, std::string& buff)
{
	bool	ended = false;

	if (client.is_reading_body)
		client.request_collector = "";
	client.request_collector += buff;
	ended = client.rqst.parse_request_data(client.request_collector, client.is_reading_body);

	if (client.is_reading_body)
	{
		if (!client.body_file_opened)
		{
			client.rqst.file_name = random_file_name();
			client.body_file_opened = true;
		}
		std::fstream	body_file;

		body_file.open(client.rqst.file_name.c_str(), std::ios_base::binary|std::ios_base::out|std::ios_base::app);

		if (client.rqst.headers.count("Content-Length"))
		{
			body_file << client.request_collector;
			body_file.flush();
			int	content_length;
			std::istringstream(client.rqst.headers["Content-Length"]) >> content_length;
			if (filesize(client.rqst.file_name.c_str()) >= content_length)
			{
				ended = true;
			}
		}
		if (client.rqst.headers.count("Transfer-Encoding"))
		{
			ended = client.rqst.body_chunked_encoding(client.request_collector);
		}
		if (client.rqst.headers.count("Transfer-Encoding") && client.rqst.headers.count("Content-Length"))
		{
			ended = client.rqst.body_chunked_encoding(client.request_collector);
		}
	}

	if (ended)
	{
		client.rqst.print_request();
		client.is_reading_body = false;
		client.request_collector = "";
	}

	return ended;
}

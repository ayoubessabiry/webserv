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

std::string	random_file_name()
{
	std::string alphas = "abcdefghijklmnopqrstuvwxyz123456789";
	std::string	file_name = "";

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

bool request::body_chunked_encoding(std::string &req)
{
	int	body_chunk_size = req.size();

	is_reading_new_chunk_part = true;
	while(body_chunk_size)
	{
		if (!found_next_hexa)
		{
			if (is_reading_new_chunk_part)
				chunk_part += req;
			if (!strstr(chunk_part.c_str(), "\r\n"))
				return false;
			std::size_t find = chunk_part.find("\r\n");
			next_hex_saver = chunk_part.substr(0, find);

            found_next_hexa = true;
			chunk_size = covert_hex_to_decimal(chunk_part);
			if (chunk_size == 0)
			{
				found_next_hexa = false;
				return true ;
			}
            chunk_saver = chunk_part.substr(find + 2, chunk_part.size());
            is_reading_new_chunk_part = false;
			body_chunk_size -= next_hex_saver.size();
			body_chunk_size -= 2;
		}
		if (found_next_hexa)
		{
			std::fstream	body_file;
			body_file.open(file_name.c_str(), 
							std::ios_base::binary|std::ios_base::out|
							std::ios_base::app);

			if (is_reading_new_chunk_part)
				chunk_saver += req;
			if (chunk_saver.size() >= chunk_size)
			{
				std::string chunk = chunk_saver;
				chunk_part = chunk.substr(chunk_size, chunk_saver.size());
                chunk_saver = chunk.substr(0, chunk_size);
                body_file << chunk_saver;
                body_chunk_size -= chunk_saver.size();
				if (is_reading_new_chunk_part)
					body_chunk_size += req.size();
            	found_next_hexa = false;
			}
            else
                body_chunk_size -= chunk_saver.size();

			is_reading_new_chunk_part = false;
		
            if (body_chunk_size <= 0)
                body_chunk_size = 0;
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
			std::cout << "here" << std::endl;
			body_file << client.request_collector;
			int	content_length;
			std::istringstream(client.rqst.headers["Content-Length"]) >> content_length;
			if (client.rqst.body.size() >= content_length)
			{
				std::cout << content_length << " wana 3ndi " << client.rqst.body.size() << std::endl;
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
		//client.rqst.print_request();
		client.is_reading_body = false;
		std::cout << "\nRequest Ended\n";

		client.request_collector = "";
	}

	return ended;
}

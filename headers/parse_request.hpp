
#pragma once

# include <string.h>
# include <iostream>
# include <sstream>
# include <cctype>
# include <ctime>
# include <vector>
# include <cstring>
# include <string>
# include <cstdio>
# include <fstream>
# include <map>

struct request
{
	std::string		method;
	std::string		uri;
	std::string		body;
	std::string		status;
	std::string 	file_name;
	std::map<std::string, std::string>		headers;

	std::ofstream	body_file;

	bool			is_reading_body;

	std::string	random_file_name_generate();
	bool	parse_request_data(std::string&);
	bool	body_chunked_encoding(std::string&);
	void	print_request();
};

bool	valid_method(std::string body);


# include <string.h>
# include <iostream>
# include <cctype>
# include <ctime>
# include <vector>
# include <cstring>
# include <string>
# include <cstdio>
# include <fstream>
# include <map>

struct request_line
{
	std::string	method;
	std::string	request_uri;
};

struct header_fields
{
			 //Header	 //Header Value
	std::map<std::string, std::string>		request_headers;
	std::map<std::string, std::string>		general_headers;
	std::map<std::string, std::string>		representation_headers;
};

struct request
{
	request_line	first_line;
	header_fields	headers;
	std::string		body;
	std::string		status;

	bool parse_request_data(std::string&);
	void print_request();                   // For debugging the parsing
};


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
	std::map<std::string, std::string>		headers;

	bool	parse_request_data(std::string&);
	void	print_request();                   // For debugging the parsing
};

bool	body_chunked_encoding(std::string body);
bool	valid_method(std::string body);

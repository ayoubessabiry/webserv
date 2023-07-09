#ifndef CLIENT_HPP_GARD
# define CLIENT_HPP_GARD
# define MAX_REQUEST_SIZE 60000
# include "webserv.hpp"
# include "parse_request.hpp"

class request
{
	public:
		std::string		method;
		std::string		uri;
		std::string		body;
		std::string		status;
		std::string 	file_name;
		std::map<std::string, std::string>		headers;


		std::string	random_file_name_generate();
		bool	parse_request_data(std::string&, bool);
		bool	body_chunked_encoding(std::string&);
		void	print_request();
};

bool	valid_method(std::string body);


class Client  
{
public : 	
	socklen_t			addr_size;
	sockaddr_storage	client_add;
	std::string			request_collector;
	bool				is_reading_body;
	request				rqst;
	ssize_t				recv_byte;
	ssize_t				header_byte;
	ssize_t				body_byte;
	char				buff[MAX_REQUEST_SIZE];
	int					socket;
};

bool	send_request(Client& client);

#endif
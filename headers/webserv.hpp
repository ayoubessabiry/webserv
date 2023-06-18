#ifndef WEBSERV_GARD
# define WEBSERV_GARD

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
// # include <string.h>
// # include <iostream>
// # include <cctype>
// # include <ctime>
// # include <vector>
// # include <cstring>
// # include <string>
// # include <cstdio>
// # include <fstream>
// # include <map>
#include "../parse_request.hpp"


# define PORT "8080"
# define BACKLOG 10

bool	send_request(char	*buff);

#endif

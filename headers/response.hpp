#include "header.hpp"

class response
{

	std::map<int, std::string> httpStatusStrings;
	std::map<int, std::string> httpStatusFiles;
	std::string startLineString;
	std::string contentLength;
	std::string contentType;
	std::string fileName;
	std::string	date;
	std::string	responseHeaders;
	std::string resp;
    _MAP types_map;
	int StatusCode;

	void initHttpStatusStrings();
	void initHttpStatusFiles();
	void startLine(); //
	void calculateContentLength(); //
	bool canNotOpenFile(); //
	void setContentType(); //
	void setDate(); //

public:
	response();
	void setStatusCode(int s);
};
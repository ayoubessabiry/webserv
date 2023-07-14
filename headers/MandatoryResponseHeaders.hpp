#pragma once
#include "header.hpp"

class MandatoryResponseHeaders
{

protected:
	std::map<int, std::string> httpStatusStrings;
	std::map<int, std::string> httpStatusFiles;
	std::vector<std::string> allowedMethods;
	std::string locationRedirectionPath;
	std::string locationRedirectionHeader;
	std::string startLineString;
	std::string contentLength;
	std::string contentType;
	std::string bodyRequestFile;
	std::string fileName;
	std::string date;
	std::string responseHeaders;
	std::string responseBody;
	size_t bytesSent;
	size_t bufferSize;
	std::string autoIndexFile;
    std::string uploadDirectory;
    bool autoIndex;
	_MAP types_map;
	int StatusCode;

	/*
	** --------------------------------- CONSTRUCTOR ----------------------------------
	*/
public:
	MandatoryResponseHeaders();

	/*
	** --------------------------------- METHODS ----------------------------------
	*/
protected:
	void initHttpStatusStrings();
	void initHttpStatusFiles();
	void startLine();			   //
	void calculateContentLength(); //
	bool canNotOpenFileForRead();
	bool canNotOpenFileForWrite();
	void setContentType(); //
	void setDate();		   //
	void setResponseHeaders();
	void setLocationRedirectionHeader();
	void setFileNameToFileError();
	void setResponseBody();

	/*
	** --------------------------------- GETTERS ----------------------------------
	*/

public:
	const std::string getResponseHeaders();
	const std::string getResponseBody();

	/*
	** --------------------------------- SETTERS ----------------------------------
	*/
public:
	void setBytesSent(const size_t b);
	void setStatusCode(const int s);
	void setBufferSize(const size_t b);
	void setAutoIndex(const bool b);
	void setUploadDirectory(const std::string& upDir);
	void setAllowedMethods(const std::vector<std::string>& a);
	void setLocationRedirectionPath(const std::string& l);
	void setfileName(const std::string& f);
	void setHttpStatusFiles(int	statusCode, const char* path);
	void setBodyRequestFile(const std::string& f);

};

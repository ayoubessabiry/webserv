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
	std::string bodyRequest;
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

	MandatoryResponseHeaders();

	/*
	** --------------------------------- METHODS ----------------------------------
	*/

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

	std::string getResponseHeaders();
	std::string getResponseBody();

	/*
	** --------------------------------- SETTERS ----------------------------------
	*/
	void setBytesSent(size_t b);
	void setStatusCode(int s);
	void setBufferSize(size_t b);
	void setAutoIndex(bool b);
	void setUploadDirectory(std::string& upDir);
	void setAllowedMethods(std::vector<std::string>& a);
	void setLocationRedirectionPath(std::string& l);
	void setfileName(std::string& f);
};
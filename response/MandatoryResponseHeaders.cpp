#include "../headers/MandatoryResponseHeaders.hpp"

// Public

MandatoryResponseHeaders::MandatoryResponseHeaders() : bytesSent(0), autoIndex(false), StatusCode(200)
{
    initHttpStatusStrings();
    initHttpStatusFiles();
    types_map = get_map();
}
void MandatoryResponseHeaders::setStatusCode(const int s)
{
    StatusCode = s;
    // THINK ABOUT THIS AS IT WILL BE CALLED ANYWAY ON SetResponseHeaders
    // startLine();
}

// Private

void MandatoryResponseHeaders::setResponseHeaders()
{
    startLine();
    responseHeaders.append(startLineString);
    if (locationRedirectionPath.empty())
    {
        calculateContentLength();
        setContentType();
        responseHeaders.append(contentLength);
        responseHeaders.append(contentType);
    }
    else
    {
        setLocationRedirectionHeader();
        responseHeaders.append(locationRedirectionHeader);
        locationRedirectionPath.clear();
    }
    setDate();
    responseHeaders.append(date);
    responseHeaders.append("\r\n");
}

void MandatoryResponseHeaders::setLocationRedirectionHeader()
{
    locationRedirectionHeader.append("Location: ");
    locationRedirectionHeader.append(locationRedirectionPath);
    locationRedirectionHeader.append("\r\n");
}

void MandatoryResponseHeaders::startLine()
{
    startLineString.clear();
    startLineString.append("HTTP/1.1");
    startLineString.append(" ");
    startLineString.append(intToString(StatusCode));
    startLineString.append(" ");
    startLineString.append(httpStatusStrings[StatusCode]);
    startLineString.append("\r\n");
}
void MandatoryResponseHeaders::calculateContentLength()
{
    contentLength.append("Content-Length: ");
    std::ifstream file;
    size_t fileLength = 0;

    if (autoIndexFile.empty())
    {
        file.open(fileName, std::ios::binary);
        file.seekg(0, file.end);
        fileLength = file.tellg();
        file.seekg(0, file.beg);
        file.close();
    }
    else
    {
        fileLength = autoIndexFile.size();
    }

    contentLength.append(intToString(fileLength).c_str());
    contentLength.append("\r\n");
}
bool MandatoryResponseHeaders::canNotOpenFileForRead()
{
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        if (access(fileName.c_str(), F_OK))
        {
            StatusCode = 404;
        }
        else
        {
            StatusCode = 403;
        }
        setFileNameToFileError();
        return true;
    }
    file.close();
    return false;
}
bool MandatoryResponseHeaders::canNotOpenFileForWrite()
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        StatusCode = 403;
        setFileNameToFileError();
        return true;
    }
    file.close();
    return false;
}
void MandatoryResponseHeaders::initHttpStatusStrings()
{
    httpStatusStrings[200] = "OK";
    httpStatusStrings[201] = "Created";
    httpStatusStrings[202] = "Accepted";
    httpStatusStrings[301] = "Moved Permanently";
    httpStatusStrings[400] = "Bad Request";
    httpStatusStrings[404] = "Not Found";
    httpStatusStrings[403] = "Forbidden";
    httpStatusStrings[409] = "Conflict";
    httpStatusStrings[405] = "Method Not Allowed";
    httpStatusStrings[500] = "Internal Server Error";
    httpStatusStrings[502] = "Bad Gateway";
}
void MandatoryResponseHeaders::initHttpStatusFiles()
{
    httpStatusFiles[201] = "./errors/201.html";
    httpStatusFiles[202] = "./errors/202.html";
    httpStatusFiles[400] = "./errors/400.html";
    httpStatusFiles[404] = "./errors/404.html";
    httpStatusFiles[405] = "./errors/405.html";
    httpStatusFiles[409] = "./errors/409.html";
    httpStatusFiles[403] = "./errors/403.html";
    httpStatusFiles[500] = "./errors/500.html";
    httpStatusFiles[502] = "./errors/502.html";
}

void MandatoryResponseHeaders::setHttpStatusFiles(int	statusCode, const char* path)
{
	httpStatusFiles[statusCode] = path;
}

void MandatoryResponseHeaders::setContentType()
{
    std::string extention = get_ext(fileName);
    std::string contentTypeOfExt = getContentTypeOfExt(types_map, extention);

    contentType.append("Content-Type: ");
    contentType.append(contentTypeOfExt);
    contentType.append("\r\n");
}

void MandatoryResponseHeaders::setDate()
{
    date.append("Date: ");
    date.append(get_time());
    date.append("\r\n");
}

void MandatoryResponseHeaders::setFileNameToFileError()
{
    if (StatusCode != 200)
    {
        fileName.clear();
        fileName.append(httpStatusFiles[StatusCode]);
    }
}

const std::string MandatoryResponseHeaders::getResponseHeaders()
{
    setResponseHeaders();
    return responseHeaders;
}
void MandatoryResponseHeaders::setResponseBody()
{
    if (autoIndex && !autoIndexFile.empty() && responseBody.empty())
    {       
        responseBody = autoIndexFile;
        autoIndexFile.clear();
    }
    else
    {
        responseBody = getRangeFromFile(fileName.c_str(), bytesSent, bufferSize);
    }
}
const std::string MandatoryResponseHeaders::getResponseBody()
{
    setResponseBody();
    return responseBody;
}
void MandatoryResponseHeaders::setBytesSent(const size_t b)
{
    bytesSent = b;
}
void MandatoryResponseHeaders::setBufferSize(const size_t b)
{
    bufferSize = b;
}
void MandatoryResponseHeaders::setAutoIndex(const bool b)
{
    autoIndex = b;
}
void MandatoryResponseHeaders::setUploadDirectory(const std::string &upDir)
{
    uploadDirectory = upDir;
}
void MandatoryResponseHeaders::setAllowedMethods(const std::vector<std::string> &a)
{
    allowedMethods = a;
}
void MandatoryResponseHeaders::setLocationRedirectionPath(const std::string &l)
{
    locationRedirectionPath = l;
}
// MandatoryResponseHeaders::
void MandatoryResponseHeaders::setfileName(const std::string &f)
{
    fileName = f;
}

void MandatoryResponseHeaders::setBodyRequestFile(const std::string& f)
{
    bodyRequestFile = f;
}

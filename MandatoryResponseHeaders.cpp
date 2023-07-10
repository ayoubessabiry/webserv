#include "headers/MandatoryResponseHeaders.hpp"

// Public

MandatoryResponseHeaders::MandatoryResponseHeaders() : StatusCode(200), bytesSent(0)
{
    initHttpStatusStrings();
    initHttpStatusFiles();
    types_map = get_map();
}
void MandatoryResponseHeaders::setStatusCode(int s)
{
    StatusCode = s;
    startLine();
}

// Private

void MandatoryResponseHeaders::setResponseHeaders()
{
    startLine();
    if (locationRedirectionPath.empty())
    {
        calculateContentLength();
        setContentType();
        responseHeaders.append(contentLength);
        responseHeaders.append(contentType);
    }
    else
    {
        responseHeaders.append(locationRedirectionHeader);
    }
    setDate();
    responseHeaders.append(startLineString);
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

    file.open(fileName.c_str(), std::ios::binary);
    file.seekg(0, file.end);
    size_t fileLength = file.tellg();
    file.seekg(0, file.beg);
    file.close();

    contentLength.append(intToString(fileLength).c_str());
    contentLength.append("\r\n");
}
bool MandatoryResponseHeaders::canNotOpenFileForRead()
{
    std::ifstream file(fileName.c_str());

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
    std::ofstream file(fileName.c_str());

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
void MandatoryResponseHeaders::initHttpStatusStrings()
{
    httpStatusStrings[200] = "OK";
    httpStatusStrings[201] = "Created";
    httpStatusStrings[301] = "Moved Permanently";
    httpStatusStrings[400] = "Bad Request";
    httpStatusStrings[404] = "Not Found";
    httpStatusStrings[403] = "Forbidden";
    httpStatusStrings[405] = "Method Not Allowed";
    httpStatusStrings[500] = "Internal Server Error";
}
void MandatoryResponseHeaders::initHttpStatusFiles()
{
    httpStatusFiles[201] = "/Users/adbaich/Desktop/test/errors/201.html";
    httpStatusFiles[400] = "/Users/adbaich/Desktop/test/errors/400.html";
    httpStatusFiles[404] = "/Users/adbaich/Desktop/test/errors/404.html";
    httpStatusFiles[403] = "/Users/adbaich/Desktop/test/errors/403.html";
    httpStatusFiles[500] = "/Users/adbaich/Desktop/test/errors/500.html";
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

std::string MandatoryResponseHeaders::getResponseHeaders()
{
    setResponseHeaders();
    return responseHeaders;
}
void MandatoryResponseHeaders::setResponseBody()
{
    if (autoIndex)
    {
        responseBody = autoIndexFile;
    }
    else
    {
        responseBody = getRangeFromFile(fileName.c_str(), bytesSent, bufferSize);
    }
}
std::string MandatoryResponseHeaders::getResponseBody()
{
    setResponseBody();
    return responseBody;
}
void MandatoryResponseHeaders::setBytesSent(size_t b)
{
    bytesSent = b;
}
void MandatoryResponseHeaders::setBufferSize(size_t b)
{
    bufferSize = b;
}
void MandatoryResponseHeaders::setAutoIndex(bool b)
{
    autoIndex = b;
}
void MandatoryResponseHeaders::setUploadDirectory(std::string &upDir)
{
    uploadDirectory = upDir;
}
void MandatoryResponseHeaders::setAllowedMethods(std::vector<std::string> &a)
{
    allowedMethods = a;
}
void MandatoryResponseHeaders::setLocationRedirectionPath(std::string &l)
{
    locationRedirectionPath = l;
}
// MandatoryResponseHeaders::
void MandatoryResponseHeaders::setfileName(std::string &f)
{
    fileName = f;
}

#include "../headers/GetMethod.hpp"

void GetMethod::setInvalidResource()
{
    invalidResource = canNotOpenFileForRead();
}

void GetMethod::DirectoryResponse()
{
    if (uri[uri.size() - 1] != '/')
    {
        uri.append("/");
        StatusCode = 301;
        locationRedirectionPath = uri;
    }
    else
    {
        if (isIndexWithinDirectory(fileName, indexes))
        {
            return;
        }
        else
        {
            if (autoIndex)
            {
                autoIndexFile = getAutoIndex(fileName.c_str());
            }
            else
            {
                StatusCode = 403;
                setFileNameToFileError();
            }
        }
    }
}

GetMethod::GetMethod()
{
    resourceIsDirectory = false;
}

void GetMethod::initGetMethod()
{
    if (!methodIsAllowed("GET", allowedMethods))
    {
        StatusCode = 405;
        setFileNameToFileError();
    }
    else
    {
        setInvalidResource();
        if (!invalidResource)
        {
            if (isDirectory(fileName.c_str()))
            {
                resourceIsDirectory = true;
                DirectoryResponse();
            }
        }
    }
}

void GetMethod::setIndexes(const std::vector<std::string>& a)
{
    indexes = a;
}

void GetMethod::setUri(const std::string& a)
{
    uri = a;
}


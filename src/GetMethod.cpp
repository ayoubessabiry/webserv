#include "../include/GetMethod.hpp"

void GetMethod::setInvalidResource()
{
    invalidResource = canNotOpenFileForRead();
}

void GetMethod::DirectoryResponse()
{
    if (fileName[fileName.size() - 1] != '/')
    {
        fileName.append("/");
        StatusCode = 301;
        locationRedirectionPath = fileName;
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
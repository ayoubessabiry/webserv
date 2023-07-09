#include "../include/DeleteMethod.hpp"

DeleteMethod::DeleteMethod()
{
    DeleteIsAllowed = false;
    resourceIsDirectory = false;
}
void DeleteMethod::deleteProcess()
{
    if (methodIsAllowed("DELETE", allowedMethods))
    {
        DeleteIsAllowed = true;
    }
    if (DeleteIsAllowed && isDirectory(fileName.c_str()))
    {
        resourceIsDirectory = true;
    }
    if (DeleteIsAllowed)
    {
        if (pathDoesNotExist(fileName.c_str()))
        {
            StatusCode = 404;
            setFileNameToFileError();
        }
        else
        {
            if (resourceIsDirectory)
            {
                deleteDirectory();
            }
            else
            {
                deleteFile();
            }
        }
    }
    else
    {
        StatusCode = 405;
        setFileNameToFileError();
    }
}
void DeleteMethod::deleteDirectory()
{
    if (fileName[fileName.size() - 1] != '/')
    {
        StatusCode = 409;
        setFileNameToFileError();
    }
    else
    {
        if (canNotOpenDirectoryOrSubDirectories(fileName.c_str()))
        {
            StatusCode = 403;
            setFileNameToFileError();
        }
        else
        {
            deleteDirectoryContent(fileName);
        }
    }
}
void DeleteMethod::deleteFile()
{
    if (remove(fileName.c_str()) < 0)
    {
        StatusCode = 500;
        setFileNameToFileError();
    }
}
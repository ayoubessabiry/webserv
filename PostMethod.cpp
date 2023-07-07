#include "include/PostMethod.hpp"

PostMethod::PostMethod()
{
    postIsAllowed = false;
}
void PostMethod::postProcess()
{
    if (methodIsAllowed("POST", allowedMethods) || !uploadDirectory.empty())
    {
        postIsAllowed = true;
    }
    if (postIsAllowed)
    {
        fileName = uploadDirectory.append(fileName);
        if (canNotOpenFileForWrite())
        {
            std::cerr << "Can't open file : " << fileName << std::endl;
        }
        else
        {
            uploadFile(fileName, bodyRequest);
            StatusCode = 201;
            setFileNameToFileError();
        }
    }
    else
    {
        StatusCode = 405;
        setFileNameToFileError();
    }
}
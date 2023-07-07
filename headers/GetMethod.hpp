#include "MandatoryResponseHeaders.hpp"

class GetMethod : public MandatoryResponseHeaders
{
    std::vector<std::string> indexes;
    bool invalidResource;
    bool resourceIsDirectory;

    /*
    ** --------------------------------- CONSTRUCTOR ----------------------------------
    */

    GetMethod();

    /*
    ** --------------------------------- METHODS ----------------------------------
    */

    void initGetMethod();
    void setInvalidResource();
    void DirectoryResponse();
};
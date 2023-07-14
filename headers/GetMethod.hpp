#include "MandatoryResponseHeaders.hpp"

class GetMethod : public MandatoryResponseHeaders
{
    std::vector<std::string> indexes;
    bool invalidResource;
    bool resourceIsDirectory;

    /*
    ** --------------------------------- CONSTRUCTOR ----------------------------------
    */

public:
    GetMethod();

    /*
    ** --------------------------------- METHODS ----------------------------------
    */
    void initGetMethod();
    void setIndexes(const std::vector<std::string>& a);
private:
    void setInvalidResource();
    void DirectoryResponse();
};
#include "MandatoryResponseHeaders.hpp"

class GetMethod : public MandatoryResponseHeaders
{
    std::vector<std::string> indexes;
    std::string                 uri;
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
    void setUri(const std::string& a);
private:
    void setInvalidResource();
    void DirectoryResponse();
};
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

public:
    GetMethod();
    void setIndexes( std::vector<std::string> indexes)
    {
        this->indexes = indexes;
    }
    /*
    ** --------------------------------- METHODS ----------------------------------
    */
    void initGetMethod();
private:
    void setInvalidResource();
    void DirectoryResponse();
};
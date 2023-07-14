#include "MandatoryResponseHeaders.hpp"

class DeleteMethod : public MandatoryResponseHeaders
{
    bool DeleteIsAllowed;
    bool resourceIsDirectory;

    /*
	** --------------------------------- CONSTRUCTOR ----------------------------------
	*/

public:
    DeleteMethod();

    /*
	** --------------------------------- METHODS ----------------------------------
	*/

    void deleteProcess();
private:
    void deleteDirectory();
    void deleteFile();
};
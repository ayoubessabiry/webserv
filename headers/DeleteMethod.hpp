#include "MandatoryResponseHeaders.hpp"

class DeleteMethod : public MandatoryResponseHeaders
{
    bool DeleteIsAllowed;
    bool resourceIsDirectory;

    /*
	** --------------------------------- CONSTRUCTOR ----------------------------------
	*/

    DeleteMethod();

    /*
	** --------------------------------- METHODS ----------------------------------
	*/

    void deleteProcess();
    void deleteDirectory();
    void deleteFile();
};
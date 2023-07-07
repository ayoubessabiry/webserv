#include "MandatoryResponseHeaders.hpp"

class PostMethod : public MandatoryResponseHeaders
{
    bool postIsAllowed;

	/*
	** --------------------------------- CONSTRUCTOR ----------------------------------
	*/

    PostMethod();

    /*
	** --------------------------------- METHODS ----------------------------------
	*/

    void postProcess();
};
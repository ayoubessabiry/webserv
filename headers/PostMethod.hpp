#include "MandatoryResponseHeaders.hpp"

class PostMethod : public MandatoryResponseHeaders
{
    bool postIsAllowed;

	/*
	** --------------------------------- CONSTRUCTOR ----------------------------------
	*/

public:
    PostMethod();

    /*
	** --------------------------------- METHODS ----------------------------------
	*/

    void postProcess();
};
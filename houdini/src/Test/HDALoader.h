#include <HAPI/HAPI.h>

namespace Echo
{
	// OTL - Operator Type Library file.
	// The OTL can contain many HDAs

	// The OP_OTLManager class manages the operator type libraries.It also maintains the information about various preferences 
	// and settings associated with HDAs and OTLs, such as whether to use the OPlibrary files or not.However, its main task is 
	// to load and maintain a list of OP_OTLLibrary objects, which can be accessed with the OP_OTLManager::getLibrary() method.

	void iterateAllLibraries(const char* path);
}
#include "HDALoader.h"
#include <OP/OP_Director.h>
#include <OP/OP_OTLLibrary.h>

namespace Echo
{
	void iterateAllLibraries(const char* path)
	{
		// There is only one instance of OP_OTLManager class and it is owned by the OP_Director global object. 
		OP_OTLManager& mgr = OPgetDirector()->getOTLManager();

		// iterate through the list of libaries and print their file name
		for (int i = 0; i < mgr.getNumLibraries(); i++)
		{
			UT_String	   fullPath;
			OP_OTLLibrary* library = mgr.getLibrary(i);
			const char*	   filePath = library->getSource().buffer();

			OP_OTLManager::fullyQualifyLibFileName(filePath, fullPath);
		}
	}
}
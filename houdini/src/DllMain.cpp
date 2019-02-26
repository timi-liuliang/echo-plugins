#include "DllMain.h"
#include "HoudiniModule.h"

using namespace Echo;

bool loadHoudiniPlugin()
{
	registerModules();

	return true;
}

bool unloadHoudiniPlugin()
{
	return false;
}
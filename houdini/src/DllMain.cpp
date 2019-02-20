#include "DllMain.h"
#include "houdini_module.h"

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
#include "DllMain.h"
#include "HoudiniModule.h"
#include "Test/GeoisoSurface.h"

using namespace Echo;

bool loadHoudiniEnginePlugin()
{
	geoisoSurfaceTest();

	registerModules();

	return true;
}

bool unloadHoudiniEnginePlugin()
{
	return false;
}
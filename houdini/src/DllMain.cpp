#include "DllMain.h"
#include "HoudiniModule.h"
#include "Test/GeoisoSurface.h"
#include "Test/HAPISession.h"

using namespace Echo;

bool loadHoudiniEnginePlugin()
{
	//geoisoSurfaceTest();

	HAPI_Session session;
	startHAPISessionNamedPipe(session);

	registerModules();

	return true;
}

bool unloadHoudiniEnginePlugin()
{
	return false;
}
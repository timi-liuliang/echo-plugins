#include "DllMain.h"
#include "houdini_module.h"

using namespace Echo;

bool loadHoudiniPlugin()
{
    //REGISTER_MODULE(HoudiniModule)
    
	return true;
}

bool unloadHoudiniPlugin()
{
	return false;
}

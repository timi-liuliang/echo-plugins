#pragma once

#ifdef WIN32
    #define ECHO_PLUGIN_EXPORT __declspec(dllexport)
#else
    #define ECHO_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

extern "C"
{
	// load houdini plugin
	bool ECHO_PLUGIN_EXPORT loadHoudiniPlugin();

	// unload houdini plugin
	bool ECHO_PLUGIN_EXPORT unloadHoudiniPlugin();
}

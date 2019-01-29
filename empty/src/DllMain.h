#pragma once

#define ECHO_PLUGIN_EXPORT __declspec(dllexport)

extern "C"
{
	// load houdini plugin
	bool ECHO_PLUGIN_EXPORT loadEmptyPlugin();

	// unload houdini plugin
	bool ECHO_PLUGIN_EXPORT unloadEmptyPlugin();
}
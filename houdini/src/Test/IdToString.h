#pragma once

#include <string>
#include <HAPI/HAPI.h>

namespace Echo
{
	std::string HAPI_IdToString(int stringId, HAPI_Session& session);
}
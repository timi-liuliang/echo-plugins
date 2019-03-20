#include "IdToString.h"
#include <vector>

namespace Echo
{
	std::string HAPI_IdToString(int stringId, HAPI_Session& session)
	{
		if (stringId >= 0)
		{
			int NameLength = 0;
			if (HAPI_GetStringBufLength(&session, stringId, &NameLength) == HAPI_RESULT_SUCCESS)
			{
				if (NameLength)
				{
					std::vector<char> NameBuffer(NameLength, '\0');
					if (HAPI_GetString(&session, stringId, &NameBuffer[0], NameLength) == HAPI_RESULT_SUCCESS)
					{
						return std::string(NameBuffer.begin(), NameBuffer.end());
					}
				}
			}
		}

		return std::string();
	}
}
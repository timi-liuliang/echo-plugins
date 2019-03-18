#include "HoudiniModule.h"
#include "engine/core/memory/MemAllocDef.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Echo
{
	void registerModules()
	{
		//REGISTER_MODULE(HoudiniModule)
		FT_Library m_library;
		FT_Error result = FT_Init_FreeType(&m_library);
		if (result == FT_Err_Ok)
		{

		}
	}

    HoudiniModule::HoudiniModule()
    {
    }
    
    void HoudiniModule::registerTypes()
    {
        
    }
    
    void HoudiniModule::update(float elapsedTime)
    {
        
    }
}

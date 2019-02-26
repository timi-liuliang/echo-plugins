#include "HoudiniModule.h"
#include "Api/HoudiniApi.h"
#include "engine/core/memory/MemAllocDef.h"

namespace Echo
{
	void registerModules()
	{
		REGISTER_MODULE(HoudiniModule)
	}

    HoudiniModule::HoudiniModule()
    {
		m_houdiniApi = EchoNew(HoudninApi);
		m_houdiniApi->initialize();
    }
    
    void HoudiniModule::registerTypes()
    {
        
    }
    
    void HoudiniModule::update(float elapsedTime)
    {
        
    }
}

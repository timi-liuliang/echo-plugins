#pragma once

#include <engine/core/main/module.h>

namespace Echo
{
    class HoudiniModule : public Module
    {
    public:
        HoudiniModule();
        
        // register all types of the module
        virtual void registerTypes() override;
        
        // update box2d world
        virtual void update(float elapsedTime) override;
    };
}

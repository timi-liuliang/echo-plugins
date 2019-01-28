/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_VexResolver.h ( VEX Library, C++)
 *
 * COMMENTS:	A class with virtual functions for getting VEX code from
 * 		style sheet material definitions.
 */

#ifndef __STY_VexResolver__
#define __STY_VexResolver__

#include "STY_API.h"
#include <VEX/VEX_VexResolver.h>
#include <VEX/VCC_VexCompiler.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>

class STY_API STY_VexResolver : public VEX_VexResolver
{
public:
			 STY_VexResolver();
    virtual 		~STY_VexResolver();

    void		 registerShader(VEX_ContextType context_type,
				const UT_StringHolder &shader_name,
				const UT_StringHolder &shader_code);

protected:
    virtual bool	 canResolveCode(const char *shader);
    virtual bool	 supportedByMantra(const char *shader);
    virtual bool	 resolveVflCode(const char *shader, std::ostream &os,
				VEX_CodeGenFlags flags,
				int context_type);
    virtual bool	 resolveVexCode(const char *shader, std::ostream &os,
				VEX_CodeGenFlags flags, int context_type, 
				bool &is_pure_compiled_out,
				VEX_ErrorLog &errors);
    // No concept of pre-caching here like there is in VOPs.
    virtual bool	 resolveCacheVexCode(const char *shader,
				int context_type, VEX_ErrorLog &errors)
			 { return true; }
    virtual bool	 resolveAddDependency(const char *shader,
				DEP_MicroNode *depnode);
    virtual bool	 resolveDependencies(const char *shader,
				UT_StringArray &shader_deps,
				int context_type);
    virtual int		 resolveCodeTimeStamp(const char *shader,
				int context_type);
    virtual bool	 resolveVexContext(const char *shader, UT_String &ctx,
				int requested_context_type); 
    virtual bool	 resolveCanonicalPath(const char *shader,
				UT_String &canonical_shader);
    virtual bool	 resolveAlias(const char *alias, UT_String &shader); 

private:
    UT_Map<int, UT_StringMap<UT_StringHolder> >	 myShaders;
    UT_UniquePtr<VCC_VexCompiler>		 myCompiler;
};

STY_API extern STY_VexResolver	*STYgetVexResolver();

#endif


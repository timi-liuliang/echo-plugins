/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_CodeVexResolver.h ( VEX Library, C++)
 *
 * COMMENTS:	A class with virtual functions for getting VEX code from
 * 		Vop networks.
 */

#ifndef __VOP_CodeVexResolver__
#define __VOP_CodeVexResolver__

#include "VOP_API.h"
#include <VEX/VEX_VexResolver.h>

class VOP_API VOP_CodeVexResolver : public VEX_VexResolver
{
public:
			 VOP_CodeVexResolver();
    virtual 		~VOP_CodeVexResolver();

protected:
    virtual bool	 canResolveCode(const char *shader);
    virtual bool	 supportedByMantra(const char *shader);
    virtual bool	 resolveVflCode(const char *shader, std::ostream &os,
                	        VEX_CodeGenFlags flags, int context_type);
    virtual bool	 resolveVexCode(const char *shader, std::ostream &os,
				VEX_CodeGenFlags flags, int context_type, 
				bool &is_pure_compiled_out,
				VEX_ErrorLog &errors);
    virtual bool	 resolveCacheVexCode(const char *shader, 
				int context_type, 
				VEX_ErrorLog &errors);
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
    virtual bool	 resolveAlias(const char *alias,
				UT_String &shader); 
};

#endif


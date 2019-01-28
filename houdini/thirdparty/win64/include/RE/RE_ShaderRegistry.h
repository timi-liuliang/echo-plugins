/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        RE_ShaderRegistry.h (RE Library, C++)
 *
 * COMMENTS:
 * RE_ShaderRegistry encapsulates a container for a family of shaders that
 * can be classified using a number of tags encoded as a bit masks.
 * In the registry, each shader is associated with the sets of tags that
 * uniquely identify it and can be looked up using the same set of tags.
 *
 * It is essential that the tags are set up and assigned to shaders in a
 * way that can ensure distinct masks for all registered shaders. When
 * registering a shader, a warning is printed to stderr and the shader is
 * dropped if its mask is already in use by another shader in the registry.
 */


#ifndef RE_SHADERREGISTRY_H_
#define RE_SHADERREGISTRY_H_

#include "RE_API.h"
#include <UT/UT_Map.h>
#include <iostream>
#include "RE_ShaderHandle.h"

class RE_API RE_ShaderRegistry
{
public:
    RE_ShaderRegistry():
	myLastLookupShader(NULL),
	myLastLookupTags(0),
	myLastLookupTagsValid(false)
    {}

    ~RE_ShaderRegistry();

    int			 registerShader(const char *program_files,
					const char *defines,
					exint tags);

    int			 registerShader(RE_ShaderHandle *sh, exint tags);
    RE_ShaderHandle	*lookupShader(exint tags) const;

    static void		 dumpShaderInfo(RE_ShaderHandle *sh, std::ostream &os);

private:
    typedef UT_Map<exint, RE_ShaderHandle *> ShaderMapT;

    ShaderMapT 			 myShaderMap;
    UT_Array<RE_ShaderHandle *>  myShaders;
    mutable RE_ShaderHandle	*myLastLookupShader;
    mutable exint		 myLastLookupTags;
    mutable bool		 myLastLookupTagsValid;
};

#endif /* RE_SHADERREGISTRY_H_ */

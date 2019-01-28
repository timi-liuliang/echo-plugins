/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_VexResolver.h ( VEX Library, C++)
 *
 * COMMENTS:	A class with virtual functions for getting VEX code from
 * 		Vop networks. See VOPNET_VexResolver for the real
 * 		implementation.
 */

#ifndef __VEX_VexResolver__
#define __VEX_VexResolver__

#include "VEX_API.h"
#include <iosfwd>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_ValArray.h>

#include "VEX_VexTypes.h"

class VEX_VexResolver;
class VEX_ErrorLog;
class DEP_MicroNode;
class UT_StringView;


typedef UT_ValArray<VEX_VexResolver *>	 VEX_VexResolverArray;

/// @{
/// Check whether this line marks the beginning of a compiled VEX shader
bool	VEX_API VEXisVexCodeBeginMarker(const UT_String &str);
bool	VEX_API VEXstartsWithVexCodeBeginMarker(const UT_String &str);
bool	VEX_API VEXisVexCodeBeginMarker(const UT_StringView &str);
bool	VEX_API VEXstartsWithVexCodeBeginMarker(const UT_StringView &str);
/// @}

/// Check whether this line marks the end of a compiled VEX shader
bool	VEX_API VEXisVexCodeEndMarker(const UT_String &str);
bool	VEX_API VEXisVexCodeEndMarker(const UT_StringView &str);

class VEX_API VEX_VexResolver
{
public:
    static bool		 needsVexResolver(const char *shader);
    static bool		 needsVexResolverForMantraOutput(const char *shader);
    static bool		 getVflCode(const char *shader, std::ostream &os,
				VEX_CodeGenFlags code_flags = VEX_CG_DEFAULT,
				int context_type = 0,
				VEX_ErrorLog *errors = nullptr);

    /// Obtains an object (vex) shader code in os for the given shader name
    /// and for the given context type. 
    /// If provided, pure_compiled_out will be true for shaders from
    /// fully compiled code-only operators or HDAs.
    /// If provided, warnings_or_errors will be filled with messages; if
    /// no issues were encountered it will be empty; if there are warnings
    /// it will contain them (but the method will return true); if there are
    /// errors it will contain them and the method will return false.
    /// Returns true if a valid vex code has been obtained (though, note
    /// there may still be some warnings in the outgoing argument). 
    /// Returns false if vex could not be obtained.
    static bool		 getVexCode(const char *shader, std::ostream &os,
				VEX_CodeGenFlags code_flags = VEX_CG_DEFAULT,
				int context_type = 0, 
				bool *is_pure_compiled_out = nullptr,
				VEX_ErrorLog *errors = nullptr);

    /// Returns a list of shaders that the given shader depends on. This is
    /// usually a list of encapsulated shaders that can be shared among multiple
    /// shader instances.
    static bool		 getDependencies(const char *shader,
               		        UT_StringArray &shader_dependencies,
               		        int context_type = 0);

    /// Marks a given micronode as dependent on our code.  It will be
    /// dirtied whenever the generating VOPs change.
    static bool		 addDependency(const char *shader,
				DEP_MicroNode *depmicronode);

    // Returns an integer that can be used to check whether code has
    // changed since it was last resolved, without fetching the code.  If
    // an identical value is returned, the code is unchanged.  If the code
    // does not exist, a value of -1 is returned.
    static int		 getCodeTimeStamp(const char *shader,
				int context_type = 0);

    static void		 getEmptyVexCode(const char *shader, std::ostream &os);
    static bool		 getVexContext(const char *shader, UT_String &ctx,
				int requested_context_type = 0,
				VEX_ErrorLog *errors = nullptr);

    /// Resolvers may support relative paths in the shader string.  Sometimes,
    /// however, you'll want to use the shader string as an identifier.  For
    /// example, you might want to cache the resolved result.  To safely do so
    /// you'll need a canonical string with the relative paths resolved.  This
    /// method performs this conversion, returning true and setting the output
    /// string only when some resolution had to be performed.
    static bool		 convertToCanonicalPath(const char *shader,
				UT_String &canonical_shader);

    /// Resolves the alias to a proper shader name.
    /// Sometimes, a shader may be referred to using its function name as alias
    /// rather than operator name as is the convention for resolving code.
    static bool		 convertAlias(const char *alias, UT_String &shader);

    /// Resolves the shader & adds it to the cache, returning true
    /// if successfully cached.
    static bool		 cacheVexCode(const char *shader, int context_type = 0,
				VEX_ErrorLog *errors = nullptr);

protected:
			 VEX_VexResolver();
    virtual 		~VEX_VexResolver();

    virtual bool	 canResolveCode(const char *shader) = 0;

    /// Returns true when mantra is able to independently retrieve the VEX
    /// shader code without requiring it to be embedded in the ifd.
    virtual bool	 supportedByMantra(const char *shader) = 0;

    virtual bool	 resolveVflCode(const char *shader,
                	        std::ostream &os,
				VEX_CodeGenFlags code_flags,
				int context_type) = 0;

    /// See comments for getVexCode().
    virtual bool	 resolveVexCode(const char *shader,
                	        std::ostream &os,
				VEX_CodeGenFlags code_flags,
				int context_type,
				bool& is_pure_compiled_out,
				VEX_ErrorLog &errors) = 0;
    virtual bool	 resolveCacheVexCode(const char *shader,
				int context_type, 
				VEX_ErrorLog &errors) = 0;
    virtual bool	 resolveDependencies(const char *shader,
				UT_StringArray &shader_deps,
				int context_type) = 0;
    /// Marks a given micronode as dependent on our code.  It will be
    /// dirtied whenever the generating VOPs change.
    virtual bool	 resolveAddDependency(const char *shader,
				DEP_MicroNode *depmicronode) = 0;


    virtual int		 resolveCodeTimeStamp(const char *shader,
				int context_type) = 0;
    virtual bool	 resolveVexContext(const char *shader,
				UT_String &ctx,
				int requested_context_type) = 0;
    virtual bool	 resolveCanonicalPath(const char *shader,
				UT_String &canonical_shader) = 0;
    virtual bool	 resolveAlias(const char *alias,
				UT_String &shader) = 0;

private:
    static void			 addVexResolver(VEX_VexResolver *resolver);
    static void			 removeVexResolver(VEX_VexResolver *resolver);
    static VEX_VexResolverArray	&getVexResolvers();
    static VEX_VexResolver	*getVexResolver(const char *shader);
};

#endif



/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:
 *	Provides a handle to a shader and convenient methods for constructing
 *	shaders and testing for errors.
 *
 *	Program files (.prog) are supported, with a limited syntax for
 *	constructing shaders. They contain shader files to be linked, in version
 *	blocks. A program file may have multiple version blocks. The highest
 *	supported GLSL version block is used.
 *
 *	The directives are:
 *
 *	#name <string>	     The name of the shader is "string"
 *	#version <ver>       Specifies a base GLSL version (ie 110, 330, 410)
 *	#extension <ext>     Extension ext must be supported (ie GL_ARB_shadows)
 *	#output <name> <num> Fragment output <name> is mapped to buffer <num>
 *
 *	If either the #version check or any #extension check within the block
 *	fails, the entire block is discarded. A new block begins with #version.
 *
 *	#name is used to specify an english name for the shader, otherwise
 *	the filename of the program is used. It resides outside of any version
 *	block.
 *
 *	#output is used to connect fragment shader outputs to draw buffer
 *      indices. It is only used for GLSL versions >= 130.
 *
 *	Comments are supported if the line begins with //. Comments are not
 *	supported elsewhere. All other non-blank lines are expected to be
 *	shader filenames. Indentation is supported but ignored.
 *
 *		#name Blended Matte Shader
 *		// fallback shader
 *		#version 110
 *		basic/GL20/pos_transform.vert
 *		basic/GL20/const_color.frag
 *
 *		#version 130
 *		#extension GL_ARB_draw_buffers
 *		#output fgcolor 0
 *		#output bgcolor 1
 *		basic/GL30/pos_transform.vert
 *		basic/GL30/blend_color.frag
 *
 *		#version 400
 *		#output fgcolor 0
 *		#output bgcolor 1
 *		basic/GL40/pos_transform.vert
 *		basic/GL40/blend_color.frag
 *
 *	If GLSL 4.0 is supported, the 400 block shaders (GL40) will be used.
 *	If GLSL 1.30 to 3.3 is supported along with the GL_ARB_draw_buffers
 *	   extension, then 130 block shaders (GL30) are used.
 *	Otherwise, the 1.10 block's shaders will be used.
 *
 *	You can determine the shader that was loaded via getCodeVersion().
 */
#ifndef RE_ShaderHandle_h
#define RE_ShaderHandle_h

#include "RE_Types.h"
#include <iosfwd>
#include <UT/UT_Lock.h>
#include <UT/UT_String.h>
#include <UT/UT_UniquePtr.h>

class RE_Render;
class RE_Shader;

/// Simple interface to building a shader from a .prog file. 
class RE_API RE_ShaderHandle
{
public:
    /// Create a shader handle using a .prog file.
    RE_ShaderHandle(const char *program_file,
		    bool register_shader = true,
		    const char *defines = NULL);

    /// Create a shader handle with source files. All source files must share
    /// the same GLSL version (100,110,120,130,140,150,330,400,410...) and be
    /// whitespace separated in the string.
    RE_ShaderHandle(const char *program_name,
		    int code_version,
		    const char *files,
		    bool register_shader = true,
		    const char *defines = NULL);

    RE_ShaderHandle(const RE_ShaderHandle &handle);
    ~RE_ShaderHandle();

    RE_Shader	    *operator->()
			{
			    UT_ASSERT(myShader);
			    return myShader.get();
			}

    /// Set the defines for the program, overwriting existing ones.
    void	    setProgramDefines(const char *defines)
			{ myDefines.harden(defines); }
    /// Add more defines to the defines list
    void	    appendProgramDefines(const char *defines)
			{ myDefines +="\n"; myDefines += defines; }
    
    /// Default language is GLSL.
    void	    setShaderLanguage(RE_ShaderLanguage lang);

    /// Returns true if the shader was initialized. The shader still may be
    /// invalid (compiled improperly).
    bool	    isInitialized() const { return myInitFlag; }

    /// Returns true if the shader successfully compiled. This may force a
    /// compile.
    bool	    isValid(RE_Render *r)
		    {
			if(!myInitFlag)
			    return compile(r);
			return (myShader != NULL);
		    }

    /// Explicitly compile the shader. Has no effect if the shader is already
    /// compiled. Returns true if the shader compiled successfully or was
    /// already compiled, or false if the compile failed or previously failed.
    bool	    compile(RE_Render *r);

    /// Returns the GLSL or Cg version that the shader was compiled with.
    int		    getCodeVersion() const { return myCodeVersion; }

    /// Returns the shader for this handle. Will return NULL if the shader
    /// did not compile.
    RE_Shader	   *getShader() const { return myShader.get(); }

    /// Prints this shader's errors, warnings and messages.
    void	    printErrors(std::ostream &os) const;
    
    /// Returns a NULL-terminated string of errors. Empty string if no errors
    /// exist, or  if the shader has not been compiled yet.
    const char	   *getErrors() const { return myErrors.nonNullBuffer(); }

    /// Name of the shader (if any) or the .prog pathname
    const char     *getName() const;

    const char     *getSourceFiles() const { return mySourceFiles; }
    const char	   *getDefines() const { return myDefines; }

    /// Create a shader that is owner by the caller
    /// If the shader failed to compile, then an empty pointer is returned.
    UT_UniquePtr<RE_Shader>
		    createDetachedShader(RE_Render *r,
					 UT_String *errors = nullptr) const;

    /// Returns a list of #defines for platform, vendor, driver version.
    static const char *getSystemDefines(RE_Render *r)
		    {
			if(!theVendorDefines.isstring())
			    initializeDriverInformation(r);
			return theVendorDefines;
		    }

private:
    static void	    initializeDriverInformation(RE_Render *r);

    UT_UniquePtr<RE_Shader>
		    compileImpl(RE_Render *r, UT_String *errors) const;

    UT_String		myName;
    RE_ShaderLanguage	myShaderLanguage;
    int			myCodeVersion;
    UT_String		mySourceFiles;
    UT_String		myDefines;
    UT_UniquePtr<RE_Shader>
			myShader;
    bool		myInitFlag;
    UT_String		myErrors;
    mutable UT_Lock	myLock;
    bool		myRegisterFlag;
    static UT_String	theVendorDefines;
};

#endif

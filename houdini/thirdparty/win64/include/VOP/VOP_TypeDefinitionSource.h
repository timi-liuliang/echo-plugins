/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_TypeDefinitionSource.h ( VOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __VOP_TypeDefinitionSource__
#define __VOP_TypeDefinitionSource__

#include "VOP_API.h"
#include "VOP_TypeDefinition.h"	    // for VOP_TypeDefinitionHandle
class VOP_LanguageManager;


/// A generic source of the vop (connector wire) type definitions.
class VOP_API VOP_TypeDefinitionSource
{
public:
    /// Constructor and destructor.
		     VOP_TypeDefinitionSource();
    virtual	    ~VOP_TypeDefinitionSource();

    /// Returns a type definition provided by this source.
    virtual VOP_TypeDefinitionHandle
		    getTypeDefinition( const char *type_name ) = 0;

    /// Returns the path indentifying the source. 
    /// Used for displaying in the default editor.
    virtual void    getSourcePath( UT_String &path ) const;

    /// Sets the manager in which this source has been registered as definition
    /// provider.
    void	    setRegisteredMgr( VOP_LanguageManager *mgr );

private:
    /// Manager in which this source has been registered to provide definitions.
    VOP_LanguageManager *   myRegisteredMgr;
};

#endif


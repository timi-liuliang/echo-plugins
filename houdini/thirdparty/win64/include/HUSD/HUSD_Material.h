/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Material_h__
#define __HUSD_Material_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_TimeCode.h"
#include <VOP/VOP_Types.h>

class HUSD_FindPrims;
class VOP_Node;
class OP_Network;
class UT_Options;

// ============================================================================
class HUSD_API HUSD_Material
{
public:
    /// Standard c-tor.
		HUSD_Material(HUSD_AutoWriteLock &lock);

    /// Defines a USD material primitive at a given @p usd_mat_path 
    /// based on given @p material_vop material node.
    bool	defineMaterial( VOP_Node &material_vop, 
			const UT_StringRef &usd_mat_path ) const;

    /// Creates a new USD material primitive at @p usd_mat_path, which inherits 
    /// from the material given by @p base_material_path, and sets 
    /// the parameter override values on the created material.
    bool	defineDerivedMaterial( 
			const UT_StringRef &base_material_path,
			const UT_Options &material_parameters,
			const UT_StringRef &usd_mat_path) const;

    /// Sets the time code at which shader parameters are evaluated.
    void	 setTimeCode( const HUSD_TimeCode &time_code )
			{ myTimeCode = time_code; }

    /// Sets the primitive type that should be used when creating parents
    /// that don't exist yet in the USD hierarchy.
    void	setParentPrimType( const UT_StringHolder &type )
			{ myParentType = type; }

private:
    /// Helper for defining a shader inside a material primitive.
    bool	defineShader( const UT_StringRef &usd_material_path,
			VOP_Node &shader_node, VOP_Type shader_type ) const;

private:
    HUSD_AutoWriteLock	&myWriteLock;
    UT_StringHolder	 myParentType;	// Type of intermediate ancestors.
    HUSD_TimeCode	 myTimeCode;	// Time at which to eval shader parms.
};


// ============================================================================
class HUSD_API HUSD_MaterialEdit
{
public:
    /// Standard c-tor.
		HUSD_MaterialEdit(HUSD_AutoReadLock &lock);

    /// Creates the VOP children inside the LOP @p material_lop parent
    /// to reflect the USD materials defined at the given @p path.
    bool	loadMaterial( OP_Network &material_lop,
			const UT_StringRef &usd_container_path) const;


private:
    HUSD_AutoReadLock	&myReadLock;
};

#endif


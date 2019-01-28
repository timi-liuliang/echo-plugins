/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_BindMaterial__
#define __HUSD_BindMaterial__

#include "HUSD_API.h"
#include "HUSD_CvexBindingsMap.h"
#include "HUSD_DataHandle.h"

class HUSD_API HUSD_BindMaterial
{
public:
    /// Standard c-tor.
		 HUSD_BindMaterial(HUSD_AutoWriteLock &lock);

    /// @{ Assigns the given material to the given (geometry) primitive(s).
    bool	bindMaterial(const UT_StringRef &material_path,
			const UT_StringRef &geo_path) const;
    bool	bindMaterial(const UT_StringRef &material_path,
			const HUSD_FindPrims &findprims) const;
    /// @}

    /// Assigns material to primitives according to the configuration
    /// of the object on which it is invoked. At minumum, setMaterial() 
    /// needs to be called beforehand to specify the material 
    /// primitive to bind. The other configuration options (eg, binding method,
    /// or purpose ) are optional.
    bool	bind( const HUSD_FindPrims &findprims );

    /// Enumeration of the ways a material primitive can be specified.
    enum MaterialSpecType		
    {
	SPEC_MATERIAL_PATH,	// static material path
	SPEC_CVEX		// dynamic result of a CVEX node
    };

    /// Specifies the material that should be bound to primitives.
    void	setMaterial( MaterialSpecType specification_type, 
			const UT_StringHolder &material_spec );

    /// Enumeration of the ways material parameters can be overriden.
    enum ParameterOverrideType		
    {
	PARM_OVERRIDE_NONE,	// static material path
	PARM_OVERRIDE_CVEX	// dynamic result of a CVEX node
    };

    /// Specifies the material that should be bound to primitives.
    void	setMatParmsOverride( ParameterOverrideType override_type, 
			const UT_StringHolder &parm_override );

    /// Specify the vex_parm->usd_attrib map for CVEX parameter override script.
    void	setMatParmsOverrideMap( const HUSD_CvexBindingsMap &map );

    /// Enumeration of the ways in which a binding can be performed.
    enum BindMethod
    {
	METHOD_DIRECT,	    // direct binding
	METHOD_COLLECTION   // collection-based binding
    };

    /// Sets the method of defining bindings.
    void			setBindMethod( BindMethod method )
				{ myBindMethod = method; }
    BindMethod			getBindMethod() const 
				{ return myBindMethod; }

    /// For non-direct bindings, sets the USD primitive path on which the 
    /// collection-based binding is defined.
    void			setBindPrimPath( const UT_StringRef &p)
				{ myBindPrimPath = p; }
    const UT_StringHolder &	getBindPrimPath() const 
				{ return myBindPrimPath; }

    /// Enumeration of the material binding strength.
    enum Strength
    {
	STRENGTH_DEFAULT,   // fallback
	STRENGTH_STRONG,    // stronger than descendents
	STRENGTH_WEAK	    // weaker than descendents
    };

    /// Sets the strength preference for subsequent material assignments.
    void			setStrength( Strength strength )
				{ myStrength = strength; }
    Strength			getStrength() const 
				{ return myStrength; }

    /// Sets the purpose for subsequent material assignments.
    void			setPurpose( const UT_StringHolder &purpose )
				{ myPurpose = purpose; }
    const UT_StringHolder &	getPurpose() const 
				{ return myPurpose; }

protected:
    /// Binds the material without any further processing or partitionin of it.
    bool	bindExactMaterial(const UT_StringRef &material_path,
			const HUSD_FindPrims &findprims) const;

    /// Attempts to override material parameters and then binds primitives.
    bool	bindCvexParmOverrideMaterial(const UT_StringRef &material_path,
			const HUSD_FindPrims &findprims) const;

    /// Partitions primitives according to Cvex material path results and
    /// assigns material based on that dynamic path.
    bool	bindCvexMaterial( const HUSD_FindPrims &findprims ) const;

private:
    HUSD_AutoWriteLock &	myWriteLock;
    BindMethod			myBindMethod;
    Strength			myStrength;
    UT_StringHolder		myPurpose;
    UT_StringHolder		myBindPrimPath;

    MaterialSpecType		myMaterialSpecType;
    UT_StringHolder		myMaterialSpec;

    ParameterOverrideType	myParmOverrideType;
    UT_StringHolder		myParmOverride;
    HUSD_CvexBindingsMap	myParmOverrideMap;
};

#endif


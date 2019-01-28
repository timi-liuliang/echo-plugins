/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Cvex__
#define __HUSD_Cvex__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_TimeCode.h"
#include <UT/UT_Options.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringMap.h>
class HUSD_CvexBindingsMap;
class HUSD_CvexMaterialBucket;
class OP_Node;

// ============================================================================
class HUSD_API HUSD_Cvex
{
public:
		 HUSD_Cvex(HUSD_AutoAnyLock &lock);
		~HUSD_Cvex();

    /// Sets the ID of a node that executes the CVEX script. 
    /// It is used for channels evaluation in VEX as well as error reporting.
    void	 setCwdNodeId( int cwd_node_id ) 
			{ myCwdNodeID = cwd_node_id; }

    /// Sets the time code at which attributes are evaluated and/or set.
    void	 setTimeCode( const HUSD_TimeCode &time_code )
			{ myTimeCode = time_code; }


    /// Runs the CVEX script on the USD primitives, setting their attributes.
    bool	 runOverPrimitives(
			const HUSD_FindPrims &findprims,
			const UT_StringRef &script_cmd,
			const HUSD_CvexBindingsMap &bindings_map ) const;

    /// Runs the CVEX script on the USD primitives, setting their attributes.
    bool	 runOverArrayElements(
			const HUSD_FindPrims &findprims,
			const UT_StringRef &script_cmd,
			const HUSD_CvexBindingsMap &bindings_map ) const;

    /// Gets the primitives for which the given vexpression returns 
    /// a non-zero value.
    bool	 matchPrimitives(
			UT_StringArray &matched_prims_paths, 
			const UT_StringRef &vexpression) const;

    /// Partitions the primitives according to the common material path.
    /// The material path is obtained from the CVEX script run on primitives.
    bool	 getMaterialBuckets(
			UT_Array<HUSD_CvexMaterialBucket> &buckets,
			const HUSD_FindPrims &findprims,
			const UT_StringRef &script_cmd) const;

    /// Partitions the primitives according to the common set of material
    /// parameters override values.  The parameter values are obtained from 
    /// the CVEX script run on primitives.
    bool	 getMaterialParmsBuckets( 
			UT_Array<HUSD_CvexMaterialBucket> &buckets,
			const HUSD_FindPrims &findprims,
			const UT_StringRef &script_cmd,
			const HUSD_CvexBindingsMap &bindings_map) const;

    /// Returns true if any attribute the CVEX has run on is time varying.
    bool	 getIsTimeVarying() const
		 { return myTimeVarying; }

private:
    HUSD_AutoAnyLock	&myAnyLock;
    int			 myCwdNodeID;	    // Id of node that runs CVEX.
    HUSD_TimeCode	 myTimeCode;	    // Time at which attribs are eval'd.
    mutable bool	 myTimeVarying;	    // True if any attrib is time-var.
};

// ============================================================================
/// Represents a group of primitives and a common material path.
class HUSD_API HUSD_CvexMaterialBucket
{
public:
    void			setMatPath( const UT_StringHolder &mat )
				    { myMatPath = mat; }
    const UT_StringHolder &	getMatPath() const
				    { return myMatPath; }

    void			setMatParms( const UT_Options &mat_parms )
				    { myMatParms = mat_parms; }
    const UT_Options &		getMatParms() const
				    { return myMatParms; }

    void			setPrimPaths( const UT_StringArray &paths )
				    { myPrimPaths = paths; }
    const UT_StringArray &	getPrimPaths() const
				    { return myPrimPaths; }

private:
    UT_StringHolder	myMatPath;	// material primitive path
    UT_Options		myMatParms;	// material parameter overrides
    UT_StringArray	myPrimPaths;	// geo prims to bind material to
};

#endif


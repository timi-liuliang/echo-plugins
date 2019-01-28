/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_CvexBindingsMap__
#define __HUSD_CvexBindingsMap__

#include "HUSD_API.h"
//#include "HUSD_DataHandle.h"
//#include <UT/UT_Options.h>
//#include <UT/UT_StringArray.h>
#include <UT/UT_StringMap.h>
//class HUSD_CvexBindingsMap;
//class HUSD_CvexMaterialBucket;
//class HUSD_TimeCode; 
class OP_Node;

class HUSD_API HUSD_CvexBindingsMap 
{
public:
    /// Convenience function for building the map from node's parameters.
    static HUSD_CvexBindingsMap	
			constructBindingsMap( const OP_Node &node,
				const char *bindings_num_parm,
				const char *cvex_parm_name_parm,
				const char *attrib_name_parm, 
				const char *auto_bind_parm );

    /// Add an entry to the map.
    void		addBinding( const UT_StringHolder &parm_name,
				const UT_StringHolder &attrib_name );

    /// Specify if attrib name can be used as parm name.
    void		setDefaultToIdentity( bool do_identity );

    /// Obtain the attribute name from cvex parameter name.
    UT_StringHolder	getAttribFromParm( const UT_StringRef &parm ) const;

private:
    UT_StringMap<UT_StringHolder>	myParmToAttribMap;
    bool				myDefaultToIdentity = true;
};


#endif


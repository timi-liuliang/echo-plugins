/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_CaptureData.h (SOP Library, C++)
 *
 * COMMENTS:	Utility class for dealing with capture atttributes
 */

#ifndef __SOP_CaptureData_h__
#define __SOP_CaptureData_h__

#include "SOP_API.h"
#include <OP/OP_Context.h>
#include <GEO/GEO_Detail.h>
#include <GU/GU_DetailHandle.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <UT/UT_VectorTypes.h>

class GU_Detail;
class OP_Node;
class SOP_Node;

class SOP_API SOP_CaptureData
{
public:
    SOP_CaptureData();
    virtual ~SOP_CaptureData();

    void    initialize(const OP_Node *node, const GU_Detail *gudp,
		       GEO_Detail::CaptureType t = GEO_Detail::CAPTURE_BONE);
    void    destroy();
    void    dirtyCache();

    bool    updateRegionColors( OP_Context &context );
    bool    updateRegionSOPPaths();

    const UT_ValArray<const UT_Vector3 *> &
    getRegionColors( OP_Context &context )
    {
	if( myColorsDirtyFlag )
	    (void) updateRegionColors( context );
	return myRegionColors;
    }

    const char *
    getRegionPath( int path_index )
    {
	if( myRegionsDirtyFlag )
	    (void) updateRegions();
	return myRegionPaths(path_index);
    }
    int
    getNumRegions()
    {
	if( myRegionsDirtyFlag )
	    (void) updateRegions();
	return myRegionPaths.entries();
    }

    const char *
    getSOPPath( int path_index )
    {
	if( mySOPPathsDirtyFlag )
	    (void) updateRegionSOPPaths();
	return myRegionSOPPaths(path_index);
    }

private:    // methods
    bool		updateRegions();
    GU_DetailHandle	getRegionHandle(UT_String path,
					OP_Context &context);
    int			getRegionPrimNumber(int region_idx,
					    OP_Context &context);

private:    // data
    int					 myOpId;
    const GU_Detail			*myGdp;
    UT_Vector3Array			 myRegionColorData;
    UT_ValArray<const UT_Vector3 *>	 myRegionColors;
    UT_ValArray<const char *>		 myRegionPaths;
    UT_ValArray<const char *>		 myRegionPrims;
    UT_ValArray<char *>			 myRegionSOPPaths;
    UT_String				 myRegionRootPath;
    bool				 myColorsDirtyFlag : 1,
					 myRegionsDirtyFlag : 1,
					 mySOPPathsDirtyFlag : 1;
    GEO_Detail::CaptureType		 myCaptureType;
};

SOP_API bool SOPgetCaptureRegion(OP_Node *me, char *cregion,
				 UT_Matrix4 &parent_xform,
				 GU_DetailHandle &tube_gdh);

SOP_API int SOPaddCaptureRegion(OP_Node *me,
				GU_Detail *gdp, const char *root_path,
				const char *cregion_name,
				GEO_Detail::CaptureType capture_type);

#endif // __SOP_CaptureData_h__


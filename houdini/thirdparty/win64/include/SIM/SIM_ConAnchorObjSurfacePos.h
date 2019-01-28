/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ConAnchorObjSurfacePos.h ( SIM Library, C++)
 *
 * COMMENTS:
 *     This anchor represents a surface on an object (a group of polygons)
 *     This can be used to attach a cloth object to fixed positions on the
 *     surface of an animated, deforming body object.
 */

#ifndef __SIM_ConAnchorObjSurfacePos__
#define __SIM_ConAnchorObjSurfacePos__

#include "SIM_API.h"
#include "SIM_ConAnchor.h"
#include <vector>

// This consists of the barycentric coordinates with respect
// to a triangle and an offset from the triangle surface
// (in a direction that depends only on the winding order).
struct SIM_SurfaceAttachPoint
{
    SIM_SurfaceAttachPoint()
    {
	// invalid point indices
        myPS[0] = -1; 
        myPS[1] = -1; 
        myPS[2] = -1; 

        // invalid weights
        myBary[0] = 0;
        myBary[1] = 0;
        myBary[2] = 0;
    }

    // Indices of triangle
    GA_Index myPS[3];
    // Weights for each of the triangle corner points (should sum up to 1)
    fpreal myBary[3];
};

class SIM_API SIM_ConAnchorObjSurfacePos : public SIM_ConAnchor
{
public:
    typedef SIM_SurfaceAttachPoint AttachPoint;
    typedef std::vector<AttachPoint> AttachPoints;
 
    GETSET_DATA_FUNCS_S(SIM_NAME_OBJECT, ObjectName);
    
    /// Store the group of primitives that restrict the attachment surface
    GETSET_DATA_FUNCS_S(SIM_NAME_PRIMITIVEGROUP, PrimitiveGroup);

    const std::vector<GA_Index>& primitiveIndices() const;

    void initializeAttachPoints(AttachPoints& attach_points);

    const AttachPoints& attachPoints() const;

    // Print attachment points, for debug purposes
    void print(std::ostream& os) const;

protected:
    explicit		SIM_ConAnchorObjSurfacePos(const SIM_DataFactory *f);
    virtual		~SIM_ConAnchorObjSurfacePos();

    virtual bool	getNeedsInitSubclass() const;
    virtual void	initAnchorSubclass(const SIM_Object *object);
    
    virtual bool        getObjectSpecificationSubclass(UT_String &str) const;

    virtual void	buildAnchorGuideGeometrySubclass(
					const SIM_Options &options,
					const GU_DetailHandle &gdh,
					const SIM_Relationship &rel,
					const SIM_Time &t) const;

 
    virtual void        makeEqualSubclass(const SIM_Data *source);
    virtual void        saveSubclass(std::ostream &os) const;
    virtual bool        loadSubclass(UT_IStream &is);

private:
    // Unordered set of primitives that define the attachment surface on
    // the object
    std::vector<GA_Index> myPrimitiveIndices;

    // For each constrained point, the closest bary point anywhere on
    // any of the surface primitives
    AttachPoints myAttachPoints;

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConAnchorObjSurfacePos,
			SIM_ConAnchor,
			"Anchor: Object Surface Position",
			getDopDescription());
};

#endif


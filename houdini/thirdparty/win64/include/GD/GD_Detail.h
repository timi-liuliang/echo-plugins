/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This defines the detail class (the container for all points/primitives)
 */

#ifndef __GD_Detail_h__
#define __GD_Detail_h__

#include "GD_API.h"
#include <UT/UT_BoundingRect.h>
#include <UT/UT_ValArray.h>
#include <GA/GA_Detail.h>
#include "GD_Primitive.h"
#include "GD_Macros.h"

#include <UT/UT_VectorTypes.h>
class UT_BoundingSphere;
class GD_TrimRegion;
class GD_TrimLoop;
class GD_Primitive;
class UT_MemoryCounter;

class GD_API GD_Detail : public GA_Detail
{
public:
    GD_Detail();
    virtual ~GD_Detail();

    static void	registerFactory();

    //
    //  Clear all the points/primitives out of this detail
    //
    void	 clearAndDestroy();

    //
    //  Stream I/O METHODS
    //
    GA_Detail::IOStatus	 saveH9Stream(std::ostream& os, bool binary,
					const GA_SaveOptions *options) const;
    GA_Detail::IOStatus	 loadH9(UT_IStream &is, const GA_LoadOptions *options);

    //
    //	Merge the source detail into us.
    //
    void	 	 merge(const GD_Detail &src, bool onlyvisible = false);

    //
    // Transform/translate all or some of the primitives. The methods will run
    // more efficiently if you happen to know the point closure apriori.
    // If 'quickxform' is true, we don't transform the attributes and don't
    // check for reflection matrix.
    //
    void	 transform(const UT_Matrix3	&matx,
			   GA_PrimitiveGroup	*primgroup    = 0,
			   GA_PointGroup	*pointclosure = 0);
    void	 translate(const UT_Vector2	&delta,
			   GA_PrimitiveGroup	*primgroup    = 0,
			   GA_PointGroup	*pointclosure = 0);

    //
    // Transform/translate all or some of the points.
    //
    void	 transformPoints(const UT_Matrix3  &matx,
			         GA_PointGroup	   *pointgroup	= 0);
    void	 translatePoints(const UT_Vector2  &delta,
			         GA_PointGroup	   *pointGroup  = 0);

    //
    // Methods for adding to the point list.
    //
    GA_Offset		 insertPointOffset(GA_Index before = GA_INVALID_INDEX);
    GA_Offset		 insertPointOffset(const UT_Vector3 &pos,
					   GA_Index before = GA_INVALID_INDEX);
    GA_Offset		 appendPointOffset()
					    { return insertPointOffset(); }
    GA_Offset		 appendPointOffset(const UT_Vector3 &pos)
					    { return insertPointOffset(pos); }

    GD_Primitive	*getGDPrimitive(GA_Offset offset)
    {
	if (!GAisValid(offset))
	    return NULL;
	return static_cast<GD_Primitive *>(getPrimitiveList().get(offset));
    }
    const GD_Primitive	*getGDPrimitive(GA_Offset offset) const
    {
	if (!GAisValid(offset))
	    return NULL;
	return static_cast<const GD_Primitive *>(getPrimitiveList().get(offset));
    }

    //
    //	Inserting and appending a new primitive
    //	Insertion works the same way as insertion of points.  However,
    //	The primitive is not allocated since the type is determined
    //	by the allocator.
    //
    GD_Primitive	*insertPrimitive(const GA_PrimitiveTypeId &type,
					 const GD_Primitive *before = 0);
    GD_Primitive	*appendPrimitive(const GA_PrimitiveTypeId &type)
			 {
			    return insertPrimitive(type, 0);
			 }
    virtual bool	 destroyPrimitiveOffset(GA_Offset offset,
					       bool and_points = false);
    virtual GA_Size	 destroyPrimitiveOffsets(const GA_Range &it,
						bool and_points = false);

    //
    // Appending and deleting a trimming region:
    //
    // opencasual true will preserve direction of open profiles.
    GD_TrimRegion	*appendTrimRegion(int opencasual = 0);
    void		 deleteTrimRegion(GD_TrimRegion &region);
    void		 deleteTrimRegion(unsigned int num);
    GD_TrimLoop		*generateTrimLoops(const UT_BoundingRect &brect,
					    float offset=1e-4F, 
					    float tol=1e-4F);
    void		 destroyTrimRegions(int deletecurves=1);
    void		 createTrimRegion(GD_TrimLoop *loop);


    //
    //	Find the bounding box (optionally of a group of points)
    // instantiation. Return 0 if the box is empty and 1 otherwise.
    //
    int		 getPointBBox(UT_BoundingBox *bbox,
			      const GA_PointGroup *g = 0) const;
    int		 getPointBBox(UT_BoundingBox& bbox,
			      const UT_Matrix4R& transform,
			      const GA_PointGroup *g=0) const;
    int		 getPointBSphere(UT_BoundingSphere *bsphere,
			      const GA_PointGroup *g = 0,
			      int updateRequired = 0) const;

    //
    // Return the bounds of the valid interval of the surface we're defined
    // for.
    //
    virtual void	 getDomainBBox(float &u0, float &v0,
				       float &u1, float &v1) const = 0;
    void		 getDomainBounds(UT_BoundingRect &rect) const
			 {
			     getDomainBBox(rect.vals[0][0], rect.vals[1][0],
					    rect.vals[0][1], rect.vals[1][1]);
			 }

    // Return the number of (visible) faces.
    int			 getNumFaces(int *visible = 0) const;

    // Find out if it is a valid trimmer. At least one trim loop must have 
    // already been computed from the trim regions for this method to return
    // true.
    int			 hasTrimLoops(void) const;

    // Find out if there are any trim regions. A short-cut for
    // trimRegions().entries().
    int			 hasTrimRegions(void) const;

    // *Const* handle to the trimming regions:
    const UT_ValArray<GD_TrimRegion*> &trimRegions(void) const
			 { return myTrimRegions; }

    // Build an outer trim loop to allow us to punch holes into the surface.
    // Return loop if OK and 0 otherwise. The method does not check if the 
    // outer loop already exists. The size of the loop will be increased by
    // the tolerance. The direction of the loop is counter-clockwise.
    GD_TrimRegion	*appendOuterLoop(float tol = 0, int checkduplicate = 1);

    // Get or set the altitude flag, which affects the level where we start
    // counting the nested loops for trimming. Typically, it's sea-level: zero.
    int			 altitude(void ) const	{ return myAltitude; }
    void		 altitude(int a)	{ myAltitude = a;    }

    // Typedefs for the GA_Detail::GB_MACRO_CAST() static methods.
    typedef GD_Primitive GB_MACRO_PRIM_TYPE;

    // The trimming regions:
    UT_ValArray<GD_TrimRegion*> &trimRegions(void) { return myTrimRegions; }

    // TODO: Deprecate internal groups with user-specified names.
    GA_PointGroup	*newPointGroup(const char *name, int internal);
    GA_PrimitiveGroup	*newPrimitiveGroup(const char *name, int internal);
    GA_PointGroup	*newInternalPointGroup();
    GA_PrimitiveGroup	*newInternalPrimitiveGroup();
    void		 destroyPointGroup(GA_PointGroup *g)
			    { destroyElementGroup(g); }
    void		 destroyPrimitiveGroup(GA_PrimitiveGroup *g)
			    { destroyElementGroup(g); }
    bool		 destroyPointGroup(const char *name)
			    { return
				destroyElementGroup(GA_ATTRIB_POINT, name); }
    bool		 destroyPrimitiveGroup(const char *name)
			    { return
				destroyElementGroup(GA_ATTRIB_PRIMITIVE, name); }

    // Load and save the list of trimming regions:
    bool	loadTrimRegions(int n, UT_IStream &is);
    int		saveTrimRegions(int n, std::ostream &os, int b=0) const;

    bool	loadTrimRegions(UT_JSONParser &p, const GA_LoadMap &map);
    bool	saveTrimRegions(UT_JSONWriter &w, const GA_SaveMap &map) const;

    virtual int64 getMemoryUsage(bool inclusive) const;

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

private:
    UT_ValArray<GD_TrimRegion*>	 myTrimRegions;	// list of trim regions
    int				 myAltitude;	// sea-level for nested loops

    bool	loadPointsH9(UT_IStream &is, int count);
    bool	savePointsH9(std::ostream &os, int binary) const;
    bool	savePointH9(std::ostream &os, int binary, GA_Offset	offset) const;

    void	setPos2(GA_Offset index, const UT_Vector2 &p);

    //  Friends:
    friend std::ostream	&operator<<(std::ostream &os, const GD_Detail &d)
			{
			    d.saveClassic(os, false, NULL);
			    return os;
			}
};

#endif

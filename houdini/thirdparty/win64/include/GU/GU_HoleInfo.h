/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_HoleInfo (C++)
 *
 * COMMENTS:	This is the info structure used for hole information.  Each
 *		primitive has a an attribute added which points to one of these
 *		structures.
 *		The flag is set to 1 if there's a hole.
 *		The array is filled with all the primitives which are enclosed
 *		within the face.
 */

#ifndef __GU_HoleInfo__
#define __GU_HoleInfo__

#include "GU_API.h"
#include <GA/GA_BlobData.h>
#include <GA/GA_Types.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_BoundingBox.h>

class GU_Detail;
class GA_Attribute;
class GA_PrimitiveGroup;
class GEO_Primitive;
class GEO_Face;
class UT_MemoryCounter;

class GU_API GU_HoleInfo : public GA_BlobData
{
public:
	     GU_HoleInfo(const GEO_Primitive *prim);
    virtual ~GU_HoleInfo();

    /// @{
    /// Methods defined on GA_BlobData
    virtual uint	hash() const;
    virtual bool	isEqual(const GA_BlobData &blob) const;
    /// @}

    // isHole() will return whether the primitive is contained in another face.
    unsigned		 isHole() const	{ return myHoleFlag & 1; }

    // isReversed() will return whether the normal is opposite of the
    // surrounding primitives normal (as it should be).
    unsigned		 isReversed() const	{ return myHoleFlag & 2; }

    // Return the number of holes in the primitive and a pointer to the hole.
    int			 entries() const	{ return myHoles.entries(); }
    GEO_Face		*getHole(int i)		{ return myHoles(i); }

    /// Store @b this hole info in the given attribute for the given object
    void			 setHole(GA_Offset o, GA_Attribute *a);
    /// @{ Get the hole info out of the object
    static GU_HoleInfo		*getHole(GA_Offset o, GA_Attribute *a);
    static const GU_HoleInfo	*getHole(GA_Offset o, const GA_Attribute *a);
    /// @}

    // For promotion of primitives
    int			 setPromotedFace(GEO_Face *face)
			 {
			     myFace = face;
			     return myFace != 0;
			 }
    GEO_Face		*getPromotedFace()	{ return myFace; }

    // A bounding box for the primitive.
    const UT_BoundingBox	&getBox() const	{ return myBox; }

// Methods which really should be private...
    UT_ValArray<GEO_Face *>	&getHoles()	{ return myHoles; }
    void			 setHole(int on)
				 {
				     if (on)	myHoleFlag |= 1;
				     else	myHoleFlag &= ~1;
				 }
    void			 setReversed()	{ myHoleFlag |= 2; }

    /// Report approximate memory usage
    virtual int64	getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void	countMemory(UT_MemoryCounter &counter, bool inclusive) const;

private:
    void		 fixPromotions(GA_Attribute *attrib);

    unsigned			 myHoleFlag;
    UT_ValArray<GEO_Face *>	 myHoles;
    UT_BoundingBox		 myBox;
    GEO_Face			*myFace;		// Promoted

    friend class GU_HoleDetail;
};

class GU_API GU_HoleDetail
{
public:
    GU_HoleDetail();
    ~GU_HoleDetail();

    // Returns the primitive attribute for the hole info pointers.
    // It returns -1 if there are no face primitives in the gdp.
    // The info structures are all initialized to contain the correct hole
    // information.
    GA_Attribute *buildInfo(GU_Detail *gdp, float distance, float angle,
			       const GA_PrimitiveGroup *primGroup);

    // Build information about orientation of the holes (i.e. whether the
    // normal is the wrong way for the holes).
    void	buildReversalInfo();
    // Promote polygons to bezier when needed.  The old primitives are
    // destroyed, but the attribute information is maintained.
    void	promoteFaces();

    // Detaches us from the GU_Detail, possibly deleting our normal.
    void	destroyInfo();

protected:
    GU_Detail *myGdp;
    GA_Attribute *myCmpInfoAttrib;
    bool myAddedNormals;
};

#endif

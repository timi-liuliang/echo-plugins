/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimTriFan.h ( GEO Library, C++)
 *
 * COMMENTS: Implements a normal triangle fan.
 */

#ifndef __GEO_PrimTriFan__
#define __GEO_PrimTriFan__

#include "GEO_API.h"
#include "GEO_TriMesh.h"
#include "GEO_PrimType.h"

class GA_Detail;

/// Triangle fan.  All triangles share the first vertex
class GEO_API GEO_PrimTriFan : public GEO_TriMesh
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimTriFan(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_TriMesh(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimTriFan() {}

public:
    /// @{
    /// Interface from GEO_TriMesh
    virtual bool	isDegenerate() const;
    virtual UT_Vector3	computeNormal() const;
    virtual void	reverse();

    virtual fpreal	calcVolume(const UT_Vector3 &refpt) const;
    virtual fpreal	calcArea() const;
    virtual fpreal	calcPerimeter() const;
    /// @}

    /// Get number of triangles in the fan
    exint	getTriangleCount() const	{ return getVertexCount()-2; }

    /// Get the vertices for a single triangle in the fan
    void	getTriangleVertices(exint tri, int &v0, int &v1, int &v2) const
		{
		    v0 = 0;
		    v1 = tri + 1;
		    v2 = tri + 2;
		}

    virtual const GA_PrimitiveJSON	*getJSON() const;

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_TriMesh::buildFamilyMask(); }

    /// All subclasses should call this method to register the curve intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_TriMesh::registerIntrinsics(defn); }

    // We don't need to save anything other than what's in a face
    virtual bool	savePrivateH9(std::ostream &, bool binary) const;
    virtual bool	loadPrivateH9(UT_IStream &);

private:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimTriFan &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
};

#endif

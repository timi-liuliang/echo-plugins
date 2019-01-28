/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PolyFrame.h (Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PolyFrame__
#define __GU_PolyFrame__

#include "GU_API.h"

#include <UT/UT_Vector3.h>
#include <GA/GA_Handle.h>
#include <string.h>

// GU_PolyFrame::theMinVertices[] depends on the ordering of this enumeration.
enum GU_PolyFrameStyle
{
    GU_POLYFRAME_FIRST_EDGE,
    GU_POLYFRAME_TWO_EDGES,
    GU_POLYFRAME_CENTROID,
    GU_POLYFRAME_TEXTURE_COORDS,
    GU_POLYFRAME_TEXTURE,
    GU_POLYFRAME_STYLE__count
};

enum GU_PolyFrameVector
{
    GU_POLYFRAME_TANGENT	= 1,
    GU_POLYFRAME_BITANGENT	= 1 << 1,
    GU_POLYFRAME_NORMAL		= 1 << 2,
};

enum GU_PolyFrameError
{
    GU_POLYFRAME_NO_ERROR,
    GU_POLYFRAME_CREATE_ATTRIBUTE_FAILED,
    GU_POLYFRAME_MISSING_DETAIL,
    GU_POLYFRAME_MISSING_TEXTURE_COORDS,
    GU_POLYFRAME_ERROR__count
};

class GU_API GU_PolyFrameParms
{
public:
    GU_PolyFrameParms()
	: style(GU_POLYFRAME_FIRST_EDGE)
	, which(GU_POLYFRAME_NORMAL)
	, orthogonal(false)
        , left_handed(false)
	, uv_name(0)
    {
	::memset(names, 0, sizeof(const char *) * 3);
    }

    GU_PolyFrameStyle	 style;		// Specifies how the frame vectors are
					// to be calculated.

    int			 which;		// A bit mask that specifies which basis
					// vectors are to be evaluated.  Bits
					// can be set using members of the
					// GU_PolyFrameVector enumeration.

    bool		 orthogonal;	// Indicates that the frame vectors
					// must be made orthogonal.

    bool                 left_handed;   // When false (default), it produces a
                                        // right-handed frame, else left-handed.
                                        // This only applies when orthogonal is
                                        // true.

    const char		*names[3];	// Specifies the names of the frame
					// vector attributes that are being
					// computed.  names[0] points to the
					// name of the tangent vector.  names[1]
					// points to the name of the bitangent
					// vector.  names[2] points to the name
					// of the normal.  names[0] and/or
					// names[1] will be ignored if the
					// corresponding bit in 'which' is not
					// set.

    const char		*uv_name;	// Optionally overrides the texture
					// attribute name used when style is
					// GU_POLYFRAME_TEXTURE_COORDS
};

class GA_PointGroup;
class GA_PrimitiveGroup;
class GEO_Hull;
class GEO_Face;
class GU_Detail;
class UT_BitArray;
class UT_String;

class GU_API GU_PolyFrame
{
public:
    GU_PolyFrame(GU_Detail *gdp);
    virtual		~GU_PolyFrame();

    GU_Detail		*getDetail() const { return myGdp; }
    void		 setDetail(GU_Detail *gdp) { myGdp = gdp; }

    GU_PolyFrameError	 computeFrames(const GU_PolyFrameParms &parms,
				       const GA_PointGroup *point_group = NULL,
				       const GA_PrimitiveGroup *prim_group
					    = NULL);

private:
    void		 computeFrames(GEO_Face *prim,
				       UT_BitArray *uncomputed,
				       GA_Size &nleft);

    // The following two methods are called once per detail and once per
    // polygon, respectively.  They give you a chance to compute values that
    // will be reused by several polygons and vertices, respectively.
    GU_PolyFrameError	 detailHook();
    void		 primitiveHook();

    void		 setFrame();

    // Frame computation methods for each style in GU_PolyFrameStyle.
    void		 computeFirstEdgeFrame();
    void		 computeTwoEdgesFrame();
    void		 computeCentroidFrame();
    void		 computeTexCoordsFrame();
    void		 computeTexFramePrim(GA_Offset primoff);
    void		 computeTexFrame();
    void		 computeDefaultFrame();

    // Utility methods.
    static void		 computeMeshCentroid(const GEO_Hull *hull,
					     GA_Size vertex_no,
					     UT_Vector3 &centroid);
    static void		 createPointNormals(GU_Detail *gdp,
				    const GA_RWHandleV3 &offset,
                                    const GA_Attribute *normalattrbefore);
    static GA_RWHandleV3 findOrAddAttrib(GU_Detail *gdp,
					 const char *name,
					 bool per_vertex,
					 bool &created,
					 GA_TypeInfo type);
    static const char	*getLayeredAttribName(const GU_Detail *gdp,
					      const char *name);
    static bool		 isStylePerVertex(GU_PolyFrameStyle style);
    static void		 makeOrthogonal(UT_Vector3 *frame, bool left_handed);

    static int			 theMinVertices[];
    GU_Detail			*myGdp;
    const GU_PolyFrameParms	*myParms;
    const GA_PointGroup		*myPointGroup;
    const GA_PrimitiveGroup	*myPrimGroup;
    GEO_Face			*myCurPrim;
    GA_Size			 myCurVertexNo;
    GA_Index			 myCurPointNo;
    UT_Vector3			 myFrame[3];

    // Values computed by detailHook() and polygonHook().
    UT_Vector3			 myCentroid;
    GA_RWHandleV3		 myHandles[3];
    GA_ROHandleV2		 myUvHandle;
    bool			 myPerVertex;
    bool			 myUvPerVertex;
    bool			 myAttribsCreated[3];
};

#endif


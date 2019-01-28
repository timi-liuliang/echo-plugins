/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Brush.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_Brush__
#define __GU_Brush__

#include "GU_API.h"
#include <UT/UT_Color.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_Vector3Array.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>

#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <GEO/GEO_Detail.h>

class GA_PrimitiveGroup;
class GA_PointGroup;
class GEO_Delta;
class GU_Detail;
class GEO_Primitive;
class GEO_PointTree;
class GEO_Hull;
class GEO_PrimPolySoup;

class GU_API GU_StencilPixel
{
public:
    int		idx;		// Which point this is.
    float	val;		// Stencil value
};


enum GU_BrushStencilMode
{
    GU_BRUSHMERGEMODE_NONE,
    GU_BRUSHMERGEMODE_POINT,
    GU_BRUSHMERGEMODE_VERTEX
};


//
// GU_BrushStencil abstracts a per point or per vertex stencil.
// This stencil has a current value of 0 .. 1 for every point or vertex
// in the gdp.
//
// Stencils are in one of two modes: point or vertex; which determines the
// native mode of the stencil.
//
class GU_API GU_BrushStencil
{
public:
    GU_BrushStencil();
    ~GU_BrushStencil();
    
    void		clearStencil(bool invalidate_vtx, bool invalidate_pt);
    bool		isEmpty() const;

    // These will clear out the stencil as well if the mode changes.
    void		setVertexMode();
    void		setPointMode();
    bool		isVertexMode() const;
    bool		isPointMode() const;

    /// NOTE: If the stencil is in point mode, index is a GA_Index,
    ///       else, (in vertex mode), it is an index into myGeoVtx
    ///       and myIsectVtx.
    void		setVal(exint index, float val, float alpha, const UT_Vector3 *colour = NULL);

    void		startPointIterate();
    bool		iteratePoint(GA_Index &ptind, float &alpha, UT_Vector3 *colour = NULL);

    void		startVertexIterate();
    bool		iterateVertex(int &vtxidx, float &alpha, UT_Vector3 *colour = NULL);

    GA_Size             fillPointData(const GA_Detail &detail, GA_OffsetList &ptoffs, UT_FloatArray &alphas, UT_Vector3Array *colours = NULL);
    GA_Size             fillVertexData(const GA_Detail &detail, const UT_Array<GA_Offset> &geovtx, GA_OffsetList &ptoffs, UT_FloatArray &alphas, UT_Vector3Array *colours = NULL);

    // These are used to ensure we can convert between vertex & point
    // iterators
    void		setVtxTables(UT_Array<UT_IntArray *> *pt2vtx,
				     UT_Array<GA_Offset> *vtx,
				     const GU_Detail *gdp);

    exint               entries() const
    { return myEntries.entries(); }

protected:
    // The actual entries of the stencil
    UT_Array<GU_StencilPixel>	 myEntries;

    // This has for each of our type (whether vertex or point) the stencil
    // which corresponds to it.
    UT_IntArray			 myStencilRef;

    // This is any array, one per point, which allows us to avoid
    // double counting points in a point iterate in vertex mode.
    UT_IntArray			 myPointPass;
    
    // This is the unpremultiplied stencil colour.
    UT_Vector3Array		 myColours;

    UT_Array<UT_IntArray *>	*myPt2Vtx;
    UT_Array<GA_Offset>		*myVtx;
    const GU_Detail		*myGdp;

    // We are either a vertex stencil or point stencil:
    GU_BrushStencilMode		 myMode;

    // Iterator information:
    int				 myCurPixel, myCurSubIdx;
    int				 myCurIteratePass;
    bool			 myCurIsVertexIterate;
};

typedef void (*GU_BrushMergeModeCallback)(GA_Offset ptoff, GA_Offset vtxoff,
		    UT_Vector3 &result,
		    const UT_Vector3 &cd, const UT_Vector3 &orig, void *data);

typedef void (*GU_BrushCallback)(GA_Offset pt,
		    const UT_Array<GA_Offset> *ptneighbour,
		    GA_Offset vtx,
		    const UT_Array<GA_Offset> *vtxneighbour,
		    float alpha, GEO_Delta *delta, void *data);

// This must match the order in the SOP.
enum GU_BrushMergeMode
{
    GU_BRUSHMERGEMODE_REPLACE,
    GU_BRUSHMERGEMODE_ADD,
    GU_BRUSHMERGEMODE_SUB,
    GU_BRUSHMERGEMODE_MULT,
    GU_BRUSHMERGEMODE_SCREEN,
    GU_BRUSHMERGEMODE_HUE,
    GU_BRUSHMERGEMODE_SAT,
    GU_BRUSHMERGEMODE_LIGHT,
    GU_BRUSHMERGEMODE_RED,
    GU_BRUSHMERGEMODE_GREEN,
    GU_BRUSHMERGEMODE_BLUE,
    GU_BRUSHMERGEMODE_INVERT,
    GU_BRUSHMERGEMODE_CALLBACK
};

class GU_BrushNib;

class GU_API GU_Brush
{
public:
    GU_Brush();
    ~GU_Brush();

    // These create and destroy all the cached structures which
    // describe the topology of the geometry.
    void		 buildCaches(GU_Detail *gdp, const GU_Detail *isectgdp,
				    const GA_PrimitiveGroup *group,
				    const GA_PointGroup *ptgroup,
				    bool usevisibility);
    // This makes the myVtxClass valid.
    void		 buildVertexClasses();
    void		 clearCaches();
    // This lets you check to see if the brush has a valid cache built yet.
    bool		 hasValidCache() { return myPointTree != NULL; }

    const GU_Detail     *getGdp() const { return myGdp; }
    const GU_Detail     *getIsectGdp() const { return myIsectGdp; }

    // These allow you to tell it what name to use for custom attributes:
    // They return true if they successfully found the attribute, and false
    // otherwise.
    bool		 overrideColour(const char *name, int create_tuple_size=-1);
    bool		 overrideAlpha(const char *name, bool create=false);
    bool		 overrideNml(const char *name);

    // This tells it to use a capture region as the colour.
    void		 useCaptureRegion(int captureidx, bool normalizeweight,
					  GEO_Detail::CaptureType capturetype
						    = GEO_Detail::CAPTURE_BONE);

    // These retrieve the default colour and alpha:
    // They return non-zero if they added an attribute.
    bool		 defaultColour(bool create = false);
    bool		 defaultAlpha(bool create = false);
    bool		 defaultNormal(bool create = false);
    bool		 defaultTexture(const UT_StringHolder &uvattribname, bool create);

    // This deals with the visualization colour.
    // Initializes it:
    void		 visualizeColor(float low, float high,
					UT_ColorRamp mode);
    bool		 doVisualize() const { return myVisualizeAttrib.isValid(); }
    void		 setVisualizeCd(GA_Offset ptoff, const UT_Vector3 &cd);
    void		 clearVisualize() { myVisualizeAttrib.clear(); }
    void		 applyVisualize(GU_Detail *gdp);
    void		 applyVisualizeStencil(GU_Detail *gdp);

    // Retrieves the colour of a point taking overrides into account.
    void		 getPointCd(GA_Offset ptoff, UT_Vector3 &cd) const;
    void		 getVertexCd(GA_Offset vtxoff, UT_Vector3 &cd) const;

    // These update just the GEO_PointTrees that control where
    // the points are.
    void		 addPrimPoints(const GEO_Detail &detail,
                            GA_Offset primoff,
			    UT_BitArray &ptused,
			    UT_Vector3Array &pts, GA_IndexArray &idxlist);
    void		 buildTrees(bool usevisibility);
    void		 addUVPrimPoints(const GEO_Detail &detail,
                            GA_Offset primoff,
			    const GA_ROHandleV3 &uvattrib,
			    UT_BitArray &ptused,
			    UT_Vector3Array &uvpts, GA_IndexArray &idxlist);
    void		 buildUVTrees(const UT_StringHolder &uvattribname, bool usevisibility);
    void		 destroyTrees();
    GEO_PointTree	*getPointTree() { return myPointTree; }
    GEO_PointTree	*getUVPointTree() { return myUVPointTree; }

    // These are the ring zero caches, marking neighbours in point topology.
    void		 connectRingVtx(int vtxnum,
					GA_Offset vtx2,
					bool doublevalence = false);
    void		 buildRingCache(bool usevisibility);
    void		 clearRingCache();

    // Stencil controls:

    // Determines if we try to blend the alpha or just the colour.
    void		 setWriteAlpha(bool val) { myWriteAlpha = val; }
    bool		 getWriteAlpha() const { return myWriteAlpha; }
    // Returns if there is anything in the stencil buffer.
    int			 hasStencil() const;
    // Zeros out the stencil - MUST be done if the geometry changes!
    void		 clearStencil(bool invalidate_vtx=true, bool invalidate_pt=true);
    // Sets the callback for the callback merge mode type.
    void		 setMergeModeCallback(GU_BrushMergeModeCallback cb,
					void *data)
			 {
			     myMergeModeCallback = cb;
			     myMergeModeCallbackData = data;
			 }
    // Applies a merge mode:
    void		 applyMergeMode(GA_Offset ptoff, GA_Offset vtxoff,
			    UT_Vector3 &result,
			    const UT_Vector3 &src, const UT_Vector3 &dst,
			    GU_BrushMergeMode mergemode);
    // Sets entire stencil to a specific alpha:
    void		 setStencil(float val, bool usevisibility);
    // Colour according to the stencil:
    void		 colourStencil(GEO_Delta *delta, const UT_Vector3 &cd,
					GU_BrushMergeMode mergemode);
    // Colour according to the stencil & the stencil colours:
    void		 stampColourStencil(GEO_Delta *delta,
					GU_BrushMergeMode mergemode);

    void		 callbackStencil(GEO_Delta *delta,
					GU_BrushCallback cb, void *data);
    // Comb the normals.
    // Lift is the amount to raise the comb above or below the tangent
    // plane.
    void		 combNormals(GEO_Delta *delta, const UT_Vector3 &dir,
				     float lift, int preservenmllen);

    // lift the normals.
    // Lift is the amount to raise the comb above or below the tangent
    // plane.
    void		 liftNormals(GEO_Delta *delta, float lift,
				     int preservenmllen);

    // rotate the normals.
    void		 rotateNormals(GEO_Delta *delta, const UT_Vector3 &dir,
				     int preservenmllen);

    void		 dragTexture(GEO_Delta *delta, const UT_Vector3 &dir);
    void		 scaleTexture(GEO_Delta *delta, float scale, 
				      const UT_Vector3 &orig);
    
    // Displace points along their normals
    void		 displaceNormals(GEO_Delta *delta, float amt);
    // Displace points along a specified vector:
    void		 displaceVector(GEO_Delta *delta, 
				const UT_Vector3 &dir, float amt);
    // Smooths all the points for one iteration...
    void		 smoothPoints(GEO_Delta *delta);
    // Smooths the current point attribute for one iteration.
    void		 smoothAttribute(GEO_Delta *delta);
    // Smooths teh normals keeping them normalized.
    void		 smoothNormals(GEO_Delta *delta, int preservenmllen);
    // Smooths the current texture attribute for one iteration.
    void		 smoothTexture(GEO_Delta *delta);

    // This undoes the specified delta, putting the changes into
    // the second delta
    void		 eraseAttributes(GEO_Delta *old, GEO_Delta *change);
    void		 erasePositions(GEO_Delta *old, GEO_Delta *change);
    void		 eraseCapture(GEO_Delta *old, GEO_Delta *change);

    // In UV mode, it will find the closest point to the origin
    // Returns GA_INVALID_INDEX on failure.
    GA_Index		 findUVSeedPtNum(const UT_Vector3 &orig,
                                const UT_StringHolder &uvattribname);

    // This applies a single dab to the stencil.
    // The seed primitive is a primitive in the ISECT gdp, as is the seedpt
    void		 applyDab(GU_BrushNib *nib, float stencil_val,
				const UT_Vector3 &orig, const UT_Vector3 &dir,
				float rad,
				const GEO_Primitive *seed,
				GA_Offset seedptoff,
				bool useconnectivity,
				bool usenormals,
				bool inuv,
                                const UT_StringHolder &uvattribname);

    // Returns cached normal value of a point.
    void		 getPtNormal(GA_Index ptidx, UT_Vector3 &nml);
    // Resets all the normals:
    void		 clearNormalCache();

private:	
#ifdef INTEL_COMPILER
	 GU_Brush(const GU_Brush& ) { }
#endif
	
protected:
    bool		 testPoint(GA_Index ptind,
				const UT_Vector3 &orig, const UT_Vector3 &dir,
				float radius2, bool seeddir,
				bool usenormals);
    bool		 isPointEditable(GA_Index ptnum);

    inline void          readColour(GA_Offset offset, UT_Vector3 &dest) const
    {
        if (myColourAttribV3.isValid())
            dest = myColourAttribV3.get(offset);
        else
        {
            for (int j = 0; j < myColourSize; j++)
                dest(j) = myColourAttrib.get(offset, j);
        }
    }
    inline void          writeColour(GA_Offset offset, UT_Vector3 &src) const
    {
        if (myColourAttribV3.isValid())
            myColourAttribV3.set(offset, src);
        else
        {
            for (int j = 0; j < myColourSize; j++)
                myColourAttrib.set(offset, j, src(j));
        }
    }

    const GA_PrimitiveGroup	*myGroup;
    const GA_PointGroup	*myPointGroup;
    GU_Detail		*myGdp;
    const GU_Detail	*myIsectGdp;

    bool		 myPtNmlSet;
    UT_Vector3Array	 myPtNmlCache;

    GEO_PointTree	*myPointTree;
    GEO_PointTree	*myUVPointTree;
    UT_StringHolder      myUVTreeAttribName;
    UT_Vector3Array	 myPointPos;
    UT_Vector3Array	 myUVPointPos;
    UT_Array<UT_Array<GA_Index> > myFloodConnected;

    // Vertex offsets into myIsectGdp
    UT_Array<GA_Offset>				myIsectVtx;
    // Vertex offsets into myGdp
    UT_Array<GA_Offset>				myGeoVtx;
    UT_Array<UT_IntArray *>			myPt2Vtx;

    // This assigns each vertex to a unique number depending on
    // its texture coordinate & point number.  Thus, vertices with
    // matching texture coordinates & point numbers will have
    // the same class.
    UT_IntArray					myVtxClass;
    
    // First ring of each point...
    UT_Array<GA_OffsetArray>			myRingZero;
    // This is the number of edges attached to each point ring.
    // If it is less than twice the myRingZero.entries(), we have
    // a boundary point.
    UT_IntArray					myRingValence;

    UT_Array<UT_Array<GA_Offset> *>		myVtxRingZero;
    UT_Array<UT_IntArray *>			myVtxRingClass;
    
    GA_RWHandleF	 myColourAttrib;
    GA_RWHandleV3	 myColourAttribV3;
    int			 myColourSize;
    bool		 myColourVertex;
    GA_RWHandleF	 myAlphaAttrib;
    bool		 myAlphaVertex;
    GA_RWHandleV3 	 myTextureAttrib;
    bool		 myTextureVertex;
    GA_RWHandleV3	 myNormalAttrib;
    GA_RWHandleV3	 myVisualizeAttrib;
    float		 myVisLow, myVisHigh;
    UT_ColorRamp	 myVisMode;
    bool		 myWriteAlpha;
    bool		 myUseCaptureRegion;
    int			 myCaptureIdx;
    bool                 myNormalizeWeight;
    GEO_Detail::CaptureType myCaptureType;
    bool		 myUseVisibility;

    GU_BrushMergeModeCallback	 myMergeModeCallback;
    void			*myMergeModeCallbackData;

private:
    GU_BrushStencil			myStencil;
};

#endif


/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	Ramp class
 *
 */


#ifndef	_UT_Ramp_
#define	_UT_Ramp_

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_Color.h"
#include "UT_Map.h"
#include "UT_Pixel.h"
#include "UT_Spline.h"
#include "UT_VectorTypes.h"

#include <SYS/SYS_Types.h>

#include <tbb/spin_mutex.h>

#include <iosfwd>


class UT_RampOpResolver;
class UT_Raster;
class UT_IStream;

enum UT_RampOrient {	// Pass to redraw
	UT_RAMP_HORIZONTAL,
	UT_RAMP_VERTICAL,
	UT_RAMP_RADIAL,
	UT_RAMP_CONCENTRIC
};

class UT_API UT_ColorNode {	// Floating point RGBA values
    struct SerializableFRGBA : public UT_FRGBA
    {
	using UT_FRGBA::operator=;
	
	template <typename ARCHIVER_T>
	bool serialize(ARCHIVER_T &archiver)
	{
	    return archiver.serializeUniformArray(&r, 4);
	}
    };
    
public:
    UT_ColorNode() {}
    UT_ColorNode(fpreal offset, const UT_FRGBA &color,
		 UT_SPLINE_BASIS basis_ = UT_SPLINE_LINEAR,
		 bool enable_pos_ = true,
		 bool enable_val_ = true,
		 bool enable_interp_ = true)
    {
	t = offset;
	basis = basis_;
	rgba = color;
	enable_pos = enable_pos_;
	enable_val = enable_val_;
	enable_interp = enable_interp_;
    }

    template <typename ARCHIVER_T>
    bool	    serialize(ARCHIVER_T &archiver);

    float	    	t;
    SerializableFRGBA	rgba;
    UT_SPLINE_BASIS 	basis;
    bool	    	enable_pos:1,
			enable_val:1,
			enable_interp:1;
};

//==========================================================================

/// Utility class for containing a color ramp
class UT_API UT_Ramp {
public:
     UT_Ramp();
     /// Do not pass by value, you should be passing by
     /// const &. If you really need by value make the copy
     /// internally to your system.
     explicit UT_Ramp(const UT_Ramp &copy);
    ~UT_Ramp();

    /// Return the amount of memory owned by this UT_Ramp in bytes
    int64 getMemoryUsage(bool inclusive) const;

    void		 setColorType(UT_ColorType color_space)
			    { myColorType = color_space; }
    UT_ColorType	 getColorType() const
			    { return myColorType; }

    void		 clearAndDestroy();

    /// Force updating of the ramp after being modified.
    void		 ensureRampIsBuilt();

    bool		 getGlobalEnable() const
			    { return myGlobalEnable; }
    void		 setGlobalEnable(bool enable)
			    { myGlobalEnable = enable; }

    int			 getNodeCount() const { return myNodes.entries(); }
    const UT_ColorNode	*getNode(int i) const;

    UT_ColorNode	*getRamp()
			    { return myNodes.entries() > 0 ? &myNodes(0) : 0; }
    const UT_ColorNode	*getRamp() const
			    { return myNodes.entries() > 0 ? &myNodes(0) : 0; }

    void		 convertToRaster(   UT_Raster	  *raster,
					    UT_RampOrient  dir,
					    int		   doInterping = 1,
					    // Currently only for radial and
					    // concentric
					    fpreal	   cx = 0.0f,
					    fpreal	   cy = 0.0f,
					    fpreal	   phase = 0.0f,
					    // For radial, this means "repeat"
					    fpreal	   radius = 1.0f);

    const UT_ColorNode	*getClosestNode(fpreal pos) const;
    int			 getClosestNodeIndex(fpreal pos) const;

    const UT_ColorNode	*getCurrentNode() const;
    int			 getCurrentNodeIndex() const;
    void		 setCurrentNodeIndex(int i, bool add_to_selection);

    void		 getColor(fpreal pos, UT_FRGBA *clr,
					int doInterping = 1) const;
    void		 getColor(fpreal pos, float fvals[4],
					int doInterping = 1) const;
    void		 getColor(UT_FRGBA *values, int num,
					int doInterping = 1) const;

    void		 setColorAt(int i, float fvals[4]);
    void		 setColorAt(int i, const UT_FRGBA &clr);
    void		 setCurrentNodeColor(fpreal32 fvals[4]);
    void		 setCurrentNodeColor(fpreal64 fvals[4]);
    void		 setCurrentNodeColor(const UT_FRGBA &clr);

    void		 setCurrentNodeBasis(UT_SPLINE_BASIS basis);
    void		 setNodeBasis(int i, UT_SPLINE_BASIS basis);

    void		 setEnable(int i, bool enable_pos, bool enable_val,
					  bool enable_interp);
    bool		 getEnablePos(int i) const;
    bool		 getEnableVal(int i) const;
    bool		 getEnableInterp(int i) const;

    void		 clearSelection();
    bool		 isNodeSelected(int i);
    void		 selectNode(int i, bool add);
    void		 selectRange(int first_i, int last_i, bool add);
    void		 deselectNode(int i);
    void		 toggleSelectNode(int i);
    void		 getSelectionIndices(UT_IntArray &indices);
    void		 setSelectionIndices(const UT_IntArray &indices);

    void		 addNode(fpreal pos);
    void		 addNode(fpreal pos, fpreal32 fvals[4],
				 UT_SPLINE_BASIS basis = UT_SPLINE_LINEAR);
    void		 addNode(fpreal pos, fpreal64 fvals[4],
				 UT_SPLINE_BASIS basis = UT_SPLINE_LINEAR);
    void		 addNode(fpreal pos, const UT_FRGBA &color,
				 UT_SPLINE_BASIS basis = UT_SPLINE_LINEAR);

    bool		 deleteNode(fpreal pos);
    bool		 deleteNodeAt(int i);
    void		 deleteSelectedNodes();

    void		 moveNode(const UT_ColorNode *node, fpreal pos);

    // Move the node selection according to the delta of selected node_i's
    // position to pos.
    void		 moveNodeSelection(int node_i, fpreal pos);

    int			 load(const char *pathname);
    bool		 load(UT_IStream &is);
    int			 save(const char *pathname) const;
    int			 save(std::ostream &os) const;

    UT_Ramp		&operator=(const UT_Ramp &from);

    // Equality does not consider the selected or current node, only the ramp
    // values themselves.
    bool		 operator==(const UT_Ramp &from) const;
    
    void		 setRamp(const UT_ColorNode *r, int size);

    void		 rampLookup(fpreal pos, float values[4]) const;
    void		 rampLookupNoInterp(fpreal pos, float values[4]) const;
    void		 rampLookup(fpreal u0, fpreal u1, float values[4],
				    int ns) const;
    void		 rampLookupNoInterp(fpreal u0, fpreal u1, float values[4],
				    int ns) const;

    template <typename ARCHIVER_T>
    bool		 serialize(ARCHIVER_T &archiver);

    /// Install an OP node resolver for ramp parameters.
    /// Required for resolveOpRamp() to work. OP_Director calls this.
    static  void	 setRampOpResolver(UT_RampOpResolver *resolver);

    /// Fetch the ramp from an OP node's ramp parm. May return false
    /// if the OP or parm is not found, if the parm is not a ramp, or if the
    /// UT_RampOpResolver is not assigned by setRampOpResolver().
    /// is_rgb, op_id, and op_version are optional return values for the type
    /// of ramp, the parent node's unique ID, and the parent node's version
    /// parm serial (as reported by getVersionParms()).
    bool		 resolveOpRamp(const char *op_path,
				       const char *ramp_parm,
				       bool *is_rgb = NULL,
				       int  *op_id = NULL,
				       int  *op_version = NULL);
    
private:
    bool		 loadAsPoly(UT_IStream &is);
    bool		 loadAsJSON(UT_IStream &is);

    UT_RGBA		*sampleColors  (int nsamples, int doInterp);
    UT_BIG_RGBA		*sampleColors16(int nsamples, int doInterp);
    void		 convertToRaster8 (UT_Raster *raster,UT_RampOrient dir,
			    int doInterp, fpreal cx, fpreal cy, fpreal phase,
			    fpreal radius);
    void		 convertToRaster16(UT_Raster *raster,UT_RampOrient dir,
			    int doInterp, fpreal cx, fpreal cy, fpreal phase,
			    fpreal radius);

    void		 getColorAndMultiply(fpreal pos, int do_interp,
				UT_RGBA *pixel);
    void		 getColorAndMultiply(fpreal pos, int do_interp,
				UT_BIG_RGBA *pixel);

    void		 interpColor(fpreal pos, int i1, float vals[4]) const;

    void		 updateSplineIfNeeded() const;
    void		 evaluateSpline(fpreal pos, fpreal t0, fpreal t1,
					int i, float *val) const;

private:

    typedef tbb::spin_mutex	 SplineLock;

    UT_Array<UT_ColorNode>	 myNodes;
    UT_ColorNode		*myCurrentNode;

    UT_Map<int, UT_ColorNode *>	 mySelectedNodes;

    UT_ColorType		 myColorType;

    mutable UT_Spline		 mySpline;
    mutable bool		 mySplineUpToDate;
    mutable SplineLock		 mySplineLock;

    bool			 myGlobalEnable;
};

/// Class to create a UT_Ramp from an OP node path and parameter string.
class UT_API UT_RampOpResolver
{
public:
	     UT_RampOpResolver();
    virtual ~UT_RampOpResolver();

    /// Fetch a UT_Ramp from an OP node's ramp parm. May return false
    /// if the OP or parm is not found, or if the parm is not a ramp.
    /// is_rgb, op_id, and op_version are optional return values for the type
    /// of ramp, the parent node's unique ID, and the parent node's version
    /// parm serial (as reported by getVersionParms()
    virtual bool resolveRampParm(UT_Ramp &target_ramp,
				 const char *op_path,
				 const char *ramp_parm,
				 bool *is_rgb = NULL,
				 int  *op_id = NULL,
				 int  *op_version = NULL);
};

#endif

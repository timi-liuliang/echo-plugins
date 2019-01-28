/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: This is the base class for all brushes.
 *
 */

#ifndef __SOP_BrushBase_h__
#define __SOP_BrushBase_h__

#include "SOP_GDT.h"

#include "SOP_API.h"
#include <GDT/GDT_MirrorTransform.h>
#include <GU/GU_Brush.h>
#include <GA/GA_Names.h>


class GEO_PointTree;
class GU_RayIntersect;
class TIL_TextureMap;
class GEO_Primitive;

class PRM_MenuEntry;

enum SOP_BrushEvent {
    SOP_BRUSHSTROKE_BEGIN,
    SOP_BRUSHSTROKE_ACTIVE,
    SOP_BRUSHSTROKE_END,
    SOP_BRUSHSTROKE_CLICK,
    SOP_BRUSHSTROKE_NOP
};

enum SOP_BrushOp {
    SOP_BRUSHOP_UNASSIGNED,
    SOP_BRUSHOP_DEFORM,
    SOP_BRUSHOP_COMB,
    SOP_BRUSHOP_PAINT,
    SOP_BRUSHOP_SMOOTH,
    SOP_BRUSHOP_SCRIPT,
    SOP_BRUSHOP_SMOOTHDEFORM,
    SOP_BRUSHOP_EYEDROP,
    SOP_BRUSHOP_ERASE,
    SOP_BRUSHOP_SMOOTHATTRIB,
    SOP_BRUSHOP_SMOOTHNORMAL,
    SOP_BRUSHOP_CALLBACK,
    SOP_BRUSHOP_DRAGTEXTURE,
    SOP_BRUSHOP_SCALETEXTURE,
    SOP_BRUSHOP_SMOOTHTEXTURE,
    SOP_BRUSHOP_SMOOTHLAYER,
    SOP_BRUSHOP_SMOOTHSINGLE,
    SOP_BRUSHOP_REDUCE,
    SOP_BRUSHOP_ERASESINGLE,
    SOP_BRUSHOP_LIFT,
    SOP_BRUSHOP_ROTATE,
};

enum SOP_BrushShape {
    SOP_BRUSHSHAPE_CIRCLE,
    SOP_BRUSHSHAPE_SQUARE,
    SOP_BRUSHSHAPE_BITMAP
};

enum SOP_BrushUpType {
    SOP_BRUSHUP_STROKEDIR,
    SOP_BRUSHUP_FIXED
};

enum SOP_BrushVisType {
    SOP_BRUSHVIS_FALSECOLOUR,
    SOP_BRUSHVIS_CAPTUREWEIGHT
};

#define SOP_BRUSH_NUM_PENS 2

class SOP_API SOP_BrushBase : public SOP_GDT
{
public:
	     SOP_BrushBase(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_BrushBase();

    virtual OP_ERROR		 cookInputGroups(OP_Context &context,
						 int alone = 0);

    // These set functions adjust the brushes current state.
    // The MSS State will use them to inform the SOP how to recook.
    virtual void setAccumulateStencil(bool yesno) = 0;
    virtual void setRayOrigin(const UT_Vector3 &orig, fpreal t) = 0;
    virtual void setRayOrientation(const UT_Vector3 &orient, fpreal t) = 0;
    virtual void setHitPrimitive(int primidx, fpreal t) = 0;
    virtual void setHitPoint(int /* ptidx */, fpreal /* t */) {}
    virtual void setHitUVW(fpreal u, fpreal v, fpreal w, fpreal t) = 0;
    virtual void setHitPressure(fpreal pressure, fpreal t) = 0;
    virtual void setHitInUV(bool inuv, const UT_StringHolder &uvattribname)
    {
        myHitInUV = inuv;
        if (myHitInUV)
        {
            UT_ASSERT(uvattribname.isstring());
            if (uvattribname.isstring())
                myUVAttribName = uvattribname;
            else
                myUVAttribName = GA_Names::uv;
        }
        else
        {
            myUVAttribName = UT_StringHolder::theEmptyString;
        }
    }
    virtual void setBrushEvent(SOP_BrushEvent event) = 0;
    virtual void setBrushOp(SOP_BrushOp op) = 0;
    virtual void setBrushShape(SOP_BrushShape shape) = 0;
    virtual void setProjectionType(int projtype) = 0;
    virtual void useForegroundColor(void) = 0;
    virtual void useBackgroundColor(void) = 0;
    // These are used to update the current radius.
    virtual void setRadius(fpreal radius, fpreal t) = 0;
    virtual void setUVRadius(fpreal radius, fpreal t) = 0;

    void 	setCachedOp(int pen, SOP_BrushOp op);
    SOP_BrushOp	getCachedOp(int pen) const { UT_ASSERT(pen < SOP_BRUSH_NUM_PENS); return myCachedBrushOp[pen]; }

    virtual const GA_PrimitiveGroup	*getPrimitiveSelection(void) const
				 {
				     return (const GA_PrimitiveGroup*) myGroup;
				 }
			    
    // Gets the rayintersector for this current group setup and specified time.
    // If the checkgeo is true, it will test to see if the intersect geo
    // has changed.  We normally do NOT want to do this as rebuilding the
    // cache is expensive if it was our own change.  However, when locating
    // or mouse down we should enable this to catch other triggers.
    // The probeonly, if set to true, will prevent the rebuilding of
    // the GU_RayIntersect, so may return null.  This provides a fast way
    // to update the flags needed to check if geometry changes and
    // a rebuild is necessary later.
    virtual GU_RayIntersect *getRayIntersect(
        fpreal t,
        bool checkgeochange = false,
        bool probeonly = false);

    // Gets the GEO_PointTree for the current group setup.
    GEO_PointTree		*getPointTree();
    /// NOTE: This is only valid if gdp hasn't been evicted/replaced,
    ///       though this was exactly as unsafe as before, when
    ///       GEO_PointTree held onto a pointer to the detail, and
    ///       MSS_EditState and MSS_BrushBaseState were accessing the
    ///       detail through there, since the GEO_PointTree didn't have
    ///       a read handle on the detail.
    const GU_Detail             *lockPointTreeDetail()
    { return myGdpHandle.readLock(); }
    void                         unlockPointTreeDetail(const GU_Detail *detail)
    { myGdpHandle.unlock(detail); }

    // Gets or creates the cursor geometry:
    GU_Detail			*getCursorGdp();

    // This both orients the cursor and tracks what the current brush
    // direction is.
    // isuv is true if the intersection is from the UV viewport.
    // hitu & hitv will be the parametric coordinates of the hit location
    // on prim.  prim will be null for 2d viewports (or in the 3d viewport
    // if there was no hit)
    void			 orientCursor(fpreal t, const UT_Vector3 &orig,
					const UT_Vector3 &dir,
					float scalehistory,
					bool isuv, GA_Index prim,
					float hitu, float hitv, float hitw);

    // Gets the interesction gdp.
    virtual const GU_Detail     *getIsectGdp(fpreal t);

    // The *Static methods can be hooked into buttons as callbacks to
    // perform the listed function.

    // applyStencil will copy the current delta into the permament delta
    // and clear the current.
    static int			 applyStencilStatic(void *op, int idx, fpreal t,
					const PRM_Template *);
    void			 applyStencil(fpreal t, int recook = 1);

    // resetNormal recalculates the surface normals off the current deformed
    // geometry.
    static int			 resetNormalStatic(void *op, int idx, fpreal t,
					const PRM_Template *);
    void			 resetNormal(fpreal t);

    // flood applies the current operation (as found using OP()) to the
    // entire selection.
    static int			 floodStatic(void *op, int idx, fpreal t,
					const PRM_Template *);
    virtual void		 flood(fpreal t);
    
    // NOTE: There is a SOP_GDT::clearallStatic which can be used
    // for reset all changes.
    virtual void		 clearAllDeltas(fpreal t);

    static void 		 customMergeModeStatic(GA_Offset ptoff,
					    GA_Offset vtxoff,
					    UT_Vector3 &result,
					    const UT_Vector3 &cd,
					    const UT_Vector3 &origcol,
					    void *data);
    void 		 	 customMergeMode(GA_Offset ptoff,
					    GA_Offset vtxoff,
					    UT_Vector3 &result,
					    const UT_Vector3 &cd,
					    const UT_Vector3 &origcol);

    static void		brushOpCallbackStatic(
			    GA_Offset pt,
			    const UT_Array<GA_Offset> *ptneighbour,
			    GA_Offset vtx,
			    const UT_Array<GA_Offset> *vtxneighbor,
			    float alpha,
			    GEO_Delta *delta,
			    void *data);
    virtual void	brushOpCallback(
			    GA_Offset pt,
			    const UT_Array<GA_Offset> *ptneighbour,
			    GA_Offset vtx,
			    const UT_Array<GA_Offset> *vtxneighbour,
			    float alpha,
			    GEO_Delta *delta);

    // Returns 1 if there is anything in the brush's accumulation buffer
    bool			 hasStencil() const;

    virtual void		 setCurrentColor(const UT_Vector3 &cd);

    // Use our brush & visualization mode to do the visualization:
    void			 applyVisualizeStencil();
    void			 applyVisualize();
    void			 applyCaptureVisualize();

    int getIsectNodeId() const
    {
        return myIsectNodeId;
    }
    void setIsectNodeId(int node_id)
    {
        myIsectNodeId = node_id;
    }

    virtual void		 unloadDataHack();
    virtual bool		 unloadData();

protected:
    // cookMySop will apply the GDT and react to the Brush events,
    // but you won't be able to patch in your own code.
    virtual OP_ERROR		 cookMySop(OP_Context &context);

    // A wrapper function for duplicateChangedSource.  At this level,
    // it simply calls duplicateChnagedSource, but can be overridden
    // in subclasses to define attributes on the duplicated gdp
    // (for instance)
    virtual void		 duplicateChangedSourceWrapper(
						    unsigned idx,
						    OP_Context &context,
						    int *changed,
						    bool force=false);

    // Call this method do all the GDT application and brush parsing.
    // After calling this, any current brush strokes triggerd by the
    // brush state will be processed, and your gdp will have the result
    // of applying the GDT.
    // This should be called after you have duplicateSource (and/or merely
    // did a rollback), and have created any attributes you wish.
    // Note that as a result of this, new attributes may be created,
    // so attribute offsets should be refetched.
    virtual OP_ERROR		 processBrushOp(OP_Context &context,
					bool changed_input, bool changed_group);

    // This determines whether the given undoblock name matches one of our
    // undoblocks. If it matches, then our undo is collapsed with this block.
    // Remember to call the base class!
    virtual bool		 matchesUndo(const char *undoname) const;

protected:
    // These apply the named operation through the brush's stencil,
    // taking into account the various unusual parameters.
    void			 doDeformation(fpreal t, const UT_Vector3 &cd);
    void			 doPaint(fpreal t, const UT_Vector3 &cd,
					int enablestamping = 1);
    virtual void		 doErase() = 0;
    virtual void		 addEraseAttributes(GU_Brush &brush) {}

    // Finds the seed point from a seed primitive.  This is the closest
    // point.  Somewhat complicated with hulls to avoid n^2 search.
    GA_Offset			 getSeedPoint(const GEO_Primitive *prim,
					float u, float v, float w,
					const UT_Vector3 &orig);

    virtual OP_Node		*getRestGeoInput() { return 0; }

public:
    // If the action of the brush will change point positions, altersGeometry
    // should return 1.
    virtual int		 altersGeometry() const = 0;
    // If the action of the brush will change point colours, altersColor
    // should return 1.
    virtual int		 altersColor() const = 0;
    
    // The RADIUS and OPACITY are defined locally as they take into
    // account the current RAY_HITPRESSURE.
    // The RADIUS function will also switch its output between RAWRADIUS
    // and RAWUVRADIUS depending on the state of the brush cursor.
    virtual fpreal	 RADIUS(fpreal t);
    virtual fpreal	 RAWRADIUS(fpreal t) = 0;
    virtual fpreal	 RAWUVRADIUS(fpreal t) = 0;
    virtual fpreal	 RADIUSPRESSURE(fpreal t) = 0;
    virtual fpreal	 OPACITY(fpreal t);
    virtual fpreal	 RAWOPACITY(fpreal t) = 0;
    virtual fpreal	 OPACITYPRESSURE(fpreal t) = 0;

    // DEPTH is how far into the surface the brush should stick,
    // and HEIGHT is how far above.
    virtual fpreal	 DEPTH(fpreal /* t */) { return 1.0; }
    virtual fpreal	 HEIGHT(fpreal /* t */) { return 1.0; }

    virtual int		 USEDEPTH() { return 0; }
    virtual int		 USECONNECTIVITY() { return 1; }
    virtual int		 USENORMALS() { return 1; }
    
    virtual SOP_BrushOp	 OP(void) = 0;
    virtual int		 ACCUMSTENCIL(void)  = 0;
    virtual int		 PROJECTIONTYPE(void) = 0;
    virtual int		 REALTIME(void) = 0;
    virtual int		 SHAPE(fpreal t) = 0;
protected:
    virtual int		 MERGEMODE() = 0;
    // Script does not yet do anything, and is reserved.
    virtual void	 SCRIPT(UT_String &s, fpreal t) = 0;
    virtual int		 AXIS() = 0;
    virtual fpreal	 USERX(fpreal t) = 0;
    virtual fpreal	 USERY(fpreal t) = 0;
    virtual fpreal	 USERZ(fpreal t) = 0;

    virtual fpreal	 RAYORIENTX(fpreal t) = 0;
    virtual fpreal	 RAYORIENTY(fpreal t) = 0;
    virtual fpreal	 RAYORIENTZ(fpreal t) = 0;
    virtual fpreal	 RAYHITX(fpreal t) = 0;
    virtual fpreal	 RAYHITY(fpreal t) = 0;
    virtual fpreal	 RAYHITZ(fpreal t) = 0;
    virtual fpreal	 RAYHITU(fpreal t) = 0;
    virtual fpreal	 RAYHITV(fpreal t) = 0;
    virtual fpreal	 RAYHITW(fpreal t) = 0;
    virtual fpreal	 RAYHITPRESSURE(fpreal t) = 0;
    virtual int		 PRIMHIT(fpreal t) = 0;
    virtual int		 PTHIT(fpreal /* t */) { return -1; }
    virtual int		 EVENT()	= 0;
    
    virtual bool	 USE_FOREGROUND() = 0;
    virtual fpreal	 FGR(fpreal t) = 0;
    virtual fpreal	 FGG(fpreal t) = 0;
    virtual fpreal	 FGB(fpreal t) = 0;
    virtual fpreal	 BGR(fpreal t) = 0;
    virtual fpreal	 BGG(fpreal t) = 0;
    virtual fpreal	 BGB(fpreal t) = 0;

    virtual fpreal	 SOFTEDGE(fpreal t) = 0;
    virtual void	 KERNEL(UT_String &str, fpreal t)  = 0;
    virtual int		 UPTYPE(fpreal t) = 0;
    virtual fpreal	 UPX(fpreal t)   = 0;
    virtual fpreal	 UPY(fpreal t)   = 0;
    virtual fpreal	 UPZ(fpreal t)   = 0;

    virtual fpreal	 PAPERNOISE(fpreal t) = 0;
    virtual fpreal	 SPLATTER(fpreal t) = 0;
    virtual void	 BITMAP(UT_String &str, fpreal t) = 0;
    virtual int		 BITMAPCHAN(fpreal t) = 0;
    virtual fpreal	 ANGLE(fpreal t) = 0;
    virtual fpreal	 SQUASH(fpreal t) = 0;
    virtual int		 DOSTAMPING() = 0;
    virtual int		 WRITEALPHA() = 0;
    virtual int		 OVERRIDECD() = 0;
    virtual void	 CDNAME(UT_String &str, fpreal t) = 0;
    virtual int		 OVERRIDEALPHA() = 0;
    virtual void	 ALPHANAME(UT_String &str, fpreal t) = 0;
    virtual int		 PRESERVENML() { return 0; }
    virtual int		 OVERRIDENML() = 0;
    virtual void	 NMLNAME(UT_String &str, fpreal t) = 0;

    virtual fpreal	 SYMMETRYDIRX(fpreal t) = 0;
    virtual fpreal	 SYMMETRYDIRY(fpreal t) = 0;
    virtual fpreal	 SYMMETRYDIRZ(fpreal t) = 0;
    virtual fpreal	 SYMMETRYORIGX(fpreal t) = 0;
    virtual fpreal	 SYMMETRYORIGY(fpreal t) = 0;
    virtual fpreal	 SYMMETRYORIGZ(fpreal t) = 0;
    virtual int		 DOREFLECTION() = 0;
    virtual int		 DOPOINTREFLECTION() { return 0; };
    virtual int		 DOROTATION() = 0;
    virtual int		 SYMMETRYROT(fpreal t) = 0;
    virtual fpreal	 SYMMETRYDIST(fpreal t) = 0;

    virtual int		 DOUVREFLECTION() { return 0; }
    virtual fpreal	 SYMMETRYUVORIGU(fpreal /*t*/) { return 0.5; }
    virtual fpreal	 SYMMETRYUVORIGV(fpreal /*t*/) { return 0.5; }
    virtual fpreal	 SYMMETRYUVANGLE(fpreal /*t*/) { return 0.0; }

    virtual int		 ADDATTRIB() = 0;
    virtual void         UVATTRIB(UT_String &uvattribname, fpreal t)
    { uvattribname.clear(); }
    virtual int		 VISUALIZE() { return 0; }
    virtual int		 VISTYPE()   { return 0; }
    virtual fpreal	 VISLOW(fpreal /*t*/) { return 0.0; }
    virtual fpreal	 VISHIGH(fpreal /*t*/) { return 1.0; }
    virtual int		 VISMODE()   { return 0; }
    virtual fpreal	 ZEROWEIGHTCOLOR_R() { return 1.0; }
    virtual fpreal	 ZEROWEIGHTCOLOR_G() { return 1.0; }
    virtual fpreal	 ZEROWEIGHTCOLOR_B() { return 1.0; }

    virtual int		 NORMALIZEWEIGHT() { return 0; }

    virtual int		 USECAPTURE() { return 0; }
    virtual int		 CAPTUREIDX(fpreal /*t*/) { return 0; }
    virtual GEO_Detail::CaptureType
			 CAPTURETYPE() { return GEO_Detail::CAPTURE_BONE; }

    // Find out if the brush has moved or done anything to cause us to add its
    // effect to the current gdp.
    virtual bool	 hasStrokeChanged(fpreal t) = 0;
    virtual bool	 hasStyleChanged (fpreal t) = 0;

    // Find if the nib file has changed:
    virtual bool	 hasNibFileChanged(fpreal t) = 0;

    // Find if any of the nib description values have changed:
    virtual bool	 hasNibLookChanged(fpreal t) = 0;
    virtual bool	 hasAccumStencilChanged(fpreal t) = 0;

    virtual bool	 hasCaptureIdxChanged(fpreal /*t*/) { return 0; }
    
    virtual bool	 hasVisrangeChanged(fpreal /*t*/) { return 0; }

    // Do we want our cook selection to be updated and displayed?
    virtual bool	 wantsCookSelection(void) const { return false; }

    virtual void	 rollbackCurrentDelta();
    virtual void	 applyAndClearCurrentDelta();

    virtual const GA_PointGroup	*getPointGroup() { return 0; }
    virtual bool isGroupParmDirty(OP_Context &context)
		{
		    return isParmDirty(SOP_GDT_GRP_IDX, context.getTime());
		}

    void rebuildCaches(
        const GU_Detail *isectgdp,
        const GA_PrimitiveGroup *primgroup,
        const GA_PointGroup *ptgroup,
        bool invalidate_vtx,
        bool invalidate_pt);

    UT_Vector3		 myLastPos;
    UT_Vector3		 myBrushDir;
    
    // This stores the last uv location chosen in the 3d viewport.
    GA_Index		 myPendingLastUVPrimitive;
    UT_Vector3		 myPendingLastUVPrimitiveUV;

    // This is the last uv location chosen in the 3d viewport that
    // was cooked with.
    GA_Index		 myLastUVPrimitive;
    UT_Vector3		 myLastUVPrimitiveUV;

    // This is the last uv position that was cooked with.
    bool		 myLastUVPosValid;
    UT_Vector3		 myLastUVPos;

    UT_Vector3		 myUVBrushDir;
    UT_Vector3		 myUVBrushPos;
    GU_Brush	 	 myBrush;
    GU_RayIntersect	*myRayIntersect;
    TIL_TextureMap	*myNibFile;
    GU_Detail		*myBrushCursor;
    UT_Matrix3		 myCursorOrient;

    // These two are intentionally ephemeral variable which is NOT saved.
    // The state tweaks this to turn on/off visualization.
    int		 	 myForceVisualize;
    int			 myIsectNodeId;

    /// These track our last geo and last isectgeo to see if it has changed.
    /// @{
    int                  myLastGDPId;
    int                  myLastIsectGDPId;
    GA_Size              myLastIsectGDPPtCount;
    int64                myLastIsectGDPTopoId;
    int64                myLastIsectGDPPrimId;
    int64                myLastIsectGDPPId;
    int64                myLastIsectGDPVisId;
    int64                myLastIsectGDPPrGrpId;
    int64                myLastIsectGDPPtGrpId;
    /// @}

    // This is so our callbacks know our cook time...
    fpreal		 myCookTime;

    bool		 myHitInUV;
    UT_StringHolder      myUVAttribName;

    GDT_Detail		 myMirroredDelta;

    GDT_MirrorTransform	 myMirrorTransform;

    SOP_BrushOp		 myCachedBrushOp[SOP_BRUSH_NUM_PENS];
};

#endif

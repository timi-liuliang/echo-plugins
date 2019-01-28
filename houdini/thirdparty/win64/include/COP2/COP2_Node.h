/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Node.h
 *
 * COMMENTS:
 *	2nd Generation Composite operator base class
 */

#ifndef __COP2_Node_h__
#define __COP2_Node_h__

#include "COP2_API.h"
#include <UT/UT_ErrorManager.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_Lock.h>
#include <UT/UT_Rect.h>
#include <UT/UT_TokenString.h>
#include <UT/UT_Thread.h>
#include <UT/UT_FilterType.h>
#include <UT/UT_VectorTypes.h>

#include <OP/OP_Network.h>

#include <TIL/TIL_Defines.h>
#include <TIL/TIL_Sequence.h>
#include <TIL/TIL_Region.h>
#include <TIL/TIL_TileList.h>

#include "COP2_Common.h"
#include "COP2_Context.h"
#include "COP2_BoundsCache.h"

class UT_Color;
class UT_FileLUT;
class UT_Interrupt;
class UT_StringArray;
class IMG_TileOptions;
class TIL_Raster;
class TIL_TileCache;
class TIL_Plane;
class TIL_ProxyManager;
class TIL_Region;
class TIL_Tile;
class TIL_TileList;
class TIL_TileManager;
class TIL_TileMPlay;
class PXL_Lookup;
class OP_OperatorTable;
class OP_TemplatePair;
class OP_VariablePair;
class COP2_ImageSource;
class COP2_PixelFunction;
class COP2_TransformParms;
class COP2_ThreadCookParms;
class COP2_PlaneCookParms;
class COP2_CheckpointWrapper;
class COP2_CookAreaInfo;
class COP2_CookAreaList;
class COP2_VolatileParmManager;
class COP2_DebugTimingTable;

typedef OP_ERROR (*COP2_FullImageCB)(COP2_Context &, const TIL_Region *,
				     TIL_Region *, COP2_Node *);


class COP2_API COP2_Node : public OP_Network
{
public:

    // Lets the COP know that we are accessing, and again when we're done.
    virtual OP_ERROR	 open(short &key, int clear_errors = 0);
	    bool	 isOpen() const { return myCopOpenCount > 0; }
    virtual void	 close(short key);

    // gets sequence information for this COP: start/end, res, rate. 
    TIL_Sequence *	 getSequenceInfo(OP_ERROR *error = 0);

    // gets the sequence info for an input to this COP. 
    const TIL_Sequence  *inputInfo(int input);

    // gets the sequence time for the current time.
    float		 getSequenceTime() const;
    
    // gets the image source for the viewer.
    COP2_ImageSource	*getImageSource() { return myImageSource; }
    
    virtual void	 forceRecook(bool evensmartcache = false);

    // Used for collecting cook timing statistics all nodes in one cook.
    // You maintain ownership of the table.
    static void		 setNodeTimingTable(COP2_DebugTimingTable *table);

    // Sets the LUT (globally) that will be used for previewing nodes in
    // the network editor
    static void
    setPreviewLUT(const UT_String &name, bool is_ocio = false);

    static UT_SharedPtr<PXL_Lookup> getPreviewLUT();
    static const char* getPreviewLUTName();

    // cooks this COP and produces an output raster, stored in 'raster'.
    // the second method cooks a sub-region of the image; if subregion
    // is 1, the raster contains the full image and only the sub-region
    // is updated. If 0, the raster will only contain the sub-region.
    // returns 1 on success or 0 if an error occurred.
    int			 cookToRaster(TIL_Raster *raster, OP_Context &,
				      const TIL_Plane *plane,
				      int array_index = 0,
				      int subregion = 1, int clear = 1,
				      float black = 0.0F, float white = 1.0F,
				      int compindex = -1,
				      int ignore_bwpoints=0,
				      const int *comp_map=0,
				      float gamma = 1.0F,
				      bool interactive = false);
    
    int			 cookToRaster(TIL_Raster *raster, OP_Context &,
				      const TIL_Plane *plane,
				      int array_index,
				      int xstart,int ystart,int xend,int yend,
				      int subregion = 1, int clear = 1,
				      float black = 0.0F, float white = 1.0F,
				      int compindex = -1,
				      int ignore_bwpoints=0,
				      const int *comp_map=0,
				      float gamma = 1.0F,
				      bool interactive = false,
				      UT_ValArray<TIL_Raster *> *tiles = 0,
				      int tile_index = 0, TIL_TileMPlay * =0,
				      bool cleanup_after = true);

    // This method cooks multiple output rasters at a time. The arrays must all
    // be the same size (except in the case of flipbooking to TIL_TileMplay,
    // where rasters may be empty). Currently, rendering to different
    // OP_Context resolutions is not supported; only to different
    // context.myTime's. 
    int			 cookToRaster(UT_ValArray<TIL_Raster *> &rasters,
				      UT_ValArray<OP_Context *> &contexts,
				      UT_ValArray<const TIL_Plane *> &planes,
				      UT_IntArray &array_index,
				      UT_ValArray<const int *> &comp_map,
				      UT_Array<UT_InclusiveRect> &bounds,
				      int subregion = 1, int clear = 1,
				      float black = 0.0F, float white = 1.0F,
				      int compindex = -1,
				      int ignore_bwpoints=0,
				      float gamma = 1.0F,
				      bool interactive = false,
				      UT_ValArray<TIL_Raster *> *tiles = 0,
				      int tile_index = 0, TIL_TileMPlay * =0,
				      bool cleanup_after = true);

    // This method sets data in "plane_parms" corresponding to the other
    // parameters that are passed in.  The plane cooking parms are passed
    // to the different threads that perform the cooking.
    void		 computePlaneCookParms(COP2_PlaneCookParms &plane_parms,
					       OP_Context &context,
					       const TIL_Plane *rplane,
					       int array_index,
					       TIL_Raster *raster,
					       int thread_index,
					       const int *compmap,
					       int xstart, int ystart,
					       int xend, int yend,
					       bool flipbook,
					       UT_ValArray<TIL_Tile *> *tile_list);

    // Is the specified plane in the tile cache?
    bool		 isPlaneCached(const TIL_Plane &refplane,
				    int array_index, float time,
				    int xres, int yres,
				    int xstart, int ystart, int xend, int yend,
				    bool flipbook);
    bool		 isPlaneCached(COP2_Context &context,
				    const TIL_Plane &refplane,
				    int xstart, int ystart, int xend, int yend,
				    bool flipbook);

    // Move tiles for a particular plane from the regular cache into the
    // list of checkpointed tiles.  This method is called from the cook
    // scheduler.
    void		 checkpointCachedTilesForPlane(
				    const TIL_Plane &refplane,
				    int array_index, float time,
				    int xres, int yres,
				    int xstart, int ystart, int xend, int yend,
				    bool flipbook,
				    UT_ValArray<TIL_Tile *> &tile_list);
    void		 checkpointCachedTilesForPlane(COP2_Context &context,
				    const TIL_Plane &refplane,
				    int xstart, int ystart, int xend, int yend,
				    bool flipbook,
				    UT_ValArray<TIL_Tile *> &tile_list);

    // This method maps from playbar time to the time in the sequence when the
    // node will actually be cooked.  File COPs, for example, always cook at
    // time 0 if their sequence is a single image.
    double		 mapTimeIntoSequence(double time);


    // cooks this OP into a file (which should not be open yet). If scope isn't
    // null, only the scoped planes in that sequence will be writen to the file.
    int			 cookToFile(const UT_StringArray &filenames,
				    const IMG_TileOptions *,
				    const TIL_Sequence *scope,
				    const UT_IntArray &image_index,
				    const char *uselut = 0,
				    float gamma = 1.0F,
				    UT_InclusiveRect *bounds = 0,
				    bool single_frame = false,
				    bool thread_io = false,
				    bool env_map = false,
				    bool use_progress_dialog=true);
    
    int			 cookToFile(const UT_StringArray &filenames,
				    const IMG_TileOptions *,
				    OP_Context &context,
				    const UT_IntArray &image_index,
				    const char *cplane, const char *aplane = 0,
				    const char *uselut = 0,
				    float gamma = 1.0F,
				    UT_InclusiveRect *bounds = 0,
				    bool single_frame = false,
				    bool thread_io = false,
				    bool env_map = false,
				    bool use_progress_dialog=true);

    void		waitForLastFile();

    // Does a flipbook render. 'info' should contain the planes to render,
    // the frame range and image size.
    int			cookFlipbook(const TIL_Sequence &info, int step = 1,
				     float gamma = 1.0f,
				     const char *lut = 0, bool startnew=true,
				     bool (*updateCallback)(void *) = 0,
				     void  *updateEntity = 0,
				     bool write_houdini_frame = true,
				     const char *session_label = NULL);

    // returns the identifier for a specific image at this node, described by
    // context.xres/yres/time, plane and array_index. You must free
    // the token string when you're done with it.
    virtual UT_TokenString *getImageIdentifier(const TIL_Plane *plane,
					       int array_index, float t,
					       int xres, int yres,
					       int thread);

    // getContextData returns an existing context data or allocates a new
    // context if it's not found. Contexts only exist for a single cook, and
    // are deleted once close() is called.  
    COP2_Context		*getContextData(const TIL_Plane *plane,
						int		array_index,
						float		t,
						int		xres,
						int		yres,
						int		thread,
						bool force_bounds = true);

    // Determines the bounds of the operation. Normally (0,0) - (xres-1,yres-1)
    void		 getImageBounds(const TIL_Plane *plane, int array,
				     float t, int xres, int yres, int thread,
				     int &x1, int &y1,int &x2, int &y2);
    inline void		 getImageBounds(COP2_Context &context,
				     int &x1, int &y1,int &x2, int &y2);
					
    // Gets the bounds of input 'input'. Returns false if the input does not
    // exist.
    inline bool		 getInputBounds(int input, COP2_Context &context,
					int &x1, int &y1, int &x2, int &y2);
    
    // General version of above.
    bool		 getInputBounds(int input,
					const TIL_Plane *plane, int array,
					float t, int xres, int yres,
					int thread,
					int &x1, int &y1, int &x2, int &y2);

    // returns true if the tile at (tilex,tiley) pixel coords is aligned &
    // the same size as the corresponding tile in the input. This can only
    // be called once this nodes bounds have been determined (ie, don't call
    // from computeImageBounds). with_node is optional; it uses a different
    // node instead of 'this' ('with_node' and its input 'input').
    bool		 isTileAlignedWithInput(int input,
						COP2_Context &context,
						int tilex, int tiley,
						COP2_Node *with_node = 0);
    bool		 isTileAlignedWithInput(int input,
						const TIL_Plane *plane,
						int array_index,
						float t, int xres, int yres,
						int thread,
						int tilex, int tiley,
						COP2_Node *with_node=0);

    // The following method is called when you wish to compare the alignment
    // with the input at a different time.
    bool		 isTileAlignedWithInputAtTime(int input,
						      COP2_Context &context,
						      int tilex, int tiley,
						      float input_time,
						      COP2_Node *with_node=0);

    // The following function is very similar to isTileAlignedWithInput,
    // except it compares the tile in the specified input plane with that
    // specified in this node's context.  In particular, this is used when
    // we cannot assume that this node actually contains the input plane.
    bool		 isInputTileAlignedWithContext(int input,
						       const TIL_Plane *plane,
						       int tilex, int tiley,
						       COP2_Context &context,
						       COP2_Node *with_node=0);
    
    bool		 arePlanesAligned(COP2_Context &context,
					  const TIL_Plane *plane1, int aindex1,
					  const TIL_Plane *plane2, int aindex2,
					  int tilex, int tiley);


    bool                 isInputPlaneCompatible(COP2_Context &context,
						  int input_num);

    // Returns the difference in origin of the this bound with its input.
    bool		 getInputBoundsDiff(int input, COP2_Context &context,
					    int &dx, int &dy);
    

    // Allows input COPs to remap the time into their time space. 
    virtual fpreal	 getTimeTransform(int input, fpreal t);

    // Set the resolution in the input context to the input's resolution.
    virtual void         getInputRes(int input, fpreal t,
				    const OP_Context &context,
				    OP_Context &icontext);

    // allows generators to remap their cook time.
    virtual double	 remapTime(double t);
    virtual bool	 isTimeModifier() const { return false; }
    
    // return true if this is a slow operation that may need interactive
    // quality reduction help.
    virtual bool	 isSlowOperation() const { return true; }
    virtual bool	 isTimeDepOperation()  { return false; }
    virtual bool	 isGPUAssisted(const TIL_Plane & /*plane*/,
				       int /*array_index*/, float /*t*/,
				       int /*xres*/, int /*yres*/,
				       int /*thread_index*/)
					{ return false; }

    virtual bool	 isGenerator() const { return false; }
    virtual bool	 isPixelBased() const { return false; }

    virtual int		 getMaskInput() const	{ return -1; }
    
    // these functions return a scale factor for biasing U and V lengths for
    // things like blur kernels and transforms based on the proxy level,
    // pixel aspect ratio and interlacing.
	    void	 getScaleFactors(int xres,  int yres,
					 float &sx, float &sy) const;
	    float	 getXScaleFactor(int xres) const;
	    float	 getYScaleFactor(int yres) const;

    // returns a plane which determines what should be displayed in the icon
    // preview (RGB, alpha, depth, etc).
    TIL_Plane		*getPreviewPlane(int &index);
    int			 getPreviewPlane(int &pindex, int &aindex);
    int			 isFollowingParent(const COP2_Node *parent);
    void		 setPreviewPlane(int pindex, int aindex =0);
    void		 parentPreviewChanged();

    // return one of the follwing if your COP cannot be cooked multithreaded.
    // COP_MULTITHREADED allows multiple threads in the COP at once.
    // COP_SINGLE_THREAD allows only one thread in the COP at once, but
    //		releases the lock when the COP cooks its inputs.
    // COP_EXCLUSIVE_THREAD locks the COP until its cleanup function is called.
    //		This mode pretty much kills multithreaded performance. Use with
    //		care! (useful for debugging code). 
    virtual int		 isSingleThreadedCop() const {return COP_MULTITHREADED;}

    // clears out the cache for this node.
    void		 clearTilesInCache();

    // returns true if the frame in question exists. Only really overridden
    // by the file COP. Set adjust to be true if you can use another frame
    // in its place (like using the closest frame instead)
    virtual bool	 doesFrameExist(int /*frame*/,
					bool /*adjust*/ =true) { return true; }

    // frame locking

    // returns 1 if any plane at any time is locked.
    int			 isAnythingLocked() const;
    
    // returns 1 if the plane is ever locked.
    int			 isPlaneLocked(const TIL_Plane *p) const;

    // returns 1 if the COP is locked at time t
    int			 isSequenceLocked(float t);
	
    // returns 1 if the plane is locked at time t
    int			 isPlaneLocked(const TIL_Plane *p, float t);

    // both methods return 1 if the op was successful.
    int			 lockPlane(const TIL_Plane *p, float t);
    int			 lockPlanes(float t);
    int			 unlockPlane(const TIL_Plane *p, float t);
    int			 unlockPlanes(float t);
    int			 unlockAll();

    // Disk Caching for this node
    void		 enableDiskCache(bool enable);
    bool		 isDiskCacheEnabled() const { return myProxyEnable; }

    // Color selections.
    virtual void	 setColorSelection(float *col, int size, bool accum);
    
    // Callback from the cache manager to remove tiles from a COP's list
    // when the tile is thrown out of the cache.
    static void		 tileRemovalCallback(void *parent, TIL_Tile *removed);
    void		 tileRemoved(TIL_Tile *removed);

    static void		 setUpdateCallback(void (*callback)(float, bool, bool));
    static void		 setViewerCallback(void (*viewer)(COP2_Node *, void *,
							  TIL_ViewerType ),
					   void *data);
    
    // interactive render commands.
    static void		 setCookPosition(float u, float v);
    static void		 abortCook();
    static bool		 isCookAborted();
    static void		 resetAbortFlag();
    static bool		 isCooking();
    
    bool		 isCookAborted(int thread_index);

    bool		 isInterrupted(int threadidx);
    void		 setInterrupted(int threadidx);
    
    static TIL_TileCache	*getCache();
    static TIL_ProxyManager	*getProxyManager();
    static TIL_TileManager	*getTileManager();
    static bool			 isInitialized();

    virtual OP_ERROR	 getErrorSeverity()
			 { return error(); }
			
    virtual OP_ERROR	 error()
			 {
			     // Call OP_Network::getErrorSeverity() directly
			     // here to avoid locking.
			     OP_ERROR sev = OP_Network::getErrorSeverity();
			     OP_ERROR csev = myCookErrorManager.getSeverity();
			     
			     return sev > csev ? sev : csev;
			 }
    virtual OP_ERROR	 error(OP_Context &context)
			 {
			     // Call OP_Network::getErrorSeverity() directly
			     // here to avoid locking.
			     OP_ERROR sev = OP_Network::getErrorSeverity();
			     OP_ERROR csev = myCookErrorManager.getSeverity();
			     
			     return sev > csev ? sev : csev;
			 }
    
    // need to add a few things to the parm hash code.
    virtual UT_TokenString & getParmHashCode(OP_Context &context,
					     int group_mask = 1);


    // Collapsable Color & transform functions.
    virtual void	 getPixelFunction(const TIL_Plane *plane,
					  int array_index, float t,
					  int xres, int yres,
					  int thread,
					  COP2_PixelFunction *&function,
					  bool concatenate,
					  bool dependent_too);

    virtual void	 getImageTransform(COP2_Context &context,
					   COP2_TransformParms &parms,
					   COP2_Node *& source,
					   int &input_index);

    // returns the color of pixel (x,y) for the given plane in color.
    // 'color' should be big enough to hold the single pixels' values (use
    // 4 floats if in doubt). If it could not be evaluated, false is returned.
    // NB: This cooks the COP for the given pixel.
    //
    bool		 getPixelValue(const TIL_Plane *plane,
				       int array_index, float t,
				       int xres, int yres,
				       int x, int y,
				       float *color);

    bool		 getPixelValueByUV(const char *plane_name,
				       double u, double v, 
				       bool interpolate,
				       UT_FloatArray &vals);

    bool		 getPixelColorValueByUV(COP2_ColorComponent comp,
				       double u, double v, 
				       bool interpolate,
				       float &val);

    COP2_ThreadPref	getThreadPref() const { return myThreadPreference; }

    // by default, most nodes can have as many threads as possible in their
    // cook method. But some COPs can block with more than 1 thread, so this
    // method gives a hint to the engine as to how many threads to allow at
    // once in the cook - set 'plane' to the # of threads that can be cooking
    // a given plane/time in a node; set 'node' to the # of threads that can be
    // cooking in the node, regardless of plane/time; set 'op' to the # of
    // threads that can be in a single Operator type at a time.
    virtual void	getMaxNumThreadsInCook(COP2_Context &,
					       int &plane,
					       int &node,
					       int &op) const
			{ plane = node = op = TIL_MAX_THREADS;  }

    // if true, only thread 0 can cook this operator.
    virtual bool	onlyCookInMainThread() const { return false; }

    // These functions setup and reset the table which translates thread
    // process id's into our thread indices (0-TIL_MAX_THREADS).
    static void		setWorkingThreadId(int thread_index);
    static void		clearWorkingThreadId(int thread_index);

    // Returns the thread index based on the calling thread id
    // (setWorkingThreadId must have been called first).
    int			 getThreadIndex();
	
    // Menu building methods.
    static void		 buildPlanePreviewMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildScopeMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildPlaneScopeMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildScopeMenuNoColAlpha(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildPlaneMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildCompMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildInputScopeMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildInputPlaneMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);
    static void		 buildInputCompMenu(
			    void *, PRM_Name *, int,
			    const PRM_SpareData *, const PRM_Parm *);

    void		 buildPlaneMenu(PRM_Name *items, int maxsize,
					bool useinput = false,
					bool planes_only = false,
					bool comps_only = false,
					bool wild = true,
					int input = 0,
					bool nocoloralpha = false);

    static const char	*getColorPlaneName();
    static const char	*getAlphaPlaneName();
    static const char	*getMaskPlaneName();
    static const char	*getDepthPlaneName();
    static const char	*getLumPlaneName();
    static const char	*getBumpPlaneName();
    static const char	*getPointPlaneName();
    static const char	*getNormalPlaneName();
    static const char	*getVelocityPlaneName();
    static const char	*getThumbnailPlaneKey();

    // Override to specify a plane to view in the COP viewer. No preference
    // by default.
    virtual bool	 preferredViewPlane(UT_WorkBuffer &) { return false; }

    // DEBUG stuff:
    static int		 dumpCache(void *me, int i, float t,
				    const PRM_Template *);
    static void		 printMe(void *me, std::ostream &os);

    static void		 addToFileLoadTime(float t);
    
    // Normal Public OP Stuff ************************************************
    static const char	*theChildTableName;
    virtual const char  *getChildType() const { return COP2_OPTYPE_NAME; }
    virtual const char  *getOpType() const { return COP2_OPTYPE_NAME; }

    virtual OP_OpTypeId  getChildTypeID() const { return COP2_OPTYPE_ID; }
    virtual OP_OpTypeId  getOpTypeID() const { return COP2_OPTYPE_ID; }

    virtual int64	 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += OP_Network::getMemoryUsage(false);
        // FIXME: Count the actual memory here!!!
        return mem;
    }

    virtual OP_DataType	 getCookedDataType() const { return OP_RASTER_DATA; }
    virtual void	*getCookedData(OP_Context &) { return 0; }

    // Query color for Net Overview rendering
    virtual int		 getNetOverviewColor( UT_Color &color );
    virtual void	 opChanged(OP_EventType type, void *data);
    virtual void	 userDataChanged(const UT_StringHolder &key);

    static void		 initializeExpressions();
    static void		 initializeCache();
    static void		 buildOperatorTable(OP_OperatorTable &table);
    static void		 installCommands();

    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms);
    virtual void	 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
				const OP_NodeInfoTreeParms &parms);

    void		 inputRes(int input, float t, int xres, int yres, 
				  int &i_xres, int &i_yres);

    // This is the function called by multiple threads to cook the tiles
    // of a node.
    void		processTile(COP2_ThreadCookParms *p);

    void		setInputBlocked(bool blocked, int thread);

    // Remove all the regions for this node.  This method is called when
    // a node is uncheckpointed.
    void		 removeRegions();

    // Allows you to define which inputs (or nodes) are opened and closed
    // by this node.
    virtual int		 getNumInputsToOpen() const { return nInputs(); }
    virtual COP2_Node   *getInputToOpen(int i)
			 { return CAST_COP2NODE(getInput(i, true)); }

    // For the output area (an area of a plane belonging to this node)
    // and a set of input areas, determine which input areas and which
    // parts of these areas are needed to cook the output area.
    virtual void	 getInputDependenciesForOutputArea(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);

    // A checkpoint wrapper is a data structure that's internal to the
    // cook scheduler.  However, it's convenient to have a fast mapping
    // from a node to a wrapper, the the node stores a pointer to the wrapper.
    COP2_CheckpointWrapper *getCheckpointWrapper(int array_index);
    void		    setCheckpointWrapper(int array_index,
						 COP2_CheckpointWrapper *wrap);

    // these are basically private for the cook scheduler ----- vvvvvv
    // COP2_CheckpointWrapper is not a public class.
    int			    getNumCheckpointWrappers() const;
    COP2_CheckpointWrapper *getWrapper(int i) const;
    void		    resetCheckpointWrappers();
    // --------------------------------------------------------- ^^^^^^

    virtual void	   modifyViewMatrix(int input,
					    COP2_Context &context,
					    UT_Matrix4 &matrix,
					    float zoom);

    virtual COP2_Node	 *selectInput(int px, int py, const char *plane_name,
				      int array_index);
    /// @{
    /// Methods defined on OP_Node
    //Absolute Width
    virtual fpreal	 getW() const;
    //Absolute Height
    virtual fpreal	 getH() const;
    /// @}

    // call to reset any missing or bad frames.
    static void		 resetBadMissingFrames();

    // Global preferences controlled by the pref dialog.
    static bool		 getShowPreviewPref();
    static void		 setShowPreviewPref(bool show);
    
    virtual unsigned	 referenceAllParameters(OP_Parameters *from,
				bool relative_references = true);

protected:
	     COP2_Node(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~COP2_Node();

    virtual OP_ERROR	 cookMe (OP_Context &) { return UT_ERROR_NONE; }
    virtual bool	 cook   (OP_Context &);
    virtual OP_ERROR	 bypassMe(OP_Context &, int &) { return UT_ERROR_NONE; }

    static unsigned int	 getCookScore(float cooktime);
    
    // Set this appropriately if $W, $H should use the full resolution,
    // and not the proxy resolution.  By default, we use the proxy
    // resolution, but if the node itself performs the correction, we
    // want to use the full resolution.
    void		 setVariableUseFullRes(bool onoff);

    virtual bool	 evalVariableValue(fpreal &val, int index, int thread);
    virtual bool	 evalVariableValue(UT_String &v, int i, int thr)
			 { return OP_Network::evalVariableValue(v, i, thr); }

    virtual void	 getMyExternalReferences(UT_StringArray &reflist,
						 UT_StringArray *nodelist =0,
						 bool collapse = false,
						 bool check_missing = false,
						 bool show_missing_only=false);
    
    void		 launchViewer(TIL_ViewerType view);

    // call these methods when an error occurs during cookSequenceInfo().
    void		 addError(int code, const char *msg = 0);
    void		 addSystemError(const char *msg = 0);
    void		 addWarning(int code, const char *msg = 0);
    void		 addSystemWarning(const char *msg = 0);
    void		 addMessage(int code, const char *msg = 0);

    // Call these methods when an error occurs during cooking.
    void		 addCookError(int code, const char *msg = 0);
    void		 addCookSystemError(const char *msg = 0);
    void		 addCookWarning(int code, const char *msg = 0);
    void		 addCookSystemWarning(int code, const char *msg = 0);
    void		 addCookMessage(int code, const char *msg = 0);

    UT_ErrorManager	 myCookErrorManager;
    
    // normally, an image cooked at 100x100 will look similar to the same image
    // cooked at 200x200. If your node does NOT scale with resolution, return 1
    virtual int		 isScaledImageDifferent() const { return 0; }

    // inputOpenFailed is called if the node fails to open.
    virtual void	 inputOpenFailed(int ) { }
    

    virtual COP2_ThreadPref getThreadPreference() const
			    { return COP2_THREAD_NO_PREF; }

    // returns a short phrase describing the operation.
    virtual const char  *getOperationInfo();

    // cooks the information for the sequence.
    virtual TIL_Sequence *  cookSequenceInfo(OP_ERROR &error) = 0;

    bool		 isInputBlocked(int thread);
    
    // gets a specific tile of an input. 
    TIL_TileList	*inputTile(int			input_index,
				   COP2_Context		&context,
				   const TIL_Plane	*plane,
				   int			 array_index,
				   float		 t,
				   int			 xstart,
				   int			 ystart,
				   OP_ERROR		*err = 0,
				   int			 block = 1,
				   bool			*mask = 0,
				   COP2_Node		*fromtile = 0,
				   bool			 correctorig = true,
				   bool			*blocked = 0,
				   bool			*was_corrected = 0);

    // gets a tile out of the plane specified by the context.
    inline TIL_TileList	*inputTile(int			 input_index,
				   COP2_Context		&context,
				   int			 xstart,
				   int			 ystart,
				   OP_ERROR		*err = 0,
				   int			 block = 1,
				   bool			*mask = 0,
				   bool			*blocked = 0,
				   bool			*was_corrected = 0);
    
    // gets a specific tile of an input that corresponds to this tile.
    inline TIL_TileList	*inputTile(int			input,
				   COP2_Context	       &context,
				   TIL_TileList	       *mytile,
				   float		t,
				   OP_ERROR	       *err = 0,
				   int			block = 1,
				   bool		       *mask = 0,
				   bool		       *blocked = 0,
				   bool		       *was_corrected = 0);

    // gets a specific tile specified by the tilelist and the context.
    inline TIL_TileList	*inputTile(int			input,
				   COP2_Context	       &context,
				   TIL_TileList	       *mytile,
				   OP_ERROR	       *err = 0,
				   int			block = 1,
				   bool		       *mask = 0,
				   bool		       *blocked = 0,
				   bool		       *was_corrected = 0);

    // copies the input tile specified in tilelist. 
    bool		 copyInput(int			input,
				   COP2_Context	       &context,
				   TIL_TileList	       *tilelist,
				   float		t,
				   OP_ERROR	       *err = 0,
				   int			block = 1,
				   bool		       *mask = 0);
    
    bool		 copyInput(int			input,
				   COP2_Context	       &context,
				   TIL_TileList	       *mytile,
				   OP_ERROR	       *err = 0,
				   int			block = 1,
				   bool		       *mask = 0);

    // this is a modified version of inputTile() which should be called from
    // passThroughTiles() only, instead of inputTile().
    TIL_TileList	*passInputTile(int		 input_index,
				       COP2_Context	&context,
				       const TIL_Plane	*plane,
				       int		 array_index,
				       float		 t,
				       int		 xstart,
				       int		 ystart,
				       int		 block,
				       bool		*blocked,
				       bool		*mask,
				       COP2_Node	*fromTile = 0);
    
    // must be used to release a tile returned by inputTile when you are
    // finished working with it. 'reset' removes the tiles from the cache.
    void		 releaseTile(TIL_TileList *&, bool reset = false);

    // gets a region of an input channel, cooking tiles where necessary.
    // region_data holds a list of num_region regions, which are allocated
    // by the caller. returns 0 if errors were encountered.
    TIL_Region *	inputRegion(int			 input_index,
				    COP2_Context	&context,
				    const TIL_Plane	*plane,
				    int			 array_index,
				    float		 t,
				    int			 xstart,
				    int			 ystart,
				    int			 xend,
				    int			 yend,
				    TIL_RegionExtend	 hold = TIL_BLACK,
				    int			 share = 1,
				    void *regionmem[PLANE_MAX_VECTOR_SIZE]=0,
				    bool  correct_aspect = true,
				    bool  correct_bounds = true,
				    int   scan_alignment = 0);

    // gets a region out of an input. Plane & array index are specified in the
    // context.
    inline TIL_Region *	inputRegion(int			input_index,
				    COP2_Context       &context,
				    int			xstart,
				    int			ystart,
				    int			xend,
				    int			yend,
				    TIL_RegionExtend	hold = TIL_BLACK,
				    int			share = 1,
				    void *regionmem[PLANE_MAX_VECTOR_SIZE]=0,
				    bool  correct_aspect = true,
				    bool  correct_bounds = true,
				    int   scan_alignment = 0);

    // gets an input region corresponding to a tilelist, an optional shift and
    // an option to write the region data directly into the tilelist data.
    TIL_Region *	inputRegion(int			input_index,
				    COP2_Context       &context,
				    const TIL_Plane    *plane,
				    int			array_index,
				    float		t,
				    TIL_TileList       *tilelist,
				    int			xshift=0,
				    int			yshift=0,
				    int			copy = 0,
				    TIL_RegionExtend	hold = TIL_BLACK);

    
    // gets a region out of an input. Plane & array index are specified in the
    // context, while the region is determined by the tile dimensions.
    inline TIL_Region *	inputRegion(int			input_index,
				    COP2_Context       &context,
				    TIL_TileList       *tilelist,
				    int			xshift=0,
				    int			yshift=0,
				    int			copy = 0,
				    TIL_RegionExtend	hold = TIL_BLACK);

    // allocates a region for output (ie, files read in scanline by scanline
    // require temporary storage before being divided into tiles for
    // efficiency. 
    inline TIL_Region *	outputRegion(const TIL_Plane *plane, int array_index,
				     float t, int xres, int yres, int thread,
				     int xstart, int ystart,
				     int xend, int yend,
				     int share = 1,
				     int rindex = 0,
				     int alignment = 0);
    
    TIL_Region *	outputRegion(COP2_Context &context,
				     int xstart, int ystart,
				     int xend, int yend,
				     int share = 1,
				     int rindex = 0,
				     int alignment = 0);

    // The following version of outputRegion takes a reference plane to use
    // to compute the bounds.  This is useful, for example, when the region
    // we wish to store only has one channel, but we wish the plane to have
    // 3 channels in our sequence info.
    TIL_Region *	outputRegion(const TIL_Plane *plane,
				     const TIL_Plane *ref_plane,
				     int array_index, float t,
				     int xres, int yres, int thread,
				     int xstart, int ystart,
				     int xend, int yend,
				     int share = 1,
				     int rindex = 0,
				     int alignment = 0);

    ////////////////////////////////////////////////////////////////////////
    // Simple Scale related functions
    // Implemented in COP2_NodeScale.C

    // This method simply scales the input to the current resolution and
    // copies it into the given tilelist.
    void 		scaleInput(int input, COP2_Context &context,
				   const TIL_Plane *plane, int array_index,
				   float t, UT_FilterType xfilter,
				   UT_FilterType yfilter,
				   TIL_TileList *tilelist);
    
    // This utility method is used to scale the supplied image bounds by
    // the specified factors.
    static void		scaleImageArea(float sx, float sy,
				       int ix1, int iy1, int ix2, int iy2,
				       int &x1, int &y1, int &x2, int &y2);

    ////////////////////////////////////////////////////////////////////////
    // Transform Related Fucntions
    // Implemented in COP2_NodeTransform.C

    // given an area, this grabs the transformed image data from the input.
    TIL_Region *	transformRegion(int		 input,
					COP2_Context	&context,
					const TIL_Plane *plane,
					int		 aindex,
					float		 t,
					int		 xres,
					int		 yres,
					int		 xstart,
					int		 ystart,
					int		 xend,
					int		 yend,
					const COP2_TransformParms &parms,
					bool input_aspect_adjust = true,
					bool streak_off_frame = false);

    // transforms the input area/plane given by the region.
    // The region bounds should be in the input's canvas space, if they aren't,
    // set bounds_in_canvas_space to false;
    bool		transformRegion(int		 input,
					COP2_Context	&context,
					TIL_Region	*region,
					const COP2_TransformParms &parms,
					bool input_aspect_adjust = true,
					bool bounds_in_canvas_space = true,
					bool streak_off_frame = false);

    bool		isFullTransform(int input,
					COP2_Context &context,
					const COP2_TransformParms &cparms,
					bool input_aspect_adjust=true,
					COP2_Node **source_node=0,
					int	  *source_index=0,
					float *aspectx1=0,
					float *aspectx2=0,
					float *aspecty1=0,
					float *aspecty2=0,
					COP2_TransformParms *prepped_parms=0);

    TIL_Region	*	transformBlurRegion(int		 input,
					    COP2_Context &context,
					    const TIL_Plane *plane,
					    int		 aindex,
					    float	 t,
					    int		 xres,
					    int		 yres,
					    int		 xstart,
					    int		 ystart,
					    int		 xend,
					    int		 yend,
					    float	 amount,
					    float	 bias,
				UT_ValArray<const COP2_TransformParms*> &parms,
					    bool	 deformation = false);
    
    bool		transformBlurRegion(int		  input,
				COP2_Context   &context,
				TIL_Region     *region,
				float		amount,
				float		bias,
			        UT_ValArray<const COP2_TransformParms*>&parms,
				bool		deformation	       = false,
				bool		bounds_in_canvas_space = true);

    // Helper function for transformRegion.  It is called when all the
    // transform has to do is translate the region.
    bool		translateRegion(COP2_Context &context,
					COP2_Node &source_node,
					int source_input,
					TIL_Region *region, float tx, float ty,
					TIL_RegionExtend hold,
					bool bounds_in_canvas_space,
					bool streak_off_frame);

    // Helper function for transformRegion.  It is called on to do a full 
    // transform.
    bool		fullTransformRegion(COP2_Context &context,
					    COP2_Node &source_node,
					    int source_input,
					    TIL_Region *region,
					    COP2_TransformParms &parms,
					    float haspect, float input_haspect,
					    float vaspect, float input_vaspect,
					    bool bounds_in_canvas_space);

    UT_Matrix4		getShiftedInverseTransformMatrix(COP2_Context &context, 
					   COP2_Node &source_node,
					   int source_input,
					   const TIL_Plane *iplane,
					   int array_index,
					   const COP2_TransformParms &parms,
					   int &dx, int &dy);
    
    // Helper function for transformRegion and computeImageBounds.  It is called
    // to prepare the transform parms for extraction.  This method will do all
    // the collapsing of transforms as well.  "source_node" will contain the
    // node whose input at "source_index" should be read for the data.
    // "source_node" will be initialized, but "source_index" needs to be the
    // index of the input for which we wish to prepare the parms (it may be
    // modified if transforms have collapsed).
    void		prepareTransformParms(COP2_Context &context,
					      COP2_TransformParms &parms,
					      COP2_Node *& source_node,
					      int &source_index, 
					      bool input_aspect_adjust);

    // adjustPivotParms adjusts the pivot and translate parms in order to 
    // account for aspect ratios (it is a helper function to
    // prepareTransformParms).
    void		adjustPivotParms(int input_index,
					 COP2_TransformParms &parms,
					 bool input_aspect_adjust);

    // Helper function for transformBlurRegion and computeImageBounds.  It is
    // called to copy the set of transform parms and it prepares the new copies
    // for use.
    void		copyAndPrepareTransformParms(
				const UT_ValArray<const COP2_TransformParms *> 
								  &source_parms,
			        UT_ValArray<COP2_TransformParms *> &dest_parms);

    //
    ////////////////////////////////////////////////////////////////////////


    void		extractFromOutputRegion(TIL_TileList *tiles,
						TIL_Region *output);

    // like releaseTile for regions... call when you're finished with the
    // region.
    void		 releaseRegion(TIL_Region *, int output = 0);

    // gets tiles to process for this node. Normally, you just process the
    // nodes passed into cookMyTile(); however, if you need more to complete
    // the cook efficiently, request more using this function.
    // If block is 0, this function may return null. Only COP2_Node should
    // ever call this method with block set to 1.
    TIL_TileList	*requestWorkingTiles(COP2_Context &context,
					     const TIL_Plane *plane,
					     int array_index,
					     float t,
					     int xstart, int ystart,
					     int *cache_flag = 0,
					     int ignore_bypass = 0,
					     int block = 0);

    // once you're done processing the tiles requested with requestWorkingTiles
    // you must call this function. Do not call this function on the tilelist
    // passed in to cookMyTile(). 
    void		 releaseWorkingTiles(TIL_TileList *list);

    // Passes tiles from the input to the output.
    virtual void	 passThroughTiles(COP2_Context &context,
					  const TIL_Plane *plane,
					  int array_index,
					  float t,
					  int xstart, int ystart,
					  TIL_TileList *&tile,
					  int block = 1,
					  bool *mask = 0,
					  bool *blocked = 0);

    // return 1 if you want to pass a specific tile through; allows you to
    // scope regions of the data.
    virtual int		passThrough(COP2_Context &context,
				    const TIL_Plane *plane, int comp_index,
				    int array_index, float t,
				    int xstart, int ystart);

    TIL_TileList	*getBlackTile(const TIL_Plane *plane, int aindex,
				      float t, int xres, int yres, int thread,
				      int xstart, int ystart, bool *mask =0);

    // Called to cook a single tile. Called for each tile in the image.
    virtual OP_ERROR	 cookMyTile(COP2_Context	&context,
				    TIL_TileList	*tiles);


    OP_ERROR		 cookFullImage(COP2_Context	&context,
				       TIL_TileList	*tiles,
				       COP2_FullImageCB callback,
				       UT_Lock		&fullimagelock,
				       bool		 use_float);

    inline void		 lockParms(bool lock);
    
    // Returns a new context. Overide to return your context derived from
    // COP2_ContextData and initialize it. This should only ever be called from
    // getContextData().
    virtual COP2_ContextData	*newContextData(const TIL_Plane *plane,
						int		 array_index,
						float		 t,
						int		 xres,
						int		 yres,
						int		 thread, 
						int		 max_threads);

    // retrieves only the context, without creating a context data.
    COP2_Context	*getContext(const TIL_Plane *plane,
				    int		 array_index,
				    float	 t,
				    int		 xres,
				    int		 yres,
				    int		 thread);

    // Copies the bounds of the input specified into the context. The pointers
    // are optional; they are assigned to the new bounds if non-null.
    bool		 copyInputBounds(int input, COP2_Context &context,
					 int *x1 = 0, int *y1 = 0,
					 int *x2 = 0, int *y2 = 0);
    
    // Override if your operation expands the op bounds. This method should set
    // the bounds in the context using setBounds()
    virtual void	 computeImageBounds(COP2_Context &context);

    // Scoping
    virtual bool	 isFrameScoped(int /*image_index*/) { return true; }
    // Override to return a scope string if the COP has a scope field. If there
    // is a valid scope, return 1, otherwise 0. 
    virtual int		 scope(UT_String & /*scope*/) { return 0; } 
    virtual int		 frameScope(UT_String & /*scope*/) { return 0; } 

    virtual OP_ERROR	 save(std::ostream &os, const OP_SaveFlags &flags,
			      const char *pathPrefix,
			      const UT_String &name_override = UT_String());
    virtual OP_ERROR	 saveIntrinsic(std::ostream &os, const OP_SaveFlags &flags);
    virtual bool	 load(UT_IStream &is, const char *extension,
			      const char *path=0);

    // This function will convert a tile at index in the tilelist
    // to floating point or return the image data pointer if the tile
    // is already FP. This function returns true if it allocated memory that
    // you need to free (with delete [])
    bool		 getTileInFP(TIL_TileList *tilelist,  float *&dest,
				     int index, void *dtile =0);

    // This function will write a block of floating point data back to the
    // tile at index in the tilelist, converting it if necessary. It's
    // assumed that the FP source data is the proper size (tilelist->mySize).
    void		 writeFPtoTile(TIL_TileList *tilelist, float *&src,
				       int index);

    UT_Interrupt	*getInterrupt();

    void		 setCOPColoring(COP2_OpColorType type, bool enable);

    void		 getOvercookStats(int &redone, int &total);
    void		 clearOvercookStats();

    void		 ensureSequenceContainsColorAndAlpha();

    // This method is overridden by COP2_SubNet because its "input" is
    // special.
    virtual void	 cleanUpInputsRecursively();


    // This method returns the cook area info for a given context. Can
    // only be called during cookMyTile(). 
    const COP2_CookAreaInfo *getAreaInfo(COP2_Context &context) const;

    // This method makes the output area of the plane depend on all parts of
    // all input planes, regardless of the size of the output area.
    void		 makeOutputAreaDependOnAllInputAreas(
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);

    // This method makes the output area depended on all planes for a given
    // input.
    void		 makeOutputAreaDependOnInputAreas(int input,
			    COP2_CookAreaInfo &output_area,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList &needed_areas);
    
    // Selects a plane from the input to be dependent on. Returns the plane
    // added to the needed_areas list.
    COP2_CookAreaInfo   *makeOutputAreaDependOnInputPlane(int input,
			    const char *planename, int array_index, float t,
			    const COP2_CookAreaList &input_areas,
			    COP2_CookAreaList	    &needed_areas);

    // selects the corresponding plane from an input to be dependent on.
    // Returns the plane added to the needed_areas list.
    COP2_CookAreaInfo   *makeOutputAreaDependOnMyPlane(int input,
			    COP2_CookAreaInfo		&output_area,
			    const COP2_CookAreaList	&input_areas,
			    COP2_CookAreaList		&needed_areas);

    // This method will try to collapse the transforms in the input areas,
    // setting the real input node in the needed areas.  It will delete
    // the contents of uncollapsed_input_areas.
    void		 getInputAreasForCollapsedTransformInputs(
		    COP2_Context	&context,
		    const COP2_TransformParms &transform_parms,
		    COP2_CookAreaInfo	&output_area,
		    COP2_CookAreaList	&uncollapsed_input_areas,
		    COP2_CookAreaList	&needed_areas,
		    bool		input_aspect_adjust,
		    bool		clear_uncollapse_list);

    // Override this method if the derived COP does not support partially
    // scoped planes and uses the default scope mechanism inside 
    // COP2_PixelBase.
    virtual bool	 allowPartiallyScopedPlanes() const {return true;}


    void		 registerVolatileParm(const char *name,PRM_Callback c,
					      bool ignore_missing = false);

    // the following set methods will follow channel references, if any
    // are present on the parm.
    void		 deAnimateFollowChanRef(const char *parmname,
						int vecidx );

    void		 getDependencyInfo(fpreal t, UT_WorkBuffer &text);
    void		 getDependencyInfoTree(fpreal t, UT_InfoTree &tree);
    
    void		 swapParm(const char *name1,const char *name2,
				  int vsize);
    void		 swapStringParm(const char *name1,const char *name2,
					int vsize);
public:
    // these are the 'manager' methods for setting up, cooking tiles and
    // cleaning up. The functions 'setupCook' and 'cookMyTile' are the ones
    // that should be overridden for normal operations. These methods do things
    // like lock inputs, make sure the cook is setup, etc.
    virtual OP_ERROR	 setup(float t, int xres, int yres, int max_threads,
			       bool doinput = true);
    OP_ERROR		 cookTile(TIL_TileList *&, float t,
				  int xres, int yres, int thread_index,
				  int max_threads);

    // Clean up this node and its inputs.  Cleaning up involves cleaning up
    // cook contexts and updating the performance monitor.
    void		 recursivelyCleanUp();
    void		 cleanUpCookContexts();

    // Clean up all the regions.  This method is not called as part of the
    // normal cleanup.
    void		 cleanUpAllRegions();

    void		 bumpCookSerial() { myCookSerial++; }
    int			 getCookSerial() { return myCookSerial; }
protected:
    TIL_Sequence		mySequence;
    int				myCopOpenCount;
    unsigned char		myCopRecurseFlag :1,
				myFramesAllScopedFlag:1;
    int				myInterruptFlags;
    int				myInputBlockFlags;
    COP2_ThreadPref		myThreadPreference;
    
private:    
    UT_ValArray<COP2_Context *> myCookContexts;
    UT_ValArray<TIL_Region *>   myNonSharedRegions;
    
    int				myInitCook;
    int64			myTotalCookTime[TIL_MAX_THREADS];
    int64		        myInputTileCookTime[TIL_MAX_THREADS];
    int64		        myTotalTileCookTime[TIL_MAX_THREADS];
    unsigned char		myCookStateFlags[TIL_MAX_THREADS];
    UT_IntArray			myInputOpenKeys;
    int				myCookDepth;
    int				myCacheHits;
    int				myCacheMisses;
    OP_Context			mySequenceContext;
    UT_ValArray<copLockInfo *>	myLockedPlanes;
    int				myPreviewPlaneIndex;
    int				myPreviewPlaneArrayIndex;
    OP_ERROR			myCookError;
    OP_ERROR			myLastCopCookError;
    float			myLastErrorTime;
    bool			myProxyEnable;
    bool			myCopDirtyFlag;

    // The following serial counter is included in the image identifier
    // string for this node.  Forcing a recook will increment this counter,
    // thus preventing use of any cached tiles or images.
    int				myCookSerial;

    // locks
    static TIL_FastLock	        theParmEvalLock;
    static TIL_FastLock		theParmHashCodeLock;
    TIL_FastLock		mySingleThreadedLock;
    TIL_FastLock		mySetupFuncLock;
    TIL_FastLock		myCookVarLock;
    TIL_FastLock		myRegionLock;
    TIL_FastLock		myErrorLock;
    TIL_FastLock		myIntrLock;
    TIL_FastLock		myInputBlockLock;
    TIL_FastLock		myContextDataLock;

    // Viewer image source
    COP2_ImageSource		*myImageSource;
    
    // Local variables
    int				myVarResX[TIL_MAX_THREADS];
    int				myVarResY[TIL_MAX_THREADS];
    int				myVarImageIndex[TIL_MAX_THREADS];
    float			myVarImageTime[TIL_MAX_THREADS];
    int				myVarImageArray[TIL_MAX_THREADS];
    int				myVarPlaneIndex[TIL_MAX_THREADS];
    int				myVarPlaneChannels[TIL_MAX_THREADS];

    unsigned char		myVarResUsed :1,
				myVarImageArrayUsed :1,
				myVarImagePlaneUsed :1,
				myVarUseFullRes :1,
				myCookOccurred :1;
    // Overcooking detection
    int				myNumCookedTiles;
    int				myDupCookedTiles;

    UT_Array<COP2_CheckpointWrapper *> myCheckpointWrapper;

    COP2_VolatileParmManager	*myVolatileParms;

    // stuff for caching the image bounds.
    COP2_BoundsCache		 myBoundsCache;
    TIL_FastLock		 myBoundsLock;

    // This variable keeps track of the thread that opened the node.  
    // If the node is not open, then it is set to UT_INVALID_THREAD_ID.
    ut_thread_id_t		 myOpenThreadId;

    // This variable locks the sections in the open() and close()
    // functions.
    UT_Lock			 myOpenLock;

    static bool			 theShowPreviewPref;

private:
    // Sets node's opener thread id.  Pass in UT_INVALID_THREAD_ID
    // to signal that the node is closed.
    void			setOpenerThread(ut_thread_id_t thread_id)
				    { myOpenThreadId = thread_id; }

    // Returns true if the current thread is the same thread that
    // opened the node.  Returns false if the threads do not match or
    // if the node is closed.
    bool			isOpenerThread() const
				    { return myOpenThreadId 
					== UT_Thread::getMyThreadId(); }

    virtual float	 copTimeTransform(int input, COP2_Context &context,
					  float t);
    
    void		 setupVariables(const TIL_Plane *plane,
					int	        array,
					int		xres,
					int		yres,
					float		t,
					int		thread);

    // checks for parameter time dependency, and out-of-range keyframes.
    void		  checkTimeDepAndRange();
    
    // mostly used for debugging & correcting cache problems.
    void		 verifyCacheIntegrity() { }
    
    // Methods to fetch tiles from the cache or from an input.
    int 		 getWorkingTiles(COP2_Context &context,
					 const TIL_Plane *plane,
					 int array_index,
					 float t,
					 int xstart,int ystart,
					 TIL_TileList *tilelist,
					 int ignore_bypass = 0,
					 int block = 1,
					 bool *mask = 0,
					 double *passtime = 0);

    bool		 fillInteractiveTile(COP2_Context &context,
					     const TIL_Plane *plane,
					     int array_index, float t,
					     int xstart,int ystart,
					     TIL_Tile *tile, int cindex,
					     int findex);

    // used only by inputTile, to resolve tile size/offset problems introduced
    // by the workspace & scoping channels.
    TIL_TileList *	 correctInputTile(int input_index,
					  COP2_Context &context,
					  const TIL_Plane *plane,
					  int array_index,
					  int xstart, int ystart,
					  TIL_TileList *tilelist,
					  int block, bool *blocked, bool *mask);

    void		doneCookOnTiles(TIL_TileList *tiles, OP_ERROR errstate,
					int thread_index);

    // This function is used by processTile to send data to a tile devide.
    // This is nontrivial because the tile may extend outside the bounds
    // of the region of interest for the tile device.
    bool		writeSafeDeepTile(COP2_ThreadCookParms *p,
					  TIL_TileList *tilelist) const;
    
    // performance related functions
    void	         markTimerStart(int64 *now) const;
    void		 getElapsedTime(int64 *now) const;
    void		 appendCookTimesToPerformanceMonitor();
    void		 setCookDepth(int d);
    
    // Allocates or returns a pointer to a previously allocated region. 
    TIL_Region *	allocateRegion(const TIL_Plane *plane, int input_index,
				       int array_index, float t,
				       int xres, int yres,
				       int x1, int y1, int x2, int y2,
				       int bx1, int by1, int bx2, int by2,
				       TIL_RegionExtend hold,
				       int output, int share,
				       int scan_alignment);

    // Preps a region for use; only called by allocateRegion & inputRegion.
    void		 openRegion(TIL_Region *region, void *regionmem[4],
				    bool output);

    // called by the multiprocessor cooks as the 'thread method'.
    static void *	 threadCookTile(void *data);

    // These methods do the actual work of manipulating the rasters.
    
    // insert into full raster, possibly with a colour subrange.
    void		insertIntoRaster(TIL_Raster *r, TIL_Tile *t,int tindex,
					 float low=0.0F,float high=1.0F) const;

    // insert into subregion of a raster, possibly with a colour subrange.
    // (x1,y1) -> (x2,y2) defines the rectangular region to be updated.
    // (x,y)shift defines how the tile origins are translated to fit into the
    // region.
    void		insertIntoRaster(TIL_Raster *r, TIL_Tile *t,int tindex,
					 int x1, int y1, int x2, int y2,
					 int xshift=0, int yshift=0,
					 float low=0.0F,float high=1.0F,
					 int greyscale = 0,
					 int ignore_points =0,
					 float gamma = 1.0F) const;

    void		highlightTile(int thread, int onoff);
    void		resetHighlight();

    // These methods compute a number of bounds related variables.
    void		computeBoundsVariables(const OP_Context &context,
			    const TIL_Plane *rplane,
			    int array_index, TIL_Raster *raster,
			    int thread_index,
			    int orig_xstart, int orig_ystart,
			    int orig_xend, int orig_yend,
			    int &xstart, int &ystart, int &xend, int &yend,
			    int &xtile1, int &ytile1,
			    int &xtile2, int &ytile2,
			    int &x1, int &y1, int &x2, int &y2,
			    int &xshift, int &yshift,
			    int &bounds_xstart, int &bounds_ystart,
			    int &bounds_xend, int &bounds_yend,
			    bool flipbook);

    void		 computeBoundsVariables(const TIL_Plane *rplane,
			    float time, int xres, int yres,
			    int array_index, TIL_Raster *raster,
		            int thread_index,
			    int orig_xstart, int orig_ystart,
			    int orig_xend, int orig_yend,
			    int &xstart, int &ystart, int &xend, int &yend,
			    int &xtile1, int &ytile1, int &xtile2, int &ytile2,
			    int &x1, int &y1, int &x2, int &y2,
			    int &xshift, int &yshift,
			    int &bounds_xstart, int &bounds_ystart,
			    int &bounds_xend, int &bounds_yend,
			    bool flipbook);

    // This convenience method returns the token string for the plane and
    // the tile indices for the area of interest.  The area of interest
    // (xstart,ystart)-(xend,yend) is in frame space.
    UT_TokenString	*getPlaneIdentifierAndTileBoundaries(
			    COP2_Context &context,
			    const TIL_Plane &refplane,
			    int xstart, int ystart, int xend, int yend,
			    int &xtile1, int &ytile1, int &xtile2, int &ytile2,
			    bool flipbook);

    TIL_Region *	getTransformRegionGPU(
				 COP2_Node &source_node, int source_input,
				 COP2_Context &context,
				 const TIL_Plane *plane, int array_index,
				 int x1, int y1, int x2, int y2,
				 int input_origx, int input_origy,
				 int input_xres, int input_yres,
				 int filt_xoffset, int filt_yoffset,
				 const COP2_TransformParms &parms,
				 bool bounds_in_canvas_space,
				 int &tx1, int &ty1, int &tx2, int &ty2,
				 int preoffset[2], int postoffset[2]);
	
    bool	         fullTransformRegionCPU(COP2_Context &context,
				  COP2_Node &source_node,
				  int source_input,
				  TIL_Region *region,
				  const COP2_TransformParms &parms,
				  int x1, int y1, int x2, int y2,
				  int input_x1, int input_y1,
				  int input_x2, int input_y2,
				  int input_xres, int input_yres,
				  int full_input_xres, int full_input_yres,
				  int filt_xoffset, int filt_yoffset,
				  float haspect, float input_haspect,
				  float vaspect, float input_vaspect,
				  bool bounds_in_canvas_space,
				  const UT_Matrix4 &xform);
    
    bool	         fullTransformRegionGPU(COP2_Context &context,
				  COP2_Node &source_node,
				  int source_input,
				  TIL_Region *region,
				  const COP2_TransformParms &parms,
				  int x1, int y1, int x2, int y2,
				  int input_x1, int input_y1,
				  int node_x1, int node_y1,
				  int node_x2, int node_y2,
				  int input_xres, int input_yres,
				  int full_input_xres, int full_input_yres,
				  int filt_xoffset, int filt_yoffset,
				  float haspect, float input_haspect,
				  float vaspect, float input_vaspect,
				  bool bounds_in_canvas_space,
				  const UT_Matrix4 &xform);

    bool		transformBlurRegionCPU(int  input,
				  COP2_Context	 &context,
				  TIL_Region	 *region,
				  UT_ValArray<COP2_TransformParms*> &parms,
				  bool	deformation,
				  float prev, float next, int numtimes,
				  float itime[3], float ifact[3],
				  bool	bounds_in_canvas_space,
				  int	filt_xoffset, int filt_yoffset,
				  int   input_origx, int input_origy,
				  int	ixres, int iyres,
				  int   x1, int y1, int x2, int y2,
				  float startt, float endt, float incr,
			          float bfact);
    
    bool		transformBlurRegionGPU(int  input,
				  COP2_Context	 &context,
				  TIL_Region	 *region,
				  UT_ValArray<COP2_TransformParms*> &parms,
				  int	deformation,
				  float prev, float next, int numtimes,
				  float itime[3], float ifact[3],
				  bool	bounds_in_canvas_space,
				  int	filt_xoffset, int filt_yoffset,
				  int   input_origx, int input_origy,
				  int	ixres, int iyres,
				  int   x1, int y1, int x2, int y2,
				  float startt, float endt, float incr,
			          float bfact);
    
    //  These virtuals are methods that nobody should override...
    //
    //  I/O methods
    //
    virtual bool	 loadPacket(UT_IStream &is, short class_id, short sig, const char *path=0);
    virtual bool	 loadPacket(UT_IStream &is, const char *token, const char *path=0);

    virtual void	 deleteCookedData() {}
    virtual int		 saveCookedData(const char *, OP_Context &);
    virtual int		 saveCookedData(std::ostream &, OP_Context &, int  = 0)
			 { return 0; }
    virtual const char	*getFileExtension(int binary) const
			 { return binary ? "bcop2" : "cop2"; }

    static void		 installExpressions();

    friend class COP2_CookScheduler;
    friend class COP2_VexVariableMap;
    friend class COP2_PixelOp;
    friend class COP2_VexOp;
    friend class COP2_MaskOp;
};

inline TIL_TileList *
COP2_Node::inputTile(int input_index, COP2_Context &context,
		     int xstart, int ystart, OP_ERROR *err, int block,
		     bool *mask, bool *blocked, bool *was_corrected)
{
    return inputTile(input_index, context,
		     context.myPlane, context.myArrayIndex,
		     context.myTime, xstart, ystart, err, block, mask, 0,
		     true, blocked, was_corrected);
}


inline TIL_TileList *
COP2_Node::inputTile(int input_index, COP2_Context &context,
		     TIL_TileList *tiles, OP_ERROR *err, int block,
		     bool *mask, bool *blocked, bool *was_corrected)
{
    return inputTile(input_index, context,
		     context.myPlane, context.myArrayIndex,
		     context.myTime,
		     tiles->myX1,
		     tiles->myY1,
		     err, block, mask, 0, true, blocked, was_corrected);
}

inline TIL_TileList *
COP2_Node::inputTile(int index, COP2_Context &context,
		     TIL_TileList *tilelist, float t,
		     OP_ERROR *err,int block, bool *mask, bool *blocked,
		     bool *was_corrected)
{
    return inputTile(index, context, tilelist->myPlane, tilelist->myArrayIndex,
		     t, tilelist->myX1, tilelist->myY1, err, block, mask,
		     0, true, blocked, was_corrected);
}

inline TIL_Region *
COP2_Node::inputRegion(int input_index,
		       COP2_Context &context,
		       TIL_TileList *tilelist,
		       int xshift,int yshift, int copy, TIL_RegionExtend hold)
{
    return inputRegion(input_index, context,
		       context.myPlane, context.myArrayIndex,
		       context.myTime, tilelist, xshift, yshift, copy, hold);
}

inline TIL_Region *
COP2_Node::inputRegion(int input_index,  COP2_Context &context,
		       int xstart, int ystart, int xend,   int yend,
		       TIL_RegionExtend hold, int share,
		       void *regionmem[PLANE_MAX_VECTOR_SIZE],
		       bool correct_aspect, bool correct_bounds,
		       int scan_alignment)
{
    return inputRegion(input_index, context,
		       context.myPlane, context.myArrayIndex,
		       context.myTime, xstart, ystart, xend, yend,
		       hold, share, regionmem, correct_aspect,
		       correct_bounds, scan_alignment);
}

inline TIL_Region *
COP2_Node::outputRegion(const TIL_Plane *plane,	int array_index, float seqt,
			int xres, int yres, int thread,
			int xstart, int ystart, int xend, int yend, int share,
			int rindex, int alignment)
{
    return outputRegion(plane, plane, array_index, seqt, xres, yres, thread,
			xstart, ystart, xend, yend, share, rindex,alignment);
}

inline void
COP2_Node::lockParms(bool lock)
{
    if(lock)
	TIL_LOCK(theParmEvalLock);
    else
	TIL_UNLOCK(theParmEvalLock);
}

inline bool
COP2_Node::getInputBounds(int input, COP2_Context &context,
			  int &x1, int &y1, int &x2, int &y2)
{
    return getInputBounds(input, context.myPlane, context.myArrayIndex,
			  context.myTime, context.myXres, context.myYres,
			  context.myThreadIndex, x1,y1, x2,y2);
}

inline void
COP2_Node::getImageBounds(COP2_Context &context,
			  int &x1,int &y1, int &x2,int &y2)
{
    getImageBounds(context.myPlane, context.myArrayIndex, context.myTime,
		   context.myXres, context.myYres, context.myThreadIndex,
		   x1,y1, x2,y2);
}

// Some common defines.
extern "C" {
    SYS_VISIBILITY_EXPORT extern void	newCop2Operator(OP_OperatorTable *table);
};
	
#endif

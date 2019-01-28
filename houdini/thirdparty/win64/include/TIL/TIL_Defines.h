/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Defines.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	Some common stuff for tiles.
 */
#ifndef TIL_DEFINES_H
#define TIL_DEFINES_H

#include "TIL_API.h"
#include <limits.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include <UT/UT_SysClone.h>
#include <IMG/IMG_FileTypes.h>
#include <UT/UT_Lock.h>
#include <PXL/PXL_Common.h>

#define TIL_Lookup PXL_Lookup
#define TIL_Fill PXL_Fill
#define TIL_FillParms PXL_FillParms
#define TIL_Pixel PXL_Pixel
#define TIL_Convert PXL_Convert

class UT_String;


// Use a maximum of 32 threads because:
//  1. COPs still uses locks, and this cap helps prevent slowdowns due to lock
//     contention
//  2. A 32b bitmask is used to store per-thread information (though this can
//     easily be changed if the above condition is fixed).
#define TIL_MAX_THREADS	32

#define TIL_DEFAULT_TILE_SIZE	 320

#define TIL_DEFAULT_INT8_SIZE	  (TIL_DEFAULT_TILE_SIZE*TIL_DEFAULT_TILE_SIZE)
#define TIL_DEFAULT_INT16_SIZE	  (TIL_DEFAULT_INT8_SIZE * 2)
#define TIL_DEFAULT_FLOAT32_SIZE  (TIL_DEFAULT_INT8_SIZE * 4)


#define TILE_LOCK_WRITE		0
#define TILE_LOCK_READ_ONLY	1

// For Linux, PThread locks are faster than spin locks.
#ifdef WIN32
#include <UT/UT_SpinLock.h>
typedef  UT_RecursiveSpinLock TIL_FastLock;
#else
typedef  UT_Lock TIL_FastLock;
#endif

// how the data is stored.
//
// INT8		   8 bit unsigned,	0.0 = 0, 1.0 = 255.
// INT16	  16 bit unsigned,	0.0 = 0, 1.0 = variable (likely 65535)
// INT32	  32 bit unsigned,	0.0 = variable, 1.0 = variable.
// FLOAT32	  32 bit floating point 0.0 = 0.0, 1.0 = 1.0.

#define TIL_DataFormat  PXL_DataFormat
#define TILE_INT8	PXL_INT8
#define TILE_INT16	PXL_INT16
#define TILE_INT32	PXL_INT32
#define TILE_FLOAT32	PXL_FLOAT32
#define TILE_FLOAT16	PXL_FLOAT16
#define TILE_MAX_DATA_FORMAT PXL_MAX_DATA_FORMAT

#define TIL_Packing PXL_Packing

enum TIL_Interlace
{
    INTERLACE_OFF	= 0,
    INTERLACE_HALF	= 1,
    INTERLACE_BLACK	= 2,
    INTERLACE_DOUBLE	= 3
};

enum TIL_Dominance
{
    ODD_DOMINANT  = 0,
    EVEN_DOMINANT = 1,
    ODD_ONLY	  = 2,
    EVEN_ONLY	  = 3
};
	

// spatial & temporal extend conditions
enum TIL_Extend
{
    EXTEND_BLACK = 0,
    EXTEND_CYCLE,
    EXTEND_MIRROR,
    EXTEND_HOLD_FOREVER,
    EXTEND_HOLD
};

enum TIL_Storage
{
    STORE_CACHE = 0,
    STORE_NO_CACHE,
    STORE_LOCKED,
    STORE_PROXY,
    STORE_NEVER
};

enum TIL_HistogramType
{
    TIL_RAW_HISTOGRAM = 0,
    TIL_HUE_HISTOGRAM,
    TIL_SAT_HISTOGRAM,
    TIL_LUM_HISTOGRAM,
    TIL_RAW_VS_U,
    TIL_RAW_VS_V,
    TIL_HUE_VS_U,
    TIL_HUE_VS_V,
    TIL_SAT_VS_U,
    TIL_SAT_VS_V,
    TIL_LUM_VS_U,
    TIL_LUM_VS_V,
    TIL_HUE_VS_SAT,
    TIL_HUE_VS_LUM
};

enum TIL_ViewerType
{
    TIL_VIEW_2D = 0, 
    TIL_VIEW_TIME,
    TIL_VIEW_GRAPH_RAWHIST,
    TIL_VIEW_GRAPH_HUEHIST,
    TIL_VIEW_GRAPH_SATHIST,
    TIL_VIEW_GRAPH_LUMHIST,
    TIL_VIEW_GRAPH_CURVES,
    TIL_VIEW_GRAPH_RAW_VS_U,
    TIL_VIEW_GRAPH_RAW_VS_V,
    TIL_VIEW_GRAPH_HUE_VS_U,
    TIL_VIEW_GRAPH_HUE_VS_V,
    TIL_VIEW_GRAPH_SAT_VS_U,
    TIL_VIEW_GRAPH_SAT_VS_V,
    TIL_VIEW_GRAPH_LUM_VS_U,
    TIL_VIEW_GRAPH_LUM_VS_V,
    TIL_VIEW_GRAPH_HUE_VS_SAT,
    TIL_VIEW_GRAPH_HUE_VS_LUM
};

#define TILE_WHITE_8		UCHAR_MAX
#define TILE_WHITE_16		USHRT_MAX
#define TILE_WHITE_32		UINT_MAX

#define TILE_CACHE_MANAGER_MAX_LOCKS	4096

#define PLANE_MAX_ARRAY_SIZE	4096
#define PLANE_MAX_VECTOR_SIZE	4

#define PROXY_TABLE_SIZE	309
#define POOL_TABLE_SIZE		509


// Tile list manipulation

// list = (TIL_TileList *), tile = (TIL_Tile *)
#define FOR_EACH_TILE(list,tile, i)					\
	for(i=0, tile=list->myItems[0];  i<PLANE_MAX_VECTOR_SIZE;	\
	    i++, tile=list->myItems[SYSmin(i, PLANE_MAX_VECTOR_SIZE-1)])\
	    if(tile)

#define GET_TILE(list,tile,i)						\
    for(tile=0, i=0, tile=list->myItems[0]; i<PLANE_MAX_VECTOR_SIZE;	\
	i++, tile=list->myItems[SYSmin(i, PLANE_MAX_VECTOR_SIZE-1)])	\
	if(tile) break;

#define GET_UNCOOKED_TILE(list,tile,i)					\
	for(tile=0, i=0, tile=list->myItems[0];				\
	    i<PLANE_MAX_VECTOR_SIZE;					\
	    i++, tile=list->myItems[SYSmin(i, PLANE_MAX_VECTOR_SIZE-1)])\
	    if(tile && !tile->isCooked()) break;

#define FOR_EACH_UNCOOKED_TILE(list,tile,i)				\
	for(i=0, tile=list->myItems[0];	 i<PLANE_MAX_VECTOR_SIZE;	\
	    i++, tile=list->myItems[SYSmin(i, PLANE_MAX_VECTOR_SIZE-1)])\
	    if(tile && !tile->isCooked())

#define FOR_EACH_UNCOOKED_PAIR(outlist, inlist, out, in, i)		\
	for(i=0, out=outlist->myItems[0], in=inlist->myItems[0];	\
	    i<PLANE_MAX_VECTOR_SIZE;					\
	    i++, out=outlist->myItems[SYSmin(i,PLANE_MAX_VECTOR_SIZE-1)],\
	    in=inlist->myItems[SYSmin(i, PLANE_MAX_VECTOR_SIZE-1)])	\
	    if(out && in && !out->isCooked())

// Expands an input tile list into tiles.
#define TILE_INPUT1(list,a) {		\
    a = list->myItems[0]; }

#define TILE_INPUT2(list,a,b) {		\
    a = list->myItems[0];		\
    b = list->myItems[1]; }

#define TILE_INPUT3(list,a,b,c) {	\
    a = list->myItems[0];		\
    b = list->myItems[1];		\
    c = list->myItems[2]; }

#define TILE_INPUT4(list,a,b,c,d) {	\
    a = list->myItems[0];		\
    b = list->myItems[1];		\
    c = list->myItems[2];		\
    d = list->myItems[3]; }


// expands an output tile list into tiles, if they aren't cooked already.
#define TILE_SCALAR(list,a) {		\
    a = list->myItems[0];		\
    if(a && a->isCooked()) a = 0;	\
    }

#define TILE_VECTOR2(list,a,b)	{	\
    a = list->myItems[0];		\
    b = list->myItems[1];		\
    if(a && a->isCooked()) a = 0;	\
    if(b && b->isCooked()) b = 0;	\
    }

#define TILE_VECTOR3(list,a,b,c) {	\
    a = list->myItems[0];		\
    b = list->myItems[1];		\
    c = list->myItems[2];		\
    if(a && a->isCooked()) a = 0;	\
    if(b && b->isCooked()) b = 0;	\
    if(c && c->isCooked()) c = 0;	\
    }

#define TILE_VECTOR4(list,a,b,c,d){	\
    a = list->myItems[0];		\
    b = list->myItems[1];		\
    c = list->myItems[2];		\
    d = list->myItems[3];		\
    if(a && a->isCooked()) a = 0;	\
    if(b && b->isCooked()) b = 0;	\
    if(c && c->isCooked()) c = 0;	\
    if(d && d->isCooked()) d = 0;	\
    }

//#define TIL_DEBUG_LOCK_BLOCKING

#ifdef TIL_DEBUG_LOCK_BLOCKING
#define TIL_LOCK(loc)	TILlock(loc, __FILE__ ":", __LINE__)
#define TIL_UNLOCK(loc)	loc.unlock()
#define TILwait(n)	TILwaitF(n, __FILE__ ":", __LINE__)
#else
#define TIL_LOCK(loc)	loc.lock()
#define TIL_UNLOCK(loc)	loc.unlock()
#define TILwait(n)	TILwaitF(n)
#endif

inline int TILformatDepth(PXL_DataFormat d)
	{ return PXLformatDepth(d); }

inline int TILpackingComponents(PXL_Packing p)
	{ return PXLpackingComponents(p); }
    
inline int TILpackingDepth(PXL_Packing p)
	{ return PXLpackingDepth(p); }

inline unsigned int TILwhitePoint(PXL_DataFormat dt)
	{ return PXLwhitePoint(dt); }

// functions defined in TIL_Tile.C.
TIL_API TIL_DataFormat	TILformatConvert(IMG_DataType);
TIL_API IMG_DataType	TILformatConvert(TIL_DataFormat);
TIL_API void		TILparsePlaneName(const char *name, UT_String &pname,
					  int &arrayindex);
TIL_API void		TILgetBWPoints(TIL_DataFormat d, float b, float w,
				       unsigned &ib, unsigned &iw, bool &fast);
TIL_API void		TILgetFloatBWPoints(TIL_DataFormat d, 
					    unsigned ib, unsigned iw,
					    float &b, float &w);
TIL_API void		TILgetBestFormat(TIL_DataFormat d1, float b1, float w1,
					 TIL_DataFormat d2, float b2, float w2,
					 TIL_DataFormat &best,
					 float &black, float &white);

// Lock & wait functions (with debug support)
TIL_API void		TILlock(UT_Lock &lock,const char *file=0, int line=0);
TIL_API void		TILprintLockBlockStats();
TIL_API void		TILclearTotalWait();
TIL_API void		TILwaitF(int n, const char *file=0, int line=0);
TIL_API double		TILgetTotalWait();

// common plane name and component referencing (return 'true' if the name
// had to be allocated with strdup, false if referenced).
TIL_API bool		TILcopyOrRefPlaneName(const char *&dname,
					      const char *name);
TIL_API bool		TILcopyOrRefCompName(const char *&dname,
					     const char *name);
TIL_API void		TILinitNameRefs();


// returns true if the image data is constant-valued.
TIL_API bool		TILcheckConstant(void *idata, TIL_DataFormat format,
					 int xres,int yres);

TIL_API bool		TILemulationNeeded(int comp, float b, float w,
					   float sc, float sh, float gamma,
					   const char *lut, float aspect,
					   TIL_DataFormat type,
					   bool use8bit, bool hard_accel,
					   bool hardlut, bool frag_shader);
					   
// Properly aligned memory blocks, since memalign seems to have problems.
class TIL_API til_AlignedBlock
{
public:
    // The specified alignment must be a power of 2, ie:
    // 1, 2, 4, 8, 16, etc.
    // Non powers of two will not only not work, but may crash, etc
    static til_AlignedBlock *allocBlock(int size, int alignment);
    
     // set-it-up yourself version. Ensure that your block is aligned the way
     // you like! Note that this memory is not owned, and will not be freed
     // when this instance is destroyed.
    static til_AlignedBlock *allocBlock(void *aligned);

    static void		     freeBlock(til_AlignedBlock *&block);

    operator void *() { return myAligned; }
    operator const void *() { return myAligned; }

private:
     til_AlignedBlock();
    ~til_AlignedBlock();

    void	reset();
    
    // myReal is the actual allocated data.
    // myAligned  is guaranteed to be aligned to the given
    // alignment amount.
    // myNext is used by the allocators when this block is not in
    // circulation.
    void *myReal;
    union { 
	void *myAligned;
	til_AlignedBlock *myNext;
    };
};


// This define causes ever tile alloced to be striped with alternating
// scanlines of white and grey, so that uncooked portions can be easily
// identified. This causes a fairly big performance hit.
// #define TIL_STRIPE_UNCOOKED_TILES

// This define causes regions to be cleared to 33% grey (integer only
// - floating point looks white) so that unfilled regions can be quickly
// identified. This causes even more of a performance hit than the STRIPE
// define above.
// #define TIL_CLEAR_REGIONS_TO_GREY

#endif // TIL_DEFINES_H

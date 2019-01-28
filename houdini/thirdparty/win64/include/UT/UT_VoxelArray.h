/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_VoxelArray.h ( UT Library, C++)
 *
 * COMMENTS:
 *	This provides support for transparently tiled voxel arrays of data.
 *	The given type, T, should support normal arithmatic operations.
 *
 *	The created array has elements indexed from 0, ie: [0..xdiv-1].
 */

#ifndef __UT_VoxelArray__
#define __UT_VoxelArray__

#include "UT_API.h"
#include "UT_BoundingBox.h"
#include "UT_Vector2.h"
#include "UT_Vector3.h"
#include "UT_Vector4.h"
#include "UT_IntArray.h"
#include "UT_ValArray.h"
#include "UT_Array.h"
#include "UT_FilterType.h"
#include "UT_COW.h"
#include "UT_ThreadedAlgorithm.h"
#include "UT_Interrupt.h"
#include <VM/VM_SIMD.h>

#include <SYS/SYS_SharedMemory.h>
#include <SYS/SYS_StaticAssert.h>
#include <SYS/SYS_Types.h>

// TBB alloc results in real-world tests that are 3-4% faster.  Yay!
// But unfortunately it is less aggressive with fragmentation, so
// we use effectively 2x the memory.  Boo.

//#define VOXEL_USE_TBB_ALLOC

#ifdef VOXEL_USE_TBB_ALLOC

#include <tbb/scalable_allocator.h>

#define UT_VOXEL_ALLOC(x) scalable_malloc(x)
#define UT_VOXEL_FREE(x) scalable_free(x)

#else

#define UT_VOXEL_ALLOC(x) SYSamalloc((x), 128)
#define UT_VOXEL_FREE(x) SYSafree(x)

#endif

class UT_Filter;
class UT_JSONWriter;
class UT_JSONParser;

static const int	TILEBITS = 4;
static const int	TILESIZE = 1 << TILEBITS;
static const int	TILEMASK = TILESIZE-1;

///
/// Behaviour of out of bound reads.
///
enum UT_VoxelBorderType
{
    UT_VOXELBORDER_CONSTANT,
    UT_VOXELBORDER_REPEAT,
    UT_VOXELBORDER_STREAK,
    UT_VOXELBORDER_EXTRAP,
};

template <typename T> class UT_VoxelTile;
template <typename T> class UT_VoxelArray;
template <typename T, bool DoRead, bool DoWrite, bool TestForWrite> class UT_VoxelProbe;
template <typename T> class UT_VoxelProbeCube;
template <typename T> class UT_VoxelProbeFace;

class UT_VoxelCompressOptions
{
public:
    UT_VoxelCompressOptions()
    {
	myConstantTol = 0;
	myQuantizeTol = 0;
	myAllowFP16 = false;
    }

    // Used for quantization.
    enum DitherType
    {
	DITHER_NONE,
	DITHER_ORDERED,
    };

    /// Tiles will be constant if within this range.  This may
    /// need to be tighter than quantization tolerance as
    /// dithering can't recover partial values.
    fpreal	myConstantTol;
    /// Tolerance for quantizing to reduced bit depth
    fpreal	myQuantizeTol;

    DitherType	myDitherType;

    /// Conversion to fpreal16, only valid for scalar data.
    bool	myAllowFP16;
};

///
/// UT_VoxelTileCompress
///
/// A compression engine for UT_VoxelTiles of a specific type.  This
/// is a verb class which is invoked from the voxeltile class.
///
template <typename T>
class UT_VoxelTileCompress
{
public:
		 UT_VoxelTileCompress() {}
    virtual	~UT_VoxelTileCompress() {}
    
    /// Attempts to write data directly to the compressed tile.
    /// Returns false if not possible.
    virtual bool writeThrough(UT_VoxelTile<T> &tile, 
			      int x, int y, int z, T t) const = 0;

    /// Reads directly from the compressed data.
    /// Cannot alter the tile in any way because it must be threadsafe.
    virtual T	 getValue(const UT_VoxelTile<T> &tile,
			  int x, int y, int z) const = 0;

    /// Attempts to compress the data according to the given tolerance.
    /// If succesful, returns true.
    virtual bool tryCompress(UT_VoxelTile<T> &tile,
			  const UT_VoxelCompressOptions &options, 
			  T min, T max) const = 0;

    /// Returns the length in bytes of the data in the tile.
    /// It must be at least one byte long.
    virtual int  getDataLength(const UT_VoxelTile<T> &tile) const = 0;

    /// Returns true if the compression type is lossless
    virtual bool isLossless() const { return false; }

    /// Determines the min & max values of the tile.  A default
    /// implementation uses getValue() on all voxels.
    virtual void findMinMax(const UT_VoxelTile<T> &tile, T &min, T &max) const;

    /// Does this engine support saving and loading?
    virtual bool canSave() const { return false; }
    virtual void save(std::ostream &os, const UT_VoxelTile<T> &tile) const {}
    virtual bool save(UT_JSONWriter &w, const UT_VoxelTile<T> &tile) const
						{ return false; }
    virtual void load(UT_IStream &is, UT_VoxelTile<T> &tile) const {}
    virtual bool load(UT_JSONParser &p, UT_VoxelTile<T> &tile) const
						{ return false; }

    /// Returns the unique name of this compression engine so
    /// we can look up engines by name (the index of the compression
    /// engine is assigned at load time so isn't constant)
    virtual const char *getName() = 0;
};

UT_API UT_ValArray<UT_VoxelTileCompress<fpreal16> *> &UTvoxelTileGetCompressionEngines(fpreal16 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<fpreal32> *> &UTvoxelTileGetCompressionEngines(fpreal32 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<fpreal64> *> &UTvoxelTileGetCompressionEngines(fpreal64 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<uint8> *> &UTvoxelTileGetCompressionEngines(uint8 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<int8> *> &UTvoxelTileGetCompressionEngines(int8 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<int16> *> &UTvoxelTileGetCompressionEngines(int16 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<int32> *> &UTvoxelTileGetCompressionEngines(int32 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<int64> *> &UTvoxelTileGetCompressionEngines(int64 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<UT_Vector2> *> &UTvoxelTileGetCompressionEngines(UT_Vector2 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<UT_Vector3> *> &UTvoxelTileGetCompressionEngines(UT_Vector3 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<UT_Vector4> *> &UTvoxelTileGetCompressionEngines(UT_Vector4 *dummy);

#define DEFINE_STD_FUNC(TYPE) 					\
inline void 							\
UTvoxelTileExpandMinMax(TYPE v, TYPE &min, TYPE &max)		\
{								\
    if (v < min)						\
	min = v;						\
    else if (v > max)						\
	max = v;						\
}								\
								\
inline fpreal							\
UTvoxelTileDist(TYPE a, TYPE b)					\
{								\
    return (fpreal) SYSabs(a - b);				\
}

DEFINE_STD_FUNC(fpreal16)
DEFINE_STD_FUNC(fpreal32)
DEFINE_STD_FUNC(fpreal64)
DEFINE_STD_FUNC(uint8)
DEFINE_STD_FUNC(int8)
DEFINE_STD_FUNC(int16)
DEFINE_STD_FUNC(int32)
DEFINE_STD_FUNC(int64)

#undef DEFINE_STD_FUNC

inline void 
UTvoxelTileExpandMinMax(UT_Vector2 v, UT_Vector2 &min, UT_Vector2 &max)
{
    min.x() = SYSmin(v.x(), min.x());
    max.x() = SYSmax(v.x(), max.x());

    min.y() = SYSmin(v.y(), min.y());
    max.y() = SYSmax(v.y(), max.y());
}

inline void 
UTvoxelTileExpandMinMax(UT_Vector3 v, UT_Vector3 &min, UT_Vector3 &max)
{
    min.x() = SYSmin(v.x(), min.x());
    max.x() = SYSmax(v.x(), max.x());

    min.y() = SYSmin(v.y(), min.y());
    max.y() = SYSmax(v.y(), max.y());

    min.z() = SYSmin(v.z(), min.z());
    max.z() = SYSmax(v.z(), max.z());
}

inline void 
UTvoxelTileExpandMinMax(UT_Vector4 v, UT_Vector4 &min, UT_Vector4 &max)
{
    min.x() = SYSmin(v.x(), min.x());
    max.x() = SYSmax(v.x(), max.x());

    min.y() = SYSmin(v.y(), min.y());
    max.y() = SYSmax(v.y(), max.y());

    min.z() = SYSmin(v.z(), min.z());
    max.z() = SYSmax(v.z(), max.z());

    min.w() = SYSmin(v.w(), min.w());
    max.w() = SYSmax(v.w(), max.w());
}

inline fpreal
UTvoxelTileDist(const UT_Vector2 &a, const UT_Vector2 &b)
{
    return SYSabs(a.x() - b.x()) + SYSabs(a.y() - b.y());
}

inline fpreal
UTvoxelTileDist(const UT_Vector3 &a, const UT_Vector3 &b)
{
    return SYSabs(a.x() - b.x()) + SYSabs(a.y() - b.y())
	 + SYSabs(a.z() - b.z());
}

inline fpreal
UTvoxelTileDist(const UT_Vector4 &a, const UT_Vector4 &b)
{
    return SYSabs(a.x() - b.x()) + SYSabs(a.y() - b.y())
	 + SYSabs(a.z() - b.z()) + SYSabs(a.w() - b.w());
}

///
/// UT_VoxelTile
///
/// A UT_VoxelArray is composed of a number of these tiles.  This is
/// done for two reasons:
/// 1) Increased memory locality when processing neighbouring points.
/// 2) Ability to compress or page out unneeded tiles.
/// Currently, the only special ability is the ability to create constant
/// tiles.
///
/// To the end user of the UT_VoxelArray, the UT_VoxelTile should be
/// usually transparent.  The only exception may be if they want to do
/// a FOR_ALL_TILES in order to ensure an optimal traversal order.
///
template <typename T>
class UT_VoxelTile
{
public:
	     UT_VoxelTile();
    virtual ~UT_VoxelTile();

    // Copy constructor:
	     UT_VoxelTile(const UT_VoxelTile<T> &src);


    // Assignment operator:
    const UT_VoxelTile<T> &operator=(const UT_VoxelTile<T> &src);

    enum CompressionType
    {
	COMPRESS_RAW,
	COMPRESS_RAWFULL,
	COMPRESS_CONSTANT,
	COMPRESS_FPREAL16,
	COMPRESS_ENGINE
    };

    /// Fetch a given local value.  (x,y,z) should be local to
    /// this tile.
    SYS_FORCE_INLINE T		 operator()(int x, int y, int z) const
		 {
		     UT_ASSERT_P(x >= 0 && y >= 0 && z >= 0);
		     UT_ASSERT_P(x < myRes[0] && y < myRes[1] && z < myRes[2]);

		     switch (myCompressionType)
		     {
			 case COMPRESS_RAW:
			     return ((T *)myData)[
				 ((z * myRes[1]) + y) * myRes[0] + x ];

			 case COMPRESS_CONSTANT:
			     return rawConstVal();

			 case COMPRESS_RAWFULL:
			     return ((T *)myData)[
				 ((z * TILESIZE) + y) * TILESIZE + x ];

			 case COMPRESS_FPREAL16:
			 {
			     T result;
			     result = (((fpreal16 *)myData)[
				 ((z * myRes[1]) + y) * myRes[0] + x ]);
			     return result;
			 }
		     }

		     // By default use the compression engine.
		     UT_VoxelTileCompress<T>	*engine;

		     engine = getCompressionEngine(myCompressionType);
		     return engine->getValue(*this, x, y, z);
		 }

    /// Lerps two numbers, templated to work with T.
    SYS_STATIC_FORCE_INLINE T	 lerpValues(T v1, T v2, fpreal32 bias)
    {
	return v1 + (v2 - v1) * bias;
    }

    /// Does a trilinear interpolation.  x,y,z should be local to this
    /// as should x+1, y+1, and z+1.  fx-fz should be 0..1.
    SYS_FORCE_INLINE T		 lerp(int x, int y, int z, float fx, float fy, float fz) const;

    template <int AXIS2D>
    SYS_FORCE_INLINE T		 lerpAxis(int x, int y, int z, float fx, float fy, float fz) const;

    /// Extracts a sample of [x,y,z] to [x+1,y+1,z+1].  The sample
    /// array should have 8 elements, x minor, z major.
    /// Requires it is in bounds.
    /// Returns true if all constant, in which case only a single
    /// sample is filled, [0]
    SYS_FORCE_INLINE bool	 extractSample(int x, int y, int z,
				    T *sample) const;
    template <int AXIS2D>
    SYS_FORCE_INLINE bool	 extractSampleAxis(int x, int y, int z,
				    T *sample) const;

    /// Extracts +/- dx, +/- dy, +/- dz and then the center into
    /// 7 samples.
    SYS_FORCE_INLINE bool	 extractSamplePlus(int x, int y, int z,
				    T *sample) const;
    /// Extracts the full cube of +/- dx, dy, dz.  xminor, zmajor, into
    /// 27 elements.
    bool	 		 extractSampleCube(int x, int y, int z,
				    T *sample) const;
#if 0
    /// MSVC can't handle aligned parameters after the third so
    /// frac must come first.
    T		 lerp(v4uf frac, int x, int y, int z) const;
#endif

    /// Returns a cached line to our internal data, at local address x,y,z.
    /// cacheline is a caller allocated structure to fill out if we have
    /// to decompress.  If forcecopy isn't set and we can, the result may
    /// be an internal pointer.  stride is set to the update for moving one
    /// x position in the cache.
    /// strideofone should be set to true if you want to prevent 0 stride
    /// results for constant tiles.
    T		*fillCacheLine(T *cacheline, int &stride, int x, int y, int z, bool forcecopy, bool strideofone) const;

    /// Fills a cache line from an external buffer into our own data.
    void	 writeCacheLine(T *cacheline, int y, int z);

    /// Copies between two tiles.  The tile's voxels match up, but don't
    /// have the same offset.  The maximal overlapping voxels are copied.
    /// this->setValue(dst, dsty, dstz, src(srcx, srcy, srcz));
    void	 copyFragment(int dstx, int dsty, int dstz,
			    const UT_VoxelTile<T> &srctile,
			    int srcx, int srcy, int srcz);

    /// Flattens ourself into the given destination buffer.
    template <typename S>
    void	 flatten(S *dst, int dststride) const;

    /// Fills our values from the given dense flat buffer.  Will
    /// create a constant tile if the source is constant.
    template <typename S>
    void	 writeData(const S *src, int srcstride);

    /// The setData is intentionally seperate so we can avoid
    /// expanding constant data when we write the same value to it.
    void	 setValue(int x, int y, int z, T t);

    /// Finds the minimum and maximum T values
    void	 findMinMax(T &min, T &max) const;

    /// Determines the average value of the tile.
    void 	 findAverage(T &avg) const;

    /// Returns if this tile is constant.
    bool	 isConstant() const 
		    { return myCompressionType == COMPRESS_CONSTANT; }

    /// Returns true if any NANs are in this tile
    bool	 hasNan() const;

    /// Returns if this tile is in raw format.
    bool	 isRaw() const
		    { return myCompressionType == COMPRESS_RAW; }

    /// Returns if this tile is in raw full format.
    bool	 isRawFull() const
		    { return myCompressionType == COMPRESS_RAWFULL; }

    /// Returns true if this is a simple form of compression, either
    /// constant, raw, or a raw full that isn't padded
    bool	 isSimpleCompression() const
    {
	if (isRaw()) return true;
	if (isConstant()) return true;
	if (isRawFull() && myRes[0] == TILESIZE && myRes[1] == TILESIZE)
	    return true;
	return false;
    }

    /// Attempts to compress this tile.  Returns true if any
    /// compression performed.
    bool	 tryCompress(const UT_VoxelCompressOptions &options);

    /// Turns this tile into a constant tile of the given value.
    void	 makeConstant(T t);

    /// Explicit compress to fpreal16.  Lossy.  No-op if already constant.
    void	 makeFpreal16();

    /// Turns a compressed tile into a raw tile.
    void	 uncompress();

    /// Turns a tile into a raw full tile.
    void	 uncompressFull();

    /// Like uncompress() except it leaves the data uninitialized. Result
    /// is either COMPRESS_RAW or COMPRESS_RAWFULL depending on the tile res.
    /// @note USE WITH CAUTION!
    void	 makeRawUninitialized();

    /// Returns the raw full data of the tile.
    T		*rawFullData()
		 {
		     uncompressFull();
		     return (T *)myData;
		 }

    /// This only makes sense for simple compression.  Use with
    /// extreme care.
    T		*rawData()
		 { if (inlineConstant() && isConstant())
		   { return (T *) &myData; }
		   return (T *)myData; }
    const T	*rawData() const
		 { if (inlineConstant() && isConstant())
		   { return (const T *) &myData; }
		   return (const T *)myData; }

    /// Read the current resolution.
    int		 xres() const { return myRes[0]; }
    int		 yres() const { return myRes[1]; }
    int		 zres() const { return myRes[2]; }

    int		 getRes(int dim) const { return myRes[dim]; }


    int		 numVoxels() const { return myRes[0] * myRes[1] * myRes[2]; }

    /// Returns the amount of memory used by this tile.
    int64	 getMemoryUsage(bool inclusive) const;

    /// Returns the amount of data used by the tile myData pointer.
    exint	 getDataLength() const;

    /// A routine used by filtered evaluation to accumulated a partial
    /// filtered sum in this tile.
    ///		pstart, pend - voxel bounds (in UT_VoxelArray coordinates)
    ///		weights - weight array
    ///		start - UT_VoxelArray coordinates at [0] in the weight array
    void	 weightedSum(int pstart[3], int pend[3],
			     const float *weights[3], int start[3],
			     T &result);

    /// Designed to be specialized according to T
    
    /// Update min & max to encompass T itself.
    static void	 expandMinMax(T v, T &min, T &max)
    {
	UTvoxelTileExpandMinMax(v, min, max);
    }
    
    /// Return the "distance" of a & b.  This is used for
    /// tolerance checks on equality comparisons.
    static fpreal	 dist(T a, T b)
    {
	return UTvoxelTileDist(a, b);
    }
    
    static void	 registerCompressionEngine(UT_VoxelTileCompress<T> *engine);

    // Returns the index of the bound compression engine.
    static int	 lookupCompressionEngine(const char *name);
    // Given an index, gets the compression engine.
    static UT_VoxelTileCompress<T> *getCompressionEngine(int index);

    /// Saves this tile's data, in compressed form.  
    /// May save in uncompressed form is the compression type does
    /// not support saving.
    void		save(std::ostream &os) const; 
    bool		save(UT_JSONWriter &w) const;

    /// Loads tile data.  Uses the compression index to map the saved
    /// compression types into the correct loading compression types.
    void		load(UT_IStream &is, const UT_IntArray &compression);
    bool		load(UT_JSONParser &p, const UT_IntArray &compression);

    /// Stores a list of compresson engines to os.
    static void		 saveCompressionTypes(std::ostream &os);
    static bool		 saveCompressionTypes(UT_JSONWriter &w);

    /// Builds a translation table from the given stream's compression types
    /// into our own valid compression types.
    static void		 loadCompressionTypes(UT_IStream &is, UT_IntArray &compressions);
    static bool		 loadCompressionTypes(UT_JSONParser &p, UT_IntArray &compressions);

protected:
    // Attempts to set the value to the native compressed format
    // Some compression types allow some values to be written
    // without decompression. Eg, you can write to a constant tile
    // the tile's own value without decompression.
    // If this returns true, t has been written.
    bool	 writeThrough(int x, int y, int z, T t);

    /// Sets the local res of the tile.  Does *not* resize the allocated
    /// memory.
    void		 setRes(int xr, int yr, int zr)
    { myRes[0] = xr; myRes[1] = yr; myRes[2] = zr; }

    SYS_FORCE_INLINE bool	inlineConstant() const
    {
	return (sizeof(T) <= sizeof(T*));
    }

    SYS_FORCE_INLINE T		rawConstVal() const
    { if (inlineConstant()) { return *((const T *)&myData); }
      return *((const T*)myData); }
    SYS_FORCE_INLINE T		*rawConstData() const
    { if (inlineConstant()) { return ((T *)&myData); }
      return ((T*)myData); }

    void setForeignData(void *data, int8 compress_type)
    {
	freeData();
	myCompressionType = compress_type;
	
	if (isConstant() && inlineConstant())
	{
	    makeConstant(*(T *)data);
	}
	else
	{
	    myData = data;
	    myForeignData = true;
	}
    }

public:
    /// Frees myData and sets it to zero.  This is a bit tricky
    /// as the constant tiles may be inlined.
    /// This is only public for the compression engines.
    SYS_FORCE_INLINE void	 freeData()
    {
	if (inlineConstant() && isConstant())
	{
	    // Do nothing!
	}
	else if (myData && !myForeignData)
	{
	    UT_VOXEL_FREE(myData);
	}
	myData = 0;
	myForeignData = false;
    }
    
public:
    // This is only public so the compression engines can get to it.
    // It is blind data, do not alter!
    void	*myData;
private:

    /// Resolutions.
    int8	 myRes[3];

    /// Am I a constant tile?
    int8	 myCompressionType;

    int8	 myForeignData;
    
    static UT_ValArray<UT_VoxelTileCompress<T> *> &getCompressionEngines()
    {
	return UTvoxelTileGetCompressionEngines((T *) 0);
    }

    friend class UT_VoxelTileCompress<T>;
    friend class UT_VoxelArray<T>;
    template <typename S, bool DoWrite, bool DoRead, bool TestForWrites>
    friend class UT_VoxelProbe;
};

///
/// UT_VoxelArray
///
/// This provides data structure to hold a three dimmensional array
/// of data.  The data should be some simple arithmetic type, such
/// as uint8, fpreal16, or UT_Vector3.
///
/// Some operations, such as gradiants, may make less sense with uint8.
/// 
template <typename T>
class UT_VoxelArray
{
public:
	     UT_VoxelArray();
    virtual ~UT_VoxelArray();
    
    /// Copy constructor:
	     UT_VoxelArray(const UT_VoxelArray<T> &src);

    /// Assignment operator:
    const UT_VoxelArray<T> &operator=(const UT_VoxelArray<T> &src);

    /// This sets the voxelarray to have the given resolution, resetting
    /// all elements to 0.
    void	 size(int xres, int yres, int zres);

    /// This will ensure this voxel array matches the given voxel array
    /// in terms of dimensions & border conditions.  It may invoke
    /// a size() and hence reset the field to 0.
    void	 match(const UT_VoxelArray<T> &src);

    template <typename S>
    bool	 isMatching(const UT_VoxelArray<S> &src) const
    {
	return src.getXRes() == getXRes() &&
	       src.getYRes() == getYRes() &&
	       src.getZRes() == getZRes();
    }

    int		 getXRes() const { return myRes[0]; }
    int		 getYRes() const { return myRes[1]; }
    int		 getZRes() const { return myRes[2]; }
    int		 getRes(int axis) const { return myRes[axis]; }

    /// Return the amount of memory used by this array.
    int64	 getMemoryUsage(bool inclusive) const;

    /// Sets this voxel array to the given constant value.  All tiles
    /// are turned into constant tiles.
    THREADED_METHOD1(UT_VoxelArray<T>, numTiles() > 100,
                     constant,
                     T, t)
    void constantPartial(T t, const UT_JobInfo &info);

    /// If this voxel array is all constant tiles, returns true.
    /// The optional pointer is initialized to the constant value iff
    /// the array is constant.  (Note by constant we mean made of constant
    /// tiles of the same value - if some tiles are uncompressed but
    /// constant, it will still return false)
    bool	 isConstant(T *cval = 0) const;

    /// Returns true if any element of the voxel array is NAN
    bool	 hasNan() const;

    /// This convience function lets you sample the voxel array.
    /// pos is in the range [0..1]^3.
    /// T value trilinearly interpolated.   Edges are determined by the border
    /// mode.
    /// The cells are sampled at the center of the voxels.
    T		 operator()(UT_Vector3D pos) const;
    T		 operator()(UT_Vector3F pos) const;

    /// This convience function lets you sample the voxel array.
    /// pos is in the range [0..1]^3.
    /// The min/max is the range of the sampled values.
    void	 evaluateMinMax(T &lerp, T &lmin, T &lmax,
				UT_Vector3F pos) const;

    /// Evaluate using voxel coords, from 0,0,0 to resx,resy,resz.
    /// Allows out of range evaluation
    SYS_FORCE_INLINE T	 lerpVoxelCoord(UT_Vector3F pos) const;
    /// Evaluate using voxel coords, from 0,0,0 to resx,resy,resz.
    /// Allows out of range evaluation
    SYS_FORCE_INLINE T	 lerpVoxel(int x, int y, int z,
			    float fx, float fy, float fz) const;
    template <int AXIS2D>
    SYS_FORCE_INLINE T	 lerpVoxelCoordAxis(UT_Vector3F pos) const;
    template <int AXIS2D>
    SYS_FORCE_INLINE T	 lerpVoxelAxis(int x, int y, int z,
			    float fx, float fy, float fz) const;

    /// Evaluate using voxel coords, from 0,0,0 to resx,resy,resz.
    /// Allows out of range evaluation.  Also computes min/max of
    /// interpolated samples.
    SYS_FORCE_INLINE void lerpVoxelCoordMinMax(T &lerp, T &lmin, T &lmax, 
						UT_Vector3F pos) const;
    template <int AXIS2D>
    SYS_FORCE_INLINE void lerpVoxelCoordMinMaxAxis(T &lerp, T &lmin, T &lmax, 
						UT_Vector3F pos) const;
    /// Evaluate using voxel coords, from 0,0,0 to resx,resy,resz.
    /// Allows out of range evaluation.  Also computes min/max of
    /// interpolated samples.
    SYS_FORCE_INLINE void lerpVoxelMinMax(
			    T &lerp, T &lmin, T &lmax,
			    int x, int y, int z,
			    float fx, float fy, float fz) const;
    template <int AXIS2D>
    SYS_FORCE_INLINE void lerpVoxelMinMaxAxis(
			    T &lerp, T &lmin, T &lmax,
			    int x, int y, int z,
			    float fx, float fy, float fz) const;

    /// Extracts a sample of [x,y,z] to [x+1,y+1,z+1].  The sample
    /// array should have 8 elements, x minor, z major.
    SYS_FORCE_INLINE bool	 extractSample(int x, int y, int z,
				T *sample) const;
    template <int AXIS2D>
    SYS_FORCE_INLINE bool	 extractSampleAxis(int x, int y, int z,
				T *sample) const;

    /// Extracts a sample in a plus shape, dx, then dy, then dz, finally
    /// the center into 7 voxels.
    SYS_FORCE_INLINE bool	 extractSamplePlus(int x, int y, int z,
				T *sample) const;
    /// Extracts 27 dense 3x3x3 cube centered at x,y,z into samples
    /// z major, xminor.
    SYS_FORCE_INLINE bool	 extractSampleCube(int x, int y, int z,
				T *sample) const;

    /// Lerps the given sample using trilinear interpolation
    SYS_FORCE_INLINE T	 lerpSample(T *samples,
			    float fx, float fy, float fz) const;
    template <int AXIS2D>
    SYS_FORCE_INLINE T	 lerpSampleAxis(T *samples,
			    float fx, float fy, float fz) const;

    SYS_FORCE_INLINE void	 splitVoxelCoord(UT_Vector3F pos, int &x, int &y, int &z,
				    float &fx, float &fy, float &fz) const
    {
	// Determine integer & fractional components.
	fx = pos.x();
	SYSfastSplitFloat(fx, x);
	fy = pos.y();
	SYSfastSplitFloat(fy, y);
	fz = pos.z();
	SYSfastSplitFloat(fz, z);
    }
    template <int AXIS2D>
    SYS_FORCE_INLINE void	 splitVoxelCoordAxis(UT_Vector3F pos, int &x, int &y, int &z,
				    float &fx, float &fy, float &fz) const
    {
	// Determine integer & fractional components.
	if (AXIS2D != 0)
	{
	    fx = pos.x();
	    SYSfastSplitFloat(fx, x);
	}
	else
	{
	    fx = 0.0;
	    x = 0;
	}
	if (AXIS2D != 1)
	{
	    fy = pos.y();
	    SYSfastSplitFloat(fy, y);
	}
	else
	{
	    fy = 0.0;
	    y = 0;
	}
	if (AXIS2D != 2)
	{
	    fz = pos.z();
	    SYSfastSplitFloat(fz, z);
	}
	else
	{
	    fz = 0.0;
	    z = 0;
	}
    }
#if 0
    T		 operator()(v4uf pos) const;
#endif

    /// Filtered evaluation of the voxel array.  This operation should
    /// exhibit the same behavior as IMG3D_Channel::evaluate.  
    T		 evaluate(const UT_Vector3 &pos, const UT_Filter &filter,
			  fpreal radius, int clampaxis = -1) const;

    /// Fills this by resampling the given voxel array.  
    void	 resample(const UT_VoxelArray<T> &src,
			  UT_FilterType filtertype = UT_FILTER_POINT,
			  float filterwidthscale = 1.0f, 
			  int clampaxis = -1);

    /// Flattens this into an array.  Z major, then Y, then X.
    /// flatarray[x + y * ystride + z * zstride] = getValue(x, y, z);
    THREADED_METHOD3_CONST(UT_VoxelArray<T>, numTiles() > 16,
		     flatten,
		     T *, flatarray,
		     exint, ystride,
		     exint, zstride)
    void	 flattenPartial(T *flatarray, exint ystride, exint zstride,
				const UT_JobInfo &info) const;
    
    /// Flattens this into an array suitable for a GL 8bit texture.
    /// Z major, then Y, then X.
    /// flatarray[x + y * ystride + z * zstride] = getValue(x, y, z);
    THREADED_METHOD4_CONST(UT_VoxelArray<T>, numTiles() > 16,
			   flattenGLFixed8,
			   uint8 *, flatarray,
			   exint, ystride,
			   exint, zstride,
			   T , dummy)
    void	 flattenGLFixed8Partial(uint8 *flatarray,
					exint ystride, exint zstride,
					T dummy,
					const UT_JobInfo &info) const;

    /// Flattens this into an array suitable for a GL 16bit FP texture.
    /// Z major, then Y, then X.
    /// flatarray[x + y * ystride + z * zstride] = getValue(x, y, z);
    THREADED_METHOD4_CONST(UT_VoxelArray<T>, numTiles() > 16,
			   flattenGL16F,
			   UT_Vector4H *, flatarray,
			   exint, ystride,
			   exint, zstride,
			   T , dummy)
    void	 flattenGL16FPartial(UT_Vector4H *flatarray,
				     exint ystride, exint zstride,
				     T dummy,
				     const UT_JobInfo &info) const;

    /// Flattens this into an array suitable for a GL 32b FP texture. Note that
    /// this also works around an older Nvidia driver bug that caused very small
    /// valued texels (<1e-9) to appear as huge random values in the texture.
    /// Z major, then Y, then X.
    /// flatarray[x + y * ystride + z * zstride] = getValue(x, y, z);
    THREADED_METHOD4_CONST(UT_VoxelArray<T>, numTiles() > 16,
			   flattenGL32F,
			   UT_Vector4F *, flatarray,
			   exint, ystride,
			   exint, zstride,
			   T , dummy)
    void	 flattenGL32FPartial(UT_Vector4F *flatarray,
				     exint ystride, exint zstride,
				     T dummy,
				     const UT_JobInfo &info) const;

    /// Fills this from a flattened array.  Z major, then Y, then X.
    /// setValue(x,y,z, flatarray[x + y * ystride + z * zstride];
    THREADED_METHOD3(UT_VoxelArray<T>, numTiles() > 16,
		     extractFromFlattened,
		     const T *, flatarray,
		     exint, ystride,
		     exint, zstride)
    void	 extractFromFlattenedPartial(const T *flatarray, 
				exint ystride, exint zstride,
				const UT_JobInfo &info);

    /// Copies into this voxel array from the source array.
    /// Conceptually,
    /// this->setValue(x, y, z, src.getValue(x+offx, y+offy, z+offz);
    void	 copyWithOffset(const UT_VoxelArray<T> &src,
			int offx, int offy, int offz);
    THREADED_METHOD4(UT_VoxelArray<T>, numTiles() > 4,
		    copyWithOffsetInternal,
		    const UT_VoxelArray<T> &, src,
		    int, offx,
		    int, offy,
		    int, offz)
    void	 copyWithOffsetInternalPartial(const UT_VoxelArray<T> &src,
			int offx, int offy, int offz,
			const UT_JobInfo &info);

    /// Fills dstdata with the voxel data of listed tiles.  Stride is measured
    /// in T.  Data order is in tile-order.  So, sorted by tilelist, then
    /// z, y, x within that tile.
    template <typename S>
    S 		*extractTiles(S *dstdata, int stride,
			      const UT_IntArray &tilelist) const;

    /// Overwrites our tiles with the given data.  Does checking
    /// for constant tiles.   Input srcdata stream should match
    /// that of extractTiles.
    template <typename S>
    const S	*writeTiles(const S *srcdata, int srcstride,
			    const UT_IntArray &tilelist);

    /// Converts a 3d position in range [0..1]^3 into the closest
    /// index value.
    /// Returns false if the resulting index was out of range.  The index
    /// will still be set.
    bool	 posToIndex(UT_Vector3 pos, int &x, int &y, int &z) const;
    /// Convertes a 3d position in [0..1]^3 into the equivalent in
    /// the integer cell space.  Does not clamp to the closest value.
    bool	 posToIndex(UT_Vector3 pos, UT_Vector3 &ipos) const;
    /// Converts an index into a position.
    /// Returns false if the source index was out of range, in which case
    /// pos will be outside [0..1]^3
    bool	 indexToPos(int x, int y, int z, UT_Vector3F &pos) const; 
    bool	 indexToPos(int x, int y, int z, UT_Vector3D &pos) const; 
    void	 findexToPos(UT_Vector3F ipos, UT_Vector3F &pos) const;
    void	 findexToPos(UT_Vector3D ipos, UT_Vector3D &pos) const;

    /// Clamps the given x, y, and z values to lie inside the valid index
    /// range.
    void	 clampIndex(int &x, int &y, int &z) const
		 {
		     x = SYSclamp(x, 0, myRes[0]-1);
		     y = SYSclamp(y, 0, myRes[1]-1);
		     z = SYSclamp(z, 0, myRes[2]-1);
		 }

    /// Returns true if the given x, y, z values lie inside the valid index.
    bool	 isValidIndex(int x, int y, int z) const
		 {
		     return !((x | y | z) < 0) && 
			    (((x - myRes[0]) & (y - myRes[1]) & (z - myRes[2])) < 0);
		 }
    
    /// This allows you to read & write the raw data.
    /// Out of bound reads are illegal.
    T		 operator()(int x, int y, int z) const
		 {
		     UT_ASSERT_P(isValidIndex(x, y, z));
		     return (*getTile(x >> TILEBITS,
				      y >> TILEBITS,
				      z >> TILEBITS))
			 (x & TILEMASK, y & TILEMASK, z & TILEMASK);
		 }
    void	 setValue(int x, int y, int z, T t)
		 {
		     UT_ASSERT_P(isValidIndex(x, y, z));
		     getTile(x >> TILEBITS,
			     y >> TILEBITS,
			     z >> TILEBITS)->setValue(
				 x & TILEMASK, y & TILEMASK, z & TILEMASK, t);
		 }

    /// This will clamp the bounds to fit within the voxel array,
    /// using the border type to resolve out of range values.
    T		 getValue(int x, int y, int z) const
		 {
		     // First handle the most common case.
		     if (isValidIndex(x, y, z))
			 return (*this)(x, y, z);

		     // Verify our voxel array is non-empty.
		     if (!myTiles)
			 return myBorderValue;

		     // We now know we are out of range, adjust appropriately
		     switch (myBorderType)
		     {
		     case UT_VOXELBORDER_CONSTANT:
			 return myBorderValue;

		     case UT_VOXELBORDER_REPEAT:
			 if (x < 0 || x >= myRes[0])
			 {
			     x %= myRes[0];
			     if (x < 0)
				 x += myRes[0];
			 }
			 if (y < 0 || y >= myRes[1])
			 {
			     y %= myRes[1];
			     if (y < 0)
				 y += myRes[1];
			 }
			 if (z < 0 || z >= myRes[2])
			 {
			     z %= myRes[2];
			     if (z < 0)
				 z += myRes[2];
			 }
			 break;

		     case UT_VOXELBORDER_STREAK:
			 clampIndex(x, y, z);
			 break;
		     case UT_VOXELBORDER_EXTRAP:
			 {
			     int		cx, cy, cz;
			     T		result;

			     cx = x; cy = y; cz = z;
			     clampIndex(cx, cy, cz);

			     result = (*this)(cx, cy, cz);
			     result += (x - cx) * myBorderScale[0] +
				       (y - cy) * myBorderScale[1] +
				       (z - cz) * myBorderScale[2];
			     return result;
			 }
		     }

		     // It is now within bounds, do normal fetch.
		     return (*this)(x, y, z);
		 }

    void	 setBorder(UT_VoxelBorderType type, T t);
    void	 setBorderScale(T scalex, T scaley, T scalez);
    UT_VoxelBorderType getBorder() const { return myBorderType; }
    T		 getBorderValue() const { return myBorderValue; }
    T		 getBorderScale(int axis) const { return myBorderScale[axis]; }

    /// This tries to compress or collapse each tile.  This can
    /// be expensive (ie, converting a tile to constant), so
    /// should be saved until modifications are complete.
    THREADED_METHOD(UT_VoxelArray<T>, numTiles() > 100,
                    collapseAllTiles)
    void	 collapseAllTilesPartial(const UT_JobInfo &info);

    /// Uncompresses all tiles into non-constant tiles.  Useful
    /// if you have a multithreaded algorithm that may need to
    /// both read and write, if you write to a collapsed tile
    /// while someone else reads from it, bad stuff happens.
    /// Instead, you can expandAllTiles.  This may have serious
    /// consequences in memory use, however.
    THREADED_METHOD(UT_VoxelArray<T>, numTiles() > 100,
                    expandAllTiles)
    void     expandAllTilesPartial(const UT_JobInfo &info);

    /// Uncompresses all tiles, but leaves constant tiles alone.
    /// Useful for cleaning out any non-standard compression algorithm
    /// that some external program can't handle.
    THREADED_METHOD(UT_VoxelArray<T>, numTiles() > 100,
                    expandAllNonConstTiles)
    void     expandAllNonConstTilesPartial(const UT_JobInfo &info);
    
    /// The direct tile access methods are to make TBF writing a bit
    /// more efficient.
    UT_VoxelTile<T>	*getTile(int tx, int ty, int tz) const
			 { return &myTiles[xyzTileToLinear(tx, ty, tz)]; }
    UT_VoxelTile<T>	*getLinearTile(int idx) const
			 { return &myTiles[idx]; }
    void		 linearTileToXYZ(int idx, int &x, int &y, int &z) const
			 {
			     x = idx % myTileRes[0];
			     idx -= x;
			     idx /= myTileRes[0];
			     y = idx % myTileRes[1];
			     idx -= y;
			     idx /= myTileRes[1];
			     z = idx;
			 }
    int		 	 xyzTileToLinear(int x, int y, int z) const
			 { return (z * myTileRes[1] + y) * myTileRes[0] + x; }

    int			 indexToLinearTile(int x, int y, int z) const
			 { return ((z >> TILEBITS) * myTileRes[1] + (y >> TILEBITS)) * myTileRes[0] + (x >> TILEBITS); }

    /// Number of tiles along that axis.  Not to be confused with
    /// the resolution of the individual tiles.
    int			 getTileRes(int dim) const { return myTileRes[dim]; }
    int		 	 numTiles() const 
		 { return myTileRes[0] * myTileRes[1] * myTileRes[2]; }
    exint	 	 numVoxels() const 
		 { return ((exint)myRes[0]) * myRes[1] * myRes[2]; }

    void		 setCompressionOptions(const UT_VoxelCompressOptions &options)
    { myCompressionOptions = options; }
    const UT_VoxelCompressOptions &getCompressionOptions() const
    { return myCompressionOptions; }

    void		 setCompressionTolerance(fpreal tol)
    { myCompressionOptions.myConstantTol = tol; }
    fpreal		 getCompressionTolerance() const
    { return myCompressionOptions.myConstantTol; }

    /// Saves only the data of this array to the given stream.
    /// To reload it you will have to have a matching array in tiles
    /// dimensions and size.
    void		 saveData(std::ostream &os) const;
    bool		 saveData(UT_JSONWriter &w,
				  const char *shared_mem_owner = 0) const;

    /// Load an array, requires you have already size()d this array.
    void		 loadData(UT_IStream &is);
    bool		 loadData(UT_JSONParser &p);

    /// Copy only the data from the source array.
    /// Note that it is an error to call this unless isMatching(src).
    THREADED_METHOD1(UT_VoxelArray<T>, numTiles() > 20,
		     copyData,
		     const UT_VoxelArray<T> &, src)

    void copyDataPartial(const UT_VoxelArray<T> &src,
			 const UT_JobInfo &info);

private:
    THREADED_METHOD4(UT_VoxelArray<T>, numTiles() > 1,
		     resamplethread,
		     const UT_VoxelArray<T> &, src,
		     const UT_Filter *, filter,
		     float, radius,
		     int, clampaxis)
    void	 resamplethreadPartial(const UT_VoxelArray<T> &src,
				       const UT_Filter *filter,
				       float radius,
				       int clampaxis,
				       const UT_JobInfo &info);


    void	 deleteVoxels();

    SYS_SharedMemory *copyToSharedMemory(const char *shared_mem_owner) const;
    bool	      populateFromSharedMemory(const char *id);
    
    
    /// Number of elements in each dimension.
    int		 	 myRes[3];

    /// Inverse tile res, 1/myRes
    UT_Vector3		 myInvRes;

    /// Number of tiles in each dimension.
    int			 myTileRes[3];

    /// Compression tolerance for lossy compression.
    UT_VoxelCompressOptions	myCompressionOptions;

    /// Double dereferenced so we can theoretically resize easily.
    UT_VoxelTile<T>	*myTiles;
    
    /// Outside values get this if constant borders are used
    T			 myBorderValue;
    /// Per axis scale factors for when extrapolating.
    T			 myBorderScale[3];
    UT_VoxelBorderType	 myBorderType;

    /// For initializing the tiles from shared memory.
    SYS_SharedMemory 	 *mySharedMem;
    SYS_SharedMemoryView *mySharedMemView;
};


///
/// UT_VoxelMipMap
///
/// This provides a mip-map type structure for a voxel array.
/// It manages the different levels of voxels arrays that are needed.
/// You can create different types of mip maps: average, maximum, etc,
/// which can allow different tricks.
/// Each level is one half the previous level, rounded up.
/// Out of bound voxels are ignored from the lower levels.
///
template <typename T>
class UT_VoxelMipMap
{
public:
    /// The different types of functions that can be used for
    /// constructing a mip map.
    enum mipmaptype { MIPMAP_MAXIMUM=0, MIPMAP_AVERAGE=1, MIPMAP_MINIMUM=2 };
    
	     UT_VoxelMipMap();
    virtual ~UT_VoxelMipMap();

    /// Copy constructor.
	     UT_VoxelMipMap(const UT_VoxelMipMap<T> &src);

    /// Assignment operator:
    const UT_VoxelMipMap<T> &operator=(const UT_VoxelMipMap<T> &src);

    /// Builds from a given voxel array.  The ownership flag determines
    /// if we gain ownership of the voxel array and should delete it.
    /// In any case, the new levels are owned by us.
    void		 build(UT_VoxelArray<T> *baselevel, 
			       mipmaptype function);

    /// Same as above but construct mipmaps simultaneously for more than
    /// one function.  The order of the functions will correspond to the
    /// order of the data values passed to the traversal callback.
    void		 build(UT_VoxelArray<T> *baselevel, 
			       const UT_Array<mipmaptype> &functions);

    /// This does a top down traversal of the implicit octree defined
    /// by the voxel array.  Returning false will abort that
    /// branch of the octree.
    /// The bounding box given is in cell space and is an exclusive
    /// box of the included cells (ie: (0..1)^3 means just cell 0,0,0)
    /// Note that each bounding box will not be square, unless you
    /// have the good fortune of starting with a power of 2 cube.
    /// The boolean goes true when the the callback is invoked on a
    /// base level.
    typedef bool (*Callback)(const T *funcs,
			     const UT_BoundingBox &box,
			     bool baselevel, void *data);
    void		 traverseTopDown(Callback function,
					 void *data) const;

    /// Top down traversal on op.   OP is invoked with
    /// bool op(const UT_BoundingBoxI &indexbox, int level)
    ///
    /// indexbox is half-inclusive (0..1)^3 means cell 0,0,0 
    /// level 0 means the base level.  
    /// (box.min.x()>>level, box.min.y()>>level, box.min.z()>>level)
    /// gives the index to extract the value from level..
    template <typename OP>
    void		 traverseTopDown(OP&op) const;


    /// Top down traversal, but which quad tree is visited first
    /// is controlled by 
    /// float op.sortValue(UT_BoundingBoxI &indexbox, int level);
    /// Lower values are visited first.
    template <typename OP>
    void		 traverseTopDownSorted(OP&op) const;


    /// Return the amount of memory used by this mipmap.
    int64		 getMemoryUsage(bool inclusive) const;

    int			 numLevels() const { return myNumLevels+1; }

    /// level 0 is the original grid, each level higher is a power
    /// of two smaller.
    const UT_VoxelArray<T> *level(int level, int function) const 
    { 
	if (level == 0)
	    return myBaseLevel;

	return myLevels(function)[numLevels() - 1 - level]; 
    }

private:
    void		 doTraverse(int x, int y, int z, int level,
				    Callback function,
				    void *data) const;

    /// Note: This variant of doTraverse has the opposite sense of level!
    template <typename OP>
    void		 doTraverse(int x, int y, int z, int level,
				    OP &op) const;
    template <typename OP>
    void		 doTraverseSorted(int x, int y, int z, int level,
				    OP &op) const;
    
    void		 initializePrivate();
    void		 destroyPrivate();

    THREADED_METHOD3(UT_VoxelMipMap<T>, dst.numTiles() > 1,
		     downsample,
		     UT_VoxelArray<T> &, dst,
		     const UT_VoxelArray<T> &, src,
		     mipmaptype, function)
    void	 downsamplePartial(UT_VoxelArray<T> &dst,
				   const UT_VoxelArray<T> &src,
				   mipmaptype function,
				   const UT_JobInfo &info);

protected:
    T			 mixValues(T t1, T t2, mipmaptype function) const
    {
	switch (function)
	{
	    case MIPMAP_MAXIMUM:
		return SYSmax(t1, t2);

	    case MIPMAP_AVERAGE:
		return (t1 + t2) / 2;

	    case MIPMAP_MINIMUM:
		return SYSmin(t1, t2);
	}

	return t1;
    }

    
    /// This stores the base most level that was provided
    /// externally.
    UT_VoxelArray<T>	*myBaseLevel;
    /// If true, we will delete the base level when we are done.
    bool		 myOwnBase;

    /// Tracks the number of levels which we used to represent
    /// this hierarchy.
    int			 myNumLevels;
    /// The array of VoxelArrays, one per level.
    /// myLevels[0] is a 1x1x1 array. Each successive layer is twice
    /// as big in each each dimension.  However, every layer is clamped
    /// against the resolution of the base layer.
    /// We own all these layers.
    UT_ValArray<UT_VoxelArray<T> **>	 myLevels;
};


/// Iterator for Voxel Arrays
///
/// This class eliminates the need for having
/// for (z = 0; z < zres; z++)
///   ...
///     for (x = 0; x < xres; x++)
/// loops everywhere.
///
/// Note that the order of iteration is undefined!  (The actual order is
/// to complete each tile in turn, thereby hopefully improving cache
/// coherency)
///
/// It is safe to write to the voxel array while this iterator is active.
/// It is *not* safe to resize the voxel array (or destroy it)
///
/// The iterator is similar in principal to an STL iterator, but somewhat
/// simpler. The classic STL loop
///   for ( it = begin(); it != end(); ++it )
/// is done using
///   for ( it.rewind(); !it.atEnd(); it.advance() )
/// 
template <typename T>
class UT_VoxelArrayIterator
{
public:
	     UT_VoxelArrayIterator();
	     UT_VoxelArrayIterator(UT_VoxelArray<T> *vox);
	     UT_VoxelArrayIterator(UT_COWReadHandle<UT_VoxelArray<T> > handle);
    virtual ~UT_VoxelArrayIterator();	    

    void	setArray(UT_VoxelArray<T> *vox)
		{
		    myCurTile = -1;
		    myHandle.resetHandle();
		    myArray = vox;
		    // Reset the range
		    setPartialRange(0, 1);
		}
    void	setConstArray(const UT_VoxelArray<T> *vox)
		{
		    setArray((UT_VoxelArray<T> *) vox);
		}

    /// Iterates over the array pointed to by the handle.  Only
    /// supports read access during the iteration as it does
    /// a read lock.
    void	setHandle(UT_COWReadHandle<UT_VoxelArray<T> > handle)
		{
		    myHandle = handle;
		    // Ideally we'd have a separate const iterator
		    // from our non-const iterator so this would
		    // only be exposed in the const version.
		    myArray = const_cast<UT_VoxelArray<T> *>(&*myHandle);

		    // Reset our range.
		    myCurTile = -1;
		    setPartialRange(0, 1);
		}


    /// Restricts this iterator to only run over a subset
    /// of the tiles.  The tiles will be divided into approximately
    /// numrange equal groups, this will be the idx'th.
    /// The resulting iterator may have zero tiles.
    void	setPartialRange(int idx, int numranges);

    /// Ties this iterator to the given jobinfo so it will
    /// match the jobinfo's processing.
    void	splitByTile(const UT_JobInfo &info);

    /// Assigns an interrupt handler.  This will be tested whenever
    /// it advances to a new tile.  If it is interrupted, the iterator
    /// will jump forward to atEnd()
    void	setInterrupt(UT_Interrupt *interrupt) { myInterrupt = interrupt; }
    void	detectInterrupts() { myInterrupt = UTgetInterrupt(); }

    /// Restricts this iterator to the tiles that intersect
    /// the given bounding box of voxel coordinates.
    /// Note that this will not be a precise restriction as
    /// each tile is either included or not.
    /// You should setPartialRange() after setting the bbox range
    /// The bounding box is on the [0..1]^3 range.
    void	restrictToBBox(const UT_BoundingBox &bbox);
    /// The [xmin, xmax] are inclusive and measured in voxels.
    void	restrictToBBox(int xmin, int xmax,
			       int ymin, int ymax,
			       int zmin, int zmax);

    /// Resets the iterator to point to the first voxel.
    void	rewind();

    /// Returns true if we have iterated over all of the voxels.
    bool	atEnd() const
		{ return myCurTile < 0; }

    /// Advances the iterator to point to the next voxel.
    void	advance()
		{
		    // We try to advance each axis, rolling over to the next.
		    // If we exhaust this tile, we call advanceTile.
		    myPos[0]++;
		    myTileLocalPos[0]++;
		    if (myTileLocalPos[0] >= myTileSize[0])
		    {
			// Wrapped in X.
			myPos[0] -= myTileLocalPos[0];
			myTileLocalPos[0] = 0;

			myPos[1]++;
			myTileLocalPos[1]++;
			if (myTileLocalPos[1] >= myTileSize[1])
			{
			    // Wrapped in Y.
			    myPos[1] -= myTileLocalPos[1];
			    myTileLocalPos[1] = 0;

			    myPos[2]++;
			    myTileLocalPos[2]++;
			    if (myTileLocalPos[2] >= myTileSize[2])
			    {
				// Wrapped in Z!  Finished this tile!
				advanceTile();
			    }
			}
		    }
		}

    /// Retrieve the current location of the iterator.
    int		x() const { return myPos[0]; }
    int		y() const { return myPos[1]; }
    int		z() const { return myPos[2]; }
    int		idx(int idx) const { return myPos[idx]; }
    
    /// Retrieves the value that we are currently pointing at.
    /// This is faster than an operator(x,y,z) as we already know
    /// our current tile and that bounds checking isn't needed.
    T	 	 getValue() const
		 {
		     UT_ASSERT_P(myCurTile >= 0);

		     UT_VoxelTile<T>	*tile;

		     tile = myArray->getLinearTile(myCurTile);
		     return (*tile)(myTileLocalPos[0],
				    myTileLocalPos[1],
				    myTileLocalPos[2]);
		 }
    
    /// Sets the voxel we are currently pointing to the given value.
    void	 setValue(T t) const
		 {
		     UT_ASSERT_P(myCurTile >= 0);

		     UT_VoxelTile<T>	*tile;

		     tile = myArray->getLinearTile(myCurTile);

		     tile->setValue(myTileLocalPos[0],
				    myTileLocalPos[1],
				    myTileLocalPos[2], t);
		 }

    /// Returns true if the tile we are currently in is a constant tile.
    bool	 isTileConstant() const
		 {
		     UT_ASSERT_P(myCurTile >= 0);

		     UT_VoxelTile<T>	*tile;

		     tile = myArray->getLinearTile(myCurTile);
		     return tile->isConstant();
		 }

    /// This tile will iterate over the voxels indexed [start,end).
    void	 getTileVoxels(UT_Vector3I &start, UT_Vector3I &end) const
		 {
		     start.x() = myTilePos[0] * TILESIZE;
		     start.y() = myTilePos[1] * TILESIZE;
		     start.z() = myTilePos[2] * TILESIZE;
		     end = start;
		     end.x() += myTileSize[0];
		     end.y() += myTileSize[1];
		     end.z() += myTileSize[2];
		 }

    /// This tile will iterate over the *inclusive* voxels indexed
    /// in the returned boudning box.
    UT_BoundingBoxI getTileBBox() const
		{
		    UT_Vector3I start, end;
		    getTileVoxels(start, end);
		    return UT_BoundingBoxI(start, end);
		}

    /// Returns true if we are at the start of a new tile.
    bool	 isStartOfTile() const
		 { return !(myTileLocalPos[0] ||
			    myTileLocalPos[1] ||
			    myTileLocalPos[2]); }

    /// Returns the VoxelTile we are currently processing
    UT_VoxelTile<T>	*getTile() const
			 {
			     UT_ASSERT_P(myCurTile >= 0);
			     return myArray->getLinearTile(myCurTile);
			 }
    int			 getLinearTileNum() const
			 {
			    return myCurTile;
			 }

    /// Advances the iterator to point to the next tile.  Useful if the
    /// constant test showed that you didn't need to deal with this one.
    void	 advanceTile();
    
    /// Advances the iterator to pointing just before the next tile so
    /// the next advance() will be an advanceTile().  This is useful
    /// if you want to do a continue; as your break but the forloop
    /// is doing advance()
    /// Note the iterator is in a bad state until advance() is called.
    void	skipToEndOfTile();

    /// Sets a flag which causes the iterator to tryCompress()
    /// tiles when it is done with them.  
    bool	 getCompressOnExit() const { return myShouldCompressOnExit; }
    void	 setCompressOnExit(bool shouldcompress)
		 { myShouldCompressOnExit = shouldcompress; }

    /// These templated algorithms are designed to apply simple operations
    /// across all of the voxels with as little overhead as possible.
    /// The iterator should already point to a voxel array and if multithreaded
    /// had its partial range set.  The source arrays must be matching size.
    /// The operator should support a () operator, and the result is
    /// vit.setValue( op(vit.getValue(), a->getValue(vit), ...);
    /// Passing T instead of UT_VoxelArray will treat it as a constant source
    /// Note if both source and destination tiles are constant, only
    /// a single operation is invoked.
    template <typename OP>
    void	 applyOperation(OP &op);
    template <typename OP, typename S>
    void	 applyOperation(OP &op, const UT_VoxelArray<S> &a);
    template <typename OP>
    void	 applyOperation(OP &op, T a);
    template <typename OP, typename S, typename R>
    void	 applyOperation(OP &op, const UT_VoxelArray<S> &a,
					const UT_VoxelArray<R> &b);
    template <typename OP, typename S, typename R, typename Q>
    void     applyOperation(OP &op, const UT_VoxelArray<S> &a,
                    const UT_VoxelArray<R> &b,
                    const UT_VoxelArray<Q> &c);
    /// These variants will invoke op.isNoop(a, b, ...) which will return
    /// true if those values won't affect the destination.  This allows
    /// constant source tiles to be skipped, for example when adding
    /// 0.
    template <typename OP, typename S>
    void	 applyOperationCheckNoop(OP &op, const UT_VoxelArray<S> &a);
    template <typename OP>
    void	 applyOperationCheckNoop(OP &op, T a);

    /// Assign operation works like apply operation, but *this is written
    /// to without reading, so there is one less parameter to the ()
    /// callback.  This can optimize constant tile writes as the 
    /// constant() status of the destination doesn't matter.
    template <typename OP, typename S>
    void	 assignOperation(OP &op, const UT_VoxelArray<S> &a);
    template <typename OP, typename S, typename R>
    void	 assignOperation(OP &op, const UT_VoxelArray<S> &a,
					const UT_VoxelArray<R> &b);
    template <typename OP, typename S, typename R, typename Q>
    void	 assignOperation(OP &op, const UT_VoxelArray<S> &a,
					const UT_VoxelArray<R> &b,
					const UT_VoxelArray<Q> &c);

    /// Reduction operators.
    /// op.reduce(T a) called for each voxel, *but*,
    /// op.reduceMany(T a, int n) called to reduce constant blocks.
    template <typename OP>
    void	reduceOperation(OP &op);

    UT_VoxelArray<T>	*getArray() const { return myArray; }

protected:
    /// The array we belong to.
    UT_VoxelArray<T>	*myArray;
    /// The handle that we have locked to get our array.  It is null
    /// by default which makes the lock/unlock nops.
    UT_COWReadHandle<UT_VoxelArray<T> > myHandle;
    
    /// Absolute index into voxel array.
    int		 myPos[3];

    /// Flag determining if we should compress tiles whenever we
    /// advance out of them.
    bool	 myShouldCompressOnExit;

    bool	 myUseTileList;
    UT_IntArray	 myTileList;

public:
    /// Our current linear tile idx.  A value of -1 implies at end.
    int		 myCurTile;

    /// Our current index into the tile list
    int		 myCurTileListIdx;

    /// Our start & end tiles for processing a subrange.
    /// The tile range is half open [start, end)
    int		 myTileStart, myTileEnd;

    /// Which tile we are as per tx,ty,tz rather than linear index.
    int		 myTilePos[3];

    /// Our position within the current tile.
    int		 myTileLocalPos[3];

    /// The size of the current tile
    int		 myTileSize[3];

    /// The job info to use for tilefetching
    const UT_JobInfo	*myJobInfo;

    UT_Interrupt	*myInterrupt;
};

/// Iterator for tiles inside Voxel Arrays
///
/// This class eliminates the need for having
/// for (z = 0; z < zres; z++)
///   ...
///     for (x = 0; x < xres; x++)
/// loops everywhere.
///
/// The iterator is similar in principal to an STL iterator, but somewhat
/// simpler. The classic STL loop
///   for ( it = begin(); it != end(); ++it )
/// is done using
///   for ( it.rewind(); !it.atEnd(); it.advance() )
/// 
template <typename T>
class UT_VoxelTileIterator
{
public:
	     UT_VoxelTileIterator();
	     UT_VoxelTileIterator(const UT_VoxelArrayIterator<T> &vit);
    template <typename S>
	     UT_VoxelTileIterator(const UT_VoxelArrayIterator<S> &vit,
				    UT_VoxelArray<T> *array);
    virtual ~UT_VoxelTileIterator();	    

    template <typename S>
    void	setTile(const UT_VoxelArrayIterator<S> &vit,
			UT_VoxelArray<T> *array)
    {
	UT_ASSERT_P(vit.isStartOfTile());
	myCurTile = array->getLinearTile(vit.getLinearTileNum());
	myArray = array;
	myTileStart[0] = vit.x();
	myTileStart[1] = vit.y();
	myTileStart[2] = vit.z();
    }

    void	setTile(const UT_VoxelArrayIterator<T> &vit)
    {
	setTile(vit, vit.getArray());
    }

    void	setLinearTile(exint lineartilenum, UT_VoxelArray<T> *array)
    {
	myCurTile = array->getLinearTile(lineartilenum);
	myArray = array;

	array->linearTileToXYZ(lineartilenum, 
		myTileStart[0], myTileStart[1], myTileStart[2]);
	myTileStart[0] <<= TILEBITS;
	myTileStart[1] <<= TILEBITS;
	myTileStart[2] <<= TILEBITS;
    }

    /// Resets the iterator to point to the first voxel.
    void	rewind();

    /// Returns true if we have iterated over all of the voxels.
    bool	atEnd() const
		{ return myCurTile == 0 || myAtEnd; }

    /// Advances the iterator to point to the next voxel.
    void	advance()
		{
		    // We try to advance each axis, rolling over to the next.
		    // If we exhaust this tile, we call advanceTile.
		    myPos[0]++;
		    myTileLocalPos[0]++;
		    if (myTileLocalPos[0] >= myTileSize[0])
		    {
			// Wrapped in X.
			myPos[0] -= myTileLocalPos[0];
			myTileLocalPos[0] = 0;

			myPos[1]++;
			myTileLocalPos[1]++;
			if (myTileLocalPos[1] >= myTileSize[1])
			{
			    // Wrapped in Y.
			    myPos[1] -= myTileLocalPos[1];
			    myTileLocalPos[1] = 0;

			    myPos[2]++;
			    myTileLocalPos[2]++;
			    if (myTileLocalPos[2] >= myTileSize[2])
			    {
				// Wrapped in Z!  Finished this tile!
				advanceTile();
			    }
			}
		    }
		}

    /// Retrieve the current location of the iterator, in the
    /// containing voxel array, not in the tile.
    int		x() const { return myPos[0]; }
    int		y() const { return myPos[1]; }
    int		z() const { return myPos[2]; }
    int		idx(int idx) const { return myPos[idx]; }
    
    /// Retrieves the value that we are currently pointing at.
    /// This is faster than an operator(x,y,z) as we already know
    /// our current tile and that bounds checking isn't needed.
    T	 	 getValue() const
		 {
		     UT_ASSERT_P(myCurTile);

		     return (*myCurTile)(myTileLocalPos[0],
					myTileLocalPos[1],
					myTileLocalPos[2]);
		 }
    
    /// Sets the voxel we are currently pointing to the given value.
    void	 setValue(T t) const
		 {
		     UT_ASSERT_P(myCurTile);

		     myCurTile->setValue(myTileLocalPos[0],
					 myTileLocalPos[1],
					 myTileLocalPos[2], t);
		 }

    /// Returns true if the tile we are currently in is a constant tile.
    bool	 isTileConstant() const
		 {
		     UT_ASSERT_P(myCurTile);

		     return myCurTile->isConstant();
		 }

    /// Returns true if we are at the start of a new tile.
    bool	 isStartOfTile() const
		 { return !(myTileLocalPos[0] ||
			    myTileLocalPos[1] ||
			    myTileLocalPos[2]); }

    /// Returns the VoxelTile we are currently processing
    UT_VoxelTile<T>	*getTile() const
			 {
			     return myCurTile;
			 }

    /// Advances the iterator to point to the next tile.  Since
    /// we are restricted to one tile, effectively just ends the iterator.
    void	 advanceTile();
    
    /// Sets a flag which causes the iterator to tryCompress()
    /// tiles when it is done with them.  
    bool	 getCompressOnExit() const { return myShouldCompressOnExit; }
    void	 setCompressOnExit(bool shouldcompress)
		 { myShouldCompressOnExit = shouldcompress; }

    /// Reduction operators.
    /// op.reduce(T a) called for each voxel, *but*,
    /// op.reduceMany(T a, int n) called to reduce constant blocks.
    /// Early exits if op.reduce() returns false.
    template <typename OP>
    bool	reduceOperation(OP &op);

protected:
    /// Current processing tile
    UT_VoxelTile<T>	 *myCurTile;
    UT_VoxelArray<T>	 *myArray;

    /// Absolute index into voxel array.
    int		 myPos[3];
    /// Absolute index of start of tile
    int		 myTileStart[3];

    /// Flag determining if we should compress tiles whenever we
    /// advance out of them.
    bool	 myShouldCompressOnExit;

    /// Since we want to allow multiple passes, we can't
    /// clear out myCurTile when we hit the end.
    bool	 myAtEnd;

public:
    /// Our position within the current tile.
    int		 myTileLocalPos[3];

    /// The size of the current tile
    int		 myTileSize[3];
};

/// Probe for Voxel Arrays
///
/// This class is designed to allow for efficient evaluation
/// of aligned indices of a voxel array, provided the voxels are iterated
/// in a tile-by-tile, x-inner most, manner.
///
/// This class will create a local copy of the voxel data where needed,
/// uncompressing the information once for every 16 queries.  It will
/// also create an aligned buffer so you can safely use v4uf on fpreal32
/// data.
///
/// For queries where you need surrounding values, the prex and postx can
/// specify padding on the probe.  prex should be -1 to allow reading
/// -1 offset, postx 1 to allow reading a 1 offset.
///

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
class UT_VoxelProbe
{
public:
		 UT_VoxelProbe();
		 UT_VoxelProbe(UT_VoxelArray<T> *vox, int prex = 0, int postx = 0);
    virtual	~UT_VoxelProbe();

    void	 setArray(UT_VoxelArray<T> *vox, int prex = 0, int postx = 0);
    void	 setConstArray(const UT_VoxelArray<T> *vox, 
        	               int prex = 0, int postx = 0) 
    { 
	SYS_STATIC_ASSERT(DoWrite == false); 
	setArray((UT_VoxelArray<T> *)vox, prex, postx); 
    }

    UT_VoxelArray<T>	*getArray() const { return myArray; }

    bool	 isValid() const { return myArray != 0; }

    inline T	 getValue() const 
    { 
	return *myCurLine; 
    }
    inline T	 getValue(int offset) const 
    { 
	return myCurLine[myStride*offset]; 
    }

    inline void	 setValue(T value) 
    {
	UT_ASSERT_P(DoWrite);
	*myCurLine = value;
	if (TestForWrites)
	    myDirty = true;
    }
		

    /// Resets where we currently point to.
    /// Returns true if we had to reset our cache line.  If we didn't,
    /// and you have multiple probes acting in-step, you can just
    /// advanceX() the other probes
    template <typename S> 
    bool	 setIndex(UT_VoxelArrayIterator<S> &vit)
		 { return setIndex(vit.x(), vit.y(), vit.z()); }
    template <typename S> 
    bool	 setIndex(UT_VoxelTileIterator<S> &vit)
		 { return setIndex(vit.x(), vit.y(), vit.z()); }

    bool	 setIndex(int x, int y, int z);

    /// Blindly advances our current pointer.
    inline void	 advanceX() 
    { 
	myCurLine += myStride; 
	myX++; 
	UT_ASSERT_P(myX < myMaxValidX); 
    }

    /// Adjusts our current pointer to the given absolute location,
    /// assumes the new value is inside our valid range.
    inline void	 resetX(int x) 
    { 
	myCurLine += myStride * (x - myX); 
	myX = x; 
	UT_ASSERT_P(myX < myMaxValidX && myX >= myMinValidX); 
    }

protected:
    void	 reloadCache(int x, int y, int z);

    void	 writeCacheLine();

    void	 buildConstantCache(T value);

    T		*myCurLine;
    /// myCacheLine[0] is the start of the cache line, so -1 would be
    /// the first pre-rolled value
    T		*myCacheLine;
    /// Where we actually allocated our cache line, aligned to 4x multiple
    /// to ensure SSE compatible.
    T		*myAllocCacheLine;

    int		 myX, myY, myZ;
    int		 myPreX, myPostX;
    int		 myStride;
    bool	 myForceCopy;
    /// Half inclusive [,) range of valid x queries for current cache.
    int		 myMinValidX, myMaxValidX;

    /// Determines if we have anything to write back, only
    /// valid if TestForWrites is enabled.
    bool	 myDirty;

    UT_VoxelArray<T>	*myArray;

    friend class UT_VoxelProbeCube<T>;
    friend class UT_VoxelProbeFace<T>;
};

///
/// The vector probe is three normal probes into separate voxel arrays
/// making it easier to read and write to aligned vector fields.
/// If the vector field is face-centered, see the UT_VoxelProbeFace.
///
template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
class UT_VoxelVectorProbe
{
public:
		 UT_VoxelVectorProbe()
		 { }
		 UT_VoxelVectorProbe(UT_VoxelArray<T> *vx, UT_VoxelArray<T> *vy, UT_VoxelArray<T> *vz)
		 { setArray(vx, vy, vz); }
    virtual	~UT_VoxelVectorProbe()
		 {}

    void	 setArray(UT_VoxelArray<T> *vx, UT_VoxelArray<T> *vy, UT_VoxelArray<T> *vz)
    {
	myLines[0].setArray(vx);
	myLines[1].setArray(vy);
	myLines[2].setArray(vz);
    }
    void	 setConstArray(const UT_VoxelArray<T> *vx, const UT_VoxelArray<T> *vy, const UT_VoxelArray<T> *vz)
    {
	SYS_STATIC_ASSERT(DoWrite == false); 
	setArray((UT_VoxelArray<T> *)vx, (UT_VoxelArray<T> *)vy, (UT_VoxelArray<T> *)vz);
    }

    inline UT_Vector3	 getValue() const 
    { 
	return UT_Vector3(myLines[0].getValue(), myLines[1].getValue(), myLines[2].getValue()); 
    }
    inline T	 getValue(int axis) const 
    { 
	return myLines[axis].getValue(); 
    }

    inline void	 setValue(const UT_Vector3 &v) 
    {
	myLines[0].setValue(v.x());
	myLines[1].setValue(v.y());
	myLines[2].setValue(v.z());
    }
		
    inline void	 setComponent(int axis, T val) 
    {
	myLines[axis].setValue(val);
    }

    /// Resets where we currently point to.
    /// Returns true if we had to reset our cache line.  If we didn't,
    /// and you have multiple probes acting in-step, you can just
    /// advanceX() the other probes
    template <typename S> 
    bool	 setIndex(UT_VoxelArrayIterator<S> &vit)
		 { return setIndex(vit.x(), vit.y(), vit.z()); }
    template <typename S> 
    bool	 setIndex(UT_VoxelTileIterator<S> &vit)
		 { return setIndex(vit.x(), vit.y(), vit.z()); }

    bool	 setIndex(int x, int y, int z)
		 { 
		    if (myLines[0].setIndex(x, y, z))
		    {
		       myLines[1].setIndex(x, y, z);
		       myLines[2].setIndex(x, y, z);
		       return true;
		    }
		    myLines[1].advanceX();
		    myLines[2].advanceX();
		    return false;
		 }

    void	 advanceX()
		 { myLines[0].advanceX(); myLines[1].advanceX(); myLines[2].advanceX(); }

protected:
    UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>	myLines[3];
};

template <typename T>
class
UT_VoxelProbeCube
{
public:
		 UT_VoxelProbeCube();
    virtual	~UT_VoxelProbeCube();

    void	 setCubeArray(const UT_VoxelArray<T> *vox);
    void	 setPlusArray(const UT_VoxelArray<T> *vox);

    /// Allows you to query +/-1 in each direction.  In cube update,
    /// all are valid.  In plus update, only one of x y and z may be
    /// non zero.
    inline T	 getValue(int x, int y, int z) const
		 { return myLines[y+1][z+1].getValue(x); }

    template <typename S> 
    bool	 setIndexCube(UT_VoxelArrayIterator<S> &vit)
		 { return setIndexCube(vit.x(), vit.y(), vit.z()); }
    template <typename S> 
    bool	 setIndexCube(UT_VoxelTileIterator<S> &vit)
		 { return setIndexCube(vit.x(), vit.y(), vit.z()); }
    bool	 setIndexCube(int x, int y, int z);

    template <typename S> 
    bool	 setIndexPlus(UT_VoxelArrayIterator<S> &vit)
		 { return setIndexPlus(vit.x(), vit.y(), vit.z()); }
    template <typename S> 
    bool	 setIndexPlus(UT_VoxelTileIterator<S> &vit)
		 { return setIndexPlus(vit.x(), vit.y(), vit.z()); }
    bool	 setIndexPlus(int x, int y, int z);

    /// Computes central difference gradient, does not scale
    /// by the step size (which is twice voxelsize)
    /// Requires PlusArray
    UT_Vector3	 gradient() const
		 { return UT_Vector3(getValue(1,0,0) - getValue(-1,0,0),
				     getValue(0,1,0) - getValue(0,-1,0),
				     getValue(0,0,1) - getValue(0,0,-1)); }

    /// Computes the central difference curvature using the given
    /// inverse voxelsize (ie, 1/voxelsize) at this point.
    /// Requires CubeArray.
    fpreal64	curvature(const UT_Vector3 &invvoxelsize) const;

    /// Computes the laplacian, again with a given 1/voxelsize.
    /// Requires PlusArray
    fpreal64	laplacian(const UT_Vector3 &invvoxelsize) const;

protected:
    /// Does an rotation of our cache lines, ym becomes y0 and y0 becomes yp,
    /// so further queries with y+1 will be cache hits for 2 out of 3.
    static void	 rotateLines(UT_VoxelProbe<T, true, false, false> &ym, UT_VoxelProbe<T, true, false, false> &y0, UT_VoxelProbe<T, true, false, false> &yp);

    UT_VoxelProbe<T, true, false, false>	myLines[3][3];
    /// Cached look up position.  myValid stores if they are
    /// valid values or not
    bool			myValid;
    int				myX, myY, myZ;
    /// Half inclusive [,) range of valid x queries for current cache.
    int				myMinValidX, myMaxValidX;
};

///
/// UT_VoxelProbeConstant
///
/// Looks like a voxel probe but only returns a constant value.
///
template <typename T>
class
UT_VoxelProbeConstant
{
public:
		 UT_VoxelProbeConstant() {}
    virtual	~UT_VoxelProbeConstant() {}

    template <typename S> 
    bool	 setIndex(UT_VoxelArrayIterator<S> &vit)
		 { return true; }
    template <typename S> 
    bool	 setIndex(UT_VoxelTileIterator<S> &vit)
		 { return true; }
    bool	 setIndex(int x, int y, int z)
		 { return true; }

    void	 setValue(T val) { myValue = val; }
    T		 getValue() const { return myValue; }
protected:
    T		 myValue;
};

///
/// UT_VoxelProbeAverage
/// 
/// When working with MAC grids one often has slightly misalgined
/// fields.  Ie, one field is at the half-grid spacing of another field.
/// The step values are 0 if the dimension is algined, -1 for half a step
/// back (ie, (val(-1)+val(0))/2) and 1 for half a step forward
/// (ie, (val(0)+val(1))/2)
///
template <typename T, int XStep, int YStep, int ZStep>
class
UT_VoxelProbeAverage
{
public:
		 UT_VoxelProbeAverage() {}
    virtual	~UT_VoxelProbeAverage() {}

    void	 setArray(const UT_VoxelArray<T> *vox);

    template <typename S> 
    bool	 setIndex(UT_VoxelArrayIterator<S> &vit)
		 { return setIndex(vit.x(), vit.y(), vit.z()); }
    template <typename S> 
    bool	 setIndex(UT_VoxelTileIterator<S> &vit)
		 { return setIndex(vit.x(), vit.y(), vit.z()); }
    bool	 setIndex(int x, int y, int z);

    /// Returns the velocity centered at this index, thus an average
    /// of the values in each of our internal probes.
    inline T	 getValue() const
    {
	if (ZStep)
	    return (valueZ(1) + valueZ(0)) * 0.5;
	return valueZ(0);
    }

protected:
    inline T	 valueZ(int z) const
    {
	if (YStep)
	    return (valueYZ(1, z) + valueYZ(0, z)) * 0.5;
	return valueYZ(0, z);
    }

    inline T	 valueYZ(int y, int z) const
    {
	if (XStep > 0)
	    return (myLines[y][z].getValue(1) + myLines[y][z].getValue(0)) * 0.5;
	if (XStep < 0)
	    return (myLines[y][z].getValue(-1) + myLines[y][z].getValue(0)) * 0.5;
	return myLines[y][z].getValue();
    }

    // Stores [Y][Z] lines.
    UT_VoxelProbe<T, true, false, false>	myLines[2][2];
};


///
/// UT_VoxelProbeFace is designed to walk over three velocity
/// fields that store face-centered values.  The indices refer
/// to the centers of the voxels.
///
template <typename T>
class
UT_VoxelProbeFace
{
public:
		 UT_VoxelProbeFace();
    virtual	~UT_VoxelProbeFace();

    void	 setArray(const UT_VoxelArray<T> *vx, const UT_VoxelArray<T> *vy, const UT_VoxelArray<T> *vz);
    void	 setVoxelSize(const UT_Vector3 &voxelsize);

    template <typename S> 
    bool	 setIndex(UT_VoxelArrayIterator<S> &vit)
		 { return setIndex(vit.x(), vit.y(), vit.z()); }
    template <typename S> 
    bool	 setIndex(UT_VoxelTileIterator<S> &vit)
		 { return setIndex(vit.x(), vit.y(), vit.z()); }
    bool	 setIndex(int x, int y, int z);

    /// Get the face values on each face component.
    /// Parameters are axis then side.
    /// 0 is the lower face, 1 the higher face.
    inline T	face(int axis, int side) const
    {
	if (axis == 0)
	    return myLines[0][0].getValue(side);
	else
	    return myLines[axis][side].getValue();
    }

    /// Returns the velocity centered at this index, thus an average
    /// of the values in each of our internal probes.
    inline UT_Vector3	 value() const
    {
	return UT_Vector3(0.5f * (face(0, 0) + face(0, 1)),
			  0.5f * (face(1, 0) + face(1, 1)),
			  0.5f * (face(2, 0) + face(2, 1)));
    }

    /// Returns the divergence of this cell.
    inline T		divergence() const
    {
	return    (face(0,1)-face(0,0)) * myVoxelSize.x()
		+ (face(1,1)-face(1,0)) * myVoxelSize.y()
		+ (face(2,1)-face(2,0)) * myVoxelSize.z();

    }

protected:
    
    static void swapLines(UT_VoxelProbe<T, true, false, false> &ym, 
			    UT_VoxelProbe<T, true, false, false> &yp);
    
    
    UT_VoxelProbe<T, true, false, false>	myLines[3][2];

    /// Cached look up position.  myValid stores if they are
    /// valid values or not
    bool			myValid;
    int				myX, myY, myZ;
    /// Half inclusive [,) range of valid x queries for current cache.
    int				myMinValidX, myMaxValidX;

    UT_Vector3			myVoxelSize, myInvVoxelSize;
};


#if defined( WIN32 ) || defined( LINUX ) || defined( MBSD ) || defined(GAMEOS)
    #include "UT_VoxelArray.C"
#endif


// Typedefs for common voxel array types
typedef UT_VoxelArray<fpreal32>			UT_VoxelArrayF;
typedef UT_VoxelArray<int64>			UT_VoxelArrayI;
typedef UT_VoxelArray<UT_Vector4>		UT_VoxelArrayV4;

typedef UT_VoxelMipMap<fpreal32>		UT_VoxelMipMapF;
typedef UT_VoxelArrayIterator<fpreal32>		UT_VoxelArrayIteratorF;
typedef UT_VoxelArrayIterator<int64>		UT_VoxelArrayIteratorI;
typedef UT_VoxelTileIterator<fpreal32>		UT_VoxelTileIteratorF;
typedef UT_VoxelTileIterator<int64>		UT_VoxelTileIteratorI;
typedef UT_VoxelArrayIterator<UT_Vector4>	UT_VoxelArrayIteratorV4;
typedef UT_VoxelTileIterator<UT_Vector4>		UT_VoxelTileIteratorV4;
// Read only probe
typedef UT_VoxelProbe<fpreal32, true, false, false>	UT_VoxelProbeF;
typedef UT_VoxelVectorProbe<fpreal32, true, false, false> UT_VoxelVectorProbeF;
typedef UT_VoxelProbe<UT_Vector4, true, false, false>	UT_VoxelProbeV4;
// Write only
typedef UT_VoxelProbe<fpreal32, false, true, false>	UT_VoxelWOProbeF;
typedef UT_VoxelVectorProbe<fpreal32, false, true, false> UT_VoxelVectorWOProbeF;
typedef UT_VoxelProbe<UT_Vector4, false, true, false>	UT_VoxelWOProbeV4;
// Read/Write always writeback.
typedef UT_VoxelProbe<fpreal32, true, true, false>	UT_VoxelRWProbeF;
typedef UT_VoxelVectorProbe<fpreal32, true, true, false> UT_VoxelVectorRWProbeF;
typedef UT_VoxelProbe<UT_Vector4, true, true, false>	UT_VoxelRWProbeV4;
// Read/Write with testing
typedef UT_VoxelProbe<fpreal32, true, true, true>	UT_VoxelRWTProbeF;
typedef UT_VoxelVectorProbe<fpreal32, true, true, true>	UT_VoxelVectorRWTProbeF;
typedef UT_VoxelProbe<UT_Vector4, true, true, true>	UT_VoxelRWTProbeV4;


typedef UT_VoxelProbeCube<fpreal32>		UT_VoxelProbeCubeF;

typedef UT_COWHandle<UT_VoxelArray<fpreal32> >	UT_VoxelArrayHandleF;
typedef UT_COWReadHandle<UT_VoxelArray<fpreal32> >	UT_VoxelArrayReadHandleF;
typedef UT_COWWriteHandle<UT_VoxelArray<fpreal32> >	UT_VoxelArrayWriteHandleF;

typedef UT_COWHandle<UT_VoxelArray<UT_Vector4> >	UT_VoxelArrayHandleV4;
typedef UT_COWReadHandle<UT_VoxelArray<UT_Vector4> >	UT_VoxelArrayReadHandleV4;
typedef UT_COWWriteHandle<UT_VoxelArray<UT_Vector4> >	UT_VoxelArrayWriteHandleV4;

#endif


/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_VoxelArray.C ( UT Library, C++)
 *
 * COMMENTS:
 *	Tiled Voxel Array Implementation.
 */

#include "UT_VoxelArray.h"
#include "UT_BoundingBox.h"
#include "UT_Filter.h"
#include "UT_IStream.h"
#include "UT_JSONParser.h"
#include "UT_JSONWriter.h"
#include "UT_SharedMemoryManager.h"
#include "UT_StackBuffer.h"
#include "UT_VoxelArrayJSON.h"
#include <SYS/SYS_Align.h>
#include <SYS/SYS_Math.h>

///
/// fpreal16 conversion functions
///
inline fpreal16
UTvoxelConvertFP16(fpreal16 a) { return a; }
inline fpreal16
UTvoxelConvertFP16(fpreal32 a) { return a; }
inline fpreal16
UTvoxelConvertFP16(fpreal64 a) { return a; }
inline fpreal16
UTvoxelConvertFP16(int8 a) { return a; }
inline fpreal16
UTvoxelConvertFP16(int16 a) { return a; }
inline fpreal16
UTvoxelConvertFP16(int32 a) { return a; }
inline fpreal16
UTvoxelConvertFP16(int64 a) { return a; }
inline fpreal16
UTvoxelConvertFP16(UT_Vector2 a) { return a.x(); }
inline fpreal16
UTvoxelConvertFP16(UT_Vector3 a) { return a.x(); }
inline fpreal16
UTvoxelConvertFP16(UT_Vector4 a) { return a.x(); }

template <typename T>
inline T UTvoxelConvertFromFP16(fpreal16 v)
{
    return T(v);
}

template <>
inline UT_Vector3F UTvoxelConvertFromFP16(fpreal16 v)
{
    return UT_Vector3F(v, v, v);
}
template <>
inline UT_Vector3D UTvoxelConvertFromFP16(fpreal16 v)
{
    return UT_Vector3D(v, v, v);
}
template <>
inline UT_Vector4F UTvoxelConvertFromFP16(fpreal16 v)
{
    return UT_Vector4F(v, v, v, v);
}
template <>
inline UT_Vector4D UTvoxelConvertFromFP16(fpreal16 v)
{
    return UT_Vector4D(v, v, v, v);
}

///
/// VoxelTileCompress definitions
///
template <typename T>
void
UT_VoxelTileCompress<T>::findMinMax(const UT_VoxelTile<T> &tile,
		    T &min, T &max) const
{
    int		x, y, z;

    min = getValue(tile, 0, 0, 0);
    max = min;

    // Generic approach is to just use the getValue() ability.
    for (z = 0; z < tile.zres(); z++)
    {
	for (y = 0; y < tile.yres(); y++)
	{
	    for (x = 0; x < tile.xres(); x++)
	    {
		tile.expandMinMax(getValue(tile, x, y, z), min, max);
	    }
	}
    }
    return;
}

//
// VoxelTile definitions.
//

template <typename T>
UT_VoxelTile<T>::UT_VoxelTile()
{
    myRes[0] = 0;
    myRes[1] = 0;
    myRes[2] = 0;

    myCompressionType = COMPRESS_CONSTANT;
    myForeignData = false;
    
    if (sizeof(T) <= sizeof(T*))
    {
	myData = 0;
    }
    else
    {
	myData = UT_VOXEL_ALLOC(sizeof(T));
	
	// It is not accidental that this is not a static_cast!
	// There isn't a UT_Vector3(fpreal) constructor (as we have a foolish
	// UT_Vector3(fpreal *) constructor that would cause massive problems)
	// but there is a UT_Vector3 operator=(fpreal)!
	((T *)myData)[0] = 0;
    }
}

template <typename T>
UT_VoxelTile<T>::~UT_VoxelTile()
{
    freeData();
}

template <typename T>
UT_VoxelTile<T>::UT_VoxelTile(const UT_VoxelTile<T> &src)
{
    myData = 0;

    // Use assignment operator.
    *this = src;
}

template <typename T>
const UT_VoxelTile<T> &
UT_VoxelTile<T>::operator=(const UT_VoxelTile<T> &src)
{
    if (&src == this)
	return *this;
    
    freeData();
    
    myRes[0] = src.myRes[0];
    myRes[1] = src.myRes[1];
    myRes[2] = src.myRes[2];

    myCompressionType = src.myCompressionType;
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	    myData = UT_VOXEL_ALLOC(
		    sizeof(T) * myRes[0] * myRes[1] * myRes[2]);
	    memcpy(myData, src.myData, sizeof(T) * myRes[0] * myRes[1] * myRes[2]);
	    break;
	case COMPRESS_CONSTANT:
	    if (inlineConstant())
	    {
		myData = src.myData;
	    }
	    else
	    {
		myData = UT_VOXEL_ALLOC(sizeof(T));
		memcpy(myData, src.myData, sizeof(T));
	    }
	    break;
	case COMPRESS_RAWFULL:
	    myData = UT_VOXEL_ALLOC(
		    sizeof(T) * TILESIZE * TILESIZE * myRes[2]);
	    memcpy(myData, src.myData,
		    sizeof(T) * TILESIZE * TILESIZE * myRes[2]);
	    break;
	case COMPRESS_FPREAL16:
	    myData = UT_VOXEL_ALLOC(
		    sizeof(fpreal16) * myRes[0] * myRes[1] * myRes[2]);
	    memcpy(myData, src.myData,
		    sizeof(fpreal16) * myRes[0] * myRes[1] * myRes[2]);
	    break;
	default:
	{
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);
	    myData = UT_VOXEL_ALLOC(engine->getDataLength(src));
	    memcpy(myData, src.myData, engine->getDataLength(src));
	    break;
	}
    }

    return *this;
}

template <typename T>
bool
UT_VoxelTile<T>::writeThrough(int x, int y, int z, T t)
{
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	    // Do the assignment.
	    ((T *)myData)[ ((z * myRes[1]) + y) * myRes[0] + x ] = t;
	    return true;

	case COMPRESS_CONSTANT:
	    if (rawConstVal() == t)
	    {
		// Trivially true.
		return true;
	    }
	    return false;

	case COMPRESS_RAWFULL:
	    ((T *)myData)[ ((z * TILESIZE) + y) * TILESIZE + x ] = t;
	    return true;

	case COMPRESS_FPREAL16:
	    return false;
    }

    // Use the compression engine.
    UT_VoxelTileCompress<T>	*engine;

    engine = getCompressionEngine(myCompressionType);
    return engine->writeThrough(*this, x, y, z, t);
}

template <typename T>
T
UT_VoxelTile<T>::lerp(int x, int y, int z, fpreal32 fx, fpreal32 fy, fpreal32 fz) const
{
    T		vx, vx1, vy, vy1, vz;

    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    // Lerp x:x+1, y, z
	    vx = lerpValues(data[offset], data[offset+1], fx);
	    // Lerp x:x+1, y+1, z
	    vx1 = lerpValues(data[offset+yinc], data[offset+yinc+1], fx);
	    
	    // Lerp x:x+1, y:y+1, z
	    vy = lerpValues(vx, vx1, fy);
	    
	    // Lerp x:x+1, y, z+1
	    vx = lerpValues(data[offset+zinc], data[offset+zinc+1], fx);
	    // Lerp x:x+1, y+1, z+1
	    vx1 = lerpValues(data[offset+zinc+yinc], data[offset+zinc+yinc+1], fx);

	    // Lerp x:x+1, y:y+1, z+1
	    vy1 = lerpValues(vx, vx1, fy);

	    // Lerp x:x+1, y:y+1, z:z+1
	    vz = lerpValues(vy, vy1, fz);
	    break;
	}
	case COMPRESS_RAWFULL:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * TILESIZE + y) * TILESIZE + x;
	    int		 yinc = TILESIZE;
	    int		 zinc = TILESIZE * TILESIZE;

	    // Lerp x:x+1, y, z
	    vx = lerpValues(data[offset], data[offset+1], fx);
	    // Lerp x:x+1, y+1, z
	    vx1 = lerpValues(data[offset+yinc], data[offset+yinc+1], fx);
	    
	    // Lerp x:x+1, y:y+1, z
	    vy = lerpValues(vx, vx1, fy);
	    
	    // Lerp x:x+1, y, z+1
	    vx = lerpValues(data[offset+zinc], data[offset+zinc+1], fx);
	    // Lerp x:x+1, y+1, z+1
	    vx1 = lerpValues(data[offset+zinc+yinc], data[offset+zinc+yinc+1], fx);

	    // Lerp x:x+1, y:y+1, z+1
	    vy1 = lerpValues(vx, vx1, fy);

	    // Lerp x:x+1, y:y+1, z:z+1
	    vz = lerpValues(vy, vy1, fz);
	    break;
	}
	case COMPRESS_FPREAL16:
	{
	    fpreal16	*data = (fpreal16 *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];
	    fpreal16	 vx, vx1, vy, vy1, vz;

	    // Lerp x:x+1, y, z
	    vx = SYSlerp(data[offset], data[offset+1], fx);
	    // Lerp x:x+1, y+1, z
	    vx1 = SYSlerp(data[offset+yinc], data[offset+yinc+1], fx);
	    
	    // Lerp x:x+1, y:y+1, z
	    vy = SYSlerp(vx, vx1, fy);
	    
	    // Lerp x:x+1, y, z+1
	    vx = SYSlerp(data[offset+zinc], data[offset+zinc+1], fx);
	    // Lerp x:x+1, y+1, z+1
	    vx1 = SYSlerp(data[offset+zinc+yinc], data[offset+zinc+yinc+1], fx);

	    // Lerp x:x+1, y:y+1, z+1
	    vy1 = SYSlerp(vx, vx1, fy);

	    // Lerp x:x+1, y:y+1, z:z+1
	    vz = SYSlerp(vy, vy1, fz);

	    return UTvoxelConvertFromFP16<T>(vz);
	}
	case COMPRESS_CONSTANT:
	{
	    // This is quite trivial to do a trilerp on.
	    vz = rawConstVal();
	    break;
	}
	    
	default:
	{
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);
	    // Lerp x:x+1, y, z
	    vx  = lerpValues(engine->getValue(*this, x,   y, z),
		 	     engine->getValue(*this, x+1, y, z),
			     fx);
	    // Lerp x:x+1, y+1, z
	    vx1 = lerpValues(engine->getValue(*this, x,   y+1, z),
			     engine->getValue(*this, x+1, y+1, z),
			     fx);
	    
	    // Lerp x:x+1, y:y+1, z
	    vy = lerpValues(vx, vx1, fy);
	    
	    // Lerp x:x+1, y, z+1
	    vx  = lerpValues(engine->getValue(*this, x,   y, z+1),
		 	     engine->getValue(*this, x+1, y, z+1),
			     fx);
	    // Lerp x:x+1, y+1, z+1
	    vx1 = lerpValues(engine->getValue(*this, x,   y+1, z+1),
			     engine->getValue(*this, x+1, y+1, z+1),
			     fx);

	    // Lerp x:x+1, y:y+1, z+1
	    vy1 = lerpValues(vx, vx1, fy);

	    // Lerp x:x+1, y:y+1, z:z+1
	    vz = lerpValues(vy, vy1, fz);
	    break;
	}
    }

    return vz;
}

template <typename T>
template <int AXIS2D>
T
UT_VoxelTile<T>::lerpAxis(int x, int y, int z, fpreal32 fx, fpreal32 fy, fpreal32 fz) const
{
    T		vx, vx1, vy, vy1, vz;

    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    // Lerp x:x+1, y, z
	    if (AXIS2D != 0)
		vx = lerpValues(data[offset],
				 data[offset+1],
				 fx);
	    else
		vx = data[offset];

	    if (AXIS2D != 1)
	    {
		// Lerp x:x+1, y+1, z
		if (AXIS2D != 0)
		    vx1= lerpValues(data[offset+yinc],
				     data[offset+yinc+1],
				     fx);
		else
		    vx1 = data[offset+yinc];
		// Lerp x:x+1, y:y+1, z
		vy = lerpValues(vx, vx1, fy);
	    }
	    else
		vy = vx;
	    
	    if (AXIS2D != 2)
	    {
		// Lerp x:x+1, y, z+1
		if (AXIS2D != 0)
		    vx = lerpValues(data[offset+zinc],
				     data[offset+zinc+1],
				     fx);
		else
		    vx = data[offset+zinc];

		if (AXIS2D != 1)
		{
		    // Lerp x:x+1, y+1, z+1
		    if (AXIS2D != 0)
			vx1= lerpValues(data[offset+zinc+yinc],
					 data[offset+zinc+yinc+1],
					 fx);
		    else
			vx1 = data[offset+zinc+yinc];
		    // Lerp x:x+1, y:y+1, z+1
		    vy1 = lerpValues(vx, vx1, fy);
		}
		else
		    vy1 = vx;

		// Lerp x:x+1, y:y+1, z:z+1
		vz = lerpValues(vy, vy1, fz);
	    }
	    else
		vz = vy;
	    break;
	}
	case COMPRESS_RAWFULL:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * TILESIZE + y) * TILESIZE + x;
	    int		 yinc = TILESIZE;
	    int		 zinc = TILESIZE * TILESIZE;

	    // Lerp x:x+1, y, z
	    if (AXIS2D != 0)
		vx = lerpValues(data[offset],
				 data[offset+1],
				 fx);
	    else
		vx = data[offset];

	    if (AXIS2D != 1)
	    {
		// Lerp x:x+1, y+1, z
		if (AXIS2D != 0)
		    vx1= lerpValues(data[offset+yinc],
				     data[offset+yinc+1],
				     fx);
		else
		    vx1 = data[offset+yinc];
		// Lerp x:x+1, y:y+1, z
		vy = lerpValues(vx, vx1, fy);
	    }
	    else
		vy = vx;
	    
	    if (AXIS2D != 2)
	    {
		// Lerp x:x+1, y, z+1
		if (AXIS2D != 0)
		    vx = lerpValues(data[offset+zinc],
				     data[offset+zinc+1],
				     fx);
		else
		    vx = data[offset+zinc];

		if (AXIS2D != 1)
		{
		    // Lerp x:x+1, y+1, z+1
		    if (AXIS2D != 0)
			vx1= lerpValues(data[offset+zinc+yinc],
					 data[offset+zinc+yinc+1],
					 fx);
		    else
			vx1 = data[offset+zinc+yinc];
		    // Lerp x:x+1, y:y+1, z+1
		    vy1 = lerpValues(vx, vx1, fy);
		}
		else
		    vy1 = vx;

		// Lerp x:x+1, y:y+1, z:z+1
		vz = lerpValues(vy, vy1, fz);
	    }
	    else
		vz = vy;
	    break;
	}
	case COMPRESS_FPREAL16:
	{
	    fpreal16	*data = (fpreal16 *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];
	    fpreal16	 vx, vx1, vy, vy1, vz;

	    // Lerp x:x+1, y, z
	    if (AXIS2D != 0)
		vx = SYSlerp(data[offset],
				 data[offset+1],
				 fx);
	    else
		vx = data[offset];

	    if (AXIS2D != 1)
	    {
		// Lerp x:x+1, y+1, z
		if (AXIS2D != 0)
		    vx1= SYSlerp(data[offset+yinc],
				     data[offset+yinc+1],
				     fx);
		else
		    vx1 = data[offset+yinc];
		// Lerp x:x+1, y:y+1, z
		vy = SYSlerp(vx, vx1, fy);
	    }
	    else
		vy = vx;
	    
	    if (AXIS2D != 2)
	    {
		// Lerp x:x+1, y, z+1
		if (AXIS2D != 0)
		    vx = SYSlerp(data[offset+zinc],
				     data[offset+zinc+1],
				     fx);
		else
		    vx = data[offset+zinc];

		if (AXIS2D != 1)
		{
		    // Lerp x:x+1, y+1, z+1
		    if (AXIS2D != 0)
			vx1= SYSlerp(data[offset+zinc+yinc],
					 data[offset+zinc+yinc+1],
					 fx);
		    else
			vx1 = data[offset+zinc+yinc];
		    // Lerp x:x+1, y:y+1, z+1
		    vy1 = SYSlerp(vx, vx1, fy);
		}
		else
		    vy1 = vx;

		// Lerp x:x+1, y:y+1, z:z+1
		vz = SYSlerp(vy, vy1, fz);
	    }
	    else
		vz = vy;

	    return UTvoxelConvertFromFP16<T>(vz);
	}
	case COMPRESS_CONSTANT:
	{
	    // This is quite trivial to do a bilerp on.
	    vz = rawConstVal();
	    break;
	}
	    
	default:
	{
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);
	    // Lerp x:x+1, y, z
	    if (AXIS2D != 0)
		vx = lerpValues(engine->getValue(*this, x,   y, z),
				 engine->getValue(*this, x+1, y, z),
				 fx);
	    else
		vx = engine->getValue(*this, x, y, z);

	    if (AXIS2D != 1)
	    {
		// Lerp x:x+1, y+1, z
		if (AXIS2D != 0)
		    vx1= lerpValues(engine->getValue(*this, x,   y+1, z),
				     engine->getValue(*this, x+1, y+1, z),
				     fx);
		else
		    vx1 = engine->getValue(*this, x, y+1, z);
		// Lerp x:x+1, y:y+1, z
		vy = lerpValues(vx, vx1, fy);
	    }
	    else
		vy = vx;
	    
	    if (AXIS2D != 2)
	    {
		// Lerp x:x+1, y, z+1
		if (AXIS2D != 0)
		    vx = lerpValues(engine->getValue(*this, x,   y, z+1),
				     engine->getValue(*this, x+1, y, z+1),
				     fx);
		else
		    vx = engine->getValue(*this, x, y, z+1);

		if (AXIS2D != 1)
		{
		    // Lerp x:x+1, y+1, z+1
		    if (AXIS2D != 0)
			vx1= lerpValues(engine->getValue(*this, x,   y+1, z+1),
					 engine->getValue(*this, x+1, y+1, z+1),
					 fx);
		    else
			vx1 = engine->getValue(*this, x, y+1, z+1);
		    // Lerp x:x+1, y:y+1, z+1
		    vy1 = lerpValues(vx, vx1, fy);
		}
		else
		    vy1 = vx;

		// Lerp x:x+1, y:y+1, z:z+1
		vz = lerpValues(vy, vy1, fz);
	    }
	    else
		vz = vy;
	    break;
	}
    }

    return vz;
}

template <typename T>
bool
UT_VoxelTile<T>::extractSample(int x, int y, int z, T *sample) const
{
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    sample[0] = data[offset];
	    sample[1] = data[offset+1];
	    sample[2+0] = data[offset+yinc];
	    sample[2+1] = data[offset+yinc+1];
	    sample[4+0] = data[offset+zinc];
	    sample[4+1] = data[offset+zinc+1];
	    sample[4+2+0] = data[offset+zinc+yinc];
	    sample[4+2+1] = data[offset+zinc+yinc+1];
	    break;
	}
	case COMPRESS_RAWFULL:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * TILESIZE + y) * TILESIZE + x;
	    int		 yinc = TILESIZE;
	    int		 zinc = TILESIZE * TILESIZE;

	    sample[0] = data[offset];
	    sample[1] = data[offset+1];
	    sample[2+0] = data[offset+yinc];
	    sample[2+1] = data[offset+yinc+1];
	    sample[4+0] = data[offset+zinc];
	    sample[4+1] = data[offset+zinc+1];
	    sample[4+2+0] = data[offset+zinc+yinc];
	    sample[4+2+1] = data[offset+zinc+yinc+1];
	    break;
	}
	case COMPRESS_FPREAL16:
	{
	    fpreal16	*data = (fpreal16 *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    sample[0] = data[offset];
	    sample[1] = data[offset+1];
	    sample[2+0] = data[offset+yinc];
	    sample[2+1] = data[offset+yinc+1];
	    sample[4+0] = data[offset+zinc];
	    sample[4+1] = data[offset+zinc+1];
	    sample[4+2+0] = data[offset+zinc+yinc];
	    sample[4+2+1] = data[offset+zinc+yinc+1];
	    break;
	}
	case COMPRESS_CONSTANT:
	{
	    sample[0] = rawConstVal();
	    return true;
	}
	    
	default:
	{
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);
	    // Lerp x:x+1, y, z
	    sample[0] = engine->getValue(*this, x, y, z);
	    sample[1] = engine->getValue(*this, x+1, y, z);
	    sample[2+0] = engine->getValue(*this, x, y+1, z);
	    sample[2+1] = engine->getValue(*this, x+1, y+1, z);
	    sample[4+0] = engine->getValue(*this, x, y, z+1);
	    sample[4+1] = engine->getValue(*this, x+1, y, z+1);
	    sample[4+2+0] = engine->getValue(*this, x, y+1, z+1);
	    sample[4+2+1] = engine->getValue(*this, x+1, y+1, z+1);
	    break;
	}
    }
    return false;
}

template <typename T>
bool
UT_VoxelTile<T>::extractSamplePlus(int x, int y, int z, T *sample) const
{
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    sample[0] = data[offset-1];
	    sample[1] = data[offset+1];
	    sample[2+0] = data[offset-yinc];
	    sample[2+1] = data[offset+yinc];
	    sample[4+0] = data[offset-zinc];
	    sample[4+1] = data[offset+zinc];
	    sample[6] = data[offset];
	    break;
	}
	case COMPRESS_RAWFULL:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * TILESIZE + y) * TILESIZE + x;
	    int		 yinc = TILESIZE;
	    int		 zinc = TILESIZE * TILESIZE;

	    sample[0] = data[offset-1];
	    sample[1] = data[offset+1];
	    sample[2+0] = data[offset-yinc];
	    sample[2+1] = data[offset+yinc];
	    sample[4+0] = data[offset-zinc];
	    sample[4+1] = data[offset+zinc];
	    sample[6] = data[offset];
	    break;
	}
	case COMPRESS_FPREAL16:
	{
	    fpreal16	*data = (fpreal16 *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    sample[0] = data[offset-1];
	    sample[1] = data[offset+1];
	    sample[2+0] = data[offset-yinc];
	    sample[2+1] = data[offset+yinc];
	    sample[4+0] = data[offset-zinc];
	    sample[4+1] = data[offset+zinc];
	    sample[6] = data[offset];
	    break;
	}
	case COMPRESS_CONSTANT:
	{
	    sample[0] = rawConstVal();
	    return true;
	}
	    
	default:
	{
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);
	    // Lerp x:x+1, y, z
	    sample[0] = engine->getValue(*this, x-1, y, z);
	    sample[1] = engine->getValue(*this, x+1, y, z);
	    sample[2+0] = engine->getValue(*this, x, y-1, z);
	    sample[2+1] = engine->getValue(*this, x, y+1, z);
	    sample[4+0] = engine->getValue(*this, x, y, z+1);
	    sample[4+1] = engine->getValue(*this, x, y, z-1);
	    sample[6] = engine->getValue(*this, x, y, z);
	    break;
	}
    }
    return false;
}

template <typename T>
bool
UT_VoxelTile<T>::extractSampleCube(int x, int y, int z, T *sample) const
{
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];
	    int		 sampidx = 0;

	    offset -= zinc;
	    for (int dz = -1; dz <= 1; dz++)
	    {
		offset -= yinc;
		for (int dy = -1; dy <= 1; dy++)
		{
		    sample[sampidx] = data[offset-1];
		    sample[sampidx+1] = data[offset];
		    sample[sampidx+2] = data[offset+1];
		    sampidx += 3;
		    offset += yinc;
		}
		offset -= yinc * 3;
		offset += zinc;
	    }
	    break;
	}
	case COMPRESS_RAWFULL:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * TILESIZE + y) * TILESIZE + x;
	    int		 yinc = TILESIZE;
	    int		 zinc = TILESIZE * TILESIZE;
	    int		 sampidx = 0;

	    offset -= zinc;
	    for (int dz = -1; dz <= 1; dz++)
	    {
		offset -= yinc;
		for (int dy = -1; dy <= 1; dy++)
		{
		    sample[sampidx] = data[offset-1];
		    sample[sampidx+1] = data[offset];
		    sample[sampidx+2] = data[offset+1];
		    sampidx += 3;
		    offset += yinc;
		}
		offset -= yinc * 3;
		offset += zinc;
	    }
	    break;
	}
	case COMPRESS_FPREAL16:
	{
	    fpreal16	*data = (fpreal16 *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];
	    int		 sampidx = 0;

	    offset -= zinc;
	    for (int dz = -1; dz <= 1; dz++)
	    {
		offset -= yinc;
		for (int dy = -1; dy <= 1; dy++)
		{
		    sample[sampidx] = data[offset-1];
		    sample[sampidx+1] = data[offset];
		    sample[sampidx+2] = data[offset+1];
		    sampidx += 3;
		    offset += yinc;
		}
		offset -= yinc * 3;
		offset += zinc;
	    }
	    break;
	}
	case COMPRESS_CONSTANT:
	{
	    sample[0] = rawConstVal();
	    return true;
	}
	    
	default:
	{
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);
	    int			sampidx = 0;
	    // Lerp x:x+1, y, z
	    for (int dz = -1; dz <= 1; dz++)
	    {
		for (int dy = -1; dy <= 1; dy++)
		{
		    for (int dx = -1; dx <= 1; dx++)
		    {
			sample[sampidx++] = engine->getValue(*this, x+dx, y+dy, z+dz);
		    }
		}
	    }
	    break;
	}
    }
    return false;
}

template <typename T>
template <int AXIS2D>
bool
UT_VoxelTile<T>::extractSampleAxis(int x, int y, int z, T *sample) const
{
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    sample[0] = data[offset];
	    if (AXIS2D != 0)
		sample[1] = data[offset+1];
	    if (AXIS2D != 1)
	    {
		sample[2+0] = data[offset+yinc];
		if (AXIS2D != 0)
		    sample[2+1] = data[offset+yinc+1];
	    }
	    if (AXIS2D != 2)
	    {
		sample[4+0] = data[offset+zinc];
		if (AXIS2D != 0)
		    sample[4+1] = data[offset+zinc+1];
		if (AXIS2D != 1)
		{
		    sample[4+2+0] = data[offset+zinc+yinc];
		    if (AXIS2D != 0)
			sample[4+2+1] = data[offset+zinc+yinc+1];
		}
	    }
	    break;
	}
	case COMPRESS_RAWFULL:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * TILESIZE + y) * TILESIZE + x;
	    int		 yinc = TILESIZE;
	    int		 zinc = TILESIZE * TILESIZE;

	    sample[0] = data[offset];
	    if (AXIS2D != 0)
		sample[1] = data[offset+1];
	    if (AXIS2D != 1)
	    {
		sample[2+0] = data[offset+yinc];
		if (AXIS2D != 0)
		    sample[2+1] = data[offset+yinc+1];
	    }
	    if (AXIS2D != 2)
	    {
		sample[4+0] = data[offset+zinc];
		if (AXIS2D != 0)
		    sample[4+1] = data[offset+zinc+1];
		if (AXIS2D != 1)
		{
		    sample[4+2+0] = data[offset+zinc+yinc];
		    if (AXIS2D != 0)
			sample[4+2+1] = data[offset+zinc+yinc+1];
		}
	    }
	    break;
	}
	case COMPRESS_FPREAL16:
	{
	    fpreal16	*data = (fpreal16 *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    sample[0] = data[offset];
	    if (AXIS2D != 0)
		sample[1] = data[offset+1];
	    if (AXIS2D != 1)
	    {
		sample[2+0] = data[offset+yinc];
		if (AXIS2D != 0)
		    sample[2+1] = data[offset+yinc+1];
	    }
	    if (AXIS2D != 2)
	    {
		sample[4+0] = data[offset+zinc];
		if (AXIS2D != 0)
		    sample[4+1] = data[offset+zinc+1];
		if (AXIS2D != 1)
		{
		    sample[4+2+0] = data[offset+zinc+yinc];
		    if (AXIS2D != 0)
			sample[4+2+1] = data[offset+zinc+yinc+1];
		}
	    }
	    break;
	}
	case COMPRESS_CONSTANT:
	{
	    sample[0] = rawConstVal();
	    return true;
	}
	    
	default:
	{
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);
	    // Lerp x:x+1, y, z
	    sample[0] = engine->getValue(*this, x, y, z);
	    if (AXIS2D != 0)
		sample[1] = engine->getValue(*this, x+1, y, z);
	    if (AXIS2D != 1)
	    {
		sample[2+0] = engine->getValue(*this, x, y+1, z);
		if (AXIS2D != 0)
		    sample[2+1] = engine->getValue(*this, x+1, y+1, z);
	    }
	    if (AXIS2D != 2)
	    {
		sample[4+0] = engine->getValue(*this, x, y, z+1);
		if (AXIS2D != 0)
		    sample[4+1] = engine->getValue(*this, x+1, y, z+1);
		if (AXIS2D != 1)
		{
		    sample[4+2+0] = engine->getValue(*this, x, y+1, z+1);
		    if (AXIS2D != 0)
			sample[4+2+1] = engine->getValue(*this, x+1, y+1, z+1);
		}
	    }
	    break;
	}
    }
    return false;
}

#if 0
template <typename T>
T
UT_VoxelTile<T>::lerp(v4uf frac, int x, int y, int z) const
{
    v4uf		a, b;

    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	case COMPRESS_RAWFULL:
	{
	    T		*data = (T *) myData;
	    int		 offset = (z * myRes[1] + y) * myRes[0] + x;
	    int		 yinc = myRes[0];
	    int		 zinc = myRes[0] * myRes[1];

	    a = v4uf( data[offset],
		      data[offset+zinc],
		      data[offset+yinc],
		      data[offset+yinc+zinc] );
	    b = v4uf( data[offset+1],
		      data[offset+zinc+1],
		      data[offset+yinc+1],
		      data[offset+yinc+zinc+1] );
	    break;
	}

	case COMPRESS_CONSTANT:
	{
	    // This is quite trivial to do a trilerp on.
	    return rawConstVal();
	}
	    
	default:
	{
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);
	    // Lerp x:x+1, y, z
	    a = v4uf( engine->getValue(*this, x, y, z),
		      engine->getValue(*this, x, y, z+1),
		      engine->getValue(*this, x, y+1, z),
		      engine->getValue(*this, x, y+1, z+1) );
	    b = v4uf( engine->getValue(*this, x+1, y, z),
		      engine->getValue(*this, x+1, y, z+1),
		      engine->getValue(*this, x+1, y+1, z),
		      engine->getValue(*this, x+1, y+1, z+1) );
	    break;
	}
    }

    v4uf		fx, fy, fz;

    fx = frac.swizzle<0, 0, 0, 0>();
    fy = frac.swizzle<1, 1, 1, 1>();
    fz = frac.swizzle<2, 2, 2, 2>();

    b -= a;
    a = madd(b, fx, a);

    b = a.swizzle<2, 3, 0, 1>();
    b -= a;
    a = madd(b, fy, a);

    b = a.swizzle<1, 2, 3, 0>();
    b -= a;
    a = madd(b, fz, a);

    return a[0];
}
#endif

template <typename T>
T *
UT_VoxelTile<T>::fillCacheLine(T *cacheline, int &stride, int x, int y, int z, bool forcecopy, bool strideofone) const
{
    UT_ASSERT_P(x >= 0 && y >= 0 && z >= 0);
    UT_ASSERT_P(x < myRes[0] && y < myRes[1] && z < myRes[2]);

    T		*src;
    int		 i, xres = myRes[0];

    // All the directly handled types exit directly from this switch.
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	    stride = 1;
	    src = (T *)myData + (z * myRes[1] + y) * xres;
	    if (!forcecopy)
		return &src[x];

	    for (i = 0; i < xres; i++)
		cacheline[i] = src[i];

	    return &cacheline[x];

	case COMPRESS_FPREAL16:
	{
	    fpreal16		*src;

	    stride = 1;
	    src = (fpreal16 *)myData + (z * myRes[1] + y) * xres;

	    for (i = 0; i < xres; i++)
		cacheline[i] = src[i];

	    return &cacheline[x];
	}


	case COMPRESS_CONSTANT:
	    src = rawConstData();
	    if (!forcecopy && !strideofone)
	    {
		stride = 0;
		return src;
	    }
	    stride = 1;
	    // Fill out a constant value
	    for (i = 0; i < xres; i++)
		cacheline[i] = *src;

	    return &cacheline[x];


	case COMPRESS_RAWFULL:
	    stride = 1;
	    src = (T *)myData + (z * TILESIZE + y) * TILESIZE;
	    if (!forcecopy)
		return &src[x];

	    for (i = 0; i < xres; i++)
		cacheline[i] = src[i];

	    return &cacheline[x];
    }

    // By default use the compression engine.
    UT_VoxelTileCompress<T>	*engine;

    engine = getCompressionEngine(myCompressionType);

    // We could add support for a direct cacheline fill to accelerate
    // this case as well.
    stride = 1;
    for (i = 0; i < xres; i++)
	cacheline[i] = engine->getValue(*this, i, y, z);

    return &cacheline[x];
}

template <typename T>
void
UT_VoxelTile<T>::writeCacheLine(T *cacheline, int y, int z) 
{
    UT_ASSERT_P(y >= 0 && z >= 0);
    UT_ASSERT_P(y < myRes[1] && z < myRes[2]);

    T		*dst, value;
    int		 i, xres = myRes[0];

    // All the directly handled types exit directly from this switch.
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	    dst = (T *)myData + (z * myRes[1] + y) * xres;
	    for (i = 0; i < xres; i++)
		*dst++ = *cacheline++;
	    return;

	case COMPRESS_CONSTANT:
	    value = rawConstVal();
	    for (i = 0; i < xres; i++)
		if (cacheline[i] != value)
		    break;
	    // If everything was equal, our write is trivial.
	    if (i == xres)
		return;
	    
	    break;

	case COMPRESS_RAWFULL:
	    dst = (T *)myData + (z * TILESIZE + y) * TILESIZE;
	    for (i = 0; i < TILESIZE; i++)
		*dst++ = *cacheline++;

	    return;
    }

    // Switch back to invoking writeThrough.  Ideally we'd have
    // a version that can handle a whole cache line at once
    for (i = 0; i < xres; i++)
	if (!writeThrough(i, y, z, cacheline[i]))
	    break;

    // Determine if we failed to write everything through
    if (i != xres)
    {
	// Uncompress and reinvoke ourselves.
	uncompress();
	writeCacheLine(cacheline, y, z);
    }
}

template <typename T>
void
UT_VoxelTile<T>::copyFragment(int dstx, int dsty, int dstz,
			    const UT_VoxelTile<T> &srctile,
			    int srcx, int srcy, int srcz)
{
    int		 w = SYSmin(xres() - dstx, srctile.xres() - srcx);
    int		 h = SYSmin(yres() - dsty, srctile.yres() - srcy);
    int		 d = SYSmin(zres() - dstz, srctile.zres() - srcz);

#if 1
    if (srctile.isSimpleCompression())
    {
	T		*dst;
	const T		*src;
	int		 srcinc;

	src = srctile.rawData();
	srcinc = srctile.isConstant() ? 0 : 1;

	// Ensure we are easy to write to.
	uncompress();

	dst = rawData();
	dst += dstx + (dsty + dstz*yres())*xres();

	if (srcinc)
	    src += srcx + (srcy + srcz*srctile.yres())*srctile.xres();

	for (int z = 0; z < d; z++)
	{
	    for (int y = 0; y < h; y++)
	    {
		if (srcinc)
		{
		    for (int x = 0; x < w; x++)
			dst[x] = src[x];
		}
		else
		{
		    for (int x = 0; x < w; x++)
			dst[x] = *src;
		}
		dst += xres();
		if (srcinc)
		    src += srctile.xres();
	    }
	    dst += (yres()-h) * xres();
	    if (srcinc)
		src += (srctile.yres() - h) * srctile.xres();
	}

	return;
    }
#endif

    // Fail safe approach.
    for (int z = 0; z < d; z++)
	for (int y = 0; y < h; y++)
	    for (int x = 0; x < w; x++)
	    {
		setValue(dstx+x, dsty+y, dstz+z,
			srctile(srcx+x, srcy+y, srcz+z));
	    }
}

template <typename T>
template <typename S>
void
UT_VoxelTile<T>::flatten(S *dst, int stride) const
{
    int		w = xres();
    int		h = yres();
    int		d = zres();

    if (isSimpleCompression())
    {
	const T		*src;
	int		 srcinc;

	src = rawData();
	srcinc = isConstant() ? 0 : 1;

	if (stride == 1)
	{
	    if (srcinc == 1)
	    {
		// Super trivial!
		for (int i = 0; i < w * h * d; i++)
		{
		    *dst++ = T(*src++);
		}
	    }
	    else
	    {
		// Constant, also trivial!
		T		cval = T(*src);

		for (int i = 0; i < w * h * d; i++)
		{
		    *dst++ = cval;
		}
	    }
	}
	else
	{
	    for (int z = 0; z < d; z++)
	    {
		for (int y = 0; y < h; y++)
		{
		    if (srcinc)
		    {
			for (int x = 0; x < w; x++)
			{
			    *dst = S(src[x]);
			    dst += stride;
			}
		    }
		    else
		    {
			for (int x = 0; x < w; x++)
			{
			    *dst = S(*src);
			    dst += stride;
			}
		    }
		    if (srcinc)
			src += w;
		}
	    }
	}

	return;
    }

    // Fail safe approach.
    for (int z = 0; z < d; z++)
	for (int y = 0; y < h; y++)
	    for (int x = 0; x < w; x++)
	    {
		*dst = S((*this)(x, y, z));
		dst += stride;
	    }
}

template <typename T>
template <typename S>
void
UT_VoxelTile<T>::writeData(const S *srcdata, int srcstride)
{
    int		 w = xres();
    int		 h = yres();
    int		 d = zres();
    int		 i, n = w * h * d;

    // Check if src is constant
    S		 compare = srcdata[0];
    int		 srcoff = srcstride;

    if (srcstride == 0)
    {
	// Trivially constant
	makeConstant(T(compare));
	return;
    }

    for (i = 1; i < n; i++)
    {
	if (srcdata[srcoff] != compare)
	    break;
	srcoff += srcstride;
    }

    if (i == n)
    {
	// Constant source!
	makeConstant(compare);
	return;
    }

    // Create a raw tile, expanding constants
    uncompress();

    if (srcstride == 1)
    {
	T *dst = rawData();
	for (i = 0; i < n; i++)
	{
	    *dst++ = T(*srcdata++);
	}
    }
    else
    {
	T		*dst = rawData();

	srcoff = 0;
	for (i = 0; i < n; i++)
	{
	    dst[i] = T(srcdata[srcoff]);
	    srcoff += srcstride;
	}
    }
}

template <typename T>
void
UT_VoxelTile<T>::setValue(int x, int y, int z, T t)
{
    UT_ASSERT_P(x >= 0 && y >= 0 && z >= 0);
    UT_ASSERT_P(x < myRes[0] && y < myRes[1] && z < myRes[2]);

    // Determine if assignment is compatible with current
    // compression technique.
    if (writeThrough(x, y, z, t))
    {
	return;
    }
    // Can't write to our current type of tile with the
    // given value, so abandon.
    uncompress();

    // Attempt to write through again.  This must succeed
    // as we should now be uncompressed.
    UT_VERIFY_P(writeThrough(x, y, z, t));
}

template <typename T>
void
UT_VoxelTile<T>::uncompress()
{
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	    // Trivial.
	    return;

	case COMPRESS_CONSTANT:
	{
	    // Must expand the tile!
	    T		cval;
	    int		i, n;

	    cval = rawConstVal();
	    freeData();

	    myCompressionType = COMPRESS_RAW;

	    if (myRes[0] == TILESIZE &&
		myRes[1] == TILESIZE)
	    {
		// Flag that we can do fast lookups on this tile.
		myCompressionType = COMPRESS_RAWFULL;
	    }

	    n = myRes[0] * myRes[1] * myRes[2];
	    myData = UT_VOXEL_ALLOC(sizeof(T) * n);

	    for (i = 0; i < n; i++)
	    {
		((T *)myData)[i] = cval;
	    }
	    return;
	}
	case COMPRESS_RAWFULL:
	{
	    T		*raw;
	    int		 x, y, z, i, n;

	    if (myRes[0] == TILESIZE &&
		myRes[1] == TILESIZE)
	    {
		// Trivial
		return;
	    }

	    // Need to contract to the actual compact size.
	    n = myRes[0] * myRes[1] * myRes[2];
	    raw = (T *)UT_VOXEL_ALLOC(sizeof(T) * n);
	    i = 0;
	    for (z = 0; z < myRes[2]; z++)
	    {
		for (y = 0; y < myRes[1]; y++)
		{
		    for (x = 0; x < myRes[0]; x++)
		    {
			raw[i++] = ((T *)myData)[x+(y+z*TILESIZE)*TILESIZE];
		    }
		}
	    }

	    freeData();
	    myCompressionType = COMPRESS_RAW;
	    myData = raw;

	    return;
	}

	case COMPRESS_FPREAL16:
	{
	    T			*raw;
	    int		 	x, y, z, i, n;
	    fpreal16		*src = (fpreal16 *) myData;

	    n = myRes[0] * myRes[1] * myRes[2];
	    raw = (T *)UT_VOXEL_ALLOC(sizeof(T) * n);
	    i = 0;
	    for (z = 0; z < myRes[2]; z++)
	    {
		for (y = 0; y < myRes[1]; y++)
		{
		    for (x = 0; x < myRes[0]; x++)
		    {
			raw[i] = src[i];
			i++;
		    }
		}
	    }
	    freeData();
	    myCompressionType = COMPRESS_RAW;
	    myData = raw;
	    return;
	}
    }
    
    // Use the compression engine.
    UT_VoxelTileCompress<T>	*engine;

    engine = getCompressionEngine(myCompressionType);

    // We use the read ability to set our values.
    int			 x, y, z, i;
    T			*raw;

    raw = (T *) UT_VOXEL_ALLOC(sizeof(T) * myRes[0] * myRes[1] * myRes[2]);
    i = 0;
    for (z = 0; z < myRes[2]; z++)
    {
	for (y = 0; y < myRes[1]; y++)
	{
	    for (x = 0; x < myRes[0]; x++)
	    {
		raw[i++] = engine->getValue(*this, x, y, z);
	    }
	}
    }

    freeData();

    // Now copy in the result
    myCompressionType = COMPRESS_RAW;
    if (myRes[0] == TILESIZE &&
	myRes[1] == TILESIZE)
    {
	// Flag that we can do fast lookups on this tile.
	myCompressionType = COMPRESS_RAWFULL;
    }

    myData = raw;
}

template <typename T>
void
UT_VoxelTile<T>::uncompressFull()
{
    T		*raw;
    int		 x, y, z, i;

    if (myCompressionType == COMPRESS_RAWFULL)
	return;

    uncompress();

    UT_ASSERT(myCompressionType == COMPRESS_RAW);

    // The RAWFULL format only differs from RAW when the tile dimensions
    // are smaller than the maximum tile size.
    if (myRes[0] < TILESIZE || myRes[1] < TILESIZE)
    {
	raw = (T *)UT_VOXEL_ALLOC(sizeof(T) * TILESIZE * TILESIZE * myRes[2]);
	i = 0;
	for (z = 0; z < myRes[2]; z++)
	{
	    for (y = 0; y < myRes[1]; y++)
	    {
		for (x = 0; x < myRes[0]; x++)
		{
		    raw[x+(y+z*TILESIZE)*TILESIZE] = ((T *)myData)[i++];
		}
	    }
	}
	freeData();
	myData = raw;
    }
    myCompressionType = COMPRESS_RAWFULL;
}

template <typename T>
void
UT_VoxelTile<T>::makeRawUninitialized()
{
    if (isRaw() || isRawFull())
	return;

    freeData();

    if (myRes[0] == TILESIZE && myRes[1] == TILESIZE)
	myCompressionType = COMPRESS_RAWFULL;
    else
	myCompressionType = COMPRESS_RAW;

    myData = UT_VOXEL_ALLOC(sizeof(T) * numVoxels());
}

template <typename T>
void
UT_VoxelTile<T>::findAverage(T &avg) const
{
    float		irx, iry, irz;

    irx = 1.0 / myRes[0];
    iry = 1.0 / myRes[1];
    irz = 1.0 / myRes[2];
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	{
	    int		i;
	    const T	*data = (const T*) myData;

	    i = 0;
	    T		zavg;
	    zavg = 0;
	    for (int z = 0; z < myRes[2]; z++)
	    {
		T		yavg;
		yavg = 0;
		for (int y = 0; y < myRes[1]; y++)
		{
		    T		xavg;
		    xavg = 0;
		    for (int x = 0; x < myRes[0]; x++)
		    {
			xavg += data[i++];
		    }
		    xavg *= irx;
		    yavg += xavg;
		}
		yavg *= iry;
		zavg += yavg;
	    }
	    zavg *= irz;

	    avg = zavg;
	    return;
	}
	    
	case COMPRESS_FPREAL16:
	{
	    int		i;
	    const fpreal16 *data = (fpreal16 *) myData;

	    i = 0;
	    fpreal16		zavg = 0;
	    for (int z = 0; z < myRes[2]; z++)
	    {
		fpreal16		yavg = 0;
		for (int y = 0; y < myRes[1]; y++)
		{
		    fpreal16		xavg = 0;
		    for (int x = 0; x < myRes[0]; x++)
		    {
			xavg += data[i++];
		    }
		    xavg *= irx;
		    yavg += xavg;
		}
		yavg *= iry;
		zavg += yavg;
	    }
	    zavg *= irz;

	    avg = zavg;
	    return;
	}
	    
	case COMPRESS_CONSTANT:
	    avg = rawConstVal();
	    return;

	case COMPRESS_RAWFULL:
	{
	    int		offset;
	    const T	*data = (const T*) myData;

	    offset = 0;
	    T		zavg;
	    zavg = 0;
	    for (int z = 0; z < myRes[2]; z++)
	    {
		T		yavg;
		yavg = 0;
		for (int y = 0; y < myRes[1]; y++)
		{
		    T		xavg;
		    xavg = 0;
		    for (int x = 0; x < myRes[0]; x++)
		    {
			xavg += data[x+offset];
		    }
		    xavg *= irx;
		    yavg += xavg;
		    offset += TILESIZE;
		}
		yavg *= iry;
		zavg += yavg;
	    }
	    zavg *= irz;

	    avg = zavg;
	    return;
	}
	    
	default:
	{
	    T		zavg;
	    zavg = 0;
	    for (int z = 0; z < myRes[2]; z++)
	    {
		T		yavg;
		yavg = 0;
		for (int y = 0; y < myRes[1]; y++)
		{
		    T		xavg;
		    xavg = 0;
		    for (int x = 0; x < myRes[0]; x++)
		    {
			xavg += (*this)(x, y, z);
		    }
		    xavg *= irx;
		    yavg += xavg;
		}
		yavg *= iry;
		zavg += yavg;
	    }
	    zavg *= irz;

	    avg = zavg;
	    return;
	}
    }
}

template <typename T>
void
UT_VoxelTile<T>::findMinMax(T &min, T &max) const
{
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	{
	    int		n = myRes[0] * myRes[1] * myRes[2];
	    int		i;

	    min = max = *(T*)myData;
	    for (i = 1; i < n; i++)
	    {
		expandMinMax( ((T*)myData)[i], min, max );
	    }
	    return;
	}
	    
	case COMPRESS_FPREAL16:
	{
	    int		n = myRes[0] * myRes[1] * myRes[2];
	    int		i;
	    fpreal16	*src = (fpreal16 *)myData;

	    min = max = *src;
	    for (i = 1; i < n; i++)
	    {
		T		val;
		val = src[i];
		expandMinMax( val, min, max );
	    }
	    return;
	}
	    
	case COMPRESS_CONSTANT:
	    min = max = rawConstVal();
	    return;

	case COMPRESS_RAWFULL:
	{
	    int		x, y, z, offset;

	    min = max = *(T*)myData;
	    offset = 0;
	    for (z = 0; z < myRes[2]; z++)
	    {
		for (y = 0; y < myRes[1]; y++)
		{
		    for (x = 0; x < myRes[0]; x++)
		    {
			expandMinMax(
				((T*)myData)[x+offset],
				min, max );
		    }
		    offset += TILESIZE;
		}
	    }
	    return;
	}
	    
	default:
	{
	    // Use the compression engine.
	    UT_VoxelTileCompress<T>	*engine;

	    engine = getCompressionEngine(myCompressionType);

	    engine->findMinMax(*this, min, max);
	    return;
	}
    }
}

template <typename T>
bool
UT_VoxelTile<T>::hasNan() const
{
    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	case COMPRESS_RAWFULL:
	{
	    int		n = myRes[0] * myRes[1] * myRes[2];
	    int		i;

	    for (i = 0; i < n; i++)
	    {
		if (SYSisNan( ((T*)myData)[i] ))
		    return true;
	    }
	    return false;
	}

	case COMPRESS_FPREAL16:
	{
	    return false;
	}
	    
	case COMPRESS_CONSTANT:
	    if (SYSisNan(rawConstVal()))
		return true;
	    return false;

	default:
	{
	    // Use the compression engine.
	    UT_VoxelTileCompress<T>	*engine;
	    int		x, y, z;

	    engine = getCompressionEngine(myCompressionType);

	    for (z = 0; z < myRes[2]; z++)
		for (y = 0; y < myRes[1]; y++)
		    for (x = 0; x < myRes[0]; x++)
			if (SYSisNan(engine->getValue(*this, x, y, z)))
			{
			    return true;
			}

	    return false;
	}
    }
}

template <typename T>
bool
UT_VoxelTile<T>::tryCompress(const UT_VoxelCompressOptions &options)
{
    T		min, max;
    bool	losslessonly = (options.myQuantizeTol == 0.0);
    int		i;
    UT_VoxelTileCompress<T>	*engine;

    // This is as easy as it gets.
    if (myCompressionType == COMPRESS_CONSTANT)
	return false;

    findMinMax(min, max);

    // See if we can be made into a constant tile.
    if (dist(min, max) <= options.myConstantTol)
    {
	// Ignore if we are already constant.
	if (myCompressionType == COMPRESS_CONSTANT)
	    return false;
	
	// We are fully constant!
	if (min != max)
	{
	    T		avg;
	    findAverage(avg);
	    makeConstant(avg);
	}
	else
	{
	    makeConstant(min);
	}
	return true;
    }

    bool		result = false;

    if (options.myAllowFP16)
    {
	if (myCompressionType == COMPRESS_RAW ||
	    myCompressionType == COMPRESS_RAWFULL)
	{
	    makeFpreal16();
	    result = true;
	}
    }

    for (i = 0; i < getCompressionEngines().entries(); i++)
    {
	engine = getCompressionEngines()(i);

	// Ignore possibly expensive lossy compressions.
	if (losslessonly && !engine->isLossless())
	    continue;

	if (engine->tryCompress(*this, options, min, max))
	{
	    myCompressionType = i + COMPRESS_ENGINE;
	    result = true;
	    // We keep testing in case another compression engine
	    // can get a better number...
	}
    }

    // If we are RAW compress, check to see if we could become
    // RAWFULL for faster access.
    if (myCompressionType == COMPRESS_RAW)
    {
	if (myRes[0] == TILESIZE && myRes[1] == TILESIZE)
	{
	    myCompressionType = COMPRESS_RAWFULL;
	}
    }

    // No suitable compression found.
    return result;
}

template <typename T>
void
UT_VoxelTile<T>::makeConstant(T t)
{
    if (!isConstant())
    {
	freeData();

	if (!inlineConstant())
	    myData = UT_VOXEL_ALLOC(sizeof(T));
    }

    myCompressionType = COMPRESS_CONSTANT;
    *rawConstData() = t;
}

template <typename T>
void
UT_VoxelTile<T>::makeFpreal16()
{
    if (isConstant())
    {
	return;
    }

    if (myCompressionType == COMPRESS_FPREAL16)
	return;

    // Get our new data.
    int			 len = myRes[2] * myRes[1] * myRes[0];
    fpreal16		*data = (fpreal16 *)UT_VOXEL_ALLOC(sizeof(fpreal16) * len);

    if (myCompressionType == COMPRESS_RAW || 
	myCompressionType == COMPRESS_RAWFULL)
    {
	for (int i = 0; i < len; i++)
	{
	    data[i] = UTvoxelConvertFP16( ((T*)myData)[i] );
	}
    }
    else
    {
	// Apply any converters.
	int		i = 0;

	for (int z = 0; z < myRes[2]; z++)
	{
	    for (int y = 0; y < myRes[1]; y++)
	    {
		for (int x = 0; x < myRes[0]; x++)
		{
		    data[i++] = UTvoxelConvertFP16( (*this)(x, y, z));
		}
	    }
	}
    }

    freeData();
    myData = data;
    myCompressionType = COMPRESS_FPREAL16;
}

template <typename T>
int64
UT_VoxelTile<T>::getMemoryUsage(bool inclusive) const
{
    int64 mem = inclusive ? sizeof(*this) : 0;
    mem += getDataLength();
    return mem;
}

template <typename T>
int64
UT_VoxelTile<T>::getDataLength() const
{
    exint		usage;

    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	    usage = sizeof(T) * xres() * yres() * zres();
	    break;
	case COMPRESS_FPREAL16:
	    usage = sizeof(fpreal16) * xres() * yres() * zres();
	    break;
	case COMPRESS_CONSTANT:
	    if (inlineConstant())
		usage = 0;
	    else
		usage = sizeof(T);
	    break;
	case COMPRESS_RAWFULL:
	    usage = sizeof(T) * TILESIZE * TILESIZE * zres();
	    break;
	default:
	{
	    // Use the compression engine.
	    UT_VoxelTileCompress<T>	*engine;
	    engine = getCompressionEngine(myCompressionType);
	    usage = engine->getDataLength(*this);
	    break;
	}
    }
    return usage;
}

template <typename T>
void
UT_VoxelTile<T>::weightedSum(int pstart[3], int pend[3],
			     const float *weights[3], int start[3],
			     T &result)
{
    int		ix, iy, iz, i;
    int		px, py, pz;
    int		ixstart, ixend;
    int		tstart[3];
    fpreal	w, pw;
    T		psumx, psumy;

    switch (myCompressionType)
    {
	case COMPRESS_CONSTANT:
	{
	    w = 1;
	    for (i = 0; i < 3; i++)
	    {
		pw = 0;
		for (ix = 0; ix < pend[i]-pstart[i]; ix++)
		    pw += weights[i][ix+pstart[i]-start[i]];
		w *= pw;
	    }
	    result += w * rawConstVal();
	    break;
	}

	case COMPRESS_RAW:
	{
	    tstart[0] = pstart[0] & TILEMASK;
	    tstart[1] = pstart[1] & TILEMASK;
	    tstart[2] = pstart[2] & TILEMASK;
	    ixstart	= pstart[0]-start[0];
	    ixend	= pend[0]-start[0];
	    pz = tstart[2];
	    UT_ASSERT(pz < myRes[2]);
	    UT_ASSERT(ixend - ixstart <= myRes[0]);
	    for (iz = pstart[2]; iz < pend[2]; iz++, pz++)
	    {
		psumy = 0;
		py = tstart[1];
		UT_ASSERT(py < myRes[1]);
		for (iy = pstart[1]; iy < pend[1]; iy++, py++)
		{
		    psumx = 0;
		    px = ((pz * myRes[1]) + py) * myRes[0] + tstart[0];
		    for (ix = ixstart; ix < ixend; ix++, px++)
		    {
			psumx += weights[0][ix]* ((T*)myData)[px];
		    }
		    psumy += weights[1][iy-start[1]] * psumx;
		}
		result += weights[2][iz-start[2]] * psumy;
	    }
	    break;
	}

	case COMPRESS_FPREAL16:
	{
	    fpreal16		*src = (fpreal16 *) myData;

	    tstart[0] = pstart[0] & TILEMASK;
	    tstart[1] = pstart[1] & TILEMASK;
	    tstart[2] = pstart[2] & TILEMASK;
	    ixstart	= pstart[0]-start[0];
	    ixend	= pend[0]-start[0];
	    pz = tstart[2];
	    UT_ASSERT(pz < myRes[2]);
	    UT_ASSERT(ixend - ixstart <= myRes[0]);
	    for (iz = pstart[2]; iz < pend[2]; iz++, pz++)
	    {
		psumy = 0;
		py = tstart[1];
		UT_ASSERT(py < myRes[1]);
		for (iy = pstart[1]; iy < pend[1]; iy++, py++)
		{
		    psumx = 0;
		    px = ((pz * myRes[1]) + py) * myRes[0] + tstart[0];
		    for (ix = ixstart; ix < ixend; ix++, px++)
		    {
			T		val;
			val = src[px];
			psumx += weights[0][ix]* val;
		    }
		    psumy += weights[1][iy-start[1]] * psumx;
		}
		result += weights[2][iz-start[2]] * psumy;
	    }
	    break;
	}
	case COMPRESS_RAWFULL:
	{
	    tstart[0] = pstart[0] & TILEMASK;
	    tstart[1] = pstart[1] & TILEMASK;
	    tstart[2] = pstart[2] & TILEMASK;
	    ixstart	= pstart[0]-start[0];
	    ixend	= pend[0]-start[0];
	    pz = tstart[2];
	    for (iz = pstart[2]; iz < pend[2]; iz++, pz++)
	    {
		psumy = 0;
		py = tstart[1];
		for (iy = pstart[1]; iy < pend[1]; iy++, py++)
		{
		    psumx = 0;
		    px = ((pz * TILESIZE) + py) * TILESIZE + tstart[0];
		    for (ix = ixstart; ix < ixend; ix++, px++)
		    {
			psumx += weights[0][ix]* ((T*)myData)[px];
		    }
		    psumy += weights[1][iy-start[1]] * psumx;
		}
		result += weights[2][iz-start[2]] * psumy;
	    }
	    break;
	}

	default:
	{
	    // For all other compression types, we use our
	    // getValue() accessor rather than trying to
	    // do anything fancy.
	    tstart[0] = pstart[0] & TILEMASK;
	    tstart[1] = pstart[1] & TILEMASK;
	    tstart[2] = pstart[2] & TILEMASK;
	    ixstart	= pstart[0]-start[0];
	    ixend	= pend[0]-start[0];
	    pz = tstart[2];
	    for (iz = pstart[2]; iz < pend[2]; iz++, pz++)
	    {
		psumy = 0;
		py = tstart[1];
		for (iy = pstart[1]; iy < pend[1]; iy++, py++)
		{
		    psumx = 0;
		    px = tstart[0];
		    for (ix = ixstart; ix < ixend; ix++, px++)
		    {
			psumx += weights[0][ix] * 
				  (*this)(px, py, pz);
		    }
		    psumy += weights[1][iy-start[1]] * psumx;
		}
		result += weights[2][iz-start[2]] * psumy;
	    }
	    break;
	}
    }
}

template <typename T>
void
UT_VoxelTile<T>::registerCompressionEngine(UT_VoxelTileCompress<T> *engine)
{
    int				 i;

    // Repalce old copy, if any.
    for (i = 0; i < getCompressionEngines().entries(); i++)
    {
	if (!strcmp(engine->getName(), getCompressionEngines()(i)->getName()))
	{
	    getCompressionEngines()(i) = engine;
	    return;
	}
    }
	    
    getCompressionEngines().append(engine);
}

template <typename T>
int
UT_VoxelTile<T>::lookupCompressionEngine(const char *name)
{
    int				 i;

    if (!name)
	return -1;

    for (i = 0; i < getCompressionEngines().entries(); i++)
    {
	if (!strcmp(name, getCompressionEngines()(i)->getName()))
	{
	    return i + COMPRESS_ENGINE;
	}
    }

    return -1;
}

template <typename T>
UT_VoxelTileCompress<T> *
UT_VoxelTile<T>::getCompressionEngine(int index)
{
    index -= COMPRESS_ENGINE;

    return getCompressionEngines()(index);
}

template <typename T>
void
UT_VoxelTile<T>::save(std::ostream &os) const
{
    // Always save in our native format...
    if (myCompressionType >= COMPRESS_ENGINE)
    {
	UT_VoxelTileCompress<T>	*engine = getCompressionEngine(myCompressionType);

	if (engine->canSave())
	{
	    char	type = myCompressionType;

	    UTwrite(os, &type, 1);
	    engine->save(os, *this);
	    return;
	}

	// Can't save, must save as raw.
	char		type = COMPRESS_RAW;
	T		value;

	UTwrite(os, &type, 1);

	for (int z = 0; z < zres(); z++)
	    for (int y = 0; y < yres(); y++)
		for (int x = 0; x < xres(); x++)
		{
		    value = (*this)(x, y, z);
		    UTwrite<T>(os, &value, 1);
		}
	return;
    }

    int		len;
    char	type = myCompressionType;

    if (type == COMPRESS_RAWFULL &&
	myRes[0] == TILESIZE &&
	myRes[1] == TILESIZE &&
	myRes[2] != TILESIZE)
    {
	// Forbid saving this as a raw full as that will confuse
	// older versions of Houdini.
	type = COMPRESS_RAW;
    }

    UT_ASSERT(type >= 0 && type < COMPRESS_ENGINE);

    UTwrite(os, &type, 1);

    switch ((CompressionType) type)
    {
	case COMPRESS_RAW:
	    len = myRes[2] * myRes[1] * myRes[0];
	    UTwrite<T>(os, (T *) myData, len);
	    break;

	case COMPRESS_FPREAL16:
	    len = myRes[2] * myRes[1] * myRes[0];
	    UTwrite(os, (int16 *) myData, len);
	    break;

	case COMPRESS_RAWFULL:
	    len = TILESIZE * TILESIZE * TILESIZE;
	    UTwrite<T>(os, (T *) myData, len);
	    break;

	case COMPRESS_CONSTANT:
	    UTwrite<T>(os, rawConstData(), 1);
	    break;
	
	case COMPRESS_ENGINE:
	    UT_ASSERT(!"Invalid compression type");
	    break;
    }
}

template <typename T>
void
UT_VoxelTile<T>::load(UT_IStream &is, const UT_IntArray &compress)
{
    char		type, otype;
    int			len;

    is.readChar(type);

    // Perform the indirection to find out our native type.
    if (type >= 0 && type < compress.entries())
    {
	otype = type;
	type = compress(type);
	if (type == -1)
	{
	    std::cerr << "Missing compression engine " << (int) otype << "\n";
	}
    }

    if (type >= COMPRESS_ENGINE)
    {
	freeData();
	myCompressionType = type;

	if (type - COMPRESS_ENGINE >= getCompressionEngines().entries())
	{
	    // Invalid type!
	    std::cerr << "Invalid compression engine " << (int) otype << "\n";
	    return;
	}

	UT_VoxelTileCompress<T>	*engine = getCompressionEngine(myCompressionType);

	engine->load(is, *this);

	return;
    }

    UT_ASSERT(type >= 0);
    if (type < 0)
    {
	return;
    }

    freeData();

    myCompressionType = type;

    switch ((CompressionType) myCompressionType)
    {
	case COMPRESS_RAW:
	    len = myRes[2] * myRes[1] * myRes[0];
	    myData = UT_VOXEL_ALLOC(sizeof(T) * len);
	    is.read<T>((T *) myData, len);
	    break;

	case COMPRESS_FPREAL16:
	    len = myRes[2] * myRes[1] * myRes[0];
	    myData = UT_VOXEL_ALLOC(sizeof(fpreal16) * len);
	    is.read((int16 *) myData, len);
	    break;

	case COMPRESS_RAWFULL:
	    len = TILESIZE * TILESIZE * TILESIZE;
	    myData = UT_VOXEL_ALLOC(sizeof(T) * len);
	    is.read<T>((T *) myData, len);
	    break;

	case COMPRESS_CONSTANT:
	    if (!inlineConstant())
		myData = UT_VOXEL_ALLOC(sizeof(T));
	    is.read<T>(rawConstData(), 1);
	    break;
	
	case COMPRESS_ENGINE:
	    UT_ASSERT(!"Invalid compression type");
	    break;
    }

    // Recompress raw full that are not complete in z
    if (myCompressionType == COMPRESS_RAW &&
	myRes[0] == TILESIZE &&
	myRes[1] == TILESIZE)
	myCompressionType = COMPRESS_RAWFULL;
}

template <typename T>
bool
UT_VoxelTile<T>::save(UT_JSONWriter &w) const
{
    bool	ok = true;

    // Always save in our native format...
    if (myCompressionType >= COMPRESS_ENGINE)
    {
	UT_VoxelTileCompress<T>	*engine = getCompressionEngine(myCompressionType);

	if (engine->canSave())
	{
	    char	type = myCompressionType;

	    ok = ok && w.jsonBeginArray();
	    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::TILE_COMPRESSION));
	    ok = ok && w.jsonInt(type);
	    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::TILE_DATA));
	    ok = ok && engine->save(w, *this);
	    ok = ok && w.jsonEndArray();
	    return ok;
	}

	// Can't save, must save as raw.
	char		type = COMPRESS_RAW;
	T		value;

	ok = ok && w.jsonBeginArray();
	ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::TILE_COMPRESSION));
	ok = ok && w.jsonInt(type);
	ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::TILE_DATA));

	ok = ok && w.beginUniformArray(xres()*yres()*zres(),
				    w.jidFromValue(&value));
	for (int z = 0; z < zres(); z++)
	    for (int y = 0; y < yres(); y++)
		for (int x = 0; x < xres(); x++)
		{
		    value = (*this)(x, y, z);
		    ok = ok && w.uniformWrite(value);
		}
	ok = ok && w.endUniformArray();
	ok = ok && w.jsonEndArray();
	return ok;
    }

    int		len;
    char	type = myCompressionType;

    UT_ASSERT(type >= 0 && type < COMPRESS_ENGINE);
    ok = ok && w.jsonBeginArray();
    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::TILE_COMPRESSION));
    ok = ok && w.jsonInt(type);
    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::TILE_DATA));

    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	    len = myRes[2] * myRes[1] * myRes[0];
	    ok = ok && w.jsonUniformArray(len, (T *)myData);
	    break;

	case COMPRESS_FPREAL16:
	    len = myRes[2] * myRes[1] * myRes[0];
	    ok = ok && w.jsonUniformArray(len, (fpreal16 *)myData);
	    break;

	case COMPRESS_RAWFULL:
	    len = TILESIZE * TILESIZE * myRes[2];
	    ok = ok && w.jsonUniformArray(len, (T *)myData);
	    break;

	case COMPRESS_CONSTANT:
	    ok = ok && w.jsonValue(rawConstVal());
	    break;
    }

    ok = ok && w.jsonEndArray();
    return ok;
}

template <typename T>
bool
UT_VoxelTile<T>::load(UT_JSONParser &p, const UT_IntArray &compress)
{
    UT_JSONParser::traverser	 it;
    UT_WorkBuffer		 key;
    int8			 type, otype;
    int				 len;

    it = p.beginArray();
    if (it.atEnd() || !it.getLowerKey(key))
	return false;
    if (UT_VoxelArrayJSON::getTileID(key.buffer()) !=
					    UT_VoxelArrayJSON::TILE_COMPRESSION)
	return false;
    if (!p.parseNumber(type))
	return false;

    ++it;

    if (it.atEnd() || !it.getLowerKey(key))
	return false;
    if (UT_VoxelArrayJSON::getTileID(key.buffer()) !=
					    UT_VoxelArrayJSON::TILE_DATA)
	return false;

    // Perform the indirection to find out our native type.
    if (type >= 0 && type < compress.entries())
    {
	otype = type;
	type = compress(type);
	if (type == -1)
	{
	    std::cerr << "Missing compression engine " << (int) otype << "\n";
	}
    }

    if (type >= COMPRESS_ENGINE)
    {
	freeData();
	myCompressionType = type;

	if (type - COMPRESS_ENGINE >= getCompressionEngines().entries())
	{
	    // Invalid type!
	    std::cerr << "Invalid compression engine " << (int) otype << "\n";
	    return false;
	}

	UT_VoxelTileCompress<T>	*engine = getCompressionEngine(myCompressionType);

	bool engine_ok = engine->load(p, *this);
	++it;
	return it.atEnd() && engine_ok;
    }

    UT_ASSERT(type >= 0);
    if (type < 0)
    {
	return false;
    }

    freeData();

    myCompressionType = type;

    switch (myCompressionType)
    {
	case COMPRESS_RAW:
	    len = myRes[2] * myRes[1] * myRes[0];
	    myData = UT_VOXEL_ALLOC(sizeof(T) * len);
	    if (p.parseUniformArray((T *)myData, len) != len)
		return false;
	    break;

	case COMPRESS_FPREAL16:
	    len = myRes[2] * myRes[1] * myRes[0];
	    myData = UT_VOXEL_ALLOC(sizeof(fpreal16) * len);
	    if (p.parseUniformArray((fpreal16 *)myData, len) != len)
		return false;
	    break;

	case COMPRESS_RAWFULL:
	    len = TILESIZE * TILESIZE * myRes[2];
	    myData = UT_VOXEL_ALLOC(sizeof(T) * len);
	    if (p.parseUniformArray((T *)myData, len) != len)
		return false;
	    break;

	case COMPRESS_CONSTANT:
	    if (!inlineConstant())
		myData = UT_VOXEL_ALLOC(sizeof(T));
	    if (!p.parseNumber(*rawConstData()))
		return false;
	    break;
    }

    ++it;
    return it.atEnd();
}

template <typename T>
void
UT_VoxelTile<T>::saveCompressionTypes(std::ostream &os)
{
    int16	ntype = getCompressionEngines().entries();
    int		i;

    ntype += COMPRESS_ENGINE;

    UTwrite(os, &ntype);

    UT_ASSERT(COMPRESS_ENGINE == 4);	// Change lower lines!
    UTsaveStringBinary(os, "raw", UT_STRING_8BIT_IO);
    UTsaveStringBinary(os, "rawfull", UT_STRING_8BIT_IO);
    UTsaveStringBinary(os, "constant", UT_STRING_8BIT_IO);
    UTsaveStringBinary(os, "fpreal16", UT_STRING_8BIT_IO);

    ntype -= COMPRESS_ENGINE;
    for (i = 0; i < ntype; i++)
    {
	UTsaveStringBinary(os, getCompressionEngines()(i)->getName(), UT_STRING_8BIT_IO);
    }
}

template <typename T>
void
UT_VoxelTile<T>::loadCompressionTypes(UT_IStream &is, UT_IntArray &compress)
{
    int16	ntype;
    int		i, idx;

    compress.entries(0);

    is.read(&ntype);

    for (i = 0; i < ntype; i++)
    {
	UT_String		name;

	is.readBinaryString(name, UT_ISTREAM_8BIT_IO);
	if (name == "raw")
	    compress.append(COMPRESS_RAW);
	else if (name == "rawfull")
	    compress.append(COMPRESS_RAWFULL);
	else if (name == "constant")
	    compress.append(COMPRESS_CONSTANT);
	else if (name == "fpreal16")
	    compress.append(COMPRESS_FPREAL16);
	else
	{
	    idx = lookupCompressionEngine(name);

	    // -1 means a failure to find it in our set..
	    // this is only a bad thing if a tile actually uses this engine.

	    compress.append(idx);
	}
    }
}

template <typename T>
bool
UT_VoxelTile<T>::saveCompressionTypes(UT_JSONWriter &w)
{
    int16	ntype = getCompressionEngines().entries();
    int		i;
    bool	ok = true;

    ntype += COMPRESS_ENGINE;

    UT_ASSERT(COMPRESS_ENGINE == 4);	// Change lower lines!
    ok = ok && w.beginUniformArray(ntype, UT_JID_STRING);
    ok = ok && w.uniformWrite("raw");
    ok = ok && w.uniformWrite("rawfull");
    ok = ok && w.uniformWrite("constant");
    ok = ok && w.uniformWrite("fpreal16");

    ntype -= COMPRESS_ENGINE;
    for (i = 0; i < ntype; i++)
    {
	ok = ok && w.uniformWrite(getCompressionEngines()(i)->getName());
    }

    ok = ok && w.endUniformArray();
    return ok;
}

template <typename T>
bool
UT_VoxelTile<T>::loadCompressionTypes(UT_JSONParser &p, UT_IntArray &compress)
{
    UT_JSONParser::traverser	 it;
    UT_WorkBuffer		 buffer;
    int				 idx;

    compress.entries(0);
    for (it = p.beginArray(); !it.atEnd(); ++it)
    {
	if (!p.parseString(buffer))
	    return false;

	if (!buffer.strcmp("raw"))
	    compress.append(COMPRESS_RAW);
	else if (!buffer.strcmp("rawfull"))
	    compress.append(COMPRESS_RAWFULL);
	else if (!buffer.strcmp("constant"))
	    compress.append(COMPRESS_CONSTANT);
	else if (!buffer.strcmp("fpreal16"))
	    compress.append(COMPRESS_FPREAL16);
	else
	{
	    idx = lookupCompressionEngine(buffer.buffer());

	    // -1 means a failure to find it in our set..
	    // this is only a bad thing if a tile actually uses this engine.

	    compress.append(idx);
	}
    }
    return true;
}



//
// VoxelArray definitions.
//

template <typename T>
UT_VoxelArray<T>::UT_VoxelArray()
{
    myRes[0] = 0;
    myRes[1] = 0;
    myRes[2] = 0;
    myTileRes[0] = 0;
    myTileRes[1] = 0;
    myTileRes[2] = 0;

    myTiles = 0;

    myInvRes = 1;

    myBorderValue = 0;
    myBorderScale[0] = 0;
    myBorderScale[1] = 0;
    myBorderScale[2] = 0;
    myBorderType = UT_VOXELBORDER_STREAK;

    mySharedMem = 0;
    mySharedMemView = 0;
}

template <typename T>
UT_VoxelArray<T>::~UT_VoxelArray()
{
    deleteVoxels();
}

template <typename T>
UT_VoxelArray<T>::UT_VoxelArray(const UT_VoxelArray<T> &src)
{
    myRes[0] = 0;
    myRes[1] = 0;
    myRes[2] = 0;
    myInvRes = 1;
    myTileRes[0] = 0;
    myTileRes[1] = 0;
    myTileRes[2] = 0;

    myTiles = 0;
    
    mySharedMem = 0;
    mySharedMemView = 0;
    
    *this = src;
}

template <typename T>
void
UT_VoxelArray<T>::copyDataPartial(const UT_VoxelArray<T> &src,
				  const UT_JobInfo &info)
{
    UT_ASSERT(isMatching(src));
    UT_VoxelArrayIterator<T>   vit(this);
    vit.splitByTile(info);
    for (vit.rewind(); !vit.atEnd(); vit.advanceTile())
    {
        int i = vit.getLinearTileNum();
        myTiles[i] = src.myTiles[i];
    }
}

template <typename T>
const UT_VoxelArray<T> &
UT_VoxelArray<T>::operator=(const UT_VoxelArray<T> &src)
{
    // Paranoid code:
    if (&src == this)
	return *this;

    // Clear out my own data (this is done again by size, but not
    // maybe with resize?)
    deleteVoxels();

    myBorderScale[0] = src.myBorderScale[0];
    myBorderScale[1] = src.myBorderScale[1];
    myBorderScale[2] = src.myBorderScale[2];
    myBorderValue = src.myBorderValue;
    myBorderType = src.myBorderType;

    myCompressionOptions = src.myCompressionOptions;

    // Allocate proper size.
    size(src.myRes[0], src.myRes[1], src.myRes[2]);

    copyData(src);

    return *this;
}

template <typename T>
void
UT_VoxelArray<T>::deleteVoxels()
{
    delete [] myTiles;
    myTiles = 0;

    myTileRes[0] = myTileRes[1] = myTileRes[2] = 0;
    myRes[0] = myRes[1] = myRes[2] = 0;

    delete mySharedMemView;
    mySharedMemView = 0;
    
    delete mySharedMem;
    mySharedMem = 0;
}

template <typename T>
void
UT_VoxelArray<T>::size(int xres, int yres, int zres)
{
    deleteVoxels();
    
    // Initialize our tiles.
    int tile_res[3];
    tile_res[0] = (xres + TILEMASK) >> TILEBITS;
    tile_res[1] = (yres + TILEMASK) >> TILEBITS;
    tile_res[2] = (zres + TILEMASK) >> TILEBITS;

    exint ntiles = ((exint)tile_res[0]) * tile_res[1] * tile_res[2];
    if (ntiles)
    {
	myTiles = new UT_VoxelTile<T> [ntiles];

	// Only set resolutions *AFTER* we successfully allocate
	myRes[0] = xres;
	myRes[1] = yres;
	myRes[2] = zres;

	myInvRes = 1;
	if (xres)
	    myInvRes.x() = 1.0f / myRes[0];
	if (yres)
	    myInvRes.y() = 1.0f / myRes[1];
	if (zres)
	    myInvRes.z() = 1.0f / myRes[2];

	myTileRes[0] = tile_res[0];
	myTileRes[1] = tile_res[1];
	myTileRes[2] = tile_res[2];

	int i = 0;
	for (int tz = 0; tz < myTileRes[2]; tz++)
	{
	    int zr;
	    if (tz < myTileRes[2]-1)
		zr = TILESIZE;
	    else
		zr = zres - tz * TILESIZE;

	    for (int ty = 0; ty < myTileRes[1]; ty++)
	    {
		int yr;
		if (ty < myTileRes[1]-1)
		    yr = TILESIZE;
		else
		    yr = yres - ty * TILESIZE;
		
		int tx, xr = TILESIZE;
		for (tx = 0; tx < myTileRes[0]-1; tx++)
		{
		    myTiles[i].setRes(xr, yr, zr);
		    
		    i++;
		}
		xr = xres - tx * TILESIZE;
		myTiles[i].setRes(xr, yr, zr);
		i++;
	    }
	}
    }
    else
	myTiles = 0;
}

template <typename T>
void
UT_VoxelArray<T>::match(const UT_VoxelArray<T> &src)
{
    // Only resize if our res is different.
    if (src.getXRes() != getXRes() ||
	src.getYRes() != getYRes() ||
	src.getZRes() != getZRes())
    {
	size(src.getXRes(), src.getYRes(), src.getZRes());
    }

    // Update border conditions.
    myBorderType = src.myBorderType;
    myBorderScale[0] = src.myBorderScale[0];
    myBorderScale[1] = src.myBorderScale[1];
    myBorderScale[2] = src.myBorderScale[2];
    myBorderValue = src.myBorderValue;

    // Match our compression tolerances
    myCompressionOptions = src.myCompressionOptions;
}

template <typename T>
int64
UT_VoxelArray<T>::getMemoryUsage(bool inclusive) const
{
    int64 mem = inclusive ? sizeof(*this) : 0;

    int ntiles = numTiles();
    for (int i = 0; i < ntiles; i++)
        mem += myTiles[i].getMemoryUsage(true);

    if (mySharedMem)
        mem += mySharedMem->getMemoryUsage(true);

    if (mySharedMemView)
        mem += mySharedMemView->getMemoryUsage(true);

    return mem;
}

template <typename T>
void
UT_VoxelArray<T>::constantPartial(T t, const UT_JobInfo &info)
{
    UT_VoxelArrayIterator<T>    vit(this);
    vit.splitByTile(info);
    for (vit.rewind(); !vit.atEnd(); vit.advanceTile())
    {
        int i = vit.getLinearTileNum();
        myTiles[i].makeConstant(t);
    }
}

template <typename T>
bool
UT_VoxelArray<T>::isConstant(T *t) const
{
    int		i, ntiles;
    T		cval = 0;
    const fpreal	tol = SYS_FTOLERANCE_R;

    ntiles = numTiles();
    for (i = 0; i < ntiles; i++)
    {
	if (!myTiles[i].isConstant())
	{
	    return false;
	}

	if (!i)
	{
	    // First tile, get the constant value.
	    cval = myTiles[i].rawConstVal();
	}
	else
	{
	    // See if we have deviated too much.
	    if (UT_VoxelTile<T>::dist(cval, myTiles[i].rawConstVal()) > tol)
	    {
		return false;
	    }
	}
    }

    // All tiles are both constant and within tolerance of each
    // other.  Write out our constant value and return true.
    if (t)
	*t = cval;

    return true;
}

template <typename T>
bool
UT_VoxelArray<T>::hasNan() const
{
    int		i, ntiles;

    ntiles = numTiles();
    for (i = 0; i < ntiles; i++)
    {
	if (myTiles[i].hasNan())
	{
	    return true;
	}
    }

    return false;
}

template <typename T>
T
UT_VoxelArray<T>::operator()(UT_Vector3F pos) const
{
    // We go from the position in the unit cube into the index.
    // The center of cells must map to the exact integer indices.
    pos.x() *= myRes[0];
    pos.y() *= myRes[1];
    pos.z() *= myRes[2];
    pos.x() -= 0.5;
    pos.y() -= 0.5;
    pos.z() -= 0.5;

    return lerpVoxelCoord(pos);
}

template <typename T>
T
UT_VoxelArray<T>::lerpVoxelCoord(UT_Vector3F pos) const
{
    int			x, y, z;
    // Yes, these have to be 32 becaues split float requires 32!
    fpreal32		fx, fy, fz;

    splitVoxelCoord(pos, x, y, z, fx, fy, fz);

    return lerpVoxel(x, y, z, fx, fy, fz);
}

template <typename T>
template <int AXIS2D>
T
UT_VoxelArray<T>::lerpVoxelCoordAxis(UT_Vector3F pos) const
{
    int			x, y, z;
    // Yes, these have to be 32 becaues split float requires 32!
    fpreal32		fx, fy, fz;

    splitVoxelCoordAxis<AXIS2D>(pos, x, y, z, fx, fy, fz);

    return lerpVoxelAxis<AXIS2D>(x, y, z, fx, fy, fz);
}

template <typename T>
T
UT_VoxelArray<T>::lerpVoxel(int x, int y, int z,
			    float fx, float fy, float fz) const
{
    // Do trilinear interpolation.
    T			vx, vx1, vy, vy1, vz;

    // Optimization for common cases (values are within the voxel range and
    // are all within the same tile)
    if ( !((x | y | z) < 0) &&
	(((x - myRes[0]+1) & (y - myRes[1]+1) & (z - myRes[2]+1)) < 0))
	
	// (x > 0) && (y > 0) && (z > 0) &&
	// Do not use x+1 < foo as if x is MAX_INT that will falsely
	// report in bounds!
	// (x < myRes[0]-1) && (y < myRes[1]-1) && (z < myRes[2]-1) )
    {
	int			xm, ym, zm;

	xm = x & TILEMASK;
	ym = y & TILEMASK;
	zm = z & TILEMASK;

	if ((xm != TILEMASK) && (ym != TILEMASK) && (zm != TILEMASK))
	{
	    const UT_VoxelTile<T>	*tile =
		getTile(x >> TILEBITS, y >> TILEBITS, z >> TILEBITS);

	    vz = tile->lerp(xm, ym, zm, fx, fy, fz);
	}
	else
	{
	    // We cross tile boundaries but we remain within
	    // the voxel grid.  We can thus avoid any bounds checking
	    // and use operator() rather than getValue.

	    // Lerp x:x+1, y, z
	    vx = UT_VoxelTile<T>::lerpValues((*this)(x,   y, z),
					     (*this)(x+1, y, z),
					     fx);
	    // Lerp x:x+1, y+1, z
	    vx1= UT_VoxelTile<T>::lerpValues((*this)(x,   y+1, z),
					     (*this)(x+1, y+1, z),
					     fx);
	    // Lerp x:x+1, y:y+1, z
	    vy = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
	    
	    // Lerp x:x+1, y, z+1
	    vx = UT_VoxelTile<T>::lerpValues((*this)(x,   y, z+1),
					     (*this)(x+1, y, z+1),
					     fx);
	    // Lerp x:x+1, y+1, z+1
	    vx1= UT_VoxelTile<T>::lerpValues((*this)(x,   y+1, z+1),
					     (*this)(x+1, y+1, z+1),
					     fx);

	    // Lerp x:x+1, y:y+1, z+1
	    vy1 = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);

	    // Lerp x:x+1, y:y+1, z:z+1
	    vz = UT_VoxelTile<T>::lerpValues(vy, vy1, fz);
	}
    }
    else
    {
	// Lerp x:x+1, y, z
	vx = UT_VoxelTile<T>::lerpValues(getValue(x,   y, z),
					 getValue(x+1, y, z),
					 fx);
	// Lerp x:x+1, y+1, z
	vx1= UT_VoxelTile<T>::lerpValues(getValue(x,   y+1, z),
					 getValue(x+1, y+1, z),
					 fx);
	
	// Lerp x:x+1, y:y+1, z
	vy = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
	
	// Lerp x:x+1, y, z+1
	vx = UT_VoxelTile<T>::lerpValues(getValue(x,   y, z+1),
					 getValue(x+1, y, z+1),
					 fx);
	// Lerp x:x+1, y+1, z+1
	vx1= UT_VoxelTile<T>::lerpValues(getValue(x,   y+1, z+1),
					 getValue(x+1, y+1, z+1),
					 fx);

	// Lerp x:x+1, y:y+1, z+1
	vy1 = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);

	// Lerp x:x+1, y:y+1, z:z+1
	vz = UT_VoxelTile<T>::lerpValues(vy, vy1, fz);
    }

    return vz;
}

template <typename T>
template <int AXIS2D>
T
UT_VoxelArray<T>::lerpVoxelAxis(int x, int y, int z,
			    float fx, float fy, float fz) const
{
    // Do bilinear interpolation.
    T			vx, vx1, vy, vy1, vz;

    int			lesscomp = 0, greatercomp = -1;

    if (AXIS2D != 0)
    {
	lesscomp |= x;
	greatercomp &= (x - myRes[0]+1);
    }
    if (AXIS2D != 1)
    {
	lesscomp |= y;
	greatercomp &= (y - myRes[1]+1);
    }
    if (AXIS2D != 2)
    {
	lesscomp |= z;
	greatercomp &= (z - myRes[2]+1);
    }

    // Optimization for common cases (values are within the voxel range and
    // are all within the same tile)
    if ( !(lesscomp < 0) && (greatercomp < 0) )
    {
	int			xm, ym, zm;

	xm = x & TILEMASK;
	ym = y & TILEMASK;
	zm = z & TILEMASK;

	if ((AXIS2D == 0 || xm != TILEMASK) && 
	    (AXIS2D == 1 || ym != TILEMASK) && 
	    (AXIS2D == 2 || zm != TILEMASK))
	{
	    const UT_VoxelTile<T>	*tile =
		getTile( (AXIS2D == 0) ? 0 : (x >> TILEBITS), 
			 (AXIS2D == 1) ? 0 : (y >> TILEBITS), 
			 (AXIS2D == 2) ? 0 : (z >> TILEBITS) );

	    vz = tile->template lerpAxis<AXIS2D>(xm, ym, zm, fx, fy, fz);
	}
	else
	{
	    // We cross tile boundaries but we remain within
	    // the voxel grid.  We can thus avoid any bounds checking
	    // and use operator() rather than getValue.

	    // Lerp x:x+1, y, z
	    if (AXIS2D != 0)
		vx = UT_VoxelTile<T>::lerpValues((*this)(x,   y, z),
						 (*this)(x+1, y, z),
						 fx);
	    else
		vx = (*this)(x, y, z);

	    if (AXIS2D != 1)
	    {
		// Lerp x:x+1, y+1, z
		if (AXIS2D != 0)
		    vx1= UT_VoxelTile<T>::lerpValues((*this)(x,   y+1, z),
						     (*this)(x+1, y+1, z),
						     fx);
		else
		    vx1 = (*this)(x, y+1, z);
		// Lerp x:x+1, y:y+1, z
		vy = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
	    }
	    else
		vy = vx;
	    
	    if (AXIS2D != 2)
	    {
		// Lerp x:x+1, y, z+1
		if (AXIS2D != 0)
		    vx = UT_VoxelTile<T>::lerpValues((*this)(x,   y, z+1),
						     (*this)(x+1, y, z+1),
						     fx);
		else
		    vx = (*this)(x, y, z+1);

		if (AXIS2D != 1)
		{
		    // Lerp x:x+1, y+1, z+1
		    if (AXIS2D != 0)
			vx1= UT_VoxelTile<T>::lerpValues((*this)(x,   y+1, z+1),
							 (*this)(x+1, y+1, z+1),
							 fx);
		    else
			vx1 = (*this)(x, y+1, z+1);
		    // Lerp x:x+1, y:y+1, z+1
		    vy1 = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
		}
		else
		    vy1 = vx;

		// Lerp x:x+1, y:y+1, z:z+1
		vz = UT_VoxelTile<T>::lerpValues(vy, vy1, fz);
	    }
	    else
		vz = vy;
	}
    }
    else
    {
	// Lerp x:x+1, y, z
	if (AXIS2D != 0)
	    vx = UT_VoxelTile<T>::lerpValues(getValue(x,   y, z),
					     getValue(x+1, y, z),
					     fx);
	else
	    vx = getValue(x, y, z);

	if (AXIS2D != 1)
	{
	    // Lerp x:x+1, y+1, z
	    if (AXIS2D != 0)
		vx1= UT_VoxelTile<T>::lerpValues(getValue(x,   y+1, z),
						 getValue(x+1, y+1, z),
						 fx);
	    else
		vx1 = getValue(x, y+1, z);
	    // Lerp x:x+1, y:y+1, z
	    vy = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
	}
	else
	    vy = vx;
	
	if (AXIS2D != 2)
	{
	    // Lerp x:x+1, y, z+1
	    if (AXIS2D != 0)
		vx = UT_VoxelTile<T>::lerpValues(getValue(x,   y, z+1),
						 getValue(x+1, y, z+1),
						 fx);
	    else
		vx = getValue(x, y, z+1);

	    if (AXIS2D != 1)
	    {
		// Lerp x:x+1, y+1, z+1
		if (AXIS2D != 0)
		    vx1= UT_VoxelTile<T>::lerpValues(getValue(x,   y+1, z+1),
						     getValue(x+1, y+1, z+1),
						     fx);
		else
		    vx1 = getValue(x, y+1, z+1);
		// Lerp x:x+1, y:y+1, z+1
		vy1 = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
	    }
	    else
		vy1 = vx;

	    // Lerp x:x+1, y:y+1, z:z+1
	    vz = UT_VoxelTile<T>::lerpValues(vy, vy1, fz);
	}
	else
	    vz = vy;
    }

    return vz;
}

template <typename T>
void
UT_VoxelArray<T>::evaluateMinMax(T &lerp, T &lmin, T &lmax,
			    UT_Vector3F pos) const
{
    // We go from the position in the unit cube into the index.
    // The center of cells must map to the exact integer indices.
    pos.x() *= myRes[0];
    pos.y() *= myRes[1];
    pos.z() *= myRes[2];
    pos.x() -= 0.5;
    pos.y() -= 0.5;
    pos.z() -= 0.5;

    lerpVoxelCoordMinMax(lerp, lmin, lmax, pos);
}

template <typename T>
void
UT_VoxelArray<T>::lerpVoxelCoordMinMax(T &lerp, T &lmin, T &lmax,
					UT_Vector3F pos) const
{
    int			x, y, z;
    // Yes, these have to be 32 becaues split float requires 32!
    fpreal32		fx, fy, fz;

    splitVoxelCoord(pos, x, y, z, fx, fy, fz);

    lerpVoxelMinMax(lerp, lmin, lmax, x, y, z, fx, fy, fz);
}


template <typename T>
template <int AXIS2D>
void
UT_VoxelArray<T>::lerpVoxelCoordMinMaxAxis(T &lerp, T &lmin, T &lmax,
					UT_Vector3F pos) const
{
    int			x, y, z;
    // Yes, these have to be 32 becaues split float requires 32!
    fpreal32		fx, fy, fz;

    splitVoxelCoordAxis<AXIS2D>(pos, x, y, z, fx, fy, fz);

    lerpVoxelMinMaxAxis<AXIS2D>(lerp, lmin, lmax, x, y, z, fx, fy, fz);
}


template <typename T>
void
UT_VoxelArray<T>::lerpVoxelMinMax(
			    T &lerp, T &smin, T &smax,
			    int x, int y, int z,
			    float fx, float fy, float fz) const
{
    T		samples[8];

    if (extractSample(x, y, z, samples))
    {
	lerp = smin = smax = samples[0];
	return;
    }

    lerp = lerpSample(samples, fx, fy, fz);

    T			smin1, smax1;

    SYSminmax(samples[0], samples[1], samples[2], samples[3],
		smin, smax);
    SYSminmax(samples[4+0], samples[4+1], samples[4+2], samples[4+3],
		smin1, smax1);

    smin = SYSmin(smin, smin1);
    smax = SYSmax(smax, smax1);
}

template <typename T>
template <int AXIS2D>
void
UT_VoxelArray<T>::lerpVoxelMinMaxAxis(
			    T &lerp, T &smin, T &smax,
			    int x, int y, int z,
			    float fx, float fy, float fz) const
{
    T		samples[8];

    if (extractSampleAxis<AXIS2D>(x, y, z, samples))
    {
	lerp = smin = smax = samples[0];
	return;
    }

    lerp = lerpSampleAxis<AXIS2D>(samples, fx, fy, fz);

    if (AXIS2D == 0)
	SYSminmax(samples[0], samples[2], samples[4], samples[6],
		    smin, smax);
    else if (AXIS2D == 1)
	SYSminmax(samples[0], samples[1], samples[4], samples[5],
		    smin, smax);
    else if (AXIS2D == 2)
	SYSminmax(samples[0], samples[1], samples[2], samples[3],
		    smin, smax);
    else
    {
	T			smin1, smax1;

	SYSminmax(samples[0], samples[1], samples[2], samples[3],
		    smin, smax);
	SYSminmax(samples[4+0], samples[4+1], samples[4+2], samples[4+3],
		    smin1, smax1);

	smin = SYSmin(smin, smin1);
	smax = SYSmax(smax, smax1);
    }
}

template <typename T>
bool
UT_VoxelArray<T>::extractSample(int x, int y, int z,
				T *samples) const
{
    // Optimization for common cases (values are within the voxel range and
    // are all within the same tile)
    if ( !((x | y | z) < 0) &&
	(((x - myRes[0]+1) & (y - myRes[1]+1) & (z - myRes[2]+1)) < 0))
	
	// (x > 0) && (y > 0) && (z > 0) &&
	// Do not use x+1 < foo as if x is MAX_INT that will falsely
	// report in bounds!
	// (x < myRes[0]-1) && (y < myRes[1]-1) && (z < myRes[2]-1) )
    {
	int			xm, ym, zm;

	xm = x & TILEMASK;
	ym = y & TILEMASK;
	zm = z & TILEMASK;

	if ((xm != TILEMASK) && (ym != TILEMASK) && (zm != TILEMASK))
	{
	    const UT_VoxelTile<T>	*tile =
		getTile(x >> TILEBITS, y >> TILEBITS, z >> TILEBITS);

	    return tile->extractSample(xm, ym, zm, samples);
	}
	else
	{
	    // We cross tile boundaries but we remain within
	    // the voxel grid.  We can thus avoid any bounds checking
	    // and use operator() rather than getValue.
	    samples[0]  = (*this)(x,   y, z);
	    samples[1] =  (*this)(x+1, y, z);
	    samples[2+0] = (*this)(x,   y+1, z);
	    samples[2+1] = (*this)(x+1, y+1, z);
	    samples[4+0]  = (*this)(x,   y, z+1);
	    samples[4+1] =  (*this)(x+1, y, z+1);
	    samples[4+2+0] = (*this)(x,   y+1, z+1);
	    samples[4+2+1] = (*this)(x+1, y+1, z+1);
	}
    }
    else
    {
	samples[0]  = getValue(x,   y, z);
	samples[1] =  getValue(x+1, y, z);
	samples[2+0] = getValue(x,   y+1, z);
	samples[2+1] = getValue(x+1, y+1, z);
	samples[4+0]  = getValue(x,   y, z+1);
	samples[4+1] =  getValue(x+1, y, z+1);
	samples[4+2+0] = getValue(x,   y+1, z+1);
	samples[4+2+1] = getValue(x+1, y+1, z+1);
    }

    return false;
}


template <typename T>
template <int AXIS2D>
bool
UT_VoxelArray<T>::extractSampleAxis(int x, int y, int z,
				T *samples) const
{
    // Optimization for common cases (values are within the voxel range and
    // are all within the same tile)
    int			lesscomp = 0, greatercomp = -1;

    if (AXIS2D != 0)
    {
	lesscomp |= x;
	greatercomp &= (x - myRes[0]+1);
    }
    if (AXIS2D != 1)
    {
	lesscomp |= y;
	greatercomp &= (y - myRes[1]+1);
    }
    if (AXIS2D != 2)
    {
	lesscomp |= z;
	greatercomp &= (z - myRes[2]+1);
    }

    // Optimization for common cases (values are within the voxel range and
    // are all within the same tile)
    if ( !(lesscomp < 0) && (greatercomp < 0) )
    {
	int			xm, ym, zm;

	xm = x & TILEMASK;
	ym = y & TILEMASK;
	zm = z & TILEMASK;

	if ((AXIS2D == 0 || xm != TILEMASK) && 
	    (AXIS2D == 1 || ym != TILEMASK) && 
	    (AXIS2D == 2 || zm != TILEMASK))
	{
	    const UT_VoxelTile<T>	*tile =
		getTile( (AXIS2D == 0) ? 0 : (x >> TILEBITS), 
			 (AXIS2D == 1) ? 0 : (y >> TILEBITS), 
			 (AXIS2D == 2) ? 0 : (z >> TILEBITS) );

	    return tile->template extractSampleAxis<AXIS2D>(xm, ym, zm, samples);
	}
	else
	{
	    // We cross tile boundaries but we remain within
	    // the voxel grid.  We can thus avoid any bounds checking
	    // and use operator() rather than getValue.
	    samples[0]  = (*this)(x,   y, z);
	    if (AXIS2D != 0)
		samples[1] =  (*this)(x+1, y, z);
	    if (AXIS2D != 1)
	    {
		samples[2+0] = (*this)(x,   y+1, z);
		if (AXIS2D != 0)
		    samples[2+1] = (*this)(x+1, y+1, z);
	    }
	    if (AXIS2D != 2)
	    {
		samples[4+0]  = (*this)(x,   y, z+1);
		if (AXIS2D != 0)
		    samples[4+1] =  (*this)(x+1, y, z+1);
		if (AXIS2D != 1)
		{
		    samples[4+2+0] = (*this)(x,   y+1, z+1);
		    if (AXIS2D != 0)
			samples[4+2+1] = (*this)(x+1, y+1, z+1);
		}
	    }
	}
    }
    else
    {
	samples[0]  = getValue(x,   y, z);
	if (AXIS2D != 0)
	    samples[1] =  getValue(x+1, y, z);
	if (AXIS2D != 1)
	{
	    samples[2+0] = getValue(x,   y+1, z);
	    if (AXIS2D != 0)
		samples[2+1] = getValue(x+1, y+1, z);
	}
	if (AXIS2D != 2)
	{
	    samples[4+0]  = getValue(x,   y, z+1);
	    if (AXIS2D != 0)
		samples[4+1] =  getValue(x+1, y, z+1);
	    if (AXIS2D != 1)
	    {
		samples[4+2+0] = getValue(x,   y+1, z+1);
		if (AXIS2D != 0)
		    samples[4+2+1] = getValue(x+1, y+1, z+1);
	    }
	}
    }

    return false;
}

template <typename T>
bool
UT_VoxelArray<T>::extractSamplePlus(int x, int y, int z,
				T *samples) const
{
    // Optimization for common cases (values are within the voxel range and
    // are all within the same tile)
    if ( !(((x-1) | (y-1) | (z-1)) < 0) &&
	(((x - myRes[0]+1) & (y - myRes[1]+1) & (z - myRes[2]+1)) < 0))
	
	// (x > 0) && (y > 0) && (z > 0) &&
	// Do not use x+1 < foo as if x is MAX_INT that will falsely
	// report in bounds!
	// (x < myRes[0]-1) && (y < myRes[1]-1) && (z < myRes[2]-1) )
    {
	int			xm, ym, zm;

	xm = x & TILEMASK;
	ym = y & TILEMASK;
	zm = z & TILEMASK;

	if (xm && ym && zm && (xm != TILEMASK) && (ym != TILEMASK) && (zm != TILEMASK))
	{
	    const UT_VoxelTile<T>	*tile =
		getTile(x >> TILEBITS, y >> TILEBITS, z >> TILEBITS);

	    return tile->extractSamplePlus(xm, ym, zm, samples);
	}
	else
	{
	    // We cross tile boundaries but we remain within
	    // the voxel grid.  We can thus avoid any bounds checking
	    // and use operator() rather than getValue.
	    samples[0]   = (*this)(x-1, y,   z);
	    samples[1]   = (*this)(x+1, y,   z);
	    samples[2+0] = (*this)(x,   y-1, z);
	    samples[2+1] = (*this)(x,   y+1, z);
	    samples[4+0] = (*this)(x,   y,   z-1);
	    samples[4+1] = (*this)(x,   y,   z+1);
	    samples[6]   = (*this)(x,   y,   z);
	}
    }
    else
    {
	samples[0]   = getValue(x-1, y,   z);
	samples[1]   = getValue(x+1, y,   z);
	samples[2+0] = getValue(x,   y-1, z);
	samples[2+1] = getValue(x,   y+1, z);
	samples[4+0] = getValue(x,   y,   z-1);
	samples[4+1] = getValue(x,   y,   z+1);
	samples[6]   = getValue(x,   y,   z);
    }

    return false;
}

template <typename T>
bool
UT_VoxelArray<T>::extractSampleCube(int x, int y, int z,
				T *samples) const
{
    // Optimization for common cases (values are within the voxel range and
    // are all within the same tile)
    if ( !(((x-1) | (y-1) | (z-1)) < 0) &&
	(((x - myRes[0]+1) & (y - myRes[1]+1) & (z - myRes[2]+1)) < 0))
	
	// (x > 0) && (y > 0) && (z > 0) &&
	// Do not use x+1 < foo as if x is MAX_INT that will falsely
	// report in bounds!
	// (x < myRes[0]-1) && (y < myRes[1]-1) && (z < myRes[2]-1) )
    {
	int			xm, ym, zm;

	xm = x & TILEMASK;
	ym = y & TILEMASK;
	zm = z & TILEMASK;

	if (xm && ym && zm && (xm != TILEMASK) && (ym != TILEMASK) && (zm != TILEMASK))
	{
	    const UT_VoxelTile<T>	*tile =
		getTile(x >> TILEBITS, y >> TILEBITS, z >> TILEBITS);

	    return tile->extractSampleCube(xm, ym, zm, samples);
	}
	else
	{
	    // We cross tile boundaries but we remain within
	    // the voxel grid.  We can thus avoid any bounds checking
	    // and use operator() rather than getValue.
	    int			sampidx = 0;
	    for (int dz = -1; dz <= 1; dz++)
	    {
		for (int dy = -1; dy <= 1; dy++)
		{
		    for (int dx = -1; dx <= 1; dx++)
		    {
			samples[sampidx++]   = (*this)(x+dx, y+dy, z+dz);
		    }
		}
	    }
	}
    }
    else
    {
	int			sampidx = 0;
	for (int dz = -1; dz <= 1; dz++)
	{
	    for (int dy = -1; dy <= 1; dy++)
	    {
		for (int dx = -1; dx <= 1; dx++)
		{
		    samples[sampidx++]   = getValue(x+dx, y+dy, z+dz);
		}
	    }
	}
    }

    return false;
}
template <typename T>
T
UT_VoxelArray<T>::lerpSample(T *samples,
			    float fx, float fy, float fz) const
{
#if 1
    // Do trilinear interpolation.
    T			vx, vx1, vy, vy1, vz;

    // Lerp x:x+1, y, z
    vx = UT_VoxelTile<T>::lerpValues(samples[0],
				     samples[1],
				     fx);
    // Lerp x:x+1, y+1, z
    vx1= UT_VoxelTile<T>::lerpValues(samples[2],
				     samples[2+1],
				     fx);
    // Lerp x:x+1, y:y+1, z
    vy = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
    
    // Lerp x:x+1, y, z+1
    vx = UT_VoxelTile<T>::lerpValues(samples[4],
				     samples[4+1],
				     fx);
    // Lerp x:x+1, y+1, z+1
    vx1= UT_VoxelTile<T>::lerpValues(samples[4+2],
				     samples[4+2+1],
				     fx);

    // Lerp x:x+1, y:y+1, z+1
    vy1 = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);

    // Lerp x:x+1, y:y+1, z:z+1
    vz = UT_VoxelTile<T>::lerpValues(vy, vy1, fz);

    return vz;
#else
    v4uf		a, b, vfx, vfy, vfz;

    a = v4uf(&samples[0]);
    b = v4uf(&samples[4]);

    vfx = v4uf(fx);
    vfy = v4uf(fy);
    vfz = v4uf(fz);

    b -= a;
    a = madd(b, fz, a);

    b = a.swizzle<2, 3, 0, 1>();
    b -= a;
    a = madd(b, fy, a);

    b = a.swizzle<1, 2, 3, 0>();
    b -= a;
    a = madd(b, fx, a);

    return a[0];
#endif
}

template <typename T>
template <int AXIS2D>
T
UT_VoxelArray<T>::lerpSampleAxis(T *samples,
			    float fx, float fy, float fz) const
{
    // Do trilinear interpolation.
    T			vx, vx1, vy, vy1, vz;

    // Lerp x:x+1, y, z
    if (AXIS2D != 0)
	vx = UT_VoxelTile<T>::lerpValues(samples[0], samples[1], fx);
    else
	vx = samples[0];

    if (AXIS2D != 1)
    {
	// Lerp x:x+1, y+1, z
	if (AXIS2D != 0)
	    vx1= UT_VoxelTile<T>::lerpValues(samples[2], samples[2+1], fx);
	else
	    vx1= samples[2];

	// Lerp x:x+1, y:y+1, z
	vy = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
    }
    else
	vy = vx;
    
    if (AXIS2D != 2)
    {
	// Lerp x:x+1, y, z+1
	if (AXIS2D != 0)
	    vx = UT_VoxelTile<T>::lerpValues(samples[4], samples[4+1], fx);
	else
	    vx = samples[4];

	if (AXIS2D != 1)
	{
	    // Lerp x:x+1, y+1, z+1
	    if (AXIS2D != 0)
		vx1= UT_VoxelTile<T>::lerpValues(samples[4+2], samples[4+2+1], fx);
	    else
		vx1= samples[4+2];

	    // Lerp x:x+1, y:y+1, z+1
	    vy1 = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
	}
	else
	    vy1 = vx;

	// Lerp x:x+1, y:y+1, z:z+1
	vz = UT_VoxelTile<T>::lerpValues(vy, vy1, fz);
    }
    else
	vz = vy;

    return vz;
}

template <typename T>
T
UT_VoxelArray<T>::operator()(UT_Vector3D pos) const
{
    int			x, y, z;
    // Yes, these have to be 32 becaues split float requires 32!
    fpreal32		fx, fy, fz;

    // We go from the position in the unit cube into the index.
    // The center of cells must map to the exact integer indices.
    pos.x() *= myRes[0];
    pos.y() *= myRes[1];
    pos.z() *= myRes[2];
    pos.x() -= 0.5;
    pos.y() -= 0.5;
    pos.z() -= 0.5;

    // Determine integer & fractional components.
    fx = pos.x();
    SYSfastSplitFloat(fx, x);
    fy = pos.y();
    SYSfastSplitFloat(fy, y);
    fz = pos.z();
    SYSfastSplitFloat(fz, z);

    // Do trilinear interpolation.
    T			vx, vx1, vy, vy1, vz;

    // NOTE:
    // If you get a crash reading one of these getValues, note
    // that if you are reading from the same voxel array that you
    // are writing to, and doing so in a multi-threaded, tile-by-tile
    // approach, just because your positions match doesn't mean you
    // won't access neighbouring tiles.  To avoid this, perform
    // the obvious optimization of checking for aligned tiles
    // and using direct indices.
    // (I have avoided using if (!fx && !fy && !fz) as the test
    // as that is both potentially inaccurate (round off in conversion
    // may mess us up) and said optimzation is a useful one anyways)

    // Optimization for common cases (values are within the voxel range and
    // are all within the same tile)
    if ( !((x | y | z) < 0) &&
	(((x - myRes[0]+1) & (y - myRes[1]+1) & (z - myRes[2]+1)) < 0))
    // if ( (x > 0) && (y > 0) && (z > 0) &&
	// Do not use x+1 < foo as if x is MAX_INT that will falsely
	// report in bounds!
	// (x < myRes[0]-1) && (y < myRes[1]-1) && (z < myRes[2]-1) )
    {
	int			xm, ym, zm;

	xm = x & TILEMASK;
	ym = y & TILEMASK;
	zm = z & TILEMASK;

	if ((xm != TILEMASK) && (ym != TILEMASK) && (zm != TILEMASK))
	{
	    const UT_VoxelTile<T>	*tile =
		getTile(x >> TILEBITS, y >> TILEBITS, z >> TILEBITS);

	    vz = tile->lerp(xm, ym, zm, fx, fy, fz);
	}
	else
	{
	    // We cross tile boundaries but we remain within
	    // the voxel grid.  We can thus avoid any bounds checking
	    // and use operator() rather than getValue.

	    // Lerp x:x+1, y, z
	    vx = UT_VoxelTile<T>::lerpValues((*this)(x,   y, z),
					     (*this)(x+1, y, z),
					     fx);
	    // Lerp x:x+1, y+1, z
	    vx1= UT_VoxelTile<T>::lerpValues((*this)(x,   y+1, z),
					     (*this)(x+1, y+1, z),
					     fx);
	    // Lerp x:x+1, y:y+1, z
	    vy = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
	    
	    // Lerp x:x+1, y, z+1
	    vx = UT_VoxelTile<T>::lerpValues((*this)(x,   y, z+1),
					     (*this)(x+1, y, z+1),
					     fx);
	    // Lerp x:x+1, y+1, z+1
	    vx1= UT_VoxelTile<T>::lerpValues((*this)(x,   y+1, z+1),
					     (*this)(x+1, y+1, z+1),
					     fx);

	    // Lerp x:x+1, y:y+1, z+1
	    vy1 = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);

	    // Lerp x:x+1, y:y+1, z:z+1
	    vz = UT_VoxelTile<T>::lerpValues(vy, vy1, fz);
	}
    }
    else
    {
	// Lerp x:x+1, y, z
	vx = UT_VoxelTile<T>::lerpValues(getValue(x,   y, z),
					 getValue(x+1, y, z),
					 fx);
	// Lerp x:x+1, y+1, z
	vx1= UT_VoxelTile<T>::lerpValues(getValue(x,   y+1, z),
					 getValue(x+1, y+1, z),
					 fx);
	
	// Lerp x:x+1, y:y+1, z
	vy = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);
	
	// Lerp x:x+1, y, z+1
	vx = UT_VoxelTile<T>::lerpValues(getValue(x,   y, z+1),
					 getValue(x+1, y, z+1),
					 fx);
	// Lerp x:x+1, y+1, z+1
	vx1= UT_VoxelTile<T>::lerpValues(getValue(x,   y+1, z+1),
					 getValue(x+1, y+1, z+1),
					 fx);

	// Lerp x:x+1, y:y+1, z+1
	vy1 = UT_VoxelTile<T>::lerpValues(vx, vx1, fy);

	// Lerp x:x+1, y:y+1, z:z+1
	vz = UT_VoxelTile<T>::lerpValues(vy, vy1, fz);
    }

    return vz;
}

#if 0
template <typename T>
T
UT_VoxelArray<T>::operator()(v4uf pos) const
{
    int			x, y, z;

    v4ui		idx;

    // We go from the position in the unit cube into the index.
    // The center of cells must map to the exact integer indices.
    pos *= v4uf((float) myRes[0], (float) myRes[1], (float) myRes[2], 0.0f);

    // Because we use truncation we'll get inaccurate results
    // at the zero boundary, we thus bump up by two
    pos += 1.5;

    idx = pos.splitFloat();

    // And counteract that bump
    idx -= 2;

    x = idx[0];
    y = idx[1];
    z = idx[2];

    // Do trilinear interpolation.
    //	    A      |        B
    // 0 +z +y +yz | +x +xz +xy +xyz

    v4uf		a, b, fx, fy, fz;

    int			xm, ym, zm;

    xm = x & TILEMASK;
    ym = y & TILEMASK;
    zm = z & TILEMASK;

    // Optimization for common case (values are within the voxel range and
    // are all within the same tile)
    if ( (x > 0) & (y > 0) & (z > 0) &
	 (x < myRes[0]-1) & (y < myRes[1]-1) & (z < myRes[2]-1) &
	 (xm != TILEMASK) & (ym != TILEMASK) & (zm != TILEMASK) )
/*
    if (isValidIndex(x, y, z) && isValidIndex(x+1, y+1, z+1) &&
	    (x & TILEMASK) != TILEMASK &&
	    (y & TILEMASK) != TILEMASK &&
	    (z & TILEMASK) != TILEMASK)
*/
    {
	const UT_VoxelTile<T>	*tile =
	    getTile(x >> TILEBITS, y >> TILEBITS, z >> TILEBITS);

	return tile->lerp(pos, xm, ym, zm);
    }
    else
    {
	a = v4uf( getValue(x, y, z),
		  getValue(x, y, z+1),
		  getValue(x, y+1, z),
		  getValue(x, y+1, z+1) );
	b = v4uf( getValue(x+1, y, z),
		  getValue(x+1, y, z+1),
		  getValue(x+1, y+1, z),
		  getValue(x+1, y+1, z+1) );
    }

    fx = pos.swizzle<0, 0, 0, 0>();
    fy = pos.swizzle<1, 1, 1, 1>();
    fz = pos.swizzle<2, 2, 2, 2>();

    b -= a;
    a = madd(b, fx, a);

    b = a.swizzle<2, 3, 0, 1>();
    b -= a;
    a = madd(b, fy, a);

    b = a.swizzle<1, 2, 3, 0>();
    b -= a;
    a = madd(b, fz, a);

    return a[0];
}
#endif

static inline int
firstTile(int &start, int &end, int res)
{
    if (start < 0)
    {
	start += res;
	end += res;
    }
    return start;
}

static inline int
nextTile(int &tile, int &pstart, int &start, int &end, int res)
{
    int		pend;

    if (pstart >= res)
    {
	pstart -= res;
	start -= res;
	end -= res;
    }
    tile = pstart >> TILEBITS;
    pend = SYSmin((tile+1) * TILESIZE, end, res);

    UT_ASSERT(pstart >= 0 && pstart < res);
    UT_ASSERT(pend > 0 && pstart <= res);
    UT_ASSERT(pend-pstart > 0);
    UT_ASSERT(pend-pstart <= TILESIZE);

    return pend;
}

template <typename T>
T
UT_VoxelArray<T>::evaluate(const UT_Vector3 &pos, const UT_Filter &filter,
			   fpreal radius, int clampaxis) const
{
    UT_Vector3		 tpos;
    UT_FilterWindow	 win[3];
    UT_FilterWrap	 wrap[3];
    UT_VoxelTile<T>	*tile;
    const float		*weights[3];
    fpreal		 visible;
    int			 start[3], end[3], size[3];
    int			 pstart[3], pend[3];
    int			 tx, ty, tz, i;
    T			 result;

    if (!myTiles)
    {
	// If the array is empty, just use the border value.
	return myBorderValue;
    }

    UT_FilterWrap	basewrap;
    switch (myBorderType)
    {
	case UT_VOXELBORDER_CONSTANT:	basewrap = UT_WRAP_BORDER; break;
	case UT_VOXELBORDER_REPEAT:	basewrap = UT_WRAP_REPEAT; break;
	case UT_VOXELBORDER_STREAK:	basewrap = UT_WRAP_CLAMP; break;
	
	// NOTE: This is incorrect!
	case UT_VOXELBORDER_EXTRAP:	basewrap = UT_WRAP_CLAMP; break;
	default:			basewrap = UT_WRAP_CLAMP; break;
    }
    for (i = 0; i < 3; i++)
    {
	if (i == clampaxis)
	    wrap[i] = UT_WRAP_CLAMP;
	else
	    wrap[i] = basewrap;
    }
    
    memset(&result, 0, sizeof(result));

    radius *= filter.getSupport();

    // Make a local copy of the position so that we can modify it.
    tpos = pos;
    visible = 1;
    for (i = 0; i < 3; i++)
    {
	tpos[i] = tpos[i]*myRes[i];
	if (!win[i].setWeights(filter, tpos[i], radius, myRes[i], wrap[i]))
	    return result;

	weights[i] = win[i].getWeights();
	start[i] = win[i].getStart() % myRes[i];
	size[i] = win[i].getSize();

	UT_ASSERT(start[i] >= 0);
	UT_ASSERT(size[i] <= myRes[i]);

	end[i] = start[i] + size[i];
	visible *= win[i].getVisible();
    }

    // Accumulate filtered results
    pstart[2] = firstTile(start[2], end[2], myRes[2]);
    while (pstart[2] < end[2])
    {
	pend[2] = nextTile(tz, pstart[2], start[2], end[2], myRes[2]);
	pstart[1] = firstTile(start[1], end[1], myRes[1]);
	while (pstart[1] < end[1])
	{
	    pend[1] = nextTile(ty, pstart[1], start[1], end[1], myRes[1]);
	    pstart[0] = firstTile(start[0], end[0], myRes[0]);
	    while (pstart[0] < end[0])
	    {
		pend[0] = nextTile(tx, pstart[0], start[0], end[0], myRes[0]);
		tile = getTile(tx, ty, tz);
		UT_ASSERT(tile);
		tile->weightedSum(pstart, pend, weights, start, result);
		pstart[0] = pend[0];
	    }
	    pstart[1] = pend[1];
	}
	pstart[2] = pend[2];
    }

    if (visible < 1)
    {
	result += (1-visible)*myBorderValue;
    }

    return result;
}

template <typename T>
void
UT_VoxelArray<T>::resample(const UT_VoxelArray<T> &src,
			  UT_FilterType filtertype,
			  float filterwidthscale,
			  int clampaxis)
{
    fpreal		 radius;
    UT_Filter		*filter;

    filter = UT_Filter::getFilter(filtertype);

    radius = SYSmax( src.getXRes() / (fpreal)getXRes(),
		     src.getYRes() / (fpreal)getYRes(),
		     src.getZRes() / (fpreal)getZRes(),
		     1.0f );
    radius *= 0.5 * filterwidthscale;

    resamplethread(src, filter, radius, clampaxis);

    UT_Filter::releaseFilter(filter);
}

template <typename T>
void
UT_VoxelArray<T>::flattenPartial(T *flatarray, exint ystride, exint zstride,
				const UT_JobInfo &info) const
{
    UT_VoxelArrayIterator<T>	vit;

    // Const cast.
    vit.setArray((UT_VoxelArray<T> *)this);
    vit.splitByTile(info);

    for (vit.rewind(); !vit.atEnd(); vit.advance())
    {
	flatarray[vit.x() + vit.y()*ystride + vit.z()*zstride] = vit.getValue();
    }
}



template <>
inline void
UT_VoxelArray<UT_Vector4F>::flattenGLFixed8Partial(uint8 *flatarray,
						   exint ystride, exint zstride,
						   UT_Vector4F,
						   const UT_JobInfo &info) const
{
    UT_VoxelArrayIterator<UT_Vector4F>	vit;

    // Const cast.
    vit.setArray(SYSconst_cast(this));
    vit.splitByTile(info);

    for (vit.rewind(); !vit.atEnd(); vit.advance())
    {
	UT_Vector4F v = vit.getValue();
	int idx = (vit.x() + vit.y()*ystride + vit.z()*zstride) * 4;
	
	flatarray[idx]   = (uint8) SYSclamp(v.x() * 255.0f, 0.0f, 255.0f);
	flatarray[idx+1] = (uint8) SYSclamp(v.y() * 255.0f, 0.0f, 255.0f);
	flatarray[idx+2] = (uint8) SYSclamp(v.z() * 255.0f, 0.0f, 255.0f);
	flatarray[idx+3] = (uint8) SYSclamp(v.w() * 255.0f, 0.0f, 255.0f);
    }
}

template <typename T>
void
UT_VoxelArray<T>::flattenGLFixed8Partial(uint8 *flatarray,
					 exint ystride, exint zstride,
					 T, const UT_JobInfo &info) const
{
    UT_ASSERT(!"This template requires specific instantiations.");
}

template <>
inline void
UT_VoxelArray<UT_Vector4F>::flattenGL16FPartial(UT_Vector4H *flatarray,
						exint ystride, exint zstride,
						UT_Vector4F,
						const UT_JobInfo &info) const
{
    UT_VoxelArrayIterator<UT_Vector4F>	vit;

    // Const cast.
    vit.setArray(SYSconst_cast(this));
    vit.splitByTile(info);

    for (vit.rewind(); !vit.atEnd(); vit.advance())
	flatarray[vit.x() + vit.y()*ystride + vit.z()*zstride] = vit.getValue();
}

template <typename T>
void
UT_VoxelArray<T>::flattenGL16FPartial(UT_Vector4H *flatarray,
				      exint ystride, exint zstride,
				      T, const UT_JobInfo &info) const
{
    UT_ASSERT(!"This template requires specific instantiations.");
}

template <>
inline void
UT_VoxelArray<UT_Vector4F>::flattenGL32FPartial(UT_Vector4F *flatarray,
						exint ystride, exint zstride,
						UT_Vector4F,
						const UT_JobInfo &info) const
{
    UT_VoxelArrayIterator<UT_Vector4F>	vit;

    // Const cast.
    vit.setArray(SYSconst_cast(this));
    vit.splitByTile(info);

    for (vit.rewind(); !vit.atEnd(); vit.advance())
    {
	UT_Vector4F v = vit.getValue();

	// NOTE: This works around an Nvidia driver bug on OSX, and older
	//       Nvidia drivers on other platforms. The problem was that very
	//	 small FP values were causing huge random values to appear in
	//	 the 3D Texture.
	if(SYSabs(v.x()) < 1e-9)
	   v.x() = 0.0;
	if(SYSabs(v.y()) < 1e-9)
	   v.y() = 0.0;
	if(SYSabs(v.z()) < 1e-9)
	   v.z() = 0.0;
	if(SYSabs(v.w()) < 1e-9)
	   v.w() = 0.0;

	flatarray[vit.x() + vit.y()*ystride + vit.z()*zstride] = v;
    }
}

template <typename T>
void
UT_VoxelArray<T>::flattenGL32FPartial(UT_Vector4F *flatarray,
				      exint ystride, exint zstride,
				      T,const UT_JobInfo &info) const
{
    UT_ASSERT(!"This template requires specific instantiations.");
}


template <typename T>
void
UT_VoxelArray<T>::extractFromFlattenedPartial(const T *flatarray, 
				exint ystride, exint zstride,
				const UT_JobInfo &info)
{
    UT_VoxelArrayIterator<T>	vit;

    vit.setArray(this);
    vit.splitByTile(info);
    vit.setCompressOnExit(true);

    for (vit.rewind(); !vit.atEnd(); vit.advance())
    {
	vit.setValue(flatarray[vit.x() + vit.y()*ystride + vit.z()*zstride]);
    }
}

template <typename T>
template <typename S>
S *
UT_VoxelArray<T>::extractTiles(S *dstdata, int stride,
			const UT_IntArray &tilelist) const
{
    for (int i = 0; i < tilelist.entries(); i++)
    {
	UT_ASSERT(tilelist(i) >= 0 && tilelist(i) < numTiles());
	const UT_VoxelTile<T> *tile = getLinearTile(tilelist(i));

	tile->flatten(dstdata, stride);
	dstdata += tile->numVoxels() * stride;
    }
    return dstdata;
}

template <typename T>
template <typename S>
const S *
UT_VoxelArray<T>::writeTiles(const S *srcdata, int stride,
			const UT_IntArray &tilelist)
{
    for (int i = 0; i < tilelist.entries(); i++)
    {
	UT_ASSERT(tilelist(i) >= 0 && tilelist(i) < numTiles());
	UT_VoxelTile<T> *tile = getLinearTile(tilelist(i));

	tile->writeData(srcdata, stride);
	srcdata += tile->numVoxels() * stride;
    }
    return srcdata;
}

template <typename T>
void
UT_VoxelArray<T>::copyWithOffset(const UT_VoxelArray<T> &src,
		int offx, int offy, int offz)
{
    UT_VoxelBorderType		srcborder;
    T				srcborderval;

    srcborder = src.getBorder();
    srcborderval = src.getBorderValue();

    if (srcborder != UT_VOXELBORDER_EXTRAP)
    {
	// These borders may be constant friendly
	T			srcval;

	if (src.isConstant(&srcval))
	{
	    if (srcborder != UT_VOXELBORDER_CONSTANT ||
		SYSisEqual(srcborderval, srcval))
	    {
		// We can trivially make ourselves constant.
		constant(srcval);
		return;
	    }
	}
    }

    copyWithOffsetInternal(src, offx, offy, offz);
}

template <typename T>
void
UT_VoxelArray<T>::copyWithOffsetInternalPartial(const UT_VoxelArray<T> &src,
		int offx, int offy, int offz,
		const UT_JobInfo &info)
{
    UT_VoxelArrayIterator<T>	vit;
    UT_Vector3I			off(offx, offy, offz);

    vit.setArray(this);
    vit.splitByTile(info);
    vit.setCompressOnExit(true);

    // Iterate over all tiles
    for (vit.rewind(); !vit.atEnd(); vit.advanceTile())
    {
	// Check out this tile
	UT_VoxelTile<T>		*tile = vit.getTile();
	UT_VoxelTile<T>		*srctile;
	int			 tx, ty, tz;

	UT_Vector3I		 start, end, srctileidx, srctileoff, srcend, srcendtile;
	vit.getTileVoxels(start, end);

	srctileidx = start;
	srctileidx += off;
	srctileidx.x() >>= TILEBITS;
	srctileidx.y() >>= TILEBITS;
	srctileidx.z() >>= TILEBITS;
	srctileoff.x() = off.x() & TILEMASK;
	srctileoff.y() = off.y() & TILEMASK;
	srctileoff.z() = off.z() & TILEMASK;

	srcend = start;
	// We are very careful here to be inclusive, so we don't trigger
	// the next tile.  This is the largest index we expect to index.
	srcend.x() += tile->xres() - 1;
	srcend.y() += tile->yres() - 1;
	srcend.z() += tile->zres() - 1;
	srcend += off;
	srcendtile = srcend;
	srcendtile.x() >>= TILEBITS;
	srcendtile.y() >>= TILEBITS;
	srcendtile.z() >>= TILEBITS;

	UT_ASSERT(srcendtile.x() == srctileidx.x() ||
		  srcendtile.x() == srctileidx.x()+1);
	UT_ASSERT(srcendtile.y() == srctileidx.y() ||
		  srcendtile.y() == srctileidx.y()+1);
	UT_ASSERT(srcendtile.z() == srctileidx.z() ||
		  srcendtile.z() == srctileidx.z()+1);

	// Check if we are fully in bounds.
	if (srctileidx.x() >= 0 &&
	    srctileidx.y() >= 0 &&
	    srctileidx.z() >= 0 &&
	    srcendtile.x() < src.getTileRes(0) &&
	    srcendtile.y() < src.getTileRes(1) &&
	    srcendtile.z() < src.getTileRes(2) &&
	    srcend.x() < src.getXRes() &&
	    srcend.y() < src.getYRes() &&
	    srcend.z() < src.getZRes())
	{
	    bool		allconst = true, firsttile = true;
	    T			constval, cval;
	    // Check if we are all constant...
	    for (tz = srctileidx.z(); allconst && tz <= srcendtile.z(); tz++)
		for (ty = srctileidx.y(); allconst && ty <= srcendtile.y(); ty++)
		    for (tx = srctileidx.x(); tx <= srcendtile.x(); tx++)
		    {
			srctile = src.getTile(tx, ty, tz);
			if (!srctile->isConstant())
			{
			    allconst = false;
			    break;
			}
			cval = (*srctile)(0, 0, 0);
			if (firsttile)
			{
			    firsttile = false;
			    constval = cval;
			}
			if (!SYSisEqual(cval, constval))
			{
			    allconst = false;
			    break;
			}
		    }
	    if (allconst)
	    {
		// Should have found at least one tile!
		UT_ASSERT(!firsttile);
		tile->makeConstant(constval);

		// Onto the next tile!
		continue;
	    }
	}

	// All of the fragments aren't constant, or aren't all inside
	// our range.  So we have to work fragment at a time..
	for (tz = srctileidx.z(); tz <= srcendtile.z(); tz++)
	    for (ty = srctileidx.y(); ty <= srcendtile.y(); ty++)
		for (tx = srctileidx.x(); tx <= srcendtile.x(); tx++)
		{
		    int		destx, desty, destz;
		    int		srcx, srcy, srcz;

		    destx = (tx == srctileidx.x()) ? 0 : (TILESIZE-srctileoff.x());
		    desty = (ty == srctileidx.y()) ? 0 : (TILESIZE-srctileoff.y());
		    destz = (tz == srctileidx.z()) ? 0 : (TILESIZE-srctileoff.z());
		    srcx = (tx == srctileidx.x()) ? srctileoff.x() : 0;
		    srcy = (ty == srctileidx.y()) ? srctileoff.y() : 0;
		    srcz = (tz == srctileidx.z()) ? srctileoff.z() : 0;
#if 1
		    if (tx >= 0 &&
			ty >= 0 &&
			tz >= 0 &&
			tx < src.getTileRes(0) &&
			ty < src.getTileRes(1) &&
			tz < src.getTileRes(2) &&
			((tx != src.getTileRes(0)-1) ||
			  srcend.x() < src.getXRes()) &&
			((ty != src.getTileRes(1)-1) ||
			  srcend.y() < src.getYRes()) &&
			((tz != src.getTileRes(2)-1) ||
			  srcend.z() < src.getZRes())
			)
		    {
			srctile = src.getTile(tx, ty, tz);
			// In bounds
			tile->copyFragment(
				destx, desty, destz,
				*srctile,
				srcx, srcy, srcz
				);
		    }
		    else
#endif
		    {
			// Out of bounds!
			int		maxd = SYSmin(tile->zres(),
						    TILESIZE-srcz);
			int		maxh = SYSmin(tile->yres(),
						    TILESIZE-srcy);
			int		maxw = SYSmin(tile->xres(),
						    TILESIZE-srcx);
			for (int z = destz; z < maxd; z++)
			{
			    for (int y = desty; y < maxh; y++)
			    {
				for (int x = destx; x < maxw; x++)
				{
				    T		val;

				    val = src.getValue(x + vit.x() + offx,
							 y + vit.y() + offy,
							 z + vit.z() + offz);
				    tile->setValue(x, y, z, val);
				}
			    }
			}
		    }
		}
    }
}

template <typename T>
void
UT_VoxelArray<T>::resamplethreadPartial(const UT_VoxelArray<T> &src,
			const UT_Filter *filter, float radius,
			int clampaxis,
			const UT_JobInfo &info)
{
    UT_VoxelArrayIterator<T>	vit;
    UT_Vector3			pos;
    UT_Vector3			ratio;
    UT_Interrupt		*boss = UTgetInterrupt();

    vit.setArray(this);
    vit.splitByTile(info);
    vit.setCompressOnExit(true);
    vit.setInterrupt(boss);

    ratio.x() = 1.0f / getXRes();
    ratio.y() = 1.0f / getYRes();
    ratio.z() = 1.0f / getZRes();

    for (vit.rewind(); !vit.atEnd(); vit.advance())
    {
	pos.x() = vit.x()+0.5f;
	pos.y() = vit.y()+0.5f;
	pos.z() = vit.z()+0.5f;
	pos *= ratio;

	vit.setValue(src.evaluate(pos, *filter, radius, clampaxis));
    }
}

template <typename T>
bool
UT_VoxelArray<T>::posToIndex(UT_Vector3 pos, int &x, int &y, int &z) const
{
    // We go from the position in the unit cube into the index.
    // The center of cells must map to the exact integer indices.
    pos.x() *= myRes[0];
    pos.y() *= myRes[1];
    pos.z() *= myRes[2];

    // The centers of cells are now mapped .5 too high.  Ie, the true
    // center of cell index (0,0,0) would be (0.5,0.5,0.5)  This, however,
    // is exactly what we want for rounding.
    x = (int) SYSfloor(pos.x());
    y = (int) SYSfloor(pos.y());
    z = (int) SYSfloor(pos.z());

    // Determine if out of bounds.
    return isValidIndex(x, y, z);
}

template <typename T>
bool
UT_VoxelArray<T>::posToIndex(UT_Vector3 pos, UT_Vector3 &ipos) const
{
    // We go from the position in the unit cube into the index.
    // The center of cells must map to the exact integer indices.
    pos.x() *= myRes[0];
    pos.y() *= myRes[1];
    pos.z() *= myRes[2];

    // The centers of cells are now mapped .5 too high.  Ie, the true
    // center of cell index (0,0,0) would be (0.5,0.5,0.5)
    pos.x() -= 0.5;
    pos.y() -= 0.5;
    pos.z() -= 0.5;

    ipos = pos;

    // Determine if out of bounds.
    if (pos.x() < 0 || pos.x() >= myRes[0] ||
	pos.y() < 0 || pos.y() >= myRes[1] ||
	pos.z() < 0 || pos.z() >= myRes[2])
	return false;

    return true;
}

template <typename T>
bool
UT_VoxelArray<T>::indexToPos(int x, int y, int z, UT_Vector3F &pos) const
{
    pos.x() = x;
    pos.y() = y;
    pos.z() = z;

    // Move the indices to the centers of the cells.
    pos.x() += 0.5;
    pos.y() += 0.5;
    pos.z() += 0.5;

    // And scale into the unit cube.
    pos *= myInvRes;

    // Return true if the original coordinates were in range.
    return isValidIndex(x, y, z);
}

template <typename T>
bool
UT_VoxelArray<T>::indexToPos(int x, int y, int z, UT_Vector3D &pos) const
{
    pos.x() = x;
    pos.y() = y;
    pos.z() = z;

    // Move the indices to the centers of the cells.
    pos.x() += 0.5;
    pos.y() += 0.5;
    pos.z() += 0.5;

    // And scale into the unit cube.
    pos *= myInvRes;

    // Return true if the original coordinates were in range.
    return isValidIndex(x, y, z);
}

template <typename T>
void
UT_VoxelArray<T>::findexToPos(UT_Vector3F index, UT_Vector3F &pos) const
{
    pos = index;

    // Move the indices to the centers of the cells.
    pos.x() += 0.5;
    pos.y() += 0.5;
    pos.z() += 0.5;

    // And scale into the unit cube.
    pos *= myInvRes;
}

template <typename T>
void
UT_VoxelArray<T>::findexToPos(UT_Vector3D index, UT_Vector3D &pos) const
{
    pos = index;

    // Move the indices to the centers of the cells.
    pos.x() += 0.5;
    pos.y() += 0.5;
    pos.z() += 0.5;

    // And scale into the unit cube.
    pos *= myInvRes;
}

template <typename T>
void
UT_VoxelArray<T>::setBorder(UT_VoxelBorderType type, T t)
{
    myBorderType = type;
    myBorderValue = t;
}

template <typename T>
void
UT_VoxelArray<T>::setBorderScale(T sx, T sy, T sz)
{
    myBorderScale[0] = sx;
    myBorderScale[1] = sy;
    myBorderScale[2] = sz;
}

template <typename T>
void
UT_VoxelArray<T>::collapseAllTilesPartial(const UT_JobInfo &info)
{
    UT_VoxelArrayIterator<T>    vit(this);
    vit.splitByTile(info);
    for (vit.rewind(); !vit.atEnd(); vit.advanceTile())
    {
        int i = vit.getLinearTileNum();
        myTiles[i].tryCompress(getCompressionOptions());
    }
}

template <typename T>
void
UT_VoxelArray<T>::expandAllTilesPartial(const UT_JobInfo &info)
{
    UT_VoxelArrayIterator<T>    vit(this);
    vit.splitByTile(info);
    for (vit.rewind(); !vit.atEnd(); vit.advanceTile())
    {
        int i = vit.getLinearTileNum();
        myTiles[i].uncompress();
    }
}

template <typename T>
void
UT_VoxelArray<T>::expandAllNonConstTilesPartial(const UT_JobInfo &info)
{
    UT_VoxelArrayIterator<T>    vit(this);
    vit.splitByTile(info);
    for (vit.rewind(); !vit.atEnd(); vit.advanceTile())
    {
        int i = vit.getLinearTileNum();
        if (!myTiles[i].isConstant())
            myTiles[i].uncompress();
    }
}

template <typename T>
void
UT_VoxelArray<T>::saveData(std::ostream &os) const
{
    T		cval;
    char	version;

    // First determine if we are fully constant.
    if (isConstant(&cval))
    {
	// Save a constant array.
	version = 0;
	UTwrite(os, &version, 1);
	UTwrite<T>(os, &cval);
	return;
    }

    // Compressed tiles.
    version = 1;
    UTwrite(os, &version, 1);

    // Store list of compression types
    UT_VoxelTile<T>::saveCompressionTypes(os);

    int		i, ntiles;

    ntiles = numTiles();
    for (i = 0; i < ntiles; i++)
    {
	myTiles[i].save(os);
    }
}


template <typename T>
void
UT_VoxelArray<T>::loadData(UT_IStream &is)
{
    T		cval;
    char	version;

    is.readChar(version);

    // First determine if we are fully constant.
    if (version == 0)
    {
	// Save a constant array.
	is.read<T>(&cval);

	constant(cval);
	return;
    }

    if (version == 1)
    {
	UT_IntArray		compressions;

	// Store list of compression types
	UT_VoxelTile<T>::loadCompressionTypes(is, compressions);

	int		i, ntiles;

	ntiles = numTiles();
	for (i = 0; i < ntiles; i++)
	{
	    myTiles[i].load(is, compressions);
	}
    }
}

template <typename T>
bool
UT_VoxelArray<T>::saveData(UT_JSONWriter &w, const char *shared_mem_owner) const
{
    bool	ok = true;
    T		cval;
    int8	version;

    // First determine if we are fully constant.
    if (isConstant(&cval))
    {
	ok = ok && w.jsonBeginArray();
	// Save a constant array.
	ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::ARRAY_CONSTARRAY));
	ok = ok && w.jsonValue(cval);
	ok = ok && w.jsonEndArray();
	return ok;
    }
    
    if (shared_mem_owner)
    {
	SYS_SharedMemory	*shm;
	shm = copyToSharedMemory(shared_mem_owner);
	if (shm)
	{
	    ok = ok && w.jsonBeginArray();
	    // Save a shared memory array.
	    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					    UT_VoxelArrayJSON::ARRAY_SHAREDARRAY));
	    ok = ok && w.jsonString(shm->id());
	    ok = ok && w.jsonEndArray();
	    return ok;
	}
	// Fall back on raw write.
    }
    

    // Compressed tiles.
    ok = ok && w.jsonBeginArray();
    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::ARRAY_TILEDARRAY));
    ok = ok && w.jsonBeginArray();
    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::ARRAY_VERSION));
    version = 1;
    ok = ok && w.jsonValue(version);

    // Store list of compression types
    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
				UT_VoxelArrayJSON::ARRAY_COMPRESSIONTYPES));
    ok = ok && UT_VoxelTile<T>::saveCompressionTypes(w);

    ok = ok && w.jsonKey(UT_VoxelArrayJSON::getToken(
					UT_VoxelArrayJSON::ARRAY_TILES));
    ok = ok && w.jsonBeginArray();
    int		i, ntiles;

    ntiles = numTiles();
    for (i = 0; i < ntiles; i++)
    {
	ok = ok && myTiles[i].save(w);
    }
    ok = ok && w.jsonEndArray();

    ok = ok && w.jsonEndArray();

    ok = ok && w.jsonEndArray();
    return ok;
}

template <typename T>
bool
UT_VoxelArray<T>::loadData(UT_JSONParser &p)
{
    T			cval;
    int8		version;
    UT_WorkBuffer	key;
    int			key_id;
    bool		array_error = false;

    delete mySharedMemView;
    mySharedMemView = 0;
    
    delete mySharedMem;
    mySharedMem = 0;

    if (!p.parseBeginArray(array_error) || array_error)
	return false;

    if (!p.parseString(key))
	return false;
    key_id = UT_VoxelArrayJSON::getArrayID(key.buffer());
    if (key_id == UT_VoxelArrayJSON::ARRAY_CONSTARRAY)
    {
	if (!p.parseNumber(cval))
	    return false;

	constant(cval);
    }
    else if (key_id == UT_VoxelArrayJSON::ARRAY_SHAREDARRAY)
    {
	UT_WorkBuffer	shm_id;
	if (!p.parseString(shm_id))
	    return false;

	if (!populateFromSharedMemory(shm_id.buffer()))
	{
	    // If the shared memory disappears before we get a chance to read
	    // it, don't abort, just set a constant value. That way Mantra has
	    // a chance to recover. Not the most elegant solution but works for
	    // now.
	    T cval;
	    cval = 0;
	    constant(cval);
	}
    }
    else if (key_id == UT_VoxelArrayJSON::ARRAY_TILEDARRAY)
    {
	UT_JSONParser::traverser	it, tile_it;
	UT_IntArray			compressions;
	int				i, ntiles;


	for (it = p.beginArray(); !it.atEnd(); ++it)
	{
	    if (!it.getLowerKey(key))
		return false;
	    switch (UT_VoxelArrayJSON::getArrayID(key.buffer()))
	    {
		case UT_VoxelArrayJSON::ARRAY_VERSION:
		    if (!p.parseNumber(version))
			return false;
		    break;
		case UT_VoxelArrayJSON::ARRAY_COMPRESSIONTYPES:
		    // Store list of compression types
		    if (!UT_VoxelTile<T>::loadCompressionTypes(p, compressions))
			return false;
		    break;

		case UT_VoxelArrayJSON::ARRAY_TILES:
		    ntiles = numTiles();
		    for (tile_it = p.beginArray(), i = 0; !tile_it.atEnd();
			    ++tile_it, ++i)
		    {
			if (i < ntiles)
			{
			    if (!myTiles[i].load(p, compressions))
				return false;
			}
			else
			{
			    UT_VoxelTile<T>	 dummy_tile;
			    dummy_tile.setRes(TILESIZE, TILESIZE, TILESIZE);
			    if (!dummy_tile.load(p, compressions))
				return false;
			}
		    }
		    if (i != ntiles)
			return false;
		    break;
		default:
		    p.addWarning("Unexpected key for voxel data: %s",
				 key.buffer());
		    if (!p.skipNextObject())
			return false;
	    }
	}
    }
    else
    {
	p.addWarning("Unexpected voxel key: %s\n", key.buffer());
	p.skipNextObject();
	return false;
    }

    if (!p.parseEndArray(array_error) || array_error)
	return false;

    return true;
}

template<typename T>
SYS_SharedMemory *
UT_VoxelArray<T>::copyToSharedMemory(const char *shared_mem_owner) const
{
    UT_SharedMemoryManager	&shmgr = UT_SharedMemoryManager::get();

    int		ntiles;

    ntiles = numTiles();
    
    // Tally up the size we need.
    exint		total_mem_size;

    total_mem_size = sizeof(exint);	// Tile count.

    UT_Array<exint>	tile_sizes;
    UT_Array<exint>	next_block_offsets;

    for (int i = 0; i < ntiles; i++)
    {
	exint	tile_size, block_size;
	if (myTiles[i].isConstant())
	    tile_size = sizeof(T);
	else
	    tile_size = myTiles[i].getDataLength();

	tile_sizes.append(tile_size);
	
	block_size = SYSroundUpToMultipleOf<exint>(tile_size, sizeof(exint));
	block_size += sizeof(exint) * 2;	// Offset + compression type

	if (i < (ntiles-1))
	    next_block_offsets.append(block_size / sizeof(exint));
	else
	    next_block_offsets.append(0);
	
	total_mem_size += block_size;
    }

    // Start with an empty shared memory.
    SYS_SharedMemory	*shared_mem;
    UT_WorkBuffer	 shared_mem_key;
    
    shared_mem_key.sprintf("%s:%p", shared_mem_owner, this);
    
    shared_mem = shmgr.get(shared_mem_key.buffer());

    // Does the existing block have the same number of tiles and same
    // offsets? In that case we don't reset the shared memory, just write
    // the tiles out again. There's very little adverse effect in changing
    // the voxel tile data.
    bool	same_meta_data = false;
    if (shared_mem->size() >= sizeof(exint))
    {
	same_meta_data = true;
	{
	    SYS_SharedMemoryView	sv_tc(*shared_mem, 0, sizeof(exint));

	    exint		*ptr_ds = (exint *)sv_tc.data();
	    if (*ptr_ds != ntiles)
		same_meta_data = false;
	}

	if (same_meta_data)
	{
	    // Check if the offsets + compression type is the same.
	    exint		offset = sizeof(exint);
	    for (int i = 0; i < ntiles; i++)
	    {
		SYS_SharedMemoryView	sv_tile(*shared_mem, offset,
						sizeof(exint) * 2);

		exint	*ptr_tile = (exint *)sv_tile.data();
		if (ptr_tile[0] != next_block_offsets(i) ||
		    ptr_tile[1] != (exint)myTiles[i].myCompressionType)
		{
		    same_meta_data = false;
		    break;
		}
		offset += next_block_offsets(i) * sizeof(exint);
	    }
	}
    }
    
    if (!same_meta_data)
    {
	if (!shared_mem->reset(total_mem_size) ||
	    shared_mem->size() != total_mem_size)
	{
	    return NULL;
	}
    }
    
    {
	SYS_SharedMemoryView	sv_tc(*shared_mem, 0, sizeof(exint));

	exint		*ptr_ds = (exint *)sv_tc.data();
	*ptr_ds = ntiles;
    }

    exint		 offset = sizeof(exint);
    for (int i = 0; i < ntiles; i++)
    {
	SYS_SharedMemoryView	sv_tile(*shared_mem, offset,
					    sizeof(exint) * 2 + tile_sizes(i));

	exint	*ptr_tile = (exint *)sv_tile.data();

	// Offset to next tile in exint units.
	ptr_tile[0] = next_block_offsets(i);
	ptr_tile[1] = myTiles[i].myCompressionType;

	::memcpy(&ptr_tile[2], myTiles[i].rawData(), tile_sizes(i));

	offset += ptr_tile[0] * sizeof(exint);
    }
    
    return shared_mem;
}

template<typename T>
bool
UT_VoxelArray<T>::populateFromSharedMemory(const char *id)
{
    mySharedMem = new SYS_SharedMemory(id, /*read_only=*/true);
    if (!mySharedMem->size())
    {
	delete mySharedMem;
	mySharedMem = 0;
	return false;
    }

    exint		ntiles;
    {
	SYS_SharedMemoryView	sv_tc(*mySharedMem, 0, sizeof(exint));
	ntiles = *(const exint *)sv_tc.data();
    }
    if (ntiles != numTiles())
    {
	delete mySharedMem;
	mySharedMem = 0;
	return false;
    }

    mySharedMemView = new SYS_SharedMemoryView(*mySharedMem, sizeof(exint));

    exint		*data = (exint *)mySharedMemView->data();

    for (int i = 0; i < ntiles; i++)
    {
	exint	offset = data[0];
	int8	ctype = (int8)data[1];

	myTiles[i].setForeignData(&data[2], ctype);

	data += offset;
    }

    return true;
}




//
// UT_VoxelMipMap functions
//
template <typename T>
UT_VoxelMipMap<T>::UT_VoxelMipMap()
{
    initializePrivate();
}

template <typename T>
UT_VoxelMipMap<T>::~UT_VoxelMipMap()
{
    destroyPrivate();
}

template <typename T>
UT_VoxelMipMap<T>::UT_VoxelMipMap(const UT_VoxelMipMap<T> &src)
{
    initializePrivate();

    *this = src;
}

template <typename T>
const UT_VoxelMipMap<T> &
UT_VoxelMipMap<T>::operator=(const UT_VoxelMipMap<T> &src)
{
    UT_VoxelArray<T>	**levels;
    int			 level, i;
    
    if (&src == this)
	return *this;

    destroyPrivate();

    // We do a deep copy.  We have learned from UT_String that
    // shallow copies will destroy us in the end.
    myOwnBase = true;
    myBaseLevel = new UT_VoxelArray<T>;
    *myBaseLevel = *src.myBaseLevel;

    myNumLevels = src.myNumLevels;

    for (i = 0; i < src.myLevels.entries(); i++)
    {
	levels = new UT_VoxelArray<T> *[myNumLevels];
	myLevels.append(levels);
	
	for (level = 0; level < myNumLevels; level++)
	{
	    levels[level] = new UT_VoxelArray<T>;
	    *levels[level] = *src.myLevels(i)[level];
	}
    }
    return *this;
}

template <typename T>
void
UT_VoxelMipMap<T>::build(UT_VoxelArray<T> *baselevel,
			 mipmaptype function)
{
    UT_Array<mipmaptype>	functions;

    functions.append(function);
    build(baselevel, functions);
}

template <typename T>
void
UT_VoxelMipMap<T>::build(UT_VoxelArray<T> *baselevel,
			 const UT_Array<mipmaptype> &functions)
{
    destroyPrivate();

    // Setup our baselevel.
    myBaseLevel = baselevel;
    myOwnBase = false;

    // Now build from bottom up.  First, calculate the number
    // of levels we'll need.
    myNumLevels = 0;
    int			maxres, level;

    maxres = SYSmax(myBaseLevel->getXRes(), myBaseLevel->getYRes());
    maxres = SYSmax(maxres, myBaseLevel->getZRes());

    //cerr << "Max res " << maxres << endl;

    while (maxres > 1)
    {
	myNumLevels++;
	maxres++;
	maxres >>= 1;
    }

    //cerr << "Levels: " << myNumLevels << endl;

    // Create our desired voxel array list.
    for (int i = 0; i < functions.entries(); i++)
    {
	mipmaptype	function = functions(i);

	UT_VoxelArray<T>	**levels = new UT_VoxelArray<T> *[myNumLevels];
	myLevels.append(levels);

	UT_VoxelArray<T>	*lastlevel = myBaseLevel;
	for (level = myNumLevels-1; level >= 0; level--)
	{
	    // Find our resolutions.
	    int xres = (lastlevel->getXRes() + 1) >> 1;
	    int yres = (lastlevel->getYRes() + 1) >> 1;
	    int zres = (lastlevel->getZRes() + 1) >> 1;

	    levels[level] = new UT_VoxelArray<T>;
	    levels[level]->size(xres, yres, zres);

	    downsample(*levels[level], *lastlevel, function);

	    lastlevel = levels[level];    
	}
    }
}

template <typename T>
void
UT_VoxelMipMap<T>::downsamplePartial(
	UT_VoxelArray<T> &dst,
	const UT_VoxelArray<T> &src,
	mipmaptype function,
	const UT_JobInfo &info)
{
    UT_VoxelArrayIterator<T>	vit(&dst);

    vit.splitByTile(info);
    vit.setCompressOnExit(true);

    for (vit.rewind(); !vit.atEnd(); vit.advance())
    {
	int	x = 2*vit.x();
	int	y = 2*vit.y();
	int	z = 2*vit.z();
	T	sam[8];

	if (src.extractSample(x, y, z, sam))
	{
	    vit.setValue(sam[0]);
	}
	else
	{
	    vit.setValue(
		    mixValues(
			mixValues(
			    mixValues(sam[0], sam[1], function),
			    mixValues(sam[2], sam[3], function), function),
			mixValues(
			    mixValues(sam[4], sam[5], function),
			    mixValues(sam[6], sam[7], function), function),
			function));
	}
    }
}


template <typename T>
int64
UT_VoxelMipMap<T>::getMemoryUsage(bool inclusive) const
{
    int64 mem = inclusive ? sizeof(*this) : 0;

    mem += myLevels.getMemoryUsage(false);
    mem += myLevels.entries() * myNumLevels * sizeof(UT_VoxelArray<T>*);
    for (exint j = 0; j < myLevels.entries(); j++)
    {
        for (int i = 0; i < myNumLevels; i++)
            mem += myLevels(j)[i]->getMemoryUsage(true);
    }

    return mem;
}

template <typename T>
void
UT_VoxelMipMap<T>::traverseTopDown(Callback function, void *data) const
{
    doTraverse(0, 0, 0, 0, function, data);
}

template <typename T>
void
UT_VoxelMipMap<T>::doTraverse(int x, int y, int z, int level,
			      Callback function, void *data) const
{
    UT_StackBuffer<T>	 tval(myLevels.entries());
    bool		 isfinal;
    UT_VoxelArray<T>	*vox;
    int			 shift;

    if (level == myNumLevels)
    {
	isfinal = true;
	vox = myBaseLevel;
	tval[0] = (*vox)(x, y, z);
	for (int i = 1; i < myLevels.entries(); i++)
	    tval[i] = tval[0];
    }
    else
    {
	isfinal = false;
	for (int i = 0; i < myLevels.entries(); i++)
	{
	    vox = myLevels(i)[level];
	    tval[i] = (*vox)(x, y, z);
	}
    }

    shift = myNumLevels - level;

    UT_BoundingBox box;

    box.initBounds(SYSmin(x << shift, myBaseLevel->getXRes()),
		   SYSmin(y << shift, myBaseLevel->getYRes()),
		   SYSmin(z << shift, myBaseLevel->getZRes()));
    box.enlargeBounds(SYSmin((x+1) << shift, myBaseLevel->getXRes()),
		      SYSmin((y+1) << shift, myBaseLevel->getYRes()),
		      SYSmin((z+1) << shift, myBaseLevel->getZRes()));

    if (!function(tval, box, isfinal, data))
    {
	// Function asked for an early exit.
	// Give it.
	return;
    }

    // We now want to proceed to the next level.
    if (isfinal)
	return;

    level++;
    shift--;
    x <<= 1;
    y <<= 1;
    z <<= 1;

    bool		xinc, yinc, zinc;

    // Determine which of our children are valid.
    if ( ((x+1) << shift) < myBaseLevel->getXRes() )
	xinc = true;
    else
	xinc = false;
    if ( ((y+1) << shift) < myBaseLevel->getYRes() )
	yinc = true;
    else
	yinc = false;
    if ( ((z+1) << shift) < myBaseLevel->getZRes() )
	zinc = true;
    else
	zinc = false;
    
    //
    // Traverse all valid children.  Note that this is deliberately a gray
    // order traversal for full nodes.
    //

    doTraverse(x, y, z, level, function, data);

    if (yinc)
    {
	doTraverse(x, y+1, z, level, function, data);
	if (zinc)
	    doTraverse(x, y+1, z+1, level, function, data);
    }
    if (zinc)
	doTraverse(x, y, z+1, level, function, data);

    if (xinc)
    {
	if (zinc)
	    doTraverse(x+1, y, z+1, level, function, data);
	doTraverse(x+1, y, z, level, function, data);
	if (yinc)
	{
	    doTraverse(x+1, y+1, z, level, function, data);
	    if (zinc)
		doTraverse(x+1, y+1, z+1, level, function, data);
	}
    }
}

template <typename T>
template <typename OP>
void
UT_VoxelMipMap<T>::traverseTopDown(OP &op) const
{
    doTraverse(0, 0, 0, numLevels()-1, op);
}

template <typename T>
template <typename OP>
void
UT_VoxelMipMap<T>::doTraverse(int x, int y, int z, int level,
			      OP &op) const
{
    int			 shift;

    shift = level;

    UT_BoundingBoxI box;

    box.initBounds(SYSmin(x << shift, myBaseLevel->getXRes()),
		   SYSmin(y << shift, myBaseLevel->getYRes()),
		   SYSmin(z << shift, myBaseLevel->getZRes()));
    box.enlargeBounds(SYSmin((x+1) << shift, myBaseLevel->getXRes()),
		      SYSmin((y+1) << shift, myBaseLevel->getYRes()),
		      SYSmin((z+1) << shift, myBaseLevel->getZRes()));

    if (!op(box, level))
    {
	// Function asked for an early exit.
	// Give it.
	return;
    }

    level--;
    // We now want to proceed to the next level.
    if (level < 0)
	return;

    x <<= 1;
    y <<= 1;
    z <<= 1;

    bool		xinc, yinc, zinc;

    // Determine which of our children are valid.
    if ( ((x+1) << level) < myBaseLevel->getXRes() )
	xinc = true;
    else
	xinc = false;
    if ( ((y+1) << level) < myBaseLevel->getYRes() )
	yinc = true;
    else
	yinc = false;
    if ( ((z+1) << level) < myBaseLevel->getZRes() )
	zinc = true;
    else
	zinc = false;
    
    //
    // Traverse all valid children.  Note that this is deliberately a gray
    // order traversal for full nodes.
    //

    doTraverse(x, y, z, level, op);

    if (yinc)
    {
	doTraverse(x, y+1, z, level, op);
	if (zinc)
	    doTraverse(x, y+1, z+1, level, op);
    }
    if (zinc)
	doTraverse(x, y, z+1, level, op);

    if (xinc)
    {
	if (zinc)
	    doTraverse(x+1, y, z+1, level, op);
	doTraverse(x+1, y, z, level, op);
	if (yinc)
	{
	    doTraverse(x+1, y+1, z, level, op);
	    if (zinc)
		doTraverse(x+1, y+1, z+1, level, op);
	}
    }
}

template <typename T>
template <typename OP>
void
UT_VoxelMipMap<T>::traverseTopDownSorted(OP &op) const
{
    doTraverseSorted(0, 0, 0, numLevels()-1, op);
}

class ut_VoxelMipMapSortCompare
{
public:
    float		value;
    int			key;
};

inline bool operator<(const ut_VoxelMipMapSortCompare &lhs, const ut_VoxelMipMapSortCompare &rhs)
{
    return lhs.value < rhs.value;
}

template <typename T>
template <typename OP>
void
UT_VoxelMipMap<T>::doTraverseSorted(int x, int y, int z, int level,
			      OP &op) const
{
    UT_BoundingBoxI box;

    box.initBounds(SYSmin(x << level, myBaseLevel->getXRes()),
		   SYSmin(y << level, myBaseLevel->getYRes()),
		   SYSmin(z << level, myBaseLevel->getZRes()));
    box.enlargeBounds(SYSmin((x+1) << level, myBaseLevel->getXRes()),
		      SYSmin((y+1) << level, myBaseLevel->getYRes()),
		      SYSmin((z+1) << level, myBaseLevel->getZRes()));

    if (!op(box, level))
    {
	// Function asked for an early exit.
	// Give it.
	return;
    }

    level--;
    // We now want to proceed to the next level.
    if (level < 0)
	return;

    x <<= 1;
    y <<= 1;
    z <<= 1;

    bool		xinc, yinc, zinc;

    // Determine which of our children are valid.
    if ( ((x+1) << level) < myBaseLevel->getXRes() )
	xinc = true;
    else
	xinc = false;
    if ( ((y+1) << level) < myBaseLevel->getYRes() )
	yinc = true;
    else
	yinc = false;
    if ( ((z+1) << level) < myBaseLevel->getZRes() )
	zinc = true;
    else
	zinc = false;
    
    UT_BoundingBoxI		boxes[8];
    int				numboxes = 0;

    // Build all of our bounding boxes.
    for (int dz = 0; dz < 2; dz++)
    {
	if (dz && !zinc)
	    continue;
	for (int dy = 0; dy < 2; dy++)
	{
	    if (dy && !yinc)
		continue;
	    for (int dx = 0; dx < 2; dx++)
	    {
		if (dx && !xinc)
		    continue;
		boxes[numboxes].initBounds(
			       SYSmin((x+dx) << level, myBaseLevel->getXRes()),
			       SYSmin((y+dy) << level, myBaseLevel->getYRes()),
			       SYSmin((z+dz) << level, myBaseLevel->getZRes()));
		boxes[numboxes].enlargeBounds(
			       SYSmin((x+dx+1) << level, myBaseLevel->getXRes()),
			       SYSmin((y+dy+1) << level, myBaseLevel->getYRes()),
			       SYSmin((z+dz+1) << level, myBaseLevel->getZRes()));
		numboxes++;
	    }
	}
    }

    ut_VoxelMipMapSortCompare	sortstats[8];
    for (int i = 0; i < numboxes; i++)
    {
	sortstats[i].value = op.sortValue(boxes[i], level);
	sortstats[i].key = i;
    }
    std::stable_sort(sortstats, &sortstats[numboxes]);

    for (int i = 0; i < numboxes; i++)
    {
	int		whichbox = sortstats[i].key;
	doTraverseSorted(boxes[whichbox](0,0)>>level, boxes[whichbox](1,0)>>level, boxes[whichbox](2,0)>>level, level, op);
    }
}

template <typename T>
void
UT_VoxelMipMap<T>::initializePrivate()
{
    myNumLevels = 0;
    myBaseLevel = 0;
    myOwnBase = false;
}

template <typename T>
void
UT_VoxelMipMap<T>::destroyPrivate()
{
    for (exint i = 0; i < myLevels.entries(); i++)
    {
	for (exint level = 0; level < myNumLevels; level++)
	    delete myLevels(i)[level];
	delete [] myLevels(i);
    }
    myLevels.entries(0);

    if (myOwnBase)
	delete myBaseLevel;

    initializePrivate();
}

//
// UT_VoxelArrayIterator implementation
//
template <typename T>
UT_VoxelArrayIterator<T>::UT_VoxelArrayIterator()
{
    myArray = 0;
    myHandle.resetHandle();
    myCurTile = -1;
    myShouldCompressOnExit = false;
    myUseTileList = false;
    myJobInfo = 0;
    myInterrupt = 0;
}

template <typename T>
UT_VoxelArrayIterator<T>::UT_VoxelArrayIterator(UT_VoxelArray<T> *vox)
{
    myShouldCompressOnExit = false;
    myUseTileList = false;
    myJobInfo = 0;
    setArray(vox);
    myInterrupt = 0;
}

template <typename T>
UT_VoxelArrayIterator<T>::UT_VoxelArrayIterator(UT_COWReadHandle<UT_VoxelArray<T> > handle)
{
    myShouldCompressOnExit = false;
    myUseTileList = false;
    myJobInfo = 0;
    setHandle(handle);
    myInterrupt = 0;
}

template <typename T>
UT_VoxelArrayIterator<T>::~UT_VoxelArrayIterator()
{
}

template <typename T>
void
UT_VoxelArrayIterator<T>::setPartialRange(int idx, int numranges)
{
    int		numtiles;
    fpreal	tileperrange;
    
    // Must already have been set.
    UT_ASSERT(myArray);
    if (!myArray)
	return;

    // Divide our tiles in to num ranges groups.
    numtiles = myArray->numTiles();

    // If we are using a tile list, use it instead
    if (myUseTileList)
	numtiles = myTileList.entries();

    // Trivial case.
    if (!numtiles)
    {
	myTileStart = 0;
	myTileEnd = 0;
	return;
    }

    // Sanity check idx.
    if (idx < 0 || idx >= numranges)
    {
	UT_ASSERT(!"Idx out of bounds");
	myTileStart = -1;
	myTileEnd = -1;
	return;
    }
    
    // Sanity test numranges
    if (numranges < 1)
    {
	UT_ASSERT(!"Invalid range count!");
	numranges = 1;
    }

    // Compute tile per range.
    // We use floating point so, if you have 15 tiles and
    // 16 processors, you don't decide to do all 15
    // tiles on one processor.
    tileperrange = (fpreal)numtiles / (fpreal)numranges;
    
    myTileStart = (int) SYSfloor(idx * tileperrange);
    myTileEnd = (int) SYSfloor((idx+1) * tileperrange); 

    // Special case to ensure we always get the last tile,
    // despite the evils of floating point.
    if (idx == numranges-1)
	myTileEnd = numtiles;

    UT_ASSERT(myTileStart >= 0);
    UT_ASSERT(myTileEnd <= numtiles);
}

template <typename T>
void
UT_VoxelArrayIterator<T>::splitByTile(const UT_JobInfo &info)
{
    // Must already have been set.
    UT_ASSERT(myArray);
    if (!myArray)
	return;

    // If there is one thread, don't bother
    if (info.numJobs() == 1)
	myJobInfo = 0;
    else
	myJobInfo = &info;
}

template <typename T>
void
UT_VoxelArrayIterator<T>::restrictToBBox(const UT_BoundingBox &bbox)
{
    UT_Vector3		pmin, pmax, vmin, vmax;

    pmin = bbox.minvec();
    pmax = bbox.maxvec();

    pmin.x() = SYSmax(pmin.x(), 0.0f);
    pmin.y() = SYSmax(pmin.y(), 0.0f);
    pmin.z() = SYSmax(pmin.z(), 0.0f);
    pmax.x() = SYSmin(pmax.x(), 1.0f);
    pmax.y() = SYSmin(pmax.y(), 1.0f);
    pmax.z() = SYSmin(pmax.z(), 1.0f);

    myArray->posToIndex(pmin, vmin);
    myArray->posToIndex(pmax, vmax);

    restrictToBBox(SYSfloor(vmin.x()), SYSceil(vmax.x()),
		    SYSfloor(vmin.y()), SYSceil(vmax.y()),
		    SYSfloor(vmin.z()), SYSceil(vmax.z()));
}

template <typename T>
void
UT_VoxelArrayIterator<T>::restrictToBBox(int xmin, int xmax,
					int ymin, int ymax,
					int zmin, int zmax)
{
    int			xres, yres, zres, x, y, z;

    xres = myArray->getXRes();
    yres = myArray->getYRes();
    zres = myArray->getZRes();

    myTileList.entries(0);
    myUseTileList = true;
    // Make sure we have any tiles that are valid.
    if (xmin < xres && xmax >= 0 &&
	ymin < yres && ymax >= 0 &&
	zmin < zres && zmax >= 0)
    {
	// Clamp into valid range.
	myArray->clampIndex(xmin, ymin, zmin);
	myArray->clampIndex(xmax, ymax, zmax);

	// Convert to tiles...
	xmin >>= TILEBITS;
	ymin >>= TILEBITS;
	zmin >>= TILEBITS;
	xmax >>= TILEBITS;
	ymax >>= TILEBITS;
	zmax >>= TILEBITS;

	// Check if we are all accounted for.
	if (myArray->numTiles() == (xmax-xmin+1)*(ymax-ymin+1)*(zmax-zmin+1))
	{
	    UT_ASSERT(xmin == 0 && ymin == 0 && zmin == 0);
	    // No need for a tile list, just run everything!
	    myUseTileList = false;
	}
	else
	{
	    // Iterate over all active tiles, adding to our list.
	    for (z = zmin; z <= zmax; z++)
	    {
		for (y = ymin; y <= ymax; y++)
		{
		    for (x = xmin; x <= xmax; x++)
		    {
			myTileList.append(myArray->xyzTileToLinear(x, y, z));
		    }
		}
	    }
	}
    }

    // Recompute our ranges.
    myCurTile = -1;
    setPartialRange(0, 1);
}
    

template <typename T>
void
UT_VoxelArrayIterator<T>::rewind()
{
    // Ensure we have at least one tile in each direction
    if (!myArray ||
	!myArray->getRes(0) || !myArray->getRes(1) || !myArray->getRes(2))
    {
	myCurTile = -1;
	return;
    }

    if (myUseTileList)
    {
	if (myJobInfo)
	    myCurTileListIdx = myJobInfo->nextTask();
	else
	    myCurTileListIdx = myTileStart;
	if (myCurTileListIdx < 0 || myCurTileListIdx >= myTileEnd)
	{
	    myCurTile = -1;
	    return;
	}
	myCurTile = myTileList(myCurTileListIdx);
    }
    else
    {
	if (myJobInfo)
	    myCurTile = myJobInfo->nextTask();
	else
	    myCurTile = myTileStart;
	// If this is an empty range, we quit right away.
	if (myCurTile < 0 || myCurTile >= myTileEnd)
	{
	    myCurTile = -1;
	    return;
	}
    }

    UT_VoxelTile<T>	*tile;
    
    tile = myArray->getLinearTile(myCurTile);

    // Get the tile position
    if (myCurTile)
    {
	myArray->linearTileToXYZ(myCurTile, 
				 myTilePos[0], myTilePos[1], myTilePos[2]);
	myPos[0] = TILESIZE * myTilePos[0];
	myPos[1] = TILESIZE * myTilePos[1];
	myPos[2] = TILESIZE * myTilePos[2];
    }
    else
    {
	myTilePos[0] = 0;
	myTilePos[1] = 0;
	myTilePos[2] = 0;
	myPos[0] = 0;
	myPos[1] = 0;
	myPos[2] = 0;
    }

    myTileLocalPos[0] = 0;
    myTileLocalPos[1] = 0;
    myTileLocalPos[2] = 0;

    myTileSize[0] = tile->xres();
    myTileSize[1] = tile->yres();
    myTileSize[2] = tile->zres();
}

template <typename T>
void
UT_VoxelArrayIterator<T>::advanceTile()
{
    if (myInterrupt && myInterrupt->opInterrupt())
    {
	myCurTile = -1;
	return;
    }
    if (myUseTileList)
    {
	if (getCompressOnExit())
	{
	    // Verify our last tile was a legitimate one.
	    if (myCurTile >= 0 && myCurTileListIdx < myTileEnd)
	    {
		myArray->getLinearTile(myCurTile)->tryCompress(myArray->getCompressionOptions());
	    }
	}

	// Advance our myCurTileListIdx and rebuild from 
	if (myJobInfo)
	    myCurTileListIdx = myJobInfo->nextTask();
	else
	    myCurTileListIdx++;
	if (myCurTileListIdx >= myTileEnd)
	{
	    myCurTile = -1;
	    return;
	}

	myCurTile = myTileList(myCurTileListIdx);

	myArray->linearTileToXYZ(myCurTile, 
				 myTilePos[0], myTilePos[1], myTilePos[2]);

	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);
	myTileLocalPos[0] = 0;
	myTileLocalPos[1] = 0;
	myTileLocalPos[2] = 0;
	myTileSize[0] = tile->xres();
	myTileSize[1] = tile->yres();
	myTileSize[2] = tile->zres();

	myPos[0] = TILESIZE * myTilePos[0];
	myPos[1] = TILESIZE * myTilePos[1];
	myPos[2] = TILESIZE * myTilePos[2];
	return;
    }

    // If requested, compress our last tile.
    if (getCompressOnExit())
    {
	// Verify our last tile was a legitimate one.
	if (myCurTile >= 0 && myCurTile < myTileEnd)
	{
	    myArray->getLinearTile(myCurTile)->tryCompress(myArray->getCompressionOptions());
	}
    }
    
    if (myJobInfo)
    {
	myCurTile = myJobInfo->nextTask();
	if (myCurTile >= myTileEnd)
	{
	    myCurTile = -1;
	    return;
	}
	myArray->linearTileToXYZ(myCurTile, 
				 myTilePos[0], myTilePos[1], myTilePos[2]);
    }
    else
    {
	// Advance our myTilePos, then rebuild everything from there.
	myTilePos[0]++;
	if (myTilePos[0] >= myArray->getTileRes(0))
	{
	    myTilePos[0] = 0;
	    myTilePos[1]++;
	    if (myTilePos[1] >= myArray->getTileRes(1))
	    {
		myTilePos[1] = 0;
		myTilePos[2]++;
		if (myTilePos[2] >= myArray->getTileRes(2))
		{
		    // All done!
		    myCurTile = -1;
		    return;
		}
	    }
	}
	myCurTile = myArray->xyzTileToLinear(myTilePos[0], myTilePos[1], myTilePos[2]);
    }

    UT_VoxelTile<T>	*tile;

    // Rebuild our settings from this tile.

    // See if we hit our end.
    if (myCurTile >= myTileEnd)
    {
	myCurTile = -1;
	return;
    }

    tile = myArray->getLinearTile(myCurTile);
    myTileLocalPos[0] = 0;
    myTileLocalPos[1] = 0;
    myTileLocalPos[2] = 0;
    myTileSize[0] = tile->xres();
    myTileSize[1] = tile->yres();
    myTileSize[2] = tile->zres();

    myPos[0] = TILESIZE * myTilePos[0];
    myPos[1] = TILESIZE * myTilePos[1];
    myPos[2] = TILESIZE * myTilePos[2];
}


template <typename T>
void
UT_VoxelArrayIterator<T>::skipToEndOfTile()
{
    myTileLocalPos[0] = myTileSize[0];
    myTileLocalPos[1] = myTileSize[1];
    myTileLocalPos[2] = myTileSize[2];
}

template <typename T>
template <typename OP>
void
UT_VoxelArrayIterator<T>::applyOperation(OP &op)
{
    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);

	if (!tile->isSimpleCompression())
	    tile->uncompress();

	if (tile->isConstant())
	{
	    // Woohoo!  Too simple!
	    T		val;

	    val = tile->rawData()[0];

	    val = op(val);

	    tile->rawData()[0] = val;
	}
	else
	{
	    T		*val;

	    val = tile->rawData();

	    int		n = tile->numVoxels();
	    for (int i = 0; i < n; i++)
	    {
		val[i] = op(val[i]);
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP, typename S>
void
UT_VoxelArrayIterator<T>::applyOperation(OP &op,
			    const UT_VoxelArray<S> &a)
{
    UT_ASSERT(myArray->isMatching(a));

    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<S>		*atile;
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);
	atile = a.getLinearTile(myCurTile);

	if (!atile->isSimpleCompression())
	{
	    // Have to use getValue...
	    for (int z = 0; z < tile->zres(); z++)
		for (int y = 0; y < tile->yres(); y++)
		    for (int x = 0; x < tile->xres(); x++)
		    {
			S		aval;
			T		val;

			val = tile->operator()(x, y, z);
			aval = atile->operator()(x, y, z);

			val = op(val, aval);

			tile->setValue(x, y, z, val);
		    }
	}
	else
	{
	    if (!tile->isSimpleCompression())
		tile->uncompress();

	    int		ainc = atile->isConstant() ? 0 : 1;

	    if (tile->isConstant())
	    {
		if (!ainc)
		{
		    // Woohoo!  Too simple!
		    S		aval;
		    T		val;

		    val = tile->rawData()[0];
		    aval = atile->rawData()[0];

		    val = op(val, aval);

		    tile->rawData()[0] = val;
		}
		else
		{
		    tile->uncompress();
		}
	    }

	    // In case we uncompressed ourself above...
	    // we have to test again
	    if (!tile->isConstant())
	    {
		const S		*aval;
		T		*val;

		val = tile->rawData();
		aval = atile->rawData();
		ainc = atile->isConstant() ? 0 : 1;

		int		n = tile->numVoxels();
		for (int i = 0; i < n; i++)
		{
		    val[i] = op(val[i], *aval);
		    aval += ainc;
		}
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP>
void
UT_VoxelArrayIterator<T>::applyOperation(OP &op, T a)
{
    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);

	if (!tile->isSimpleCompression())
	    tile->uncompress();

	if (tile->isConstant())
	{
	    T		val;

	    val = tile->rawData()[0];

	    val = op(val, a);

	    tile->rawData()[0] = val;
	}
	else
	{
	    T		*val;

	    val = tile->rawData();

	    int		n = tile->numVoxels();
	    for (int i = 0; i < n; i++)
	    {
		val[i] = op(val[i], a);
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP, typename S, typename R>
void
UT_VoxelArrayIterator<T>::applyOperation(OP &op,
			    const UT_VoxelArray<S> &a,
			    const UT_VoxelArray<R> &b)
{
    UT_ASSERT(myArray->isMatching(a));
    UT_ASSERT(myArray->isMatching(b));

    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<S>		*atile;
	UT_VoxelTile<R>		*btile;
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);
	atile = a.getLinearTile(myCurTile);
	btile = b.getLinearTile(myCurTile);

	if (!atile->isSimpleCompression() || !btile->isSimpleCompression())
	{
	    // Have to use getValue...
	    for (int z = 0; z < tile->zres(); z++)
		for (int y = 0; y < tile->yres(); y++)
		    for (int x = 0; x < tile->xres(); x++)
		    {
			S		aval;
			R		bval;
			T		val;

			val = tile->operator()(x, y, z);
			aval = atile->operator()(x, y, z);
			bval = btile->operator()(x, y, z);

			val = op(val, aval, bval);

			tile->setValue(x, y, z, val);
		    }
	}
	else
	{
	    if (!tile->isSimpleCompression())
		tile->uncompress();

	    int		ainc = atile->isConstant() ? 0 : 1;
	    int		binc = btile->isConstant() ? 0 : 1;

	    if (tile->isConstant())
	    {
		if (!ainc && !binc)
		{
		    // Woohoo!  Too simple!
		    S		aval;
		    R		bval;
		    T		val;

		    val = tile->rawData()[0];
		    aval = atile->rawData()[0];
		    bval = btile->rawData()[0];

		    val = op(val, aval, bval);

		    tile->rawData()[0] = val;
		}
		else
		{
		    tile->uncompress();
		}
	    }

	    // In case we uncompressed ourself above...
	    // we have to test again
	    if (!tile->isConstant())
	    {
		const S		*aval;
		const R		*bval;
		T		*val;

		val = tile->rawData();
		aval = atile->rawData();
		bval = btile->rawData();
		ainc = atile->isConstant() ? 0 : 1;
		binc = btile->isConstant() ? 0 : 1;

		int		n = tile->numVoxels();
		for (int i = 0; i < n; i++)
		{
		    val[i] = op(val[i], *aval, *bval);
		    aval += ainc;
		    bval += binc;
		}
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP, typename S, typename R, typename Q>
void
UT_VoxelArrayIterator<T>::applyOperation(OP &op,
                const UT_VoxelArray<S> &a,
                const UT_VoxelArray<R> &b,
                const UT_VoxelArray<Q> &c)
{
    UT_ASSERT(myArray->isMatching(a));
    UT_ASSERT(myArray->isMatching(b));
    UT_ASSERT(myArray->isMatching(c));

    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<S>     *atile;
	UT_VoxelTile<R>     *btile;
	UT_VoxelTile<Q>     *ctile;
	UT_VoxelTile<T>     *tile;

	tile = myArray->getLinearTile(myCurTile);
	atile = a.getLinearTile(myCurTile);
	btile = b.getLinearTile(myCurTile);
	ctile = c.getLinearTile(myCurTile);

	if (!atile->isSimpleCompression() || !btile->isSimpleCompression() ||
	    !ctile->isSimpleCompression())
	{
	    // Have to use getValue...
	    for (int z = 0; z < tile->zres(); z++)
		for (int y = 0; y < tile->yres(); y++)
		    for (int x = 0; x < tile->xres(); x++)
		    {
			S       aval;
			R       bval;
			Q       cval;
			T       val;

			val = tile->operator()(x, y, z);
			aval = atile->operator()(x, y, z);
			bval = btile->operator()(x, y, z);
			cval = ctile->operator()(x, y, z);

			val = op(val, aval, bval, cval);

			tile->setValue(x, y, z, val);
		    }
	}
	else
	{
	    if (!tile->isSimpleCompression())
		tile->uncompress();

	    int     ainc = atile->isConstant() ? 0 : 1;
	    int     binc = btile->isConstant() ? 0 : 1;
	    int     cinc = ctile->isConstant() ? 0 : 1;

	    if (tile->isConstant())
	    {
		if (!ainc && !binc && !cinc)
		{
		    // Woohoo!  Too simple!
		    S       aval;
		    R       bval;
		    Q       cval;
		    T       val;

		    val = tile->rawData()[0];
		    aval = atile->rawData()[0];
		    bval = btile->rawData()[0];
		    cval = ctile->rawData()[0];

		    val = op(val, aval, bval, cval);

		    tile->rawData()[0] = val;
		}
		else
		{
		    tile->uncompress();
		}
	    }

	    // In case we uncompressed ourself above...
	    // we have to test again
	    if (!tile->isConstant())
	    {
		const S     *aval;
		const R     *bval;
		const Q     *cval;
		T           *val;

		val = tile->rawData();
		aval = atile->rawData();
		bval = btile->rawData();
		cval = ctile->rawData();
		ainc = atile->isConstant() ? 0 : 1;
		binc = btile->isConstant() ? 0 : 1;
		cinc = ctile->isConstant() ? 0 : 1;

		int     n = tile->numVoxels();
		for (int i = 0; i < n; i++)
		{
		    val[i] = op(val[i], *aval, *bval, *cval);
		    aval += ainc;
		    bval += binc;
		    cval += cinc;
		}
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP, typename S>
void
UT_VoxelArrayIterator<T>::applyOperationCheckNoop(OP &op,
			    const UT_VoxelArray<S> &a)
{
    UT_ASSERT(myArray->isMatching(a));

    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<S>		*atile;
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);
	atile = a.getLinearTile(myCurTile);

	if (!atile->isSimpleCompression())
	{
	    // Have to use getValue...
	    for (int z = 0; z < tile->zres(); z++)
		for (int y = 0; y < tile->yres(); y++)
		    for (int x = 0; x < tile->xres(); x++)
		    {
			S		aval;
			T		val;

			val = tile->operator()(x, y, z);
			aval = atile->operator()(x, y, z);

			val = op(val, aval);

			tile->setValue(x, y, z, val);
		    }
	}
	else
	{
	    int		ainc = atile->isConstant() ? 0 : 1;

	    // Check if the incoming tile is constant and is a no-op
	    // If so, we do not want to decompress!
	    if (!ainc)
	    {
		S		aval;
		aval = atile->rawData()[0];
		if (op.isNoop(aval))
		{
		    advanceTile();
		    continue;
		}
	    }

	    if (!tile->isSimpleCompression())
		tile->uncompress();

	    if (tile->isConstant())
	    {
		if (!ainc)
		{
		    // Woohoo!  Too simple!
		    S		aval;
		    T		val;

		    val = tile->rawData()[0];
		    aval = atile->rawData()[0];

		    val = op(val, aval);

		    tile->rawData()[0] = val;
		}
		else
		{
		    tile->uncompress();
		}
	    }

	    // In case we uncompressed ourself above...
	    // we have to test again
	    if (!tile->isConstant())
	    {
		const S		*aval;
		T		*val;

		val = tile->rawData();
		aval = atile->rawData();
		ainc = atile->isConstant() ? 0 : 1;

		int		n = tile->numVoxels();
		for (int i = 0; i < n; i++)
		{
		    val[i] = op(val[i], *aval);
		    aval += ainc;
		}
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP>
void
UT_VoxelArrayIterator<T>::applyOperationCheckNoop(OP &op, T a)
{
    rewind();

    if (op.isNoop(a))
    {
	while (!atEnd())
	    advanceTile();
	return;
    }

    applyOperation(op, a);
}

template <typename T>
template <typename OP, typename S>
void
UT_VoxelArrayIterator<T>::assignOperation(OP &op,
			    const UT_VoxelArray<S> &a)
{
    UT_ASSERT(myArray->isMatching(a));

    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<S>		*atile;
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);
	atile = a.getLinearTile(myCurTile);

	if (!atile->isSimpleCompression())
	{
	    // Have to use getValue...
	    for (int z = 0; z < tile->zres(); z++)
		for (int y = 0; y < tile->yres(); y++)
		    for (int x = 0; x < tile->xres(); x++)
		    {
			S		aval;
			T		val;

			aval = atile->operator()(x, y, z);

			val = op(aval);

			tile->setValue(x, y, z, val);
		    }
	}
	else
	{
	    if (!tile->isSimpleCompression())
		tile->uncompress();

	    if (atile->isConstant())
	    {
		S		aval;
		T		val;

		aval = atile->rawData()[0];

		val = op(aval);

		tile->makeConstant(val);
	    }
	    else
	    {
		tile->uncompress();

		const S		*aval;
		T		*val;

		val = tile->rawData();
		aval = atile->rawData();

		int		n = tile->numVoxels();
		for (int i = 0; i < n; i++)
		{
		    val[i] = op(aval[i]);
		}
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP, typename S, typename R>
void
UT_VoxelArrayIterator<T>::assignOperation(OP &op,
			    const UT_VoxelArray<S> &a,
			    const UT_VoxelArray<R> &b)
{
    UT_ASSERT(myArray->isMatching(a));
    UT_ASSERT(myArray->isMatching(b));

    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<S>		*atile;
	UT_VoxelTile<R>		*btile;
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);
	atile = a.getLinearTile(myCurTile);
	btile = b.getLinearTile(myCurTile);

	if (!atile->isSimpleCompression() || !btile->isSimpleCompression())
	{
	    // Have to use getValue...
	    for (int z = 0; z < tile->zres(); z++)
		for (int y = 0; y < tile->yres(); y++)
		    for (int x = 0; x < tile->xres(); x++)
		    {
			S		aval;
			R		bval;
			T		val;

			aval = atile->operator()(x, y, z);
			bval = btile->operator()(x, y, z);

			val = op(aval, bval);

			tile->setValue(x, y, z, val);
		    }
	}
	else
	{
	    int		ainc = atile->isConstant() ? 0 : 1;
	    int		binc = btile->isConstant() ? 0 : 1;

	    if (!ainc && !binc)
	    {
		S		aval;
		R		bval;
		T		val;

		aval = atile->rawData()[0];
		bval = btile->rawData()[0];

		val = op(aval, bval);

		tile->makeConstant(val);
	    }
	    else
	    {
		const S		*aval;
		const R		*bval;
		T		*val;

		tile->uncompress();

		val = tile->rawData();
		aval = atile->rawData();
		bval = btile->rawData();
		ainc = atile->isConstant() ? 0 : 1;
		binc = btile->isConstant() ? 0 : 1;

		int		n = tile->numVoxels();
		for (int i = 0; i < n; i++)
		{
		    val[i] = op(*aval, *bval);
		    aval += ainc;
		    bval += binc;
		}
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP, typename S, typename R, typename Q>
void
UT_VoxelArrayIterator<T>::assignOperation(OP &op,
			    const UT_VoxelArray<S> &a,
			    const UT_VoxelArray<R> &b,
			    const UT_VoxelArray<Q> &c)
{
    UT_ASSERT(myArray->isMatching(a));
    UT_ASSERT(myArray->isMatching(b));
    UT_ASSERT(myArray->isMatching(c));

    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<S>		*atile;
	UT_VoxelTile<R>		*btile;
	UT_VoxelTile<Q>		*ctile;
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);
	atile = a.getLinearTile(myCurTile);
	btile = b.getLinearTile(myCurTile);
	ctile = c.getLinearTile(myCurTile);

	if (!atile->isSimpleCompression() || !btile->isSimpleCompression() ||
	    !ctile->isSimpleCompression())
	{
	    // Have to use getValue...
	    for (int z = 0; z < tile->zres(); z++)
		for (int y = 0; y < tile->yres(); y++)
		    for (int x = 0; x < tile->xres(); x++)
		    {
			S		aval;
			R		bval;
			Q		cval;
			T		val;

			aval = atile->operator()(x, y, z);
			bval = btile->operator()(x, y, z);
			cval = ctile->operator()(x, y, z);

			val = op(aval, bval, cval);

			tile->setValue(x, y, z, val);
		    }
	}
	else
	{
	    int		ainc = atile->isConstant() ? 0 : 1;
	    int		binc = btile->isConstant() ? 0 : 1;
	    int		cinc = ctile->isConstant() ? 0 : 1;

	    if (!ainc && !binc && !cinc)
	    {
		S		aval;
		R		bval;
		Q		cval;
		T		val;

		aval = atile->rawData()[0];
		bval = btile->rawData()[0];
		cval = ctile->rawData()[0];

		val = op(aval, bval, cval);

		tile->makeConstant(val);
	    }
	    else
	    {
		const S		*aval;
		const R		*bval;
		const Q		*cval;
		T		*val;

		tile->uncompress();

		val = tile->rawData();
		aval = atile->rawData();
		bval = btile->rawData();
		cval = ctile->rawData();
		ainc = atile->isConstant() ? 0 : 1;
		binc = btile->isConstant() ? 0 : 1;
		cinc = ctile->isConstant() ? 0 : 1;

		int		n = tile->numVoxels();
		for (int i = 0; i < n; i++)
		{
		    val[i] = op(*aval, *bval, *cval);
		    aval += ainc;
		    bval += binc;
		    cval += cinc;
		}
	    }
	}

	advanceTile();
    }
}

template <typename T>
template <typename OP>
void
UT_VoxelArrayIterator<T>::reduceOperation(OP &op)
{
    rewind();

    while (!atEnd())
    {
	UT_VoxelTile<T>		*tile;

	tile = myArray->getLinearTile(myCurTile);

	if (!tile->isSimpleCompression())
	{
	    // Have to use getValue...
	    for (int z = 0; z < tile->zres(); z++)
		for (int y = 0; y < tile->yres(); y++)
		    for (int x = 0; x < tile->xres(); x++)
		    {
			T		val;

			val = tile->operator()(x, y, z);

			op.reduce(val);
		    }
	}
	else if (tile->isConstant())
	{
	    // Woohoo!  Too simple!
	    T		val;

	    val = tile->rawData()[0];
	    int		n = tile->numVoxels();

	    op.reduceMany(val, n);
	}
	else
	{
	    T		*val;

	    val = tile->rawData();

	    int		n = tile->numVoxels();
	    for (int i = 0; i < n; i++)
	    {
		op.reduce(val[i]);
	    }
	}

	advanceTile();
    }
}

//
// UT_VoxelTileIterator implementation
//
template <typename T>
UT_VoxelTileIterator<T>::UT_VoxelTileIterator()
{
    myCurTile = 0;
    myArray = 0;
    myAtEnd = true;
    myShouldCompressOnExit = false;
}

template <typename T>
UT_VoxelTileIterator<T>::UT_VoxelTileIterator(const UT_VoxelArrayIterator<T> &vit)
{
    myCurTile = 0;
    myArray = 0;
    myAtEnd = true;
    myShouldCompressOnExit = false;
    setTile(vit);
}

template <typename T>
template <typename S>
UT_VoxelTileIterator<T>::UT_VoxelTileIterator(const UT_VoxelArrayIterator<S> &vit, UT_VoxelArray<T> *array)
{
    myCurTile = 0;
    myArray = 0;
    myAtEnd = true;
    myShouldCompressOnExit = false;
    setTile(vit, array);
}

template <typename T>
UT_VoxelTileIterator<T>::~UT_VoxelTileIterator()
{
}

template <typename T>
void
UT_VoxelTileIterator<T>::rewind()
{
    // Ensure we have at least one voxel in each direction
    if (!myCurTile ||
	!myCurTile->xres() || !myCurTile->yres() || !myCurTile->zres())
    {
	myCurTile = 0;
	return;
    }

    myPos[0] = myTileStart[0];
    myPos[1] = myTileStart[1];
    myPos[2] = myTileStart[2];

    myTileLocalPos[0] = 0;
    myTileLocalPos[1] = 0;
    myTileLocalPos[2] = 0;

    myTileSize[0] = myCurTile->xres();
    myTileSize[1] = myCurTile->yres();
    myTileSize[2] = myCurTile->zres();

    myAtEnd = false;
}

template <typename T>
void
UT_VoxelTileIterator<T>::advanceTile()
{
    if (getCompressOnExit())
    {
	// Verify our last tile was a legitimate one.
	if (myCurTile)
	{
	    myCurTile->tryCompress(myArray->getCompressionOptions());
	}
    }
    myAtEnd = true;
}

template <typename T>
template <typename OP>
bool
UT_VoxelTileIterator<T>::reduceOperation(OP &op)
{
    rewind();

    if (!myCurTile->isSimpleCompression())
    {
	// Have to use getValue...
	for (int z = 0; z < myTileSize[2]; z++)
	    for (int y = 0; y < myTileSize[1]; y++)
		for (int x = 0; x < myTileSize[0]; x++)
		{
		    T		val;

		    val = myCurTile->operator()(x, y, z);

		    if (!op.reduce(val))
			return false;
		}
    }
    else if (myCurTile->isConstant())
    {
	// Woohoo!  Too simple!
	T		val;

	val = myCurTile->rawData()[0];
	int		n = myCurTile->numVoxels();

	if (!op.reduceMany(val, n))
	    return false;
    }
    else
    {
	T		*val;

	val = myCurTile->rawData();

	int		n = myCurTile->numVoxels();
	for (int i = 0; i < n; i++)
	{
	    if (!op.reduce(val[i]))
		return false;
	}
    }
    return true;
}

///
/// UT_VoxelProbe methods
///

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>::UT_VoxelProbe()
{
    myCurLine = 0;
    myAllocCacheLine = 0;
    myDirty = false;

    myArray = 0;
}

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>::UT_VoxelProbe(UT_VoxelArray<T> *vox, int prex, int postx)
{
    // A sure signal it hasn't been reset.
    myCurLine = 0;
    myAllocCacheLine = 0;
    myDirty = false;

    setArray(vox, prex, postx);
}

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>::~UT_VoxelProbe()
{
    if (DoWrite)
    {
	if (!TestForWrites || myDirty)
	{
	    // Final write...
	    writeCacheLine();
	}
    }
    delete [] myAllocCacheLine;
}

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
void
UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>::setArray(UT_VoxelArray<T> *vox, int prex, int postx)
{
    // Round up our pre and post
    int			prepad, postpad;

    myCurLine = 0;

    prepad = (prex - 3) / 4;
    postpad = (postx + 3) / 4;

    myAllocCacheLine = new T [TILESIZE - prepad*4 + postpad*4];
    myCacheLine = &myAllocCacheLine[-prepad];

    myPreX = prex;
    myPostX = postx;

    myForceCopy = false;
    if (myPreX || myPostX)
	myForceCopy = true;

    myDirty = false;

    myArray = vox;
}

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
bool
UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>::setIndex(int x, int y, int z)
{
    // Check if we have to reload our cache.
    if (myCurLine && y == myY && z == myZ)
    {
	if (x < myMaxValidX)
	{
	    if (x == myX+1)
	    {
		// A simple advanceX sufficies
		advanceX();
		return false;
	    }

	    if (x >= myMinValidX)
	    {
		// We can just recenter our search location.
		resetX(x);
		// Other functions can't just do advanceX as that
		// just does ++ in X.
		return true;
	    }
	}
    }

    // Store existing cache...
    if (DoWrite)
    {
	if (!TestForWrites || myDirty)
	    writeCacheLine();
    }

    // Everything failed, return to reloading the cache.
    reloadCache(x, y, z);

    if (TestForWrites)
	myDirty = false;

    return true;
}

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
void
UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>::reloadCache(int x, int y, int z)
{
    UT_VoxelTile<T>	*tile;
    bool		 xout = false, yout = false, zout = false;
    bool		 manualbuild = false;

    myX = x;
    myY = y;
    myZ = z;
    myMinValidX = x & ~TILEMASK;
    myMaxValidX = myMinValidX + TILESIZE;
    if (myMaxValidX > myArray->getXRes())
	myMaxValidX = myArray->getXRes(); 

    if (x < 0 || x >= myArray->getXRes())
	xout = true;
    if (y < 0 || y >= myArray->getYRes())
	yout = true;
    if (z < 0 || z >= myArray->getZRes())
	zout = true;

    // If y or z are invalid, they will be invalid for every voxel
    if (yout || zout)
    {
	// We can often handle this by clamping...
	switch (myArray->getBorder())
	{
	    case UT_VOXELBORDER_CONSTANT:
		buildConstantCache(myArray->getBorderValue());

		// ALL DONE
		return;

	    case UT_VOXELBORDER_REPEAT:
		// Simply modulate our lookup.
		if (yout)
		{
		    y %= myArray->getYRes();
		    if (y < 0)
			y += myArray->getYRes();
		}
		if (zout)
		{
		    z %= myArray->getZRes();
		    if (z < 0)
			z += myArray->getZRes();
		}
		break;

	    case UT_VOXELBORDER_STREAK:
	    {
		// Clamp
		int		tx = 0;
		myArray->clampIndex(tx, y, z);
		break;
	    }

	    case UT_VOXELBORDER_EXTRAP:
	    {
		// Force a manual build.
		manualbuild = true;
		break;
	    }
	}
    }

    // Note y and z may no longer equal myY and myZ, this is not
    // a problem however as we have set up our cached versions
    // to the unclamped versions.

    if (xout || manualbuild)
    {
	// We have to manually build if we are extrap or repeat type,
	// not just generate a single constant!
	if (myArray->getBorder() == UT_VOXELBORDER_EXTRAP ||
	    myArray->getBorder() == UT_VOXELBORDER_REPEAT)
	{
	    manualbuild = true;
	}

	// If there is no pre & post, this will always be constant.
	if (!myPreX && !myPostX && !manualbuild)
	{
	    buildConstantCache(myArray->getValue(x, y, z));

	    // ALL DONE
	    return;
	}
	else
	{
	    // If we are STREAK or CONSTANT, we have a constant
	    // value in our own cache.
	    // If we are REPEAT, we want to modulo our x value
	    // and run the normal code path.
	    int		i;

	    for (i = myPreX; i < 0; i++)
	    {
		myCacheLine[i] = myArray->getValue(myMinValidX+i, y, z);
	    }

	    if ((myArray->getBorder() == UT_VOXELBORDER_EXTRAP) || 
		(myArray->getBorder() == UT_VOXELBORDER_REPEAT))
	    {
		// Explicitly load extrap values as they are not constant.
		for (; i < TILESIZE; i++)
		    myCacheLine[i] = myArray->getValue(myMinValidX+i, y, z);
	    }
	    else
	    {
		// CONSTANT and STREAK will have constant values
		// in this 
		T		value = myArray->getValue(x, y, z);
		for (; i < TILESIZE; i++)
		{
		    myCacheLine[i] = value;
		}
	    }

	    for (; i < TILESIZE + myPostX; i++)
	    {
		myCacheLine[i] = myArray->getValue(myMinValidX+i, y, z);
	    }

	    myCurLine = &myCacheLine[x & TILEMASK];
	    myStride = 1;

	    // ALL DONE
	    return;
	}
    }

    int				 xtile, ytile, ztile, tileidx;
    int				 lx, ly, lz;
    int				 i;

    xtile = x >> TILEBITS;
    ytile = y >> TILEBITS;
    ztile = z >> TILEBITS;

    // Get our local indices
    lx = x & TILEMASK;
    ly = y & TILEMASK;
    lz = z & TILEMASK;

    tileidx = (ztile * myArray->getTileRes(1) + ytile) * myArray->getTileRes(0);

    if (myPreX)
    {
	if (xtile)
	{
	    // Simple to fetch...
	    tile = myArray->getLinearTile(tileidx+xtile-1);
	    for (i = myPreX; i < 0; i++)
	    {
		// Safe to to & TILEMASK as we know earlier tiles
		// are always full...
		myCacheLine[i] = (*tile)(i & TILEMASK, ly, lz);
	    }
	}
	else
	{
	    if (myArray->getBorder() == UT_VOXELBORDER_REPEAT)
	    {
		int		resx = myArray->getXRes();
		int		xpos;

		xpos = myPreX;
		xpos %= resx;
		// We add resx to guarantee we are in range.
		xpos += resx;

		// Manually invoke getValue()
		for (i = myPreX; i < 0; i++)
		{
		    myCacheLine[i] = (*myArray)(xpos, ly, lz);
		    xpos++;
		    if (xpos > resx)
			xpos -= resx;
		}
	    }
	    else
	    {
		T		value;

		if  (myArray->getBorder() == UT_VOXELBORDER_STREAK)
		{
		    tile = myArray->getLinearTile(tileidx+xtile);
		    value = (*tile)(0, ly, lz);
		}
		else
		    value = myArray->getBorderValue();

		// Fill in value.
		for (i = myPreX; i < 0; i++)
		    myCacheLine[i] = value;
	    }
	}
    }

    if (myPostX)
    {
	int	 	 cachelen;
	int		 resx = myArray->getXRes();

	// This is where we start writing to.
	cachelen = myMaxValidX - myMinValidX;

	// Append our end part in.
	// First, determine if we read past the end...
	if (myMaxValidX + myPostX > myArray->getXRes())
	{
	    // This can be very messy.  For example, we may have
	    // a 1 wide tile after this tile and be looking two voxels
	    // ahead, which means we can't guarantee our lookup
	    // is entirely within one tile.
	    // However, we can break it into two loops.
	    int		xpos = myMaxValidX;

	    // Portion that still fits in the next tile...
	    i = 0;
	    if (xpos < resx)
	    {
		tile = myArray->getLinearTile(tileidx+xtile+1);
		for (; i < myPostX && xpos < resx; i++)
		{
		    myCacheLine[i + cachelen] = (*tile)(i, ly, lz);
		    xpos++;
		}
	    }
	    // Portion that reads past the end.
	    if (i < myPostX)
	    {
		if (myArray->getBorder() == UT_VOXELBORDER_REPEAT)
		{
		    xpos = xpos % resx;

		    // Revert to the array operator.
		    for (; i < myPostX; i++)
		    {
			myCacheLine[i + cachelen] = (*myArray)(xpos, y, z);
			xpos++;
			if (xpos > resx)
			    xpos -= resx;
		    }
		}
		else
		{
		    T		value;

		    if (myArray->getBorder() == UT_VOXELBORDER_STREAK)
		    {
			tile = myArray->getLinearTile(tileidx+xtile);
			value = (*tile)(tile->xres()-1, ly, lz);
		    }
		    else
			value = myArray->getBorderValue();

		    for (; i < myPostX; i++)
			myCacheLine[i + cachelen] = value;
		}
	    }
	}
	else
	{
	    // All groovy, we fit in so thus must fit in the next tile
	    tile = myArray->getLinearTile(tileidx+xtile+1);
	    for (i = 0; i < myPostX; i++)
	    {
		// Safe to to & TILEMASK as we know earlier tiles
		// are always full...
		myCacheLine[i + cachelen] = (*tile)(i, ly, lz);
	    }
	}

    }

    tile = myArray->getLinearTile(tileidx+xtile);
    myCurLine = tile->fillCacheLine(myCacheLine, myStride, lx, ly, lz, myForceCopy, DoWrite);
}

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
void
UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>::buildConstantCache(T value)
{
    if (DoWrite)
    {
	// Force a full copy.
	myStride = 1;

	int		i;

	for (i = myPreX; i < TILESIZE+myPostX; i++)
	    myCacheLine[i] = value;
	myCurLine = myCacheLine;
    }
    else
    {
	myCacheLine[0] = value;
	// These are to ensure our SSE is compatible.
	myCacheLine[1] = value;
	myCacheLine[2] = value;
	myCacheLine[3] = value;

	myCurLine = myCacheLine;
	myStride = 0;
    }
}

template <typename T, bool DoRead, bool DoWrite, bool TestForWrites>
void
UT_VoxelProbe<T, DoRead, DoWrite, TestForWrites>::writeCacheLine()
{
    if (!DoWrite)
    {
	UT_ASSERT(0);
	return;
    }
    // Ensure we have a valid loaded line, otherwise no point
    // doing a write back...
    if (!myCurLine)
	return;

    // Reset our current line...
    myCurLine -= myX - myMinValidX;

    // Determine if we actually have to write back,
    // if we had a pointer inside the tile we don't have to.
    if (myCurLine != myCacheLine)
	return;

    // Look up our voxel
    int			xtile, ytile, ztile, y, z;
    UT_VoxelTile<T>	*tile;

    xtile = myMinValidX >> TILEBITS;
    ytile = myY >> TILEBITS;
    ztile = myZ >> TILEBITS;
    y = myY & TILEMASK;
    z = myZ & TILEMASK;

    tile = myArray->getTile(xtile, ytile, ztile);

    // Write back our results
    tile->writeCacheLine(myCurLine, y, z);
}

///
/// VoxelProbeCube functions
///
template <typename T>
UT_VoxelProbeCube<T>::UT_VoxelProbeCube()
{
    myValid = false;
}

template <typename T>
UT_VoxelProbeCube<T>::~UT_VoxelProbeCube()
{
}

template <typename T>
void
UT_VoxelProbeCube<T>::setCubeArray(const UT_VoxelArray<T> *vox)
{
    myLines[0][0].setConstArray(vox, -1, 1);
    myLines[0][1].setConstArray(vox, -1, 1);
    myLines[0][2].setConstArray(vox, -1, 1);

    myLines[1][0].setConstArray(vox, -1, 1);
    myLines[1][1].setConstArray(vox, -1, 1);
    myLines[1][2].setConstArray(vox, -1, 1);

    myLines[2][0].setConstArray(vox, -1, 1);
    myLines[2][1].setConstArray(vox, -1, 1);
    myLines[2][2].setConstArray(vox, -1, 1);

    myValid = false;
}

template <typename T>
void
UT_VoxelProbeCube<T>::setPlusArray(const UT_VoxelArray<T> *vox)
{
    /// This coudl be 0,0, but by keeping it the full range
    /// we ensure it is legal to rotate when we do a +1
    myLines[0][1].setConstArray(vox, -1, 1);

    myLines[1][0].setConstArray(vox, 0, 0);
    myLines[1][1].setConstArray(vox, -1, 1);
    myLines[1][2].setConstArray(vox, 0, 0);

    myLines[2][1].setConstArray(vox, -1, 1);

    myValid = false;
}

template <typename T>
bool
UT_VoxelProbeCube<T>::setIndexCube(int x, int y, int z)
{
    if (myValid && myZ == z)
    {
	if (myY == y)
	{
	    // Potential for a simple advance...
	    if (x < myMaxValidX && x == myX+1)
	    {
		// AdvanceX.
		myLines[0][0].advanceX();
		myLines[0][1].advanceX();
		myLines[0][2].advanceX();

		myLines[1][0].advanceX();
		myLines[1][1].advanceX();
		myLines[1][2].advanceX();

		myLines[2][0].advanceX();
		myLines[2][1].advanceX();
		myLines[2][2].advanceX();

		// Update our cache.
		myX = x;

		return false;
	    }
	}
#if 1
	else if (y == myY+1 && x < myMaxValidX && x >= myMinValidX)
	{
	    // We have finished our x pass and just incremented y by one
	    // Rather than resetting all our lines we can just swap
	    // our y+1 lines into our current lines and then run the
	    // normal reset.  
	    rotateLines(myLines[0][0], myLines[1][0], myLines[2][0]);
	    rotateLines(myLines[0][1], myLines[1][1], myLines[2][1]);
	    rotateLines(myLines[0][2], myLines[1][2], myLines[2][2]);

	    // The first 6 lines can just reset their X values
	    // directly
	    myLines[0][0].resetX(x);
	    myLines[0][1].resetX(x);
	    myLines[0][2].resetX(x);

	    myLines[1][0].resetX(x);
	    myLines[1][1].resetX(x);
	    myLines[1][2].resetX(x);

	    // Only the new lines need a reload.
	    myLines[2][0].setIndex(x, y+1, z-1);
	    myLines[2][1].setIndex(x, y+1, z);
	    myLines[2][2].setIndex(x, y+1, z+1);

	    // Update the cache values that have changed.
	    myX = x;
	    myY = y;

	    return true;
	}
#endif
    }

    // Now just invoke setIndex on all children
    myLines[0][0].setIndex(x, y-1, z-1);
    myLines[0][1].setIndex(x, y-1, z);
    myLines[0][2].setIndex(x, y-1, z+1);

    myLines[1][0].setIndex(x, y, z-1);
    myLines[1][1].setIndex(x, y, z);
    myLines[1][2].setIndex(x, y, z+1);

    myLines[2][0].setIndex(x, y+1, z-1);
    myLines[2][1].setIndex(x, y+1, z);
    myLines[2][2].setIndex(x, y+1, z+1);

    // update our cache values
    myX = x;
    myY = y;
    myZ = z;
    myValid = true;
    myMinValidX = myLines[1][1].myMinValidX;
    myMaxValidX = myLines[1][1].myMaxValidX;

    return true;
}

template <typename T>
bool
UT_VoxelProbeCube<T>::setIndexPlus(int x, int y, int z)
{
    if (myValid && myZ == z)
    {
	if (myY == y)
	{
	    // Potential for a simple advance...
	    if (x < myMaxValidX && x == myX+1)
	    {
		// AdvanceX.
		myLines[0][1].advanceX();

		myLines[1][0].advanceX();
		myLines[1][1].advanceX();
		myLines[1][2].advanceX();

		myLines[2][1].advanceX();

		// Update our cache.
		myX = x;

		return false;
	    }
	}
	else if (y == myY+1 && x < myMaxValidX && x >= myMinValidX)
	{
	    // We have finished our x pass and just incremented y by one
	    // We can thus rotate the meaning of our central
	    // cache lines and just reset their x pointers, leaving
	    // only three real resets to be done.
	    rotateLines(myLines[0][1], myLines[1][1], myLines[2][1]);

	    myLines[0][1].resetX(x);
	    myLines[1][1].resetX(x);

	    myLines[1][0].setIndex(x, y, z-1);
	    myLines[1][2].setIndex(x, y, z+1);

	    myLines[2][1].setIndex(x, y+1, z);

	    myX = x;
	    myY = y;
	    return true;
	}
    }

    // Now just invoke setIndex on all children
    myLines[0][1].setIndex(x, y-1, z);

    myLines[1][0].setIndex(x, y, z-1);
    myLines[1][1].setIndex(x, y, z);
    myLines[1][2].setIndex(x, y, z+1);

    myLines[2][1].setIndex(x, y+1, z);

    // update our cache values
    myX = x;
    myY = y;
    myZ = z;
    myValid = true;
    myMinValidX = myLines[1][1].myMinValidX;
    myMaxValidX = myLines[1][1].myMaxValidX;

    return true;
}

template <typename T>
fpreal64
UT_VoxelProbeCube<T>::curvature(const UT_Vector3 &invvoxelsize) const
{
    // These are our derivatives of Phi.
    fpreal64		Px, Py, Pz;
    fpreal64		Pxx, Pyy, Pzz;
    fpreal64		Pxy, Pxz, Pyz;
    fpreal64		gradlen;
    fpreal64		k;

    // Compute first derivatives.
    // dPhi = (Phi+1 - Phi-1) / 2 * dx
    
    Px = getValue(1, 0, 0) - getValue(-1, 0, 0);
    Px *= 0.5 * invvoxelsize.x();
    
    Py = getValue(0, 1, 0) - getValue(0, -1, 0);
    Py *= 0.5 * invvoxelsize.y();

    Pz = getValue(0, 0, 1) - getValue(0, 0, -1);
    Pz *= 0.5 * invvoxelsize.z();

    // Compute second derivatives.  (Note Pxy == Pyx)

    // d^2Phi = (Phi+1 - 2 Phi + Phi-1) / (dx*dx)
    Pxx = getValue(1, 0, 0)
	  - 2 * getValue(0, 0, 0) 
	  + getValue(-1, 0, 0);
    Pxx *= invvoxelsize.x() * invvoxelsize.x();

    Pyy = getValue(0, 1, 0)
	  - 2 * getValue(0, 0, 0) 
	  + getValue(0, -1, 0);
    Pyy *= invvoxelsize.y() * invvoxelsize.y();

    Pzz = getValue(0, 0, 1)
	  - 2 * getValue(0, 0, 0) 
	  + getValue(0, 0, -1);
    Pzz *= invvoxelsize.z() * invvoxelsize.z();

    // A bit more complicated :>
    Pxy  = getValue(1, 1,0) - getValue(-1, 1,0);
    Pxy -= getValue(1,-1,0) - getValue(-1,-1,0);
    Pxy *= 0.25 * invvoxelsize.x() * invvoxelsize.y();

    Pxz  = getValue(1,0, 1) - getValue(-1,0, 1);
    Pxz -= getValue(1,0,-1) - getValue(-1,0,-1);
    Pxz *= 0.25 * invvoxelsize.x() * invvoxelsize.z();

    Pyz  = getValue(0,1, 1) - getValue(0,-1, 1);
    Pyz -= getValue(0,1,-1) - getValue(0,-1,-1);
    Pyz *= 0.25 * invvoxelsize.y() * invvoxelsize.z();

    // Calculate the |grad(phi)| term;
    gradlen = SYSsqrt(Px * Px + Py * Py + Pz * Pz);

    // Finally, our curvature!
    // This is equation 1.8 from the holy book.
    // The problem is that it implies that 0 gradient means 0 curvature.
    // This is not true!
    // Even if Px,Py,Pz == 0, if Pxx != 0, we have a curved surface
    // consider a point at the maxima of a circle.
    k = Px*Px * (Pyy + Pzz) + Py*Py * (Pxx + Pzz) + Pz*Pz * (Pxx + Pyy);
    k -= 2 * (Pxy*Px*Py + Pyz*Py*Pz + Pxz*Px*Pz);

    // Avoid #IND in places with exactly zero curvature.
    if (!gradlen)
	k = 0;
    else
	k /= gradlen * gradlen * gradlen;

    // Clamp our curvature...
    fpreal64		maxk;

    maxk = invvoxelsize.maxComponent();
    if (k < -maxk)
	k = -maxk;
    if (k > maxk)
	k = maxk;

    return k;
}

template <typename T>
fpreal64
UT_VoxelProbeCube<T>::laplacian(const UT_Vector3 &invvoxelsize) const
{
    fpreal64		Pxx, Pyy, Pzz;
    fpreal64		centralval;

    centralval = getValue(0, 0, 0);

    // d^2Phi = (Phi+1 - 2 Phi + Phi-1) / (dx*dx)
    Pxx = getValue(1, 0, 0)
	  - 2 * centralval
	  + getValue(-1, 0, 0);
    Pxx *= invvoxelsize.x() * invvoxelsize.x();

    Pyy = getValue(0, 1, 0)
	  - 2 * centralval
	  + getValue(0, -1, 0);
    Pyy *= invvoxelsize.y() * invvoxelsize.y();

    Pzz = getValue(0, 0, +1)
	  - 2 * centralval
	  + getValue(0, 0, -1);
    Pzz *= invvoxelsize.z() * invvoxelsize.z();

    return Pxx + Pyy + Pzz;
}

template <typename T>
void
UT_VoxelProbeCube<T>::rotateLines(UT_VoxelProbe<T, true, false, false> &ym, UT_VoxelProbe<T, true, false, false> &y0, UT_VoxelProbe<T, true, false, false> &yp)
{
    T				*tmpcache, *tmpalloc;

    // We take advantage of the fact we know only a small portion
    // of the cache lines needs to be copied.
    tmpcache = ym.myCacheLine;
    tmpalloc = ym.myAllocCacheLine;
    //const T *tmpcur = ym.myCurLine;

    ym.myCacheLine = y0.myCacheLine;
    ym.myAllocCacheLine = y0.myAllocCacheLine;
    ym.myCurLine = y0.myCurLine;
    ym.myStride = y0.myStride;
    ym.myY++;

    y0.myCacheLine = yp.myCacheLine;
    y0.myAllocCacheLine = yp.myAllocCacheLine;
    y0.myCurLine = yp.myCurLine;
    y0.myStride = yp.myStride;
    y0.myY++;

    yp.myCacheLine = tmpcache;
    yp.myAllocCacheLine = tmpalloc;
    // Setting to zero will force a rebuild.
    yp.myCurLine = 0;
}

///
/// UT_VoxelProbeFace methods
///
template <typename T>
UT_VoxelProbeFace<T>::UT_VoxelProbeFace()
{
    myValid = false;
}

template <typename T>
UT_VoxelProbeFace<T>::~UT_VoxelProbeFace()
{
}


template <typename T>
void
UT_VoxelProbeFace<T>::setArray(const UT_VoxelArray<T> *vx, const UT_VoxelArray<T> *vy, const UT_VoxelArray<T> *vz)
{
    // We need one extra to the right on the X probe
    myLines[0][0].setConstArray(vx, 0, 1);
    
    // The rest can be direct reads
    myLines[1][0].setConstArray(vy, 0, 0);
    myLines[1][1].setConstArray(vy, 0, 0);

    myLines[2][0].setConstArray(vz, 0, 0);
    myLines[2][1].setConstArray(vz, 0, 0);
    
    myValid = false;
}

template <typename T>
void
UT_VoxelProbeFace<T>::setVoxelSize(const UT_Vector3 &size)
{
    myVoxelSize = size;
    myInvVoxelSize = 1;
    myInvVoxelSize /= myVoxelSize;
}

template <typename T>
bool
UT_VoxelProbeFace<T>::setIndex(int x, int y, int z)
{
    if (myValid && myZ == z)
    {
	if (myY == y)
	{
	    // Potential for a simple advance...
	    if (x < myMaxValidX && x == myX+1)
	    {
		// AdvanceX.
		myLines[0][0].advanceX();

		myLines[1][0].advanceX();
		myLines[1][1].advanceX();

		myLines[2][0].advanceX();
		myLines[2][1].advanceX();

		// Update our cache.
		myX = x;

		return false;
	    }
	}
	else if (y == myY+1 && x < myMaxValidX && x >= myMinValidX)
	{
	    // We have finished our x pass and just incremented y by one
	    // We can swap our y lines to get to the next read for
	    // those lines.
	    swapLines(myLines[1][0], myLines[1][1]);

	    myLines[1][0].resetX(x);

	    // All the other lines need to be reloaded.
	    myLines[0][0].setIndex(x, y, z);
	    myLines[1][1].setIndex(x, y+1, z);

	    myLines[2][0].setIndex(x, y, z);
	    myLines[2][1].setIndex(x, y, z+1);

	    myX = x;
	    myY = y;
	    return true;
	}
    }

    // Now just invoke setIndex on all children
    myLines[0][0].setIndex(x, y, z);

    myLines[1][0].setIndex(x, y, z);
    myLines[1][1].setIndex(x, y+1, z);

    myLines[2][0].setIndex(x, y, z);
    myLines[2][1].setIndex(x, y, z+1);

    // update our cache values
    myX = x;
    myY = y;
    myZ = z;
    myValid = true;
    myMinValidX = myLines[0][0].myMinValidX;
    myMaxValidX = myLines[0][0].myMaxValidX;

    return true;
}

template <typename T>
void
UT_VoxelProbeFace<T>::swapLines(UT_VoxelProbe<T, true, false, false> &ym, 
				UT_VoxelProbe<T, true, false, false> &yp)
{
    T				*tmpcache, *tmpalloc;

    // We take advantage of the fact we know only a small portion
    // of the cache lines needs to be copied.
    tmpcache = ym.myCacheLine;
    tmpalloc = ym.myAllocCacheLine;
    //const T *tmpcur = ym.myCurLine;

    ym.myCacheLine = yp.myCacheLine;
    ym.myAllocCacheLine = yp.myAllocCacheLine;
    ym.myCurLine = yp.myCurLine;
    ym.myStride = yp.myStride;
    ym.myY++;

    yp.myCacheLine = tmpcache;
    yp.myAllocCacheLine = tmpalloc;
    // Setting to zero will force a rebuild.
    yp.myCurLine = 0;
}

///
/// VoxelProbeAverage methods
///
template <typename T, int XStep, int YStep, int ZStep>
void
UT_VoxelProbeAverage<T, XStep, YStep, ZStep>::setArray(const UT_VoxelArray<T> *vox)
{
    int		prex = (XStep < 0) ? XStep : 0;
    int 	postx = (XStep > 0) ? XStep : 0;

    myLines[0][0].setArray((UT_VoxelArray<T> *)vox, prex, postx);
    if (YStep)
    {
	myLines[1][0].setArray((UT_VoxelArray<T> *)vox, prex, postx);
	if (ZStep)
	{
	    myLines[1][1].setArray((UT_VoxelArray<T> *)vox, prex, postx);
	}
    }
    if (ZStep)
	myLines[0][1].setArray((UT_VoxelArray<T> *)vox, prex, postx);
}

template <typename T, int XStep, int YStep, int ZStep>
bool
UT_VoxelProbeAverage<T, XStep, YStep, ZStep>::setIndex(int x, int y, int z)
{
    bool		result = false;

    // Adjust x, y, and z according to our half step.
    // y and z negative steps require us decrementing.  x steps
    // do not require a change as we use the pre/post to affect this,
    // if we adjusted the actual x we would get twice the cache misses.
    if (YStep < 0)
	y--;
    if (ZStep < 0)
	z--;

    result |= myLines[0][0].setIndex(x, y, z);
    if (YStep)
    {
	result |= myLines[1][0].setIndex(x, y+1, z);
	if (ZStep)
	    result |= myLines[1][1].setIndex(x, y+1, z+1);
    }
    if (ZStep)
	result |= myLines[0][1].setIndex(x, y, z+1);

    return result;
}

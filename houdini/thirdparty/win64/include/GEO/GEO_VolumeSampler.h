/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_VolumeSampler.h (GEO Library, C++)
 *
 * COMMENTS:
 *	This is an adapter class that lets you efficiently
 *	sample both PrimVolumes and PrimVDBs.
 *
 *	It is *not* threadsafe to use the same volume sampler
 *	from multiple threads!
 */

#ifndef __GEO_VOLUMESAMPLER_H_INCLUDED__
#define __GEO_VOLUMESAMPLER_H_INCLUDED__

#include <UT/UT_VoxelArray.h>

#include "GEO_API.h"
#include "GEO_PrimVolume.h"
#include "GEO_PrimVDB.h"

class GEO_API GEO_VolumeSamplerVDBCache
{
public:
    GEO_VolumeSamplerVDBCache() {}
    virtual ~GEO_VolumeSamplerVDBCache() {}

    virtual bool    isActive(const UT_Vector3 &pos) = 0;
    virtual fpreal	getValueF(const UT_Vector3 &pos) = 0;
    virtual fpreal	getValueAtIndexF(int ix, int iy, int iz) = 0;
    virtual UT_Vector3D	getValueV3(const UT_Vector3 &pos) = 0;
};

class GEO_API GEO_VolumeSampler
{
public:
    GEO_VolumeSampler()
    {
	myVDBCache = 0;
	bindPrim(0);
    }
    GEO_VolumeSampler(const GEO_Primitive *prim)
    {
	myVDBCache = 0;
	bindPrim(prim);
    }

    GEO_VolumeSampler(const GEO_VolumeSampler &src)
    {
	myVDBCache = 0;
	bindPrim(src.myPrim);
    }
    ~GEO_VolumeSampler()
    {
	delete myVDBCache;
    }

    GEO_VolumeSampler &operator=(const GEO_VolumeSampler &src)
    {
	delete myVDBCache;
	myVDBCache = 0;
	bindPrim(src.myPrim);

	return *this;
    }

    bool        isBound()
    {
        return myVol != NULL || myVdb != NULL;
    }

    void		bindPrim(const GEO_Primitive *prim)
    {
	delete myVDBCache;
	myVDBCache = 0;

	myPrim = prim;
	myVol = dynamic_cast<const GEO_PrimVolume *>(prim);
	myVdb = dynamic_cast<const GEO_PrimVDB *>(prim);
	myVectorSize = 0;
	if (myVol)
	{
	    myHandle = myVol->getVoxelHandle();
	    myVectorSize = 1;
	}
	if (myVdb)
	{
	    myVectorSize = myVdb->getTupleSize();
	    myVDBCache = createVDBCache(myVdb);

	    if (!myVDBCache)
		myVdb = 0;
	}
    }

    int				getVectorSize() const { return myVectorSize; }

    /// Checks whether the given location falls within the volume's active
    /// region.
    inline bool             isActive(const UT_Vector3 &pos)
    {
        if(myVol)
            return myVol->isInsideStrict(pos, myHandle);
        if(myVdb)
            return myVDBCache->isActive(pos);

        return false;
    }

    inline fpreal			getValueF(const UT_Vector3 &pos)
    {
	if (myVol)
	    return myVol->getValue(pos, myHandle);
	if (myVdb)
	    return myVDBCache->getValueF(pos);

	return 0;
    }

    inline fpreal			getValueAtIndexF(int ix, int iy, int iz)
    {
	if (myVol)
	    return myHandle->getValue(ix, iy, iz);
	if (myVdb)
	    return myVDBCache->getValueAtIndexF(ix, iy, iz);

	return 0;
    }

    inline UT_Vector3D			getValueV3(const UT_Vector3 &pos)
    {
	if (myVol)
	    return UT_Vector3D(myVol->getValue(pos, myHandle), 0, 0);
	if (myVdb)
	    return myVDBCache->getValueV3(pos);

	return UT_Vector3D(0, 0, 0);
    }

    static GEO_VolumeSamplerVDBCache *createVDBCache(const GEO_PrimVDB *vdb);

protected:
    const GEO_Primitive			*myPrim;
    const GEO_PrimVolume		*myVol;
    const GEO_PrimVDB			*myVdb;
    UT_VoxelArrayReadHandleF		 myHandle;
    int					 myVectorSize;
    GEO_VolumeSamplerVDBCache		*myVDBCache;
};

#endif // __GEO_VOLUMESAMPLER_H_INCLUDED__


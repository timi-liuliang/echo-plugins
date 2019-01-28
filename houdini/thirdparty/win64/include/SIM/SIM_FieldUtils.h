/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_FieldUtils.h ( SIM Library, C++)
 *
 * COMMENTS:
 *	Templated functions for common behaviours
 *	in SIM_Fields.
 */

#ifndef __SIM_FieldUtils__
#define __SIM_FieldUtils__

#include <GU/GU_Types.h>

#include "SIM_ScalarField.h"
#include "SIM_MatrixField.h"
#include "SIM_VectorField.h"
#include "SIM_IndexField.h"

///
/// Returns true if there is a slice to be performed.
/// The slice is the half inclusive interval [minvxl, maxvxl)
/// 
inline bool
SIMfieldUtilsComputeSliceWithBorder(const UT_Vector3 &slice, const UT_Vector3 &totaldiv, UT_Vector3 overlapneg, UT_Vector3 overlappos, int slicenum, UT_Vector3 &minvxl, UT_Vector3 &maxvxl)
{
    if (slice.isEqual(UT_Vector3(1, 1, 1)))
    {
	minvxl = UT_Vector3(0, 0, 0);
	maxvxl = totaldiv;
	// Trivially unsliced
	return false;
    }

    int			sx, sy, sz;
    int			x, y, z;

    sx = SYSrint(slice(0));
    sy = SYSrint(slice(1));
    sz = SYSrint(slice(2));

    if (slicenum < 0 || slicenum >= sx*sy*sz)
    {
	// Unclear if this should be impossible via UI, and if so,
	// how to make it so.
	slicenum = 0;
    }
    x = slicenum % sx;
    slicenum -= x;
    slicenum /= sx;
    y = slicenum % sy;
    slicenum -= y;
    slicenum /= sy;
    z = slicenum;

    minvxl.x() = SYSrint( x * (totaldiv.x() / sx) );
    maxvxl.x() = SYSrint( (x+1) * (totaldiv.x() / sx) );
    minvxl.y() = SYSrint( y * (totaldiv.y() / sy) );
    maxvxl.y() = SYSrint( (y+1) * (totaldiv.y() / sy) );
    minvxl.z() = SYSrint( z * (totaldiv.z() / sz) );
    maxvxl.z() = SYSrint( (z+1) * (totaldiv.z() / sz) );

    // Adjust for our borders.
    minvxl -= overlapneg;
    maxvxl += overlappos;
    
    minvxl.x() = SYSclamp(minvxl.x(), 0.0, totaldiv.x());
    maxvxl.x() = SYSclamp(maxvxl.x(), 0.0, totaldiv.x());
    minvxl.y() = SYSclamp(minvxl.y(), 0.0, totaldiv.y());
    maxvxl.y() = SYSclamp(maxvxl.y(), 0.0, totaldiv.y());
    minvxl.z() = SYSclamp(minvxl.z(), 0.0, totaldiv.z());
    maxvxl.z() = SYSclamp(maxvxl.z(), 0.0, totaldiv.z());

    // Special case: we need at least one voxel in each direction.
    if (maxvxl.x() - minvxl.x() < 1)
	maxvxl.x()++;
    if (maxvxl.y() - minvxl.y() < 1)
	maxvxl.y()++;
    if (maxvxl.z() - minvxl.z() < 1)
	maxvxl.z()++;

    // Clamp again...
    maxvxl.x() = SYSclamp(maxvxl.x(), 0.0, totaldiv.x());
    maxvxl.y() = SYSclamp(maxvxl.y(), 0.0, totaldiv.y());
    maxvxl.z() = SYSclamp(maxvxl.z(), 0.0, totaldiv.z());

    // Verify we are still valid.
    if (maxvxl.x() - minvxl.x() < 1)
	minvxl.x()--;
    if (maxvxl.y() - minvxl.y() < 1)
	minvxl.y()--;
    if (maxvxl.z() - minvxl.z() < 1)
	minvxl.z()--;

    return true;
}

template <typename FIELDTYPE>
bool
SIMfieldUtilsComputeSliceWithBorder(const FIELDTYPE *field, const UT_Vector3 &totaldiv, UT_Vector3 overlapneg, UT_Vector3 overlappos, int slicenum, UT_Vector3 &minvxl, UT_Vector3 &maxvxl)
{
    UT_Vector3		slice = field->getSliceDivisions();

    return SIMfieldUtilsComputeSliceWithBorder(slice, totaldiv, overlapneg, overlappos, slicenum, minvxl, maxvxl);
}


template <typename FIELDTYPE>
bool
SIMfieldUtilsComputeSlice(const FIELDTYPE *field, const UT_Vector3 &totaldiv, int slicenum, UT_Vector3 &minvxl, UT_Vector3 &maxvxl)
{
    return SIMfieldUtilsComputeSliceWithBorder(
	    field, totaldiv,
	    field->getSliceOverlapNeg(), field->getSliceOverlapPos(),
	    slicenum, minvxl, maxvxl);
}

/// Returns true if there is any overlap between the two
/// slices.  [minvxl, maxvxl)  is the region of overlap.
/// This is the intersection of the two slice fields.
template <typename FIELDTYPE>
bool
SIMfieldUtilsGetSliceBorder(const FIELDTYPE *field,
			int a_slice, int b_slice,
			UT_Vector3 &minvxl, UT_Vector3 &maxvxl)
{
    UT_Vector3		a_min, a_max, b_min, b_max;
    UT_Vector3		div;

    div = SIMfieldUtilsGetDivisionsNoSlice(field);

    SIMfieldUtilsComputeSlice(field, div, a_slice, a_min, a_max);
    SIMfieldUtilsComputeSlice(field, div, b_slice, b_min, b_max);
    
    UT_BoundingBox	a_box(a_min, a_max), b_box(b_min, b_max);

    if (!a_box.computeIntersection(b_box))
	return false;

    minvxl = a_box.minvec();
    maxvxl = a_box.maxvec();

    return true;
}

template <typename FIELDTYPE>
UT_Vector3
SIMfieldUtilsGetDivisionsNoSlice(const FIELDTYPE *field)
{
    UT_Vector3		div;
    int			uniform;

    uniform = field->getUniformVoxels();
    if (uniform)
    {
	UT_Vector3		size, searchsize;
	int			axis, a;
	fpreal			voxelsize;
	fpreal			uniformdiv;

	size = field->getRawSize();
	uniformdiv = field->getRawUniformDivisions();

	searchsize = size;

	// Force voxels to be uniform, by hook or by crook.
	// This axis will be taken as authoritative.
	axis = uniform-1;

	if (field->getTwoDField())
	{
	    // We cannot use an axis that isn't represented.
	    // For maximum axis case, we clamp the non represented
	    // axis to 0 size to make sure we ignore it.
	    switch (field->getVoxelPlane())
	    {
		case GU_PLANE_XY:
		    if (axis == 2)
			axis = 0;
		    searchsize(2) = 0.0;
		    break;
		case GU_PLANE_YZ:
		    if (axis == 0)
			axis = 1;
		    searchsize(0) = 0.0;
		    break;
		case GU_PLANE_XZ:
		    if (axis == 1)
			axis = 2;
		    searchsize(1) = 0.0;
		    break;
	    }
	}

	// Check to see if they requested a maximum voxel option.
	if (axis == 3)
	{
	    axis = searchsize.findMaxAbsAxis();
	}

	// If the user is specifying by voxel size...
	if (axis == 4)
	{
	    voxelsize = field->getRawDivisionSize();
	}
	else
	{
	    voxelsize = size(axis) / uniformdiv;
	}


	// It is possible for the user to accidentally specify a 
	// zero size through use of a ladder handle, which, obviously,
	// leads to bad things.
	if (SYSequalZero(voxelsize))
	{
	    // This, presumeably, also means that all axes are
	    // the same as we found no maximum!
	    div = uniformdiv;
	}
	else
	{
	    // Determine the dimensions of the divisions.
	    for (a = 0; a < 3; a++)
	    {
		div(a) = SYSrint(size(a)/voxelsize);
		if (div(a) < 1.0)
		    div(a) = 1.0;
	    }
	    // div(axis) should evaluate to uniformdiv.
	    // The exception will be if we are currently building
	    // our divisions and thus have uniformdiv == 0.
	    // Similarly, if we have specified by voxel size, they
	    // will not match.
	    UT_ASSERT(!uniformdiv || (axis == 4) || SYSisEqual(div(axis), uniformdiv));
	}
    }
    else
    {
	div = field->getRawDivisions();
    }

    if (field->getTwoDField())
    {
	// Clamp to twod.
	switch (field->getVoxelPlane())
	{
	    case GU_PLANE_XY:
		div.z() = 1.0;
		break;
	    case GU_PLANE_YZ:
		div.x() = 1.0;
		break;
	    case GU_PLANE_XZ:
		div.y() = 1.0;
		break;
	}
    }

    return div;
}

template <typename FIELDTYPE>
UT_Vector3
SIMfieldUtilsGetDivisions(const FIELDTYPE *field)
{
    UT_Vector3		minvxl, maxvxl;
    UT_Vector3		div;

    div = SIMfieldUtilsGetDivisionsNoSlice(field);

    // Now compute divisions for our particular slice.
    SIMfieldUtilsComputeSlice(field, div, field->getSlice(), minvxl, maxvxl);

    maxvxl -= minvxl;

    return maxvxl;
}

template <typename FIELDTYPE>
UT_Vector3
SIMfieldUtilsGetSizeNoSlice(const FIELDTYPE *field)
{
    UT_Vector3		size;
    int			uniform;

    size = field->getRawSize();

    uniform = field->getUniformVoxels();

    if (uniform)
    {
	int		axis;
	fpreal		voxelsize;
	UT_Vector3	div;
	UT_Vector3	searchsize;

	searchsize = size;
	axis = uniform-1;

	if (field->getTwoDField())
	{
	    // We cannot use an axis that isn't represented.
	    switch (field->getVoxelPlane())
	    {
		case GU_PLANE_XY:
		    if (axis == 2)
			axis = 0;
		    searchsize(2) = 0;
		    break;
		case GU_PLANE_YZ:
		    if (axis == 0)
			axis = 1;
		    searchsize(0) = 0;
		    break;
		case GU_PLANE_XZ:
		    if (axis == 1)
			axis = 2;
		    searchsize(1) = 0;
		    break;
	    }
	}

	// Check to see if they requested a maximum voxel option.
	if (axis == 3)
	    axis = searchsize.findMaxAbsAxis();

	// Taking into account 2d and uniformality:
	div = SIMfieldUtilsGetDivisionsNoSlice(field);

	// Recompute voxelsize...
	if (axis == 4)
	    voxelsize = field->getRawDivisionSize();
	else
	    voxelsize = size(axis) / div(axis);

	// Now the other dimensions are a direct copy
	size = voxelsize * div;
    }

    // Size should never be zero.  We demand at least one voxel, so
    // should have computed a non-zero size if we had a non-zero voxel
    // size.  If we get a zero voxelsize, thi sis the cahcne to fix
    // it.  1.0 is a random number here, there is no good answer
    // in these cases.
    if (size.x() == 0)
	size.x() = 1;
    if (size.y() == 0)
	size.y() = 1;
    if (size.z() == 0)
	size.z() = 1;

    return size;
}

template <typename FIELDTYPE>
UT_Vector3
SIMfieldUtilsGetSize(const FIELDTYPE *field)
{
    UT_Vector3		size;
    UT_Vector3		div;
    UT_Vector3		minvxl, maxvxl;

    div = SIMfieldUtilsGetDivisionsNoSlice(field);
    size = SIMfieldUtilsGetSizeNoSlice(field);

    if (SIMfieldUtilsComputeSlice(field, div, field->getSlice(), minvxl, maxvxl))
    {
	UT_Vector3		voxelsize;

	voxelsize = size / div;

	// Get actual sliced size.
	maxvxl -= minvxl;

	// Recompute oursize from there.
	size = maxvxl * voxelsize;
    }

    return size;
}

template <typename FIELDTYPE>
UT_Vector3
SIMfieldUtilsGetCenter(const FIELDTYPE *field)
{
    UT_Vector3		size;
    UT_Vector3		div;
    UT_Vector3		minvxl, maxvxl;
    UT_Vector3		center;

    div = SIMfieldUtilsGetDivisionsNoSlice(field);
    center = field->getRawCenter();

    if (SIMfieldUtilsComputeSlice(field, div, field->getSlice(), minvxl, maxvxl))
    {
	UT_Vector3		voxelsize;

	size = SIMfieldUtilsGetSizeNoSlice(field);

	voxelsize = size / div;
	// My actual center is found by averaging min/maxvxl and
	// casting back into our large voxel space.
	minvxl += maxvxl;
	minvxl *= 0.5;

	// Make our coordinates relative to the center of the box
	minvxl -= div / 2;

	// Convert to space
	minvxl *= voxelsize;

	center += minvxl;
    }

    return center;
}


template <typename FIELDTYPE, typename F2>
void
SIMfieldUtilsMatch(FIELDTYPE *field, const F2 *srcfield)
{
    field->setUniformVoxels(srcfield->getUniformVoxels());
    field->setTwoDField(srcfield->getTwoDField());
    field->setVoxelPlane(srcfield->getVoxelPlane());

    field->setRawDivisions(srcfield->getRawDivisions());
    field->setRawDivisionSize(srcfield->getRawDivisionSize());
    field->setRawUniformDivisions(srcfield->getRawUniformDivisions());
    field->setRawSize(srcfield->getRawSize());
    field->setRawCenter(srcfield->getRawCenter());

    field->setSlice(srcfield->getSlice());
    field->setSliceDivisions(srcfield->getSliceDivisions());
    field->setSliceOverlapNeg(srcfield->getSliceOverlapNeg());
    field->setSliceOverlapPos(srcfield->getSliceOverlapPos());

    UT_String		pospath;
    srcfield->getPositionPath(pospath);
    field->setPositionPath(pospath);

    field->testForNan();

    field->updateTotalVoxels();

    field->setVoxelSize(srcfield->getVoxelSize());
}

inline void 
SIMfieldUtilsStealFields(SIM_RawIndexField **raw, SIM_IndexField *field)
{
    raw[0] = field->stealField();
}

inline void 
SIMfieldUtilsStealFields(SIM_RawField **raw, SIM_ScalarField *field)
{
    raw[0] = field->stealField();
}

inline void 
SIMfieldUtilsStealFields(SIM_RawField **raw, SIM_VectorField *field)
{
    for (int i = 0; i < 3; i++)
	raw[i] = field->stealField(i);
}

inline void 
SIMfieldUtilsStealFields(SIM_RawField **raw, SIM_MatrixField *field)
{
    for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	    raw[i*3+j] = field->stealField(i, j);
}

template <typename RAWFIELD>
void
SIMfieldUtilsExchangeFields(
		const char *address, int port,
		RAWFIELD *dest,
		RAWFIELD *src,
		int slicenum,
		int offx, int offy, int offz,
		UT_Vector3 olddiv, UT_Vector3 newdiv,
		UT_Vector3 slicediv,
		UT_Vector3 overlapneg, UT_Vector3 overlappos);

inline void 
SIMfieldUtilsCopyWithOffset(SIM_IndexField *field, SIM_RawIndexField **raw, 
			    int offx, int offy, int offz,
			    bool keepdata,
			    UT_Vector3	olddivvec, UT_Vector3 newdivvec,
			    const char *address,
			    int port)
{
    if (keepdata)
    {
	field->getField()->fieldNC()->copyWithOffset(*raw[0]->field(),
				offx, offy, offz);
	if (UTisstring(address) && port > 0)
	{
	    SIMfieldUtilsExchangeFields(
		    address, port,
		    field->getField(),
		    raw[0],
		    field->getSlice(),
		    offx, offy, offz,
		    olddivvec, newdivvec,
		    field->getSliceDivisions(),
		    field->getSliceOverlapNeg(), 
		    field->getSliceOverlapPos());
	}
	
    }
    delete raw[0];
}

inline void 
SIMfieldUtilsCopyWithOffset(SIM_ScalarField *field, SIM_RawField **raw, 
			    int offx, int offy, int offz,
			    bool keepdata,
			    UT_Vector3	olddivvec, UT_Vector3 newdivvec,
			    const char *address,
			    int port)
{
    if (keepdata)
    {
	field->getField()->fieldNC()->copyWithOffset(*raw[0]->field(),
				offx, offy, offz);
	if (UTisstring(address) && port > 0)
	{
	    SIMfieldUtilsExchangeFields(
		    address, port,
		    field->getField(),
		    raw[0],
		    field->getSlice(),
		    offx, offy, offz,
		    olddivvec, newdivvec,
		    field->getSliceDivisions(),
		    field->getSliceOverlapNeg(), 
		    field->getSliceOverlapPos());
	}
    }
    delete raw[0];
}

inline void 
SIMfieldUtilsCopyWithOffset(SIM_VectorField *field, SIM_RawField **raw, 
			    int offx, int offy, int offz,
			    bool keepdata,
			    UT_Vector3	olddivvec, UT_Vector3 newdivvec,
			    const char *address,
			    int port)
{
    for (int i = 0; i < 3; i++)
    {
	if (keepdata)
	{
	    field->getField(i)->fieldNC()->copyWithOffset(*raw[i]->field(),
				    offx, offy, offz);
	    if (UTisstring(address) && port > 0)
	    {
		SIMfieldUtilsExchangeFields(
			address, port,
			field->getField(i),
			raw[i],
			field->getSlice(),
			offx, offy, offz,
			olddivvec, newdivvec,
			field->getSliceDivisions(),
			field->getSliceOverlapNeg(), 
			field->getSliceOverlapPos());
	    }
	}
	delete raw[i];
    }
}

inline void 
SIMfieldUtilsCopyWithOffset(SIM_MatrixField *field, SIM_RawField **raw, 
			    int offx, int offy, int offz,
			    bool keepdata,
			    UT_Vector3	olddivvec, UT_Vector3 newdivvec,
			    const char *address,
			    int port)
{
    for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	{
	    if (keepdata)
	    {
		field->getField(i, j)->fieldNC()->copyWithOffset(*raw[i*3+j]->field(),
					offx, offy, offz);

		if (UTisstring(address) && port > 0)
		{
		    SIMfieldUtilsExchangeFields(
			    address, port,
			    field->getField(i, j),
			    raw[i*3+j],
			    field->getSlice(),
			    offx, offy, offz,
			    olddivvec, newdivvec,
			    field->getSliceDivisions(),
			    field->getSliceOverlapNeg(), 
			    field->getSliceOverlapPos());
		}
	    }
	    delete raw[i*3+j];
	}
}

template <typename FIELDTYPE>
void
SIMfieldUtilsResizeKeepDataAndSnap(FIELDTYPE *field,
		    UT_Vector3 size, UT_Vector3 center,
		    bool keepdata,
		    const char *address,
		    int port,
		    bool &snapvalid,
		    UT_Vector3 &snapsize, UT_Vector3 &snapcenter)
{
    UT_Vector3		 voxelsize, newsize, newcenter;
    UT_Vector3		 oldcenter, oldsize, offset;
    typename FIELDTYPE::rawfield_type	*oldfields[9];
    int			 voxeloffset[3];
    int			 newdiv[3], olddiv[3], uniformdiv, axis;
    UT_Vector3		 olddivvec, searchsize;

    voxelsize = field->getVoxelSize();

    if (SYSequalZero(voxelsize.x()) ||
        SYSequalZero(voxelsize.y()) ||
	SYSequalZero(voxelsize.z()))
    {
	// Zero sized voxels can't be resized as we can't
	// meaningfully find a new size for them.
	return;
    }

    // We need the unsliced sizes.
    oldcenter = field->getRawCenter();
    oldsize = SIMfieldUtilsGetSizeNoSlice(field);
    olddivvec = SIMfieldUtilsGetDivisionsNoSlice(field);
    olddiv[0] = (int) SYSrint(olddivvec.x());
    olddiv[1] = (int) SYSrint(olddivvec.y());
    olddiv[2] = (int) SYSrint(olddivvec.z());

    // Find what range this slice consisted of before the slice.
    UT_Vector3		oldslicemin, oldslicemax;
    SIMfieldUtilsComputeSlice(field, olddivvec, field->getSlice(), oldslicemin, oldslicemax);

    // This is the offset into voxel space.  We basically want
    // to quantize all of our coordinates to be integer voxelsize
    // steps from this offset.
    // Note it is not the old center, as it is valid to center 
    // on the mid point of a voxel if we have an odd number of 
    // voxels!  Thus it is quanitzed as per the bottom left.
    UT_Vector3		 oldoffset;

    oldoffset = oldcenter - oldsize/2.0f;

    // Get the coordinates of our new boxes edges and quantize to
    // exact voxel numbers.
    UT_Vector3		newbot, newtop;

    newbot = center - size/2;
    newtop = center + size/2;

    newbot -= oldoffset;
    newtop -= oldoffset;
    newbot /= voxelsize;
    newtop /= voxelsize;
    // It is depressingly common for things to exactly match up
    // due to users using a size 0.1 and unit boxes.
    newtop.x() = SYSrint(newtop.x() + 0.0001);
    newtop.y() = SYSrint(newtop.y() + 0.0001);
    newtop.z() = SYSrint(newtop.z() + 0.0001);
    newbot.x() = SYSrint(newbot.x() - 0.0001);
    newbot.y() = SYSrint(newbot.y() - 0.0001);
    newbot.z() = SYSrint(newbot.z() - 0.0001);
    newbot *= voxelsize;
    newtop *= voxelsize;
    newbot += oldoffset;
    newtop += oldoffset;

    // We can now recompute our desired center and size from
    // these quantized values.
    newcenter = (newbot + newtop) / 2;
    newsize = newtop - newbot;

    // Calculate our new size as an integer number of voxels from the
    // old size.
    newdiv[0] = SYSrint(newsize.x()/voxelsize.x());
    newdiv[1] = SYSrint(newsize.y()/voxelsize.y());
    newdiv[2] = SYSrint(newsize.z()/voxelsize.z());

    // We can't have zero divisions on any axis.
    // Note we need to bump our center half a voxel in this case
    // because it needs to now be voxel centered.
    if (newdiv[0] == 0)
    {
	newcenter.x() -= voxelsize.x()/2;
	newdiv[0] = 1;
    }
    if (newdiv[1] == 0)
    {
	newcenter.y() -= voxelsize.y()/2;
	newdiv[1] = 1;
    }
    if (newdiv[2] == 0)
    {
	newcenter.z() -= voxelsize.z()/2;
	newdiv[2] = 1;
    }

    // Compute our new size from the new divisions.
    newsize.x() = newdiv[0] * voxelsize.x();
    newsize.y() = newdiv[1] * voxelsize.y();
    newsize.z() = newdiv[2] * voxelsize.z();

    // We refuse to move the voxel plane if it is 2d.
    if (field->getTwoDField())
    {
	switch (field->getVoxelPlane())
	{
	    case GU_PLANE_XY:
		newcenter.z() = oldcenter.z();
		break;
	    case GU_PLANE_YZ:
		newcenter.x() = oldcenter.x();
		break;
	    case GU_PLANE_XZ:
		newcenter.y() = oldcenter.y();
		break;
	}
    }

    // Check to see if we have a no-op.
    if (oldcenter.isEqual(newcenter) && 
	olddiv[0] == newdiv[0] &&
	olddiv[1] == newdiv[1] &&
	olddiv[2] == newdiv[2])
	return;

    // Before we adjust our settings, we better steal the old
    // field as this will cause it to be rebuilt!
    SIMfieldUtilsStealFields(oldfields, field);

    // Reset our raw division and uniform division choices
    // so we'll compute newdiv in the end.
    field->setRawDivisions(UT_Vector3(newdiv[0], newdiv[1], newdiv[2]));

    uniformdiv = newdiv[0];
    axis = field->getUniformVoxels()-1;

    // Avoid non-existent axes:
    searchsize = newsize;
    if (field->getTwoDField())
    {
	switch (field->getVoxelPlane())
	{
	    case GU_PLANE_XY:
		if (axis == 2)
		    axis = 0;
		searchsize(2) = 0.0;
		break;
	    case GU_PLANE_YZ:
		if (axis == 0)
		    axis = 1;
		searchsize(0) = 0.0;
		break;
	    case GU_PLANE_XZ:
		if (axis == 1)
		    axis = 2;
		searchsize(1) = 0.0;
		break;
	}
    }

    // Check to see if they requested a maximum voxel option.
    if (axis == 3)
    {
	axis = searchsize.findMaxAbsAxis();
    }

    // Read up the appropriate choice
    if (axis >= 0 && axis < 3)
	uniformdiv = newdiv[axis];

    field->setRawUniformDivisions(uniformdiv);

    // We do not need to set our DivisionSize since it is unaffected.
    if (snapvalid)
    {
	if (snapcenter.isEqual(newcenter) &&
	    snapsize.isEqual(newsize))
	{
	    newcenter = snapcenter;
	    newsize = snapsize;
	}
    }
    snapcenter = newcenter;
    snapsize = newsize;
    snapvalid = true;

    field->setCenter(newcenter);
    // Not setSize as we want to keep 2d/uniform!
    field->setRawSize(newsize);
    // This is not the same as it will round off voxels and deal
    // with 2d!
    newsize = SIMfieldUtilsGetSizeNoSlice(field);
    UT_Vector3 newdivvec = SIMfieldUtilsGetDivisionsNoSlice(field);

    // Now fill in the values...
    // We determine a voxel offset between our two fields
    offset = (newcenter - newsize/2) - (oldcenter - oldsize / 2);
    offset /= voxelsize;

    // field[x] = oldfield[x+offset];
    // However, oldfield is relative to oldslicemin, new field to
    // newslicemin
    UT_Vector3		newslicemin, newslicemax;
    SIMfieldUtilsComputeSlice(field, newdivvec, field->getSlice(), newslicemin, newslicemax);

    offset += newslicemin - oldslicemin;

    voxeloffset[0] = SYSrint(offset.x());
    voxeloffset[1] = SYSrint(offset.y());
    voxeloffset[2] = SYSrint(offset.z());

    // Filling in is very simple.
    SIMfieldUtilsCopyWithOffset(field, oldfields,
				voxeloffset[0],
				voxeloffset[1],
				voxeloffset[2],
				keepdata,
				olddivvec, newdivvec,
				address, port);

    field->updateTotalVoxels();

    field->pubHandleModification();

    field->setVoxelSize(voxelsize);
}

template <typename FIELDTYPE>
void
SIMfieldUtilsResizeKeepData(FIELDTYPE *field,
		    UT_Vector3 size, UT_Vector3 center,
		    bool keepdata,
		    const char *address,
		    int port)
{
    bool		snapvalid = false;
    UT_Vector3		snapcenter, snapsize;
    SIMfieldUtilsResizeKeepDataAndSnap(field, size, center,
			keepdata, address, port,
			snapvalid, snapsize, snapcenter);

}


#endif


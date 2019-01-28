/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_MatrixField.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_MatrixField__
#define __SIM_MatrixField__

#include "SIM_API.h"

#include <UT/UT_VoxelArray.h>

#include "SIM_Names.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"
#include "SIM_RawField.h"

class UT_IStream;
class SIM_Geometry;
class SIM_ScalarField;
class SIM_VectorField;
class SIM_IndexField;

/// This class holds a three dimensional tensor field.  
class SIM_API SIM_MatrixField : public SIM_Data,
			public SIM_OptionsUser
{
public:
    typedef SIM_RawField	 rawfield_type;

    /// Accesses the relative path to the position data associated with
    /// this geometry.
    GETSET_DATA_FUNCS_S(SIM_NAME_POSITIONPATH, PositionPath);

    /// Control the number of divisions.
    GETSET_DATA_FUNCS_I(SIM_NAME_UNIFORMVOXELS, UniformVoxels);
    GETSET_DATA_FUNCS_B(SIM_NAME_TWOD, TwoDField);
    GETSET_DATA_FUNCS_I(SIM_NAME_VOXELPLANE, VoxelPlane);
    GETSET_DATA_FUNCS_V3(SIM_NAME_DIV, RawDivisions);
    GETSET_DATA_FUNCS_I("uniformdiv", RawUniformDivisions);
    GETSET_DATA_FUNCS_F("divsize", RawDivisionSize);
    GETSET_DATA_FUNCS_V3(SIM_NAME_CENTER, RawCenter);
    GETSET_DATA_FUNCS_V3(SIM_NAME_SIZE, RawSize);

    GETSET_DATA_FUNCS_V3("slicediv", SliceDivisions);
    GETSET_DATA_FUNCS_V3("sliceoverlapneg", SliceOverlapNeg);
    GETSET_DATA_FUNCS_V3("sliceoverlappos", SliceOverlapPos);
    GETSET_DATA_FUNCS_I("slice", Slice)
    exint	getNumSlices() const { UT_Vector3D nslice = getSliceDivisions(); return exint(nslice.x() * nslice.y() * nslice.z()); }

    GETSET_DATA_FUNCS_I("totalvoxels", TotalVoxels);
    GETSET_DATA_FUNCS_V3("totalvoxelres", TotalVoxelRes);

    GETSET_DATA_FUNCS_F(SIM_NAME_TOLERANCE, Tolerance);
    GETSET_DATA_FUNCS_B("usefp16", UseFP16);
    GETSET_DATA_FUNCS_I(SIM_NAME_VOXELSAMPLE, VoxelSampleRaw);
    GETSET_DATA_FUNCS_I("border", RawBorder);
    UT_VoxelBorderType	getBorder() const { return (UT_VoxelBorderType) getRawBorder(); }
    void		setBorder(UT_VoxelBorderType border) { setRawBorder(border); }
    GETSET_DATA_FUNCS_M3("initialvalue", InitialValue);

    /// Controls the dimensions of where the field is properly defined
    /// in the field space.
    void		 getBBox(UT_BoundingBox &bbox) const;

    UT_Vector3		 getOrig() const
			 {
			     return getCenter() - getSize()/2;
			 }

    /// Calculate the size and divisions according to options
    /// such as 2d or equal sized voxels.
    UT_Vector3		 getDivisions() const;
    UT_Vector3		 getSize() const;
    UT_Vector3		 getCenter() const;

    /// Adjusts the size/divisions of this field, overriding
    /// and twod or uniform voxel settings.
    void		 setDivisions(const UT_Vector3 &div);
    void		 setSize(const UT_Vector3 &div);
    void		 setCenter(const UT_Vector3 &div);

    /// Resizes our field keeping our field data.
    /// The final size will be an integer number of voxels matching
    /// our current voxel size.  The final center will be an integer
    /// number of voxel offset from our current center.  This allows
    /// us to do a perfect copy of the data.
    void		 resizeKeepData(const UT_Vector3 &size, const UT_Vector3 &center, bool keepdata, const char *address = 0, int port = -1);

    /// Match this field to the given reference field.  We will
    /// end up with the same size/divisions/twod/uniform,
    /// but not the same sampling pattern
    void		 matchField(const SIM_ScalarField *field);
    void		 matchField(const SIM_VectorField *field);
    void		 matchField(const SIM_MatrixField *field, bool matchsamples=false);
    void		 matchField(const SIM_IndexField *field);

    bool		 isAligned(const SIM_ScalarField *field) const;
    bool		 isAligned(const SIM_VectorField *field) const;
    bool		 isAligned(const SIM_RawField *field) const;
    /// True if we are component wise aligned, the subfields may still
    /// not be aligned with respect to each other.
    bool		 isAligned(const SIM_MatrixField *field) const;

    SIM_FieldSample	 getVoxelSample(int i, int j) const;
    const UT_Vector3	&getVoxelSize(int i, int j) const;
    fpreal		 getVoxelDiameter(int i, int j) const;
    // Independent of sampling pattern, so constant between fields.
    UT_Vector3		 getVoxelSize() const { return getVoxelSize(0, 0); }
    void		 setVoxelSize(const UT_Vector3 &voxelsize)
			 { for (int i = 0; i < 3; i++)
			     for (int j = 0; j < 3; j++)
				 myFields[i][j]->setVoxelSize(voxelsize);
			 }

    /// Access the field value given a world space location.
    /// This does trilinear interpolation.
    UT_Matrix3		 getValue(const UT_Vector3 &pos) const;

    /// Gets the velocity at the given *voxel* location, interpolating
    /// if we have corner or face velocities.
    UT_Matrix3		 getCellValue(int x, int y, int z) const;

    /// Adds a velocity to the given *voxel*.  If this is face,
    /// it is divided in two and spread on each of 6 faces.  If it is
    /// corner, it is divided by 8 and spread along each of 8 corners.
    void		 addToCell(int x, int y, int z, const UT_Matrix3 &dv);

    /// Advects this field by the other given field.  
    void		 advect(const SIM_VectorField *vel, float timestep,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);
    void		 advect(sim_PointVelocity getVelocity, float timestep,
				float voxelsize,
				const SIM_RawField *collision = 0,
				float cfl = 1.0f);

    /// Advects this by the velocity field, storing our min/max
    /// interpolants into the min/max fields
    void		 advectMinMax(SIM_MatrixField *minfield,
				SIM_MatrixField *maxfield,
				const SIM_VectorField *vel, float timestep,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);

    /// Enforces boundary conditions on the array.
    void		 enforceBoundary(const SIM_ScalarField *collision=0,
			    const SIM_MatrixField *cvel = 0,
                            const SIM_MatrixField *bvel = 0);

    /// Converts an integer index into a worldspace position.
    bool		 indexToPos(int i, int j, int x, int y, int z, UT_Vector3 &pos) const;

    /// Converts a worldspace position into an integer index.
    bool		 posToIndex(int i, int j, const UT_Vector3 &pos, int &x, int &y, int &z) const;

    /// Retrieve raw field.
    SIM_RawField		*getField(int i, int j) const { return myFields[i][j]; }
    
    /// Sets the field to the given field, gaining ownership of it.
    /// The new field must already match the field it will replace.
    void			 setField(int i, int j, SIM_RawField *field);

    /// True if we contain any NANs
    bool			 hasNan() const 
    { 
	int		i, j;

	for (i = 0; i < 3; i++)
	    for (j = 0; j < 3; j++)
		if (myFields[i][j]->hasNan())
		    return true;
	return false;
    }

    /// True if we have a constant value.  Ignores end conditions
    /// in determining this.  Used as a rough guess that the field
    /// is unused.
    bool			 appearsToBeUnused() const
    { 
	for (int i = 0; i < 3; i++) 
	    for (int j = 0; j < 3; j++) 
		if (!getField(i, j)->field()->isConstant(0)) return false;
	return true;
    }

    /// Steals the field, replacing this copy with an empty field and
    /// Steals the field, replacing this copy with an empty field and
    /// returning the old version.
    SIM_RawField		*stealField(int i, int j);

    void			 testForNan() const;
    void		 pubHandleModification()
			 {
			     testForNan();
			     handleModification();
			 }
    void		 markGridAsChanged()
			 { for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) getField(i, j)->markGridAsChanged(); }

    /// Recomputes total number of voxels to be stored
    /// on our options data for ease of reading
    void		updateTotalVoxels();

    /// Creates a GDP with us as a Volume Primitive inside it.
    GU_ConstDetailHandle	 createSmokeRepresentation(const SIM_RootData &root) const;

    /// Adds a volume primitive version of our field to the given
    /// gdp.
    void			 addSmokeRepresentation(const SIM_RootData &root, GU_Detail *gdp) const;

protected:
    explicit		 SIM_MatrixField(const SIM_DataFactory *factory);
    virtual		~SIM_MatrixField();

    /// Overrides to properly implement this class as a SIM_Data.
    virtual void	 initializeSubclass();
    /// myField aware copy constructor.
    virtual void	 makeEqualSubclass(const SIM_Data *source);

    /// Saves our attributes, and our internal data if it has been set.
    virtual void	 saveSubclass(std::ostream &os) const;
    /// Loads our attributes and internal data if it was set when we saved.
    virtual bool	 loadSubclass(UT_IStream &is);

    virtual int64	 getMemorySizeSubclass() const;

    /// Override the setDivisions to rebuild our voxel array on demand.
    virtual void	 optionChangedSubclass(const char *name);

private:

    static const SIM_DopDescription	*getMatrixFieldDopDescription();

    SIM_RawField	*myFields[3][3];

    void		 rebuildFields();

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_MatrixField,
			SIM_Data,
			"MatrixField",
			getMatrixFieldDopDescription());
};
#endif


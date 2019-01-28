/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ScalarField.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_ScalarField__
#define __SIM_ScalarField__

#include "SIM_API.h"

#include <UT/UT_VoxelArray.h>

#include "SIM_Names.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"
#include "SIM_RawField.h"

class UT_IStream;
class SIM_Geometry;
class SIM_VectorField;
class SIM_MatrixField;
class SIM_IndexField;

/// This class holds a three dimensional scalar field.  
class SIM_API SIM_ScalarField : public SIM_Data,
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

    GETSET_DATA_FUNCS_I(SIM_NAME_VOXELSAMPLE, VoxelSampleRaw);
    GETSET_DATA_FUNCS_F("initialvalue", InitialValue);
    GETSET_DATA_FUNCS_F(SIM_NAME_TOLERANCE, Tolerance);
    GETSET_DATA_FUNCS_B("usefp16", UseFP16);
    GETSET_DATA_FUNCS_I("border", RawBorder);
    UT_VoxelBorderType	getBorder() const { return (UT_VoxelBorderType) getRawBorder(); }
    void		setBorder(UT_VoxelBorderType border) { setRawBorder(border); }

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
    void		 setSize(const UT_Vector3 &size);
    void		 setCenter(const UT_Vector3 &center);

    /// Resizes our field keeping our field data.
    /// The final size will be an integer number of voxels matching
    /// our current voxel size.  The final center will be an integer
    /// number of voxel offset from our current center.  This allows
    /// us to do a perfect copy of the data.
    void		 resizeKeepData(const UT_Vector3 &size, const UT_Vector3 &center, bool keepdata, const char *address = 0, int port = -1);

    /// Match this field to the given reference field.  We will
    /// end up with the same size/divisions/twod/uniform,
    /// but not the same sampling pattern
    /// This will destroy our field data.
    void		 matchField(const SIM_ScalarField *field, bool matchsample = false);
    void		 matchField(const SIM_VectorField *field);
    void		 matchField(const SIM_MatrixField *field);
    void		 matchField(const SIM_IndexField *field);

    void		 matchVolume(const GEO_PrimVolume *vol, const UT_DMatrix4 &xform);
    void		 matchVDB(const GEO_PrimVDB *vdb, const UT_DMatrix4 &xform);

    SIM_FieldSample	 getVoxelSample() const;
    void		 setVoxelSample(SIM_FieldSample sample);

    const UT_Vector3	&getVoxelSize() const { return myField->getVoxelSize(); }
    void		 setVoxelSize(const UT_Vector3 &voxelsize)
			 { myField->setVoxelSize(voxelsize); }
    fpreal		 getVoxelDiameter() const { return myField->getVoxelDiameter(); }

    /// Access the field value given a world space location.
    /// This does trilinear interpolation.
    fpreal		 getValue(const UT_Vector3 &pos) const;

    /// Determines the gradient at the given location.
    UT_Vector3		 getGradient(const UT_Vector3 &pos) const;

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
    void		 advectMinMax(SIM_ScalarField *minfield,
				SIM_ScalarField *maxfield,
				const SIM_VectorField *vel, float timestep,
				const SIM_RawField *collision,
				SIM_FieldAdvection advectmethod,
				float cfl);

    /// Enforces the boundary condition with the given collision mask
    void		 enforceBoundary(const SIM_ScalarField *collision=0,
					 const SIM_ScalarField *colvalue=0,
                                         const SIM_ScalarField *boundvalue=0);

    /// Converts an integer index into a worldspace position.
    bool		 indexToPos(int x, int y, int z, UT_Vector3 &pos) const;

    /// Converts a worldspace position into an integer index.
    bool		 posToIndex(const UT_Vector3 &pos, int &x, int &y, int &z) const;

    /// Retrieve raw field.
    SIM_RawField		*getField() const { return myField; };

    /// Sets the field to the given field, gaining ownership of it.
    void			 setField(SIM_RawField *field);

    /// True if we contain any NANs
    bool			 hasNan() const { return getField()->hasNan(); }

    /// True if we have a constant value.  Ignores end conditions
    /// in determining this.  Used as a rough guess that the field
    /// is unused.
    bool			 appearsToBeUnused() const
    { return getField()->field()->isConstant(0); }

    /// Steals the field, replacing this copy with an empty field and
    /// returning the old version.
    SIM_RawField		*stealField();

    void			 testForNan() const;

    /// Signals to the field that it has been altered externally.
    void		 pubHandleModification()
			 {
			     testForNan();
			     handleModification();
			 }
    void		 markGridAsChanged()
			 { getField()->markGridAsChanged(); }

    /// Recomputes total number of voxels to be stored
    /// on our options data for ease of reading
    void		updateTotalVoxels();

    /// Creates a GDP with us as a Volume Primitive inside it.
    GU_ConstDetailHandle	 createSmokeRepresentation(const SIM_RootData &root) const;

    /// Adds a volume primitive version of our field to the given
    /// gdp.
    void			 addSmokeRepresentation(const SIM_RootData &root, GU_Detail *gdp) const;

protected:
    explicit		 SIM_ScalarField(const SIM_DataFactory *factory);
    virtual		~SIM_ScalarField();

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
    static const SIM_DopDescription	*getScalarFieldDopDescription();

    /// Rebuilds our raw field so it matches our current parameters.
    void		 		 rebuildField();

    SIM_RawField			*myField;

    /// When creating a new scalar field, we do the following:
    /// 1) Load default values, triggering rebuildField()
    /// 2) Call initialize()
    /// 3) Load actual parameters values, triggering rebuildField()
    /// We need to set the initial value in the third stage.  The
    /// problem is that if there is a non-zero default and the other
    /// parameters are all default, stage 3 will be skipped.  We thus
    /// track our last default to avoid this case.
    fpreal				 myStashedInitialValue;

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_ScalarField,
			SIM_Data,
			"ScalarField",
			getScalarFieldDopDescription());
};
#endif


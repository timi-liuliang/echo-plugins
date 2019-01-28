/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_SDF.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	This simple procedure allows for intersection with signed
 *	distance functions.
 */

#ifndef __SIM_SDF_H__
#define __SIM_SDF_H__

#include "SIM_API.h"
#include <GU/GU_SDF.h>

#include "SIM_Names.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

class UT_IStream;
class SIM_Geometry;

/// This class holds a signed distance function representing a GU_Detail.
class SIM_API SIM_SDF : public SIM_Data,
			public SIM_OptionsUser
{
public:
    /// Control the number of divisions.
    GETSET_DATA_FUNCS_I(SIM_NAME_MODE, Mode);
    GETSET_DATA_FUNCS_I(SIM_NAME_UNIFORMVOXELS, UniformVoxels);
    GETSET_DATA_FUNCS_V3(SIM_NAME_DIV, Divisions);
    GETSET_DATA_FUNCS_I("uniformdiv", UniformDivisions);
    GETSET_DATA_FUNCS_F("divsize", DivisionSize);
    GETSET_DATA_FUNCS_B(SIM_NAME_LASERSCAN, LaserScan);
    GETSET_DATA_FUNCS_B(SIM_NAME_FIXSIGNS, FixSigns);
    GETSET_DATA_FUNCS_B(SIM_NAME_FORCEBOUNDS, ForceBounds);
    GETSET_DATA_FUNCS_F(SIM_NAME_OFFSET, Offset);
    GETSET_DATA_FUNCS_F(SIM_NAME_TOLERANCE, Tolerance);
    GETSET_DATA_FUNCS_B(SIM_NAME_INVERT, Invert);
    GETSET_DATA_FUNCS_I("sweepcount", SweepCount);
    GETSET_DATA_FUNCS_F("sweepalpha", SweepThreshold);

    /// Get the SDF.
    const GU_SDF		&getSDF() const;

    GU_SDFDelayedBuilder	 getDelayedSDF() const;

    /// Create a GU_Detail with the SDF present as a volume
    /// primitive
    GU_ConstDetailHandle	 createSmokeRepresentation(const SIM_RootData &root) const;
    void			 addSmokeRepresentation(const SIM_RootData &root, GU_Detail *gdp) const;

    /// Forcibly changes the SDF to the given mapping.
    void			 setSDF(GU_SDF *sdf);

    /// Steals the current SDF, replacing it with a blank one.
    /// This is used to allow the IsoOffset SOP to extract an SDF
    /// loaded into a simulation without having to copy it.
    GU_SDF			*stealSDF();

    /// Returns voxel mipmap for this sdf.
    UT_VoxelMipMapF		*getMipMap() const;

protected:
    explicit		 SIM_SDF(const SIM_DataFactory *factory);
    virtual		~SIM_SDF();

    /// Overrides to properly implement this class as a SIM_Data.
    virtual void	 initializeSubclass();

    /// Computes the right number of divisions/bounding box size
    /// for incoming geometry
    void		 computeDivisions(const GU_Detail *gdp, UT_BoundingBox &bbox, int &divx, int &divy, int &divz);

    /// Saves our attributes, and our internal data if it has been set.
    virtual void	 saveSubclass(std::ostream &os) const;
    /// Loads our attributes and internal data if it was set when we saved.
    virtual bool	 loadSubclass(UT_IStream &is);

    virtual int64	 getMemorySizeSubclass() const;
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);
    virtual SIM_Guide	*createGuideObjectSubclass() const;
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

private:
    /// Builds the SDF from the given geometry.
    void				 buildSDF(const SIM_Geometry *geo);
    static const SIM_DopDescription	*getSDFDopDescription();

    GU_SDF		 	*mySDF;
    UT_VoxelMipMapF		*myMipMap;

    /// Flags that we are trying to do a delayed build.
    mutable GU_SDFDelayedBuilder	*myBuilder;

    // Used to build an isosurface:
    static float		 pointDensity(const UT_Vector3 &pos, void *);

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_SDF,
			SIM_Data,
			"Volume",
			getSDFDopDescription());
};

#endif

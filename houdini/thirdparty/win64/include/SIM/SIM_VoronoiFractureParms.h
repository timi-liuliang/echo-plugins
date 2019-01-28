/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_VoronoiFractureParms_h__
#define __SIM_VoronoiFractureParms_h__

#include "SIM_Force.h"
#include "SIM_OptionsUser.h"

class SIM_VoronoiFractureParms : public SIM_Data,
				 public SIM_OptionsUser
{
public:
    
     // Impact
     GETSET_DATA_FUNCS_F("minimpactx", MinImpactX);
     GETSET_DATA_FUNCS_F("minimpacty", MinImpactY);
     GETSET_DATA_FUNCS_F("impactscalex", ImpactScaleX);
     GETSET_DATA_FUNCS_F("impactscaley", ImpactScaleY);
     GETSET_DATA_FUNCS_F("minvolume", MinVolume);
     GETSET_DATA_FUNCS_F("refracturedelay", RefractureDelay);
     GETSET_DATA_FUNCS_V3("impactradius", ImpactRadius);
     GETSET_DATA_FUNCS_I("ptsperarea", PtsPerArea);
     GETSET_DATA_FUNCS_I("numpointsx", NumPointsX);
     GETSET_DATA_FUNCS_I("numpointsy", NumPointsY);
     GETSET_DATA_FUNCS_F("nptsperarea", NPtsPerArea);
     GETSET_DATA_FUNCS_I("locationbased", LocationBased);
     GETSET_DATA_FUNCS_I("usemagnetgeo", UseMagnetGeo);
     GETSET_DATA_FUNCS_F("outsidepercentage", OutsidePercentage);        
 
     // Fracture
     GETSET_DATA_FUNCS_B("cuspnormals", CuspInteriorNormals);
     GETSET_DATA_FUNCS_F("cuspangle", CuspInteriorNormalsAngle);
     GETSET_DATA_FUNCS_B("cuspouternormals", CuspExteriorNormals);
     GETSET_DATA_FUNCS_F("cuspouterangle", CuspExteriorNormalsAngle);
     // Cut
     GETSET_DATA_FUNCS_F("cutplaneoffset", CutPlaneOffset);
     // Cluster
     GETSET_DATA_FUNCS_I("cluster", Cluster);
     GETSET_DATA_FUNCS_I("addclusternoise", AddClusterNoise);
     GETSET_DATA_FUNCS_V3("clusterfreq", ClusterFreq);
     GETSET_DATA_FUNCS_V3("clusteroffset", ClusterOffset);
     GETSET_DATA_FUNCS_V3("clusterjitter", ClusterJitter);
     GETSET_DATA_FUNCS_I("randomdetach", RandomDetach);
     GETSET_DATA_FUNCS_F("detachseed", DetachSeed);
     GETSET_DATA_FUNCS_F("detachratio", DetachRatio);
     GETSET_DATA_FUNCS_F("fusedist", FuseDist);
     // Interior Detail
     GETSET_DATA_FUNCS_I("addinteriordetail", AddInteriorDetail);
     GETSET_DATA_FUNCS_F("detailsize", DetailSize);
     GETSET_DATA_FUNCS_F("intnoiseamp", IntNoiseAmp);
     GETSET_DATA_FUNCS_I("intnoisetype", IntNoiseType);
     GETSET_DATA_FUNCS_V3("intnoisefreq", IntNoiseFreq);
     GETSET_DATA_FUNCS_V3("intnoiseoffset", IntNoiseOffset);
     GETSET_DATA_FUNCS_I("intnoiseturb", IntNoiseTurb);
     GETSET_DATA_FUNCS_I("depthnoisescalebias", DepthNoiseScaleBias);
     
     // Velocity Transfer
     GETSET_DATA_FUNCS_F("velscale", VelScale);
     GETSET_DATA_FUNCS_F("randscale", RandScale);
     GETSET_DATA_FUNCS_F("velbias", VelBias);
  
protected:
    explicit		 SIM_VoronoiFractureParms(const SIM_DataFactory *factory);
    virtual		    ~SIM_VoronoiFractureParms();

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_VoronoiFractureParms,
			SIM_Data,
			"Voronoi Fracture Parameters",
			getDopDescription());
};

#endif

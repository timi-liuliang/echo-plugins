/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Visualization_h__
#define __SIM_Visualization_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"

#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>
#include <GU/GU_Detail.h>
#include <GA/GA_Handle.h>

template<typename T>
struct simVisualizationItem
{
    UT_Array<GA_ROHandleT<T> > myAttributes;
    UT_Vector3 myColor;
    fpreal myScale;
};

/// Class for inspecting the state of a simulation.
/// Using prim/point/vertex attributes on the cloth geometry, it generates
/// guide geometry showing internal forces, energy and impacts.
class SIM_API SIM_Visualization : public SIM_Data
{
protected:
    explicit		 SIM_Visualization(const SIM_DataFactory *factory);
    virtual		~SIM_Visualization();

    // render point attributes
    virtual void	 renderPointAttribs() const;
    virtual void	 getPointVectorAttribs(
				UT_Array<simVisualizationItem<UT_Vector3> > & /*data*/) const {}
    virtual void	 getPointQuaternionAttribs(
				UT_Array<simVisualizationItem<UT_Quaternion> > & /*data*/,
				UT_Vector3Array & /* axis */) const {}

    // render primitive attributes
    virtual void	 renderPrimAttribs() const;
    virtual void	 getPrimFloatAttribs(
				UT_Array<simVisualizationItem<float> > & /*data*/) const {}

    // render vertex (edge) attributes
    virtual void	 renderVertexAttribs() const;
    virtual void	 getEdgeFloatAttribs(
				UT_Array<simVisualizationItem<float> > & /*data*/) const {}
    virtual void	 highlightEdge(const GEO_PrimPoly & /*inPoly*/,
				       int /*v*/,
				       const UT_Vector3 &/*color*/) const {}

    // render custom subclass
    virtual void         renderCustomSubclass() const {}

    // render impacts
    virtual void	 renderImpacts() const;
    virtual bool	 getShowImpacts() const { return false; }
    virtual UT_Vector3	 getImpactColor() const { return UT_Vector3(0, 0, 0); }
    virtual fpreal	 getImpactScale() const { return 1; }
    virtual bool	 getImpactScaleByImpulse() const { return true; }
    virtual bool	 getImpactShowAsVector() const { return false; }
    virtual bool	 getShowSubstepImpacts() const { return false; }

    virtual SIM_Guide	*createGuideObjectSubclass() const;
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

    mutable const SIM_Options	*myOptions;
    mutable const GU_Detail	*myInGdp;
    mutable GU_Detail		*myOutGdp;
    mutable GA_RWHandleV3        myAttOutCd;
    //mutable GA_RWHandleF       myAttOutAlpha;
    mutable const SIM_Object	*myObject;
    mutable UT_DMatrix4		*myOutXform;
    mutable const SIM_Time	*myTime;
};
#endif


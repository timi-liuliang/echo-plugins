/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Pelt GU
 */

#ifndef __GU_Pelt_h__
#define __GU_Pelt_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include <GEO/GEO_HedgeInterface.h>
#include <GA/GA_Edge.h>
#include <GA/GA_EdgeGroup.h>
#include <UT/UT_UniquePtr.h>

enum
{
	GU_PELT_XSOURCE = 0,
	GU_PELT_YSOURCE, GU_PELT_ZSOURCE,
	GU_PELT_USOURCE, GU_PELT_VSOURCE
};

enum GU_PeltingMethod
{
	GU_SPRING_RELAX = 0, GU_TUTTE_BARYCENTRIC, GU_DISCRETE_HARMONIC
};

class GU_API GU_Pelt
{
public:
	// class for the pelt frame point
	class GU_API FramePoint
	{
	public:
		fpreal coord[2];
		fpreal connectedcoord[2];
	};

	// class representing a boundary component of the geometry
	struct BoundaryComponent
	{
		UT_Array<GEO_Hedge> hedges;
		UT_Array<GA_Offset> points;

		int entries;
		fpreal length;
		int comp;

		BoundaryComponent()
		{
		    length = 0;
		    entries = 0;
		    comp = 0;
		}
	};

	// boundary components are sorted by length using this comparator
	static int Compare(const BoundaryComponent *A,
	                   const BoundaryComponent *B)
	{
		int r = 0;
		if (A->length < B->length) r = 1;
		if (A->length > B->length) r = -1;
		return r;
	}

	// the structure gathering all parameters from the sop
	struct Parameters
	{
		Parameters(int it, fpreal sc, bool ur, int uf, int vf,
		           fpreal orient, int meth, bool ot, int h, fpreal mnu,
			   fpreal mxu, fpreal mnv, fpreal mxv) :
			iterations(it), springconstant(sc), userange(ur),
			ufrom(uf), vfrom(vf), orientation(orient), method(meth),
			outputtoggle(ot), hintpoly(h), minu(mnu), maxu(mxu),
			minv(mnv), maxv(mxv)
		{
		}

		int iterations;
		fpreal springconstant;
		bool userange;
		int ufrom;
		int vfrom;
		fpreal orientation;
		int method;
		bool outputtoggle;
		int hintpoly;
		fpreal minu;
		fpreal maxu;
		fpreal minv;
		fpreal maxv;
	};

	GU_Pelt(GU_Detail *gdp, const GA_EdgeGroup *gp, const GEO_Face *face,
	        UT_String outputgroup, const Parameters &parms)
	{
		myGdp = gdp;
		myFramePointCount = 0;
		myFace = face;
		myIterations = parms.iterations;
		mySpringConstant = parms.springconstant;
		myUseRange = parms.userange;
		myUfrom = parms.ufrom;
		myVfrom = parms.vfrom;
		myOrientation = parms.orientation;
		myMethod = (GU_PeltingMethod)parms.method;
		myOutputToggle = parms.outputtoggle;
		myOutputGroup = outputgroup;
		myHintPoly = parms.hintpoly;
		myMinu = parms.minu;
		myMaxu = parms.maxu;
		myMinv = parms.minv;
		myMaxv = parms.maxv;

		splitIntoComponents(gp);
	}

	~GU_Pelt()
	{}

	static void getHintComponent(GU_Detail *gdp, const GA_EdgeGroup *cuts,
	                             int hint_prim,
	                             GA_PrimitiveGroup *connected_prims);

	void updateUVs(bool (*callback_func)(void *), void *callback_data,
	               const char *tension_attribute,
	               const char *density_attribute,
                       const GA_RWHandleV3 &uvattrib);

	int getFramePointCount()
	{
	    return myFramePointCount;
	}

	UT_Array<FramePoint> &getFramePoints()
	{
	    return myFramePoints;
	}

private:
	UT_Array<FramePoint> 	 	 myFramePoints;
	int 				 myFramePointCount;
	GU_Detail 			*myGdp;
	// hedge interface attached to the temporary gdp;
        UT_UniquePtr<GEO_DetachedHedgeInterface> myHI;
	const GEO_Face 			*myFace;
        UT_UniquePtr<GA_EdgeGroup>       myCutsGroup;
	int  				 myCurrentComp;
	UT_Array<GEO_Hedge> 		 myHintEdges;
	// hedge interface attached to the temporary gdp;
	int 				 myFrameBound;
	// the boundary geometry
	UT_Array<BoundaryComponent>	 myBoundary;
	// Parameters from the SOP
	int 				 myIterations;
	fpreal 				 mySpringConstant;
	bool 				 myUseRange;
	int 				 myUfrom;
	int 				 myVfrom;
	fpreal 				 myOrientation;
	GU_PeltingMethod 		 myMethod;
	bool 				 myOutputToggle;
	UT_String 			 myOutputGroup;
	int 				 myHintPoly;
	fpreal 				 myMinu;
	fpreal 				 myMaxu;
	fpreal 				 myMinv;
	fpreal 				 myMaxv;

	// attach a primitive attribute indicating component index
	void 	splitIntoComponents(const GA_EdgeGroup *gp);
	// returns the primitive group of a given component
	void 	getConnectedPrimitives(GU_Detail *gdp,
	     	                       GA_PrimitiveGroup *primitives, int c);
	// construct a frame using myBoundary array. Chooses the longest
	// boundary that lies in the hint component
	void 	getFrame(GU_Detail *gdp, GA_PointGroup *boundary_points, int c,
	     	         const char *density_attribute);
	// fill myBoundary array
	void 	getBoundaryComps(GU_Detail *gdp);
	// create and fill myFramePoints array
	void 	createFrame(GU_Detail *gdp,const GA_PointGroup *boundary_points,
	     	            const char *density_attribute);
	void 	remapCoord(UT_Array<FramePoint> &frame, int num_points);
	// returns the weight of an edge according to the chosen method
	fpreal	getWeight(GEO_Hedge e);

};

#endif

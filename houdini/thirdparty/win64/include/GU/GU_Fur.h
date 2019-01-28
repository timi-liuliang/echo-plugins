/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Fur.h (GU Library, C++)
 *
 * COMMENTS:	Generates fur geometry
 */

#ifndef __GU_Fur__
#define __GU_Fur__

#include "GU_API.h"

#include "GU_Detail.h"
#include <UT/UT_OpCaller.h>
#include <UT/UT_ThreadedAlgorithm.h>

enum GU_FurCurveType
{
    GU_FUR_POLYGON,
    GU_FUR_NURBS
};

class guFurSkinWrapper;
class guFurGuidesWrapper;
class guFurClumpsWrapper;
class guFurPartingLinesWrapper;
struct guFurGenerateParms;

class GU_API GU_Fur
{
public:
    GU_Fur(const GU_Detail *skin,
	   const GU_Detail *guides,
	   const GU_Detail *clumps,
	   const GU_Detail *partinglines,
	   int seed,
	   fpreal density,
	   fpreal length,
	   fpreal guideradius,
	   fpreal clumpradius,
	   fpreal partingradius,
	   const UT_String &skin_shader,
	   const UT_String &guide_shader,
	   const UT_String &skin_attribs,
	   const UT_String &guide_attribs,
	   bool set_id);
    virtual ~GU_Fur();

    // This must be called and return true before attempting to generate
    // curves with the generate() method.
    // This method can fail (return false) if the number primitives in the
    // guides geometry does not match the number of points in the skin
    // geometry or the number of vertices in guide and clump geometries
    // are inconsistent.
    bool initialize(int nvertices);

    // creates curves in 'gdp' grown from the primitives specified by
    // 'skinprims' from the skin geometry used in the constructor
    void generate(UT_OpCaller *caller,
		  GU_Detail *gdp,
		  const UT_IntArray &skinprims,
		  fpreal display,
		  bool limit_to_bounds,
		  const UT_BoundingBox &bounds,
		  bool use_closest_clump,
		  bool remove_unclumped,
		  bool remove_unguided,
		  GU_FurCurveType type,
		  GA_AttributeOwner skinowner,
		  GA_AttributeOwner guideowner,
		  bool interruptable);

    // Fetches a bounding box for fur grown from skin primitive 'pr'.
    // 'preblur' and 'postblur' are shutter times (in seconds from the frame
    // being rendered) for motion blur.
    void getBoundingBox(UT_BoundingBox &box, int pr,
			fpreal preblur, fpreal postblur) const;

    // subclasses implement these methods to record any errors and warnings
    virtual void addError(const UT_String &str) = 0;
    virtual void addWarning(const UT_String &str) = 0;

private:
    // threaded method to generate fur over a section
    THREADED_METHOD2(GU_Fur, skinprims->entries() > 1, generatePrivate, const UT_IntArray *, skinprims, guFurGenerateParms *, parms)
    void generatePrivatePartial(const UT_IntArray *skinprims, guFurGenerateParms *parms, const UT_JobInfo &info);

    guFurSkinWrapper *mySkin;
    guFurGuidesWrapper *myGuides;
    guFurClumpsWrapper *myClumps;
    guFurPartingLinesWrapper *myPartingLines;

    int mySeed;
    fpreal myDensity;
    fpreal myLength;
    const UT_String mySkinShader;
    const UT_String myGuideShader;
    const UT_String mySkinAttribs;
    const UT_String myGuideAttribs;
    bool mySetId;
    int myNumVertices;
};

#endif

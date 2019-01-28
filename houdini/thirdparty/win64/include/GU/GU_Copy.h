/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Copy.h ( GU Library, C++)
 *
 * COMMENTS:	Copy one gdp with a transform.  Optionally with a template
 *		object
 *
 *		When a template is used, the copies will be made in the
 *		loop order:
 *			foreach template_point {
 *			    foreach copy {
 *				getmatrix();
 *				makecopy();
 *			    }
 *			}
 *		This allows for a cumulative transform to be applied.
 */

#ifndef __GU_Copy__
#define __GU_Copy__

#include "GU_API.h"
#include <GA/GA_Types.h>
#include <UT/UT_VectorTypes.h>

class GA_Detail;
class GA_PrimitiveGroup;
class GA_PointGroup;
class GU_Detail;
class UT_String;
class UT_Interrupt;

class gu_CopyAttribMap;
class gu_CopyTransformAttributes;

typedef int	(*GU_CopyTransformCallback)(UT_Matrix4 &, int copynum,
				const GU_Detail *templategdp,
                                GA_Offset templateptoff,
				int template_pt_num, void *data);

typedef int	(*GU_CopyGroupNameCallback)(UT_String &, void *data);

typedef int	(*GU_CopySourceCallback)
			(const GU_Detail *&src, const GA_PrimitiveGroup *&grp,
			 void *data);

class GU_API GU_CopyParms
{
public:
     GU_CopyParms();
    ~GU_CopyParms();

    enum	GU_AttribOp { GU_SET_ATTRIB,
			      GU_ADD_ATTRIB,
			      GU_SUB_ATTRIB,
			      GU_MUL_ATTRIB };

    void	setTemplate(const GU_Detail &gdp, const GA_PointGroup *ptgrp=0);

    // Patterns for attribute copying from template to copies...
    // The template geometry must be set before these methods are called
    void	setPointAttrib(const char *pattern, GU_AttribOp op);
    void	setPrimAttrib(const char *pattern, GU_AttribOp op);
    void	setVertexAttrib(const char *pattern, GU_AttribOp op);
    void	setKeepVectorLength(int onoff) { myKeepFlag = onoff; }
    void	setPreserveGroups(int onoff) { myPreserveGroups = onoff; }
    void        setKeepInternalGroups(bool keep) { myKeepInternalGroups = keep; }
    void	setTransformCallback(GU_CopyTransformCallback callback)
		{
		    myXformCallback = callback;
		}
    void	setGroupNameCallback(GU_CopyGroupNameCallback callback)
		{
		    myGNameCallback = callback;
		}

    // If a setSource is used, the callback is ignored.

    void	setSource(const GU_Detail *gdp,
			  const GA_PrimitiveGroup *primgrp=0)
		{
		    mySource = gdp;
		    myPrimGroup = primgrp;
		}

    void	setSourceCallback(GU_CopySourceCallback callback)
		{
		    mySourceCallback = callback;
		}

    void	copy(GU_Detail &gdp, int ncopies, void *data = 0);


    // Apply attribute operations to point/primitive/vertex attributes
    void	applyPointMap(GA_Offset ptoff, GA_Offset tptoff) const;
    void	applyPrimMap(GA_Detail *detail, GA_Offset primoff, GA_Offset tptoff) const;

    bool	needToApplyPointMap() const
		{
		    return myPointMap;
		}
    bool	needToApplyPrimMap() const
		{
		    return myPrimMap || myVertexMap;
		}

private:

    bool	singleCopy(gu_CopyTransformAttributes &xformer,
			   GU_Detail *gdp, int ncopies, void *data,
			   GA_Offset tptoff, int tnum, int ltnum,
			   UT_Interrupt *boss,
			   bool multi_merge);
    bool	multiCopy(GU_Detail *gdp,
			    int ncopies,
			    void *data,
			    UT_Interrupt *boss);

    void	setAttribMap(gu_CopyAttribMap *&, const char *pattern,
			     GU_AttribOp op);

    // Ensure that we have the correct attributes on gdp and that the offsets
    // are correct (c.f. the src).  We append any maps after the source
    // attribs.
    void	fixAttribMaps(GU_Detail &gdp, const GU_Detail *src);

    GU_CopyTransformCallback	 myXformCallback;
    GU_CopyGroupNameCallback	 myGNameCallback;
    GU_CopySourceCallback	 mySourceCallback;
    const GU_Detail		*myTemplate;
    const GU_Detail		*mySource;
    const GA_PrimitiveGroup	*myPrimGroup;
    const GA_PointGroup		*myPointGroup;
    gu_CopyAttribMap		*myPointMap;
    gu_CopyAttribMap		*myPrimMap;
    gu_CopyAttribMap		*myVertexMap;

    int				 mySourcePointCount;
    int				 mySourcePrimCount;
    int				 myKeepFlag;
    int				 myPreserveGroups;
    bool                         myKeepInternalGroups;
};

#endif


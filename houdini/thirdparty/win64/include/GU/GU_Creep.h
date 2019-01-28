/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Creep.h (C++)
 *
 * COMMENTS:	Paramters used in creep sop
 *
 */

#ifndef __GU_Creep_h__
#define __GU_Creep_h__

#include "GU_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>

class gu_CreepAttribMap;
class GA_PointGroup;
class GEO_Primitive;
class GU_Detail;

class GU_API GU_CreepParms
{
public:
     GU_CreepParms();
    ~GU_CreepParms();

    enum GU_AttribOp
    {
	GU_SET_ATTRIB,
	GU_ADD_ATTRIB,
	GU_SUB_ATTRIB,
	GU_MUL_ATTRIB
    };

    // Patterns for attribute copying from path to crept geometry.  The
    // various attributes will only affect the point attributes of the
    // crept geometry.  Note that the path detail must be set before
    // these methods are called.
    void	setPointAttrib(const char *pattern, GU_AttribOp op);
    void	setPrimAttrib(const char *pattern, GU_AttribOp op);
    void	setVertexAttrib(const char *pattern, GU_AttribOp op);

    // Clear all stored attribute mappings.
    void	clearAttribMaps();

    void	setT(fpreal x, fpreal y, fpreal z)
		    { myT.assign(x, y, z); }
    void	setR(fpreal x, fpreal y, fpreal z)
		    { myR.assign(x, y, z); }
    void	setS(fpreal x, fpreal y, fpreal z)
		    { myS.assign(x, y, z); }
    void	setP(fpreal x, fpreal y, fpreal z)
		    { myP.assign(x, y, z); }

    void	identity()
		    {
			setT(0, 0, 0);
			setR(0, 0, 0);
			setS(1, 1, 1);
			setP(0, 0, 0);
		    }

    const UT_Vector3	&getT() const	{ return myT; }
    const UT_Vector3	&getR() const	{ return myR; }
    const UT_Vector3	&getS() const	{ return myS; }
    const UT_Vector3	&getP() const	{ return myP; }

    void	setUVAttribute(const char *name)
		    { myUVAttrib.harden(name); }

    bool	doInitFill() const	{ return myInitFill; }
    bool	doInitUndistort() const	{ return myInitUndistort; }

    void	setInitFill(bool f)		{ myInitFill = f; }
    void	setInitUndistort(bool f)	{ myInitUndistort = f; }

    void	setSource(const GU_Detail *g)	{ mySourceGdp = g; }
    void	setSourceGroup(const GA_PointGroup *g)	{ mySourceGroup = g; }
    void	setPath(const GU_Detail *g)	{ myPathGdp = g; }
    void	setPathPrim(const GEO_Primitive *p)	{ myPathPrim = p; }

    const GU_Detail	*getSource() const	{ return mySourceGdp; }
    const GA_PointGroup	*getSourceGroup() const	{ return mySourceGroup; }

    const GU_Detail	*getPath() const	{ return myPathGdp; }
    const GEO_Primitive	*getPathPrim() const	{ return myPathPrim; }

private:
    const GU_Detail		*myPathGdp;
    const GEO_Primitive		*myPathPrim;
    const GU_Detail		*mySourceGdp;
    const GA_PointGroup		*mySourceGroup;

    UT_Vector3				myT, myR, myS, myP;
    UT_ValArray<gu_CreepAttribMap *>	myPointMap;
    UT_ValArray<gu_CreepAttribMap *>	myVertexMap;
    UT_ValArray<gu_CreepAttribMap *>	myPrimMap;
    UT_String				myUVAttrib;
    bool 				myInitFill;
    bool 				myInitUndistort;

    friend class GU_Detail;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The Curve sop generator.
 *
 */

#ifndef __SOP_Curve_h__
#define __SOP_Curve_h__

#include "SOP_API.h"
#include <math.h>
#include <PRM/PRM_Parm.h>
#include "SOP_Node.h"


#define	  SC_CVS  		0
#define   SC_BREAKPOINTS	1
#define	  SC_FREEHANDDRAW	2

#define	  SC_POLYGON		0
#define	  SC_NURBS		1
#define   SC_BEZIER		2

#define	  SC_STR_POINT		    "%.6g,%.6g,%.6g "	    // x,y,z
#define	  SC_STR_WEIGHTED_POINT	    "%.6g,%.6g,%.6g,%.6g "  // x,y,z,w
#define	  SC_STR_RELATIVE_POINT	    "@%.6g,%.6g,%.6g "	    // @x,y,z
#define   SC_STR_REL_WEIGHTED_POINT "@%.6g,%.6g,%.6g,%.6g " // @x,y,z,w
#define	  SC_STR_REF_POINT	    "p%d "		    // pn
#define	  SC_STR_COPY_POINT	    "P%d "		    // Pn
#define	  SC_STR_BREAKPOINT_U	    "%db[%d] "		    // xb[u]
#define	  SC_STR_BREAKPOINT_UV	    "%db[%d,%d] "	    // xb[u,v]


class MSS_FaceState;

class SOP_API SOP_Curve : public SOP_Node
{

public:
    virtual const char	*inputLabel(unsigned idx) const override;
    virtual int		 isRefInput(unsigned int i) const override;

    static OP_Node	*myConstructor(OP_Network*, const char *, OP_Operator*);
    static PRM_Template	*buildTemplates();
    const SOP_NodeVerb	*cookVerb() const override;

    virtual int          getTranslateParmIndex() override;

    // given a number of vertices it calculates how many more are necessary
    // in order to have a valid gdp, taking into account the current
    // internal configuration of the SOP
    int		calcCVsNeeded(int numverts);

    void        COORDS(UT_String &str, double t) 
					{ evalString(str, "coords", 0, t); }

    void	SET_COORDS(UT_String &str, CH_StringMeaning meaning,
			   double t)
					{ setString(str, meaning, 2, 0, t); }

    // SWITCHER
    int		TYPE(void)		{ return evalInt("type", 0, 0); }
    void	SET_TYPE(int i, bool dflt = false)
					{
					  setInt("type", 0, 0, i);
					  if (dflt)
					      getParm("type").overwriteDefaults(0);
					}

    int 	METHOD(void)		{ return evalInt("method", 0, 0); }
    void	SET_METHOD(int i, bool dflt = false)
					{
					  setInt("method", 0, 0, i);
					  if (dflt)
					      getParm("method").overwriteDefaults(0);
					}

    int		CLOSED(void)		{ return evalInt("close", 0, 0); }
    void	SET_CLOSED(int i, bool dflt = false)
					{
					  setInt("close", 0, 0, i);
					  if (dflt)
					      getParm("close").overwriteDefaults(0);
					}

    bool	REVERSE(double t)	{ return evalInt("reverse", 0, t); }
    // Curve properties options
    int 	NORMBASIS(void)		{ return evalInt("normalize", 0, 0); }

    int 	ORDER(void)		{ return evalInt("order", 0, 0); }
    void	SET_ORDER(int i)	{ setInt("order", 0, 0, i); }

    int 	PARMTYPE(void)		{ return evalInt("param", 0, 0); }


    // Fitting options
    double	TOLERANCE(double t)	{ return evalFloat("tolerance", 0, t); }
    double	SMOOTH(double t)		{ return evalFloat("smooth", 0, t); }
    int		SHARP(void)		{ return evalInt("csharp", 0, 0); }

    int         KEEPINGEO(void)		{ return evalInt("keepgeo", 0, 0); }

protected:
    SOP_Curve(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Curve();

    virtual OP_ERROR		 cookMySop(OP_Context &context) override;

private:
    friend class MSS_FaceState;
};

#endif

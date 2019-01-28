/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __SOP_Stroke_h__
#define __SOP_Stroke_h__

#include "SOP_API.h"
#include <PRM/PRM_Include.h>
#include <PRM/PRM_Shared.h>
#include <PRM/PRM_Name.h>
#include "SOP_Node.h"

class SOP_API SOP_Stroke : public SOP_Node
{
public:
    struct StrokePoint
    {
	StrokePoint()
	{
	    pos = dir = projpos = projuv = 0;
	    projprim = -1;
	    pressure = 1;
	    time = 0;
	    tilt = angle = roll = 0;
	}
	UT_Vector3	pos, dir;		// Camera location of source
	UT_Vector3	projpos;		// What the state projected to
	int		projprim;		// Where it projected
	UT_Vector3	projuv;			// Where it projected
	bool		projsuccess;		// Did it hit?
	float		pressure;		// 0..1
	float		time;			// In seconds since stroke start
	float		tilt, angle, roll;	// In Degrees
    };

    class StrokeMetaData
    {
    public:
	UT_StringHolder		parmname;
	UT_StringHolder		type;
	int			tuplesize;
	UT_StringHolder		value;

	template <typename ARCHIVER_T>
	bool			serialize(ARCHIVER_T &archiver);
    };

	     SOP_Stroke(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Stroke();

    virtual bool	 updateParmsFlags();

    static bool		encodeStroke(UT_String &encoding, const UT_Array<StrokePoint> &pts);
    static bool		decodeStroke(UT_Array<StrokePoint> &pts, const char *encoding);

    static bool		encodeStrokeMeta(UT_String &encoding, const UT_Array<StrokeMetaData> &meta);
    static bool		decodeStrokeMeta(UT_Array<StrokeMetaData> &meta, const char *encoding);

    static OP_Node	*myConstructor (OP_Network  *net, const char *name,
					OP_Operator *entry);

    virtual int          isRefInput (unsigned i) const;
    static PRM_Template	 myTemplateList[];

    int			 NUMSTROKES(fpreal t) const
    { return evalInt("stroke_numstrokes", 0, t); }
protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);
    virtual const char	*inputLabel(unsigned idx) const;

private:
};

#endif

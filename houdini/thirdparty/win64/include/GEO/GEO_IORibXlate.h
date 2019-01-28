/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_IORibXlate.h ( GEO Library, C++)
 *
 * COMMENTS:	RIB Attribute translation...
 */

#ifndef __GEO_IORibXlate__
#define __GEO_IORibXlate__

#include "GEO_API.h"
#include <GA/GA_AttributeRef.h>
#include <GA/GA_Types.h>
class GEO_Detail;
class UT_String;

enum GEO_RibAttribute
{
    GEO_RIB_FLOAT,
    GEO_RIB_STRING
};

class GEO_API GEO_IORibXlate
{
public:
     GEO_IORibXlate(const char *map);
    ~GEO_IORibXlate();

    void	lookup(const GEO_Detail *gdp);
    const GA_ROAttributeRef	&getAttribRef() const	{ return myAttribRef; }

    int		isValid() const		{ return myAttribRef.isValid(); }

    enum AttribClass
    {
	ATTRIB_INVALID = -1,
	ATTRIB_POINT = 0,
	ATTRIB_VERTEX,
	ATTRIB_PRIMITIVE,
	ATTRIB_DETAIL
    };
    AttribClass	getAttribClass() const	{ return myHType; }
    int		isPoint() const		{ return myHType == ATTRIB_POINT; }
    int		isVertex() const	{ return myHType == ATTRIB_VERTEX; }
    int		isPrimitive() const	{ return myHType == ATTRIB_PRIMITIVE; }
    int		isDetail() const	{ return myHType == ATTRIB_DETAIL; }
    

#if 0
    float		 getFloat(const void *data, int idx) const ;
    void		 getString(UT_String &result, const void *data,
				    int idx) const;
#else

    float		getFloat(GA_Offset obj, int idx) const;
    void		getString(UT_String &result, GA_Offset obj,
				  int idx) const;
#endif

    GEO_RibAttribute	 getRManDataType() const	{ return myRManData; }

    const char	*getHName() const		{ return myHName; }
    const char	*getRMName() const		{ return myRManName; }
    const char	*getRMType() const		{ return myRManType; }
    const char	*getRMFaceType() const		{ return myRManFaceType; }
    int		 getRMSize() const		{ return myRManSize; }
    int		 getRMOffset() const		{ return myRManOffset; }

    // Here, RenderMan has 2 types of point attribute (vertex/varying)
    //	as well as two types of primitive attribute (uniform/constant).  What
    //	we do is overload the vertex flag to indicate both vertex and constant
    //	since the attribute is going to be either point/primitive from the
    //	Houdini end.
    int		 isRManPerVertex() const	{ return myRManVertex; }
    int		 isRManConstant() const		{ return myRManVertex; }

private:
    GA_ROAttributeRef	 myAttribRef;
    char		*myHName;
    char		*myRManName;
    const char		*myRManType;
    const char		*myRManFaceType;
    GEO_RibAttribute	 myRManData;
    AttribClass		 myHType;
    int			 myHTupleSize;
    GA_Storage		 myHStorage;
    int			 myRManOffset;
    int			 myRManSize;
    int			 myRManVertex;
};

#endif


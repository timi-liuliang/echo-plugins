/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Info.h ( GA Library, C++)
 *
 * COMMENTS:	Convenience class to dump information about a GA_Detail
 */

#ifndef __GA_Info__
#define __GA_Info__

#include "GA_API.h"
#include "GA_Types.h"

class GA_Detail;
class GA_ROAttributeRef;

class UT_WorkBuffer;

class GA_API GA_Info 
{
public:
     GA_Info(const GA_Detail &detail);
    ~GA_Info();

    void	fullInfo(UT_WorkBuffer &buf)
		{
		    indexInformation(buf, GA_ATTRIB_POINT);
		    indexInformation(buf, GA_ATTRIB_VERTEX);
		    indexInformation(buf, GA_ATTRIB_PRIMITIVE);
		    attributeInfo(buf, GA_ATTRIB_POINT);
		    attributeInfo(buf, GA_ATTRIB_VERTEX);
		    attributeInfo(buf, GA_ATTRIB_PRIMITIVE);
		    groupInfo(buf, GA_ATTRIB_POINT);
		    groupInfo(buf, GA_ATTRIB_VERTEX);
		    groupInfo(buf, GA_ATTRIB_PRIMITIVE);
		    boxInfo(buf);
		}

    void	indexInformation(UT_WorkBuffer &buf, GA_AttributeOwner owner);
    void	attributeInfo(UT_WorkBuffer &buf, GA_AttributeOwner owner);
    void	groupInfo(UT_WorkBuffer &buf, GA_AttributeOwner owner);
    void	boxInfo(UT_WorkBuffer &buf);
    void	boxInfo(UT_WorkBuffer &buf, const GA_ROAttributeRef &P);
    void	boxInfo2D(UT_WorkBuffer &buf);
    void	boxInfo2D(UT_WorkBuffer &buf, const GA_ROAttributeRef &P);
    void	boxInfo3D(UT_WorkBuffer &buf);
    void	boxInfo3D(UT_WorkBuffer &buf, const GA_ROAttributeRef &P);

private:
    const GA_Detail	&myDetail;
};

#endif


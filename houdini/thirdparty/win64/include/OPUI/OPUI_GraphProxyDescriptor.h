/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OPUI_GraphProxyDescriptor.h (User Interface Library, C++)
 *
 * COMMENTS:
 *		An objects that describes anything that can be converted into
 *		a node graph proxy.
 *
 */

#ifndef __OPUI_GraphProxyDescriptor
#define __OPUI_GraphProxyDescriptor

#include "OPUI_API.h"
#include <CV/CV_Defines.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Error.h>

class OP_NetworkBoxItem;
class OP_Node;
class PXL_Raster;

enum OPUI_GraphProxyCategory {
    OPUI_PROXY_INVALID,
    OPUI_PROXY_ITEM,
    OPUI_PROXY_WIRE,
    OPUI_PROXY_DEPENDENCY,
    OPUI_PROXY_BOUNDARY
};

class OPUI_API OPUI_GraphProxyDescriptor
{
public:
			 OPUI_GraphProxyDescriptor();
			 OPUI_GraphProxyDescriptor(
				OPUI_GraphProxyCategory category);
			 OPUI_GraphProxyDescriptor(OP_NetworkBoxItem *item,
				int input_index);
			 OPUI_GraphProxyDescriptor(OP_Node *node,
				OP_Node *other_node,
				int dependency_count,
				CV_PickType dependency_type);
			 OPUI_GraphProxyDescriptor(OP_NetworkBoxItem *item,
				UT_ErrorSeverity error_severity = UT_ERROR_NONE,
				bool current = false);
			 ~OPUI_GraphProxyDescriptor();

    void		 clear();

    OP_NetworkBoxItem		*myItem;
    UT_ErrorSeverity		 myErrorSeverity;
    UT_StringHolder		 myPlaneName;
    UT_StringHolder		 myLabel;
    const PXL_Raster		*myRaster;
    OPUI_GraphProxyCategory	 myCategory;
    int				 myInputIndex;
    OP_Node			*myOtherNode;
    CV_PickType			 myDependencyType;
    int				 myDependencyCount;
    int				 myBadgeDataId;
    bool			 myCurrent;
};

#endif


/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DT_ViewportProvider.h ( DT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DT_ViewportProvider__
#define __DT_ViewportProvider__

#include "DTUI_API.h"
#include <UI/UI_Value.h>
#include <OP/OP_Node.h>
#include <PI/PI_ClassManager.h>
#include <GA/GA_Types.h>
#include <UT/UT_WorkBuffer.h>

// ======================================================================
// The interface on the FUSE_Office that the host can use to communicate
// to the rest of Houdini (specifically the viewport).
//
class DTUI_API DT_ViewportSelectionConfig
{
public:
    DT_ViewportSelectionConfig();
    ~DT_ViewportSelectionConfig();

    void			 beginViewportSelection() const;
    bool			 isViewportSelectionRunning() const;
    void			 endViewportSelection() const;
    UI_Value			&getViewportSelectionRunningValue() const;

    const UT_StringHolder	&getPrompt() const;
    void			 setPrompt(const UT_StringHolder &prompt);

    const PI_GeometryTypeArray	&getAllowedGeoTypes() const;
    void			 addAllowedGeoType(PI_GeometryType geo_types);

    bool			 getAcceptExisting() const;
    void			 setAcceptExisting(bool accept_existing);

    bool			 getSelectComponents() const;
    void			 setSelectComponents(bool select_components);

    const UT_StringArray	&getInitialNodes() const;
    void			 setInitialNodes(const UT_StringArray &nodes);

    const UT_StringHolder	&getInitialGeometry() const;
    void			 setInitialGeometry(const UT_StringHolder &geo);

    PI_GeometryType		 getInitialGeometryType() const;
    void			 setInitialGeometryType(PI_GeometryType type);

private:
    UT_StringHolder		 myPrompt;
    PI_GeometryTypeArray	 myAllowedGeoTypes;
    UT_StringArray		 myInitialNodes;
    UT_StringHolder		 myInitialGeometry;
    PI_GeometryType		 myInitialGeometryType;
    bool			 myAcceptExisting;
    bool			 mySelectComponents;
    mutable UI_Value		 myViewportSelectionRunningValue;
};

class DTUI_API DT_ViewportSelection
{
public:
    DT_ViewportSelection()
	: myComponentType(GA_GROUP_INVALID)
    { }
    ~DT_ViewportSelection()
    { }

    OP_NodeList		 myNodes;
    GA_GroupType	 myComponentType;
    UT_WorkBuffer	 myComponentStr;
};

class DTUI_API DT_ViewportProvider
{
public:
    DT_ViewportProvider();
    virtual ~DT_ViewportProvider();

    virtual bool getViewportSelection(
			const DT_ViewportSelectionConfig &selection_config,
			DT_ViewportSelection &selection) = 0;
};

#endif


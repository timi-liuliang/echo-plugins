/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Constants_h__
#define __HUSD_Constants_h__

#include "HUSD_API.h"
#include <UT/UT_StringHolder.h>

class HUSD_API HUSD_Constants
{
public:
    static const UT_StringHolder	&getHoudiniRendererPluginName();
    static const UT_StringHolder	&getMantraRendererPluginName();

    static const UT_StringHolder	&getXformPrimType();
    static const UT_StringHolder	&getCollectionsPrimPath();

    static const UT_StringHolder	&getSaveControlImplicit();
    static const UT_StringHolder	&getSaveControlExplicit();
    static const UT_StringHolder	&getSaveControlRemoveFromSublayers();

    static const UT_StringHolder	&getAttributePointProtoIndices();
    static const UT_StringHolder	&getAttributePointIds();
    static const UT_StringHolder	&getAttributePointInvisibleIds();
    static const UT_StringHolder	&getAttributePointPositions();
    static const UT_StringHolder	&getAttributePointOrientations();
    static const UT_StringHolder	&getAttributePointScales();
    static const UT_StringHolder	&getAttributePointVelocities();
    static const UT_StringHolder	&getAttributePointAngularVelocities();

    static const UT_StringHolder	&getRelationshipPrototypes();

    static const UT_StringHolder	&getPrimSpecifierDefine();
    static const UT_StringHolder	&getPrimSpecifierOverride();
    static const UT_StringHolder	&getPrimSpecifierClass();

    static const UT_StringHolder	&getReferenceTypeFile();
    static const UT_StringHolder	&getReferenceTypePayload();
    static const UT_StringHolder	&getReferenceTypePrim();
    static const UT_StringHolder	&getReferenceTypeInherit();
    static const UT_StringHolder	&getReferenceTypeSpecialize();

    static const UT_StringHolder	&getReferenceEditOpAppendFront();
    static const UT_StringHolder	&getReferenceEditOpAppendBack();
    static const UT_StringHolder	&getReferenceEditOpPrependFront();
    static const UT_StringHolder	&getReferenceEditOpPrependBack();

    static const UT_StringHolder	&getPurposeDefault();
    static const UT_StringHolder	&getPurposeProxy();
    static const UT_StringHolder	&getPurposeRender();
    static const UT_StringHolder	&getPurposeGuide();

    static const UT_StringHolder	&getKindSubComponent();
    static const UT_StringHolder	&getKindComponent();
    static const UT_StringHolder	&getKindGroup();
    static const UT_StringHolder	&getKindAssembly();
    static const UT_StringHolder	&getKindAutomatic();

    static const UT_StringHolder	&getDrawModeDefault();
    static const UT_StringHolder	&getDrawModeOrigin();
    static const UT_StringHolder	&getDrawModeBounds();
    static const UT_StringHolder	&getDrawModeCards();

    static const UT_StringHolder	&getExpansionExplicit();
    static const UT_StringHolder	&getExpansionExpandPrims();
    static const UT_StringHolder	&getExpansionExpandPrimsAndProperties();

    static const UT_StringHolder	&getDefaultBgeoPathAttr();
    static const UT_StringHolder	&getDefaultBgeoAttribPattern();
    static const UT_StringHolder	&getDefaultBgeoPathPrefix();

    static const UT_StringHolder	&getMatPurposeAll();
    static const UT_StringHolder	&getMatPurposeFull();
    static const UT_StringHolder	&getMatPurposePreview();
};

#endif


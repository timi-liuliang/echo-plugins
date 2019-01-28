/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Attribute offsets.  This builds a cache of attribute offsets
 *		which is used instead of looking up all the time.
 *
 */

#ifndef __GU_DisplayAttribs_h__
#define __GU_DisplayAttribs_h__

#include "GU_API.h"
#include <GA/GA_AttributeRef.h>
class GU_Detail;
class GA_Attribute;

// Attribute offsets that we care about
class GU_API GU_DisplayAttribs
{
public:
    void	resetOffsets();
    void	getOffsets(const GU_Detail *gdp, int allttribs = 1);

    bool	hasNormals() const
		    { return ptNml.isValid() || vtxNml.isValid(); }
//
//  Detail attributes
//
    GA_ROAttributeRef	detLOD;		// Level of detail for the Detail
    GA_ROAttributeRef	detCd;		// Detail diffuse color/display color

//
//  Primitive attributes
//
    GA_ROAttributeRef	primCd;		// Diffuse color / Display color
    GA_ROAttributeRef	primAlpha;	// Alpha 
    GA_ROAttributeRef	primLOD;	// Level of detail
    GA_ROAttributeRef	primShop;	// SHOP offset (for OGL parameters)
    GA_ROAttributeRef	primMaterial;	// Material SHOP offset
    GA_ROAttributeRef	primLit;	// Should we light this primitive?

    const GA_Attribute	*primShopAttrib;
    const GA_Attribute	*primMaterialAttrib;

//
//  Point attributes
//
    GA_ROAttributeRef	ptVel;		// Velocity
    GA_ROAttributeRef	ptUp;		// Up vector
    GA_ROAttributeRef	ptRot;		// Rotation angle and axis
    GA_ROAttributeRef	ptTrans;	// Translation
    GA_ROAttributeRef	ptPivot;	// Pivot
    GA_ROAttributeRef	ptPScale;	// particle scale
    GA_ROAttributeRef	ptPScale3;	// particle scale (3-vector)
    GA_ROAttributeRef	ptOrient;	// instance orientation
    GA_ROAttributeRef	ptNml;		// Normal
    GA_ROAttributeRef	ptTxt;		// Texture
    GA_ROAttributeRef	ptCd;		// Point color
    GA_ROAttributeRef	ptAlpha;	// Point alpha
    GA_ROAttributeRef	ptCom;		// Point Center of Mass
    GA_ROAttributeRef	ptTangent;	// Tangent vector

//
//  Sprite related point attributes
//
    GA_ROAttributeRef	ptSpriteTxt;	// Sprite texture
    GA_ROAttributeRef	ptSpriteShop;	// Sprite SHOP (for OGL parameters)
    const GA_Attribute	*ptSpriteShopAttrib;

//
//  Vertex attributes
//
    GA_ROAttributeRef	vtxTxt;		// Vertex texture coords
    GA_ROAttributeRef	vtxCd;		// Vertex color
    GA_ROAttributeRef	vtxNml;		// Vertex normal
    GA_ROAttributeRef	vtxAlpha;	// Vertex alpha

// 
//  Internal Capture Attributes (on tubes only)
//
    GA_ROAttributeRef	tubeCapture;

//
//  A flag to distinguish between a user-defined normal and an internal
//  normal.
//
    unsigned	isPtNmlInternal:1;
};

#endif

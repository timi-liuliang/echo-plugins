/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Contains functions needed externally to library for gdxf
 *
 * COMMENTS:    Include file for libdxf.a
 * 
*/

#ifndef _DEVDXF_H_
#define _DEVDXF_H_

#include "dxf_API.h"
#include "dxf.h"
#include "libDxf.h"

dxf_API int 	PushMstack(MSTACK *);
dxf_API int 	PopMstack(MSTACK *);
dxf_API void 	MultMstack(MSTACK *, TRANSFORM);
dxf_API void 	GetMstack(MSTACK *, TRANSFORM);
dxf_API void 	TransformVert(VERTICE *, VERTICE *, TRANSFORM);
dxf_API void 	LoadMstack(MSTACK *, TRANSFORM);
dxf_API MSTACK *InitMstack(unsigned int);
dxf_API BODY   *DxfParse(DXFHandle*);
dxf_API int 	PreOrder(BODY *, int (*)(BODY *),int (*)(BODY *));

#endif

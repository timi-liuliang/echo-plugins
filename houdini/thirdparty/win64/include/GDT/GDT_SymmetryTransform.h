/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library.  (Geo Delta)
 *
 * COMMENTS:	A GDT_SymmetryTransform can transform a points delta to
 *		an appropriate mirrored delta on another point.
 */
#ifndef __GDT_SymmetryTransform_h__
#define __GDT_SymmetryTransform_h__

#include "GDT_API.h"

#include <GA/GA_AIFEdit.h>

class GEO_Detail;

class GDT_API GDT_SymmetryTransform : public GA_AIFEditSymmetry
{
public:
    virtual const GEO_Detail *getDetail() const = 0;
};

#endif

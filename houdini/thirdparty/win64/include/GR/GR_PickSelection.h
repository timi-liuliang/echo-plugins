/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_PickSelection (C++)
 *
 * COMMENTS:
 *	Defines and functions for converting pick data to GU selections.
 * 
 */

#ifndef __GR_PickSelection_h__
#define __GR_PickSelection_h__

#include "GR_API.h"
#include "GR_PickRecord.h"
#include <GU/GU_SelectType.h>
#include <GA/GA_Types.h>
#include <UT/UT_IntArray.h>

class GU_Selection;
class GU_SelectResult;
class GU_Detail;
class UT_BitArray;

class GR_API GR_PickSelection
{
public:
    static int		 pickMaskForSelType(GA_GroupType stype);
    static int		 select(GU_Detail &gdp,
				GU_SelectionHandle selection,
				const UT_Array<GR_PickRecord> &pick_records,
				GU_SelectResult &added,
				GU_SelectionRule rule = GU_AddSelect,
				unsigned *pickid = NULL,
				const UT_IntArray *prfx = NULL);
    static int		 selectEntireGeometry(
				const GU_Detail &gdp,
				GU_SelectionHandle selection,
				const UT_Array<GR_PickRecord> &pick_records,
				GU_SelectResult &added,
				GU_SelectionRule rule,
				const UT_IntArray *prfx);
    static bool		 selectWouldAddComponent(
				const GU_Detail &gdp,
				GU_SelectionHandle selection,
				const UT_Array<GR_PickRecord> &pick_records,
				GU_SelectionRule rule,
				const UT_IntArray *prfx);
    static bool		 selectWithinSelection(
				const GU_Detail &gdp,
				GU_SelectionHandle selection,
				GU_SelectResult &result,
				const GR_PickRecord &pick_record,
				const UT_IntArray *prfx);
    static GA_Index	 getPickedPrimitiveId(const GU_Detail &gdp,
				const GR_PickRecord &pick);
    static void		 expandPickForPrimitive(const GU_Detail &gdp,
				const GA_Index pidx,
				const GR_PickRecord &pick,
				UT_Array<GR_PickRecord> &pick_records,
				UT_BitArray &duplicate_checker);

private:
    static GA_GroupType selTypeForPickType(int pick_type);
};

#endif


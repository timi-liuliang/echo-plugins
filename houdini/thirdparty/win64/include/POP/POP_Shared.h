/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_Shared_h__
#define __POP_Shared_h__

#include "POP_API.h"
#include <PRM/PRM_Include.h>

// NOTE: Where POP[name]Name is the PRM_Name symbol, the string name
// is [name]. For example, the string name of POPattenName is "atten".
POP_API extern PRM_Name		POPactivateName;
POP_API extern PRM_Name		POPapplymotionName;
POP_API extern PRM_Name		POPattenName;
POP_API extern PRM_Name		POPattribName;
POP_API extern PRM_Name		POPbehaviorName;
POP_API extern PRM_Name		POPbounceName;
POP_API extern PRM_Name		POPchargeName;
POP_API extern PRM_Name		POPdensityminName;
POP_API extern PRM_Name		POPdodistanceName;
POP_API extern PRM_Name		POPdohitdiffName;
POP_API extern PRM_Name		POPdohitidName;
POP_API extern PRM_Name		POPdohitnormalName;
POP_API extern PRM_Name		POPdohitposName;
POP_API extern PRM_Name		POPdohitforceName;
POP_API extern PRM_Name		POPdohitposuvName;
POP_API extern PRM_Name		POPdohittimeName;
POP_API extern PRM_Name		POPdohituvName;
POP_API extern PRM_Name		POPdonumhitName;
POP_API extern PRM_Name		POPdragName;
POP_API extern PRM_Name		POPfullsopName;
POP_API extern PRM_Name		POPemittypeName;
POP_API extern PRM_Name		POPgainnmlName;
POP_API extern PRM_Name		POPgaintanName;
POP_API extern PRM_Name		POPhitindexName;
POP_API extern PRM_Name		POPhituseName;
POP_API extern PRM_Name		POPignorechargeName;
POP_API extern PRM_Name		POPignoremassName;
POP_API extern PRM_Name		POPignorexformName;
POP_API extern PRM_Name		POPindexuseName;
POP_API extern PRM_Name		POPinheritvelName;
POP_API extern PRM_Name		POPinitvelName;
POP_API extern PRM_Name		POPlifeName;
POP_API extern PRM_Name		POPlifevarName;
POP_API extern PRM_Name		POPmassName;
POP_API extern PRM_Name		POPmaxdistanceName;
POP_API extern PRM_Name		POPobjName;
POP_API extern PRM_Name		POPoriginindexName;
POP_API extern PRM_Name		POPoriginuseName;
POP_API extern PRM_Name		POPoverridechargeName;
POP_API extern PRM_Name		POPoverridedragName;
POP_API extern PRM_Name		POPoverridemassName;
POP_API extern PRM_Name		POPpreserveGroupName;
POP_API extern PRM_Name		POPprobName;
POP_API extern PRM_Name		POProughName;
POP_API extern PRM_Name		POPsamplethresholdName;
POP_API extern PRM_Name		POPsopName;
POP_API extern PRM_Name		POPshowAccelName;
POP_API extern PRM_Name		POPsourceName;
POP_API extern PRM_Name		POPthresholdName;
POP_API extern PRM_Name		POPvarName;
POP_API extern PRM_Name		POPellipseName;
POP_API extern PRM_Name		POPvelName;
POP_API extern PRM_Name		POPusecontextgeoName;
POP_API extern PRM_Name		POPuseDensityName;
POP_API extern PRM_Name		POPforceObjName;

POP_API extern PRM_Default	POPattribDefault;
POP_API extern PRM_Default	POPlifeDefault;
POP_API extern PRM_Default	POProughDefault;
POP_API extern PRM_Default	POPsamplethresholdDefault;

POP_API extern PRM_Range  	POPattenRange;
POP_API extern PRM_Range  	POPbounceRange;
POP_API extern PRM_Range  	POPchargeRange;
POP_API extern PRM_Range  	POPdragRange;
POP_API extern PRM_Range  	POPgainRange;
POP_API extern PRM_Range  	POPlifeRange;
POP_API extern PRM_Range  	POPlifevarRange;
POP_API extern PRM_Range  	POPmassRange;
POP_API extern PRM_Range  	POPprobRange;
POP_API extern PRM_Range  	POProughRange;
POP_API extern PRM_Range  	POPsamplethresholdRange;

POP_API extern PRM_ChoiceList	POPattribMenu;
POP_API extern PRM_ChoiceList	POPinitvelMenu;
POP_API extern PRM_ChoiceList	POPvectorMenu;
POP_API extern PRM_ChoiceList	POPapplymotionMenu;
POP_API extern PRM_ChoiceList	POPindexuseMenu;
POP_API extern PRM_ChoiceList	POPusecontextgeoMenu;
POP_API extern PRM_ChoiceList	POPobjMenu;
POP_API extern PRM_ChoiceList	POPsopMenu;
POP_API extern PRM_ChoiceList	POPforceObjMenu;

POP_API extern const char	*POPnoObjToken;
POP_API extern PRM_Default	POPobjMenuDefault;
POP_API extern const char	*POPnoSopToken;
POP_API extern PRM_Default	POPsopMenuDefault;

#endif

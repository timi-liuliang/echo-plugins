/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_PRMShared_h__
#define __SIM_PRMShared_h__

#include "SIM_API.h"
#include <PRM/PRM_Include.h>
#include <PRM/PRM_SpareData.h>
#include <PRM/PRM_Conditional.h>

SIM_API extern PRM_Name		 SIMactiveName;

SIM_API extern PRM_Name		 SIMblenddatanameName;
SIM_API extern PRM_Default	 SIMblenddatanameDefault;
SIM_API extern PRM_Name		 SIMblendfactorsrootnameName;
SIM_API extern PRM_Default	 SIMblendfactorsrootnameDefault;
SIM_API extern PRM_Name		 SIMmatchbynameName;
SIM_API extern PRM_Name		 SIMblendfactorName;
SIM_API extern PRM_Name		 SIMuseblendeddataName;
SIM_API extern PRM_Name		 SIMswitchvalueName;
SIM_API extern PRM_Name		 SIMswitchvaluenameName;
SIM_API extern PRM_Default	 SIMswitchvaluenameDefault;
SIM_API extern PRM_Name		 SIMdefaultswitchvalueName;
SIM_API extern PRM_Name		 SIMprimarysolverName;

SIM_API extern PRM_Name		 SIMcolliderlabelName;
SIM_API extern PRM_Default	 SIMcolliderlabelDefault;
SIM_API extern PRM_Name		 SIMcolliderlabelChoices[];
SIM_API extern PRM_ChoiceList	 SIMcolliderlabelMenu;
SIM_API extern PRM_Name		 SIMreverseobjectrolesName;

SIM_API extern PRM_Name		 SIMpriorityName;
SIM_API extern PRM_Default	 SIMpriorityDefault;

SIM_API extern PRM_Name		 SIMshowguideName;
SIM_API extern PRM_Name		 SIMisForDisplayName;
SIM_API extern PRM_Name		 SIMisForRenderName;
SIM_API extern PRM_Conditional	 SIMwhennoshowguideDisable;
SIM_API extern PRM_Name		 SIMintangibleName;

SIM_API extern PRM_Name		 SIMpositionpathName;
SIM_API extern PRM_Default	 SIMpositionpathDefault;
SIM_API extern PRM_Name		 SIMpositionName;
SIM_API extern PRM_Name		 SIMpivotName;
SIM_API extern PRM_Name		 SIMrotationName;
SIM_API extern PRM_Name		 SIMrotationattribnameName;
SIM_API extern PRM_Name		 SIMvelocityName;
SIM_API extern PRM_Name		 SIMangvelocityName;
SIM_API extern PRM_Name		 SIMposcompositeopName;
SIM_API extern PRM_Name		 SIMposcompositeopChoices[];
SIM_API extern PRM_ChoiceList	 SIMposcompositeopMenu;

SIM_API extern PRM_Name		 SIMfrictionName;
SIM_API extern PRM_Name		 SIMdynamicfrictionName;
SIM_API extern PRM_Name		 SIMbounceforwardName;
SIM_API extern PRM_Name		 SIMbounceName;
SIM_API extern PRM_Name		 SIMtemperatureName;

SIM_API extern PRM_Name		 SIMfluxName;
SIM_API extern PRM_Name		 SIMforcescaleName;
SIM_API extern PRM_Name		 SIMtorquescaleName;

SIM_API extern PRM_Name		 SIMforceName;
SIM_API extern PRM_Name		 SIMtorqueName;

SIM_API extern PRM_Name		 SIMforceattribnameName;
SIM_API extern PRM_Default	 SIMforceattribnameDefault;
SIM_API extern PRM_Name		 SIMtorqueattribnameName;
SIM_API extern PRM_Default	 SIMtorqueattribnameDefault;
SIM_API extern PRM_Name		 SIMusemaxdistanceName;

SIM_API extern PRM_Name		 SIMdirectionName;
SIM_API extern PRM_Name		 SIMconeangleName;
SIM_API extern PRM_Name		 SIMfalloffangleName;
SIM_API extern PRM_Name		 SIMfalloffName;
SIM_API extern PRM_Name		 SIMmaxdistanceName;
SIM_API extern PRM_Default	 SIMfalloffangleDefault;

SIM_API extern PRM_Default	 SIMgravityDefaults[];
SIM_API extern PRM_Default	 SIMdirectionDefaults[];

SIM_API extern PRM_Name		 SIMconDofName;
SIM_API extern PRM_Name		 SIMcondofattribnameName;
SIM_API extern PRM_ChoiceList	 SIMconDofSpatialMenu;
SIM_API extern PRM_ChoiceList	 SIMconDofRotationalMenu;
SIM_API extern PRM_Default	 SIMconDofDefault;
SIM_API extern PRM_Name		 SIMconDirName;
SIM_API extern PRM_Name		 SIMcondirattribnameName;
SIM_API extern PRM_Name		 SIMuseworldspaceposName;
SIM_API extern PRM_Name		 SIMworldspaceposName;
SIM_API extern PRM_Name		 SIMorientattribnameName;
SIM_API extern PRM_Name		 SIMupattribnameName;
SIM_API extern PRM_Name		 SIMoutattribnameName;
SIM_API extern PRM_Name		 SIMidattribnameName;
SIM_API extern PRM_Default	 SIMidattribnameDefault;
SIM_API extern PRM_Name		 SIMobjectaxisName;
SIM_API extern PRM_Default	 SIMobjectaxisDefaults[];

SIM_API extern PRM_Name		 SIMstrengthName;
SIM_API extern PRM_Name		 SIMrestLengthName;
SIM_API extern PRM_Range	 SIMrestLengthRange;
SIM_API extern PRM_Name		 SIMstiffnessName;
SIM_API extern PRM_Name		 SIMdampingName;

SIM_API extern PRM_Range	 SIMradiusRange;
SIM_API extern PRM_Range	 SIMfrictionRange;
SIM_API extern PRM_Range	 SIMdynamicfrictionRange;
SIM_API extern PRM_Range	 SIMtemperatureRange;

SIM_API extern PRM_Name		 SIMpointName;
SIM_API extern PRM_Name		 SIMpointidName;
SIM_API extern PRM_Name		 SIMpointnumberName;
SIM_API extern PRM_Name		 SIMpointgroupName;
SIM_API extern PRM_Name		 SIMprimitiveName;
SIM_API extern PRM_Name		 SIMprimitivegroupName;
SIM_API extern PRM_Name		 SIMprimUVName;
SIM_API extern PRM_Name		 SIMoffsetName;
SIM_API extern PRM_Name		 SIMoffsetattribnameName;
SIM_API extern PRM_Name		 SIMattributenameName;

SIM_API extern PRM_Name		 SIMindexName;
SIM_API extern PRM_Default	 SIMindexDefault;

SIM_API extern PRM_Name		 SIMsourcedataName;
SIM_API extern PRM_Default	 SIMsourcedataDefault;
SIM_API extern PRM_Name		 SIMdestinationdataName;
SIM_API extern PRM_Default	 SIMdestinationdataDefault;
SIM_API extern PRM_Name		 SIMinterpolationName;

SIM_API extern PRM_Name		 SIMsurfaceshopName;
SIM_API extern PRM_Name		 SIMpropertyshopName;
SIM_API extern PRM_Name		 SIMmaterialshopName;
SIM_API extern PRM_Name		 SIMfordisplayName;
SIM_API extern PRM_Name		 SIMforrenderName;
SIM_API	extern PRM_Name		 SIMcolorName;

SIM_API extern PRM_Name		 SIMscalarnoiseName;
SIM_API extern PRM_Name		 SIMminvalueName;
SIM_API extern PRM_Name		 SIMmaxvalueName;
SIM_API extern PRM_Name		 SIMfrequencyName;
SIM_API extern PRM_Name		 SIMfractaldepthName;
SIM_API extern PRM_Name		 SIMroughnessName;
SIM_API extern PRM_Name		 SIMattenuationName;
SIM_API extern PRM_Name		 SIMseedName;
SIM_API extern PRM_Name		 SIMnoisetypeName;
SIM_API extern PRM_ChoiceList	 SIMnoisetypeMenu;

SIM_API extern PRM_Name		 SIMcollideMaxIterName;

SIM_API extern PRM_Name		 SIMobjectName;
SIM_API extern PRM_Default	 SIMobjectDefault;

SIM_API extern PRM_Name		 SIMgeometryName;
SIM_API extern PRM_Default	 SIMgeometryDefault;

SIM_API extern PRM_Name		 SIMtreatassdfName;
SIM_API extern PRM_Name		 SIMscaleName;
SIM_API extern PRM_Name		 SIMinvertsignName;

SIM_API extern PRM_Name		 SIMshowObjectLinkName;

SIM_API extern PRM_Name		 SIMsamplemodeName;
SIM_API extern PRM_Default	 SIMsamplemodeDefault;
SIM_API extern PRM_ChoiceList	 SIMsamplemodeMenu;

SIM_API extern PRM_Name          SIMerpName;
SIM_API extern PRM_Default       SIMerpDefault;
SIM_API extern PRM_Range         SIMerpRange;
SIM_API extern PRM_Name          SIMcfmName;
SIM_API extern PRM_Range         SIMcfmRange;
SIM_API extern PRM_Name          SIMdisableCollisionsName;
SIM_API extern PRM_Name          SIMnumIterationsName;
SIM_API extern PRM_Range         SIMnumIterationsRange;

#endif


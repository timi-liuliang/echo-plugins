/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	LOP_PRMShared.h
 *
 * COMMENTS:
 *		This enum defines common LOP parameters and callbacks.
 *
*/

#include "LOP_API.h"

#ifndef __LOP_PRMShared__
#define __LOP_PRMShared__

#include <PRM/PRM_ChoiceList.h>
#include <PRM/PRM_Default.h>
#include <PRM/PRM_Name.h>
#include <PRM/PRM_SpareData.h>
#include <PRM/PRM_Template.h>
#include <UT/UT_Matrix4.h>

class LOP_Node;
class HUSD_FindPrims;
class HUSD_EditCustomData;
class OP_Node;
class OP_Context;
class OP_ContextOptionsWriteScope;
class PRM_Parm;
class UT_String;

extern LOP_API PRM_Name		 lopInputSavePathName;
extern LOP_API PRM_Name		 lopPrimPatternName;
extern LOP_API PRM_Name		 lopPrimPathName;
extern LOP_API PRM_Name		 lopPrimKindName;
extern LOP_API PRM_Name		 lopReferenceEditOpName;
extern LOP_API PRM_Default	 lopAddPrimPathDefault;
extern LOP_API PRM_Default	 lopEditPrimPathDefault;
extern LOP_API PRM_Default	 lopPrimPatternDefault;
extern LOP_API PRM_Default	 lopPrimKindDefault;
extern LOP_API PRM_Default	 lopPrimKindWithAutoDefault;
extern LOP_API PRM_Default	 lopPrimPurposeDefault;
extern LOP_API PRM_Default	 lopReferenceEditOpDefault;
extern LOP_API PRM_ChoiceList	 lopPrimPathMenu;
extern LOP_API PRM_ChoiceList	 lopPrimKindMenu;
extern LOP_API PRM_ChoiceList	 lopPrimKindWithAutoMenu;
extern LOP_API PRM_ChoiceList	 lopPrimPurposeMenu;
extern LOP_API PRM_ChoiceList	 lopReferenceEditOpMenu;
extern LOP_API PRM_ChoiceList	 lopFirstInputCollectionsMenu;
extern LOP_API PRM_ChoiceList	 lopSecondInputCollectionsMenu;
extern LOP_API PRM_SpareData	 lopPrimPathSpareData;
extern LOP_API PRM_SpareData	 lopPrimPatternSpareData;

extern LOP_API void	 lopBuildConcreteUsdPrimTypesMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildAllUsdPrimTypesMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildUsdPrimVariantSetsMenu(LOP_Node &node,
				int which_input,
				const UT_String &primpath,
				PRM_Name *menu_entries,
				int max_menu_entries);

extern LOP_API void	 lopBuildUsdPrimVariantsMenu(LOP_Node &node,
				int which_input,
				const UT_String &primpath,
				const UT_String &variantset,
				PRM_Name *menu_entries,
				int max_menu_entries);

extern LOP_API void	 lopBuildUsdPrimSpecifiersMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildUsdKindsMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildUsdKindsWithAutoMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildUsdDrawModesMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildUsdPurposesMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildUsdReferenceEditOpMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildUsdExpansionRulesMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildFirstInputCollectionsMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

extern LOP_API void	 lopBuildSecondInputCollectionsMenu(void *,
				PRM_Name *menu_entries,
				int max_menu_entries,
				const PRM_SpareData *,
				const PRM_Parm *);

enum LOP_XformParms {
    LOP_XFORM_TRS,
    LOP_XFORM_RXYZ,
    LOP_XFORM_T,
    LOP_XFORM_R,
    LOP_XFORM_S,
    LOP_XFORM_SHEAR,
    LOP_XFORM_USCALE,
    LOP_XFORM_PIVOTCONTAINER,
    LOP_XFORM_PIVOT,
    LOP_XFORM_PIVOTROT,
};

class LOP_XformComponents {
public:
    static const LOP_XformComponents &identity();

    void		 combine(const LOP_XformComponents &other);

    UT_XformOrder	 myOrder;
    UT_Vector3D		 myT;
    UT_Vector3D		 myR;
    UT_Vector3D		 myS;
    UT_Vector3D		 myShear;
    UT_Vector3D		 myP;
    UT_Vector3D		 myPR;
};
extern LOP_API const PRM_Template *lopXformTemplates();
extern LOP_API UT_Matrix4D lopBuildXform(OP_Node &node, fpreal t,
	LOP_XformComponents *accumulate = nullptr);

extern LOP_API const PRM_Template &lopContextOptionsTemplate();
extern LOP_API void lopSetContextOptions(OP_Node &node,
	OP_Context &context,
	OP_ContextOptionsWriteScope &options);

#endif

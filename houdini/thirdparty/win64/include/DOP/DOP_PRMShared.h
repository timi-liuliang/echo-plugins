/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __DOP_PRMShared_h__
#define __DOP_PRMShared_h__

#include "DOP_API.h"
#include <SIM/SIM_PRMShared.h>

DOP_API extern PRM_Name		 DOPobjectnameName;
DOP_API extern PRM_Name		 DOPsolvefirstframeName;

DOP_API extern PRM_Name		 DOPdefaultParmOpName;
DOP_API extern PRM_Name		 DOPshareDataName;
DOP_API extern PRM_Name		 DOPuniqueDataNameName;
DOP_API extern PRM_Name		 DOPdataNameName;
DOP_API extern PRM_Name		 DOPuniqueRelNameName;
DOP_API extern PRM_Name		 DOPrelNameName;

enum {
    DOP_SHARE_OFF = 0,
    DOP_SHARE_ON = 1,
    DOP_SHARE_ONESTEP = 2
};
DOP_API extern PRM_ChoiceList	 DOPsharedataMenu;

DOP_API extern PRM_Name		 DOPactivationName;
DOP_API extern PRM_Name		 DOPactivationTimeZeroName;
DOP_API extern PRM_Default	 DOPactivationDefault;
DOP_API extern PRM_Default	 DOPactivationTimeZeroDefault;
DOP_API extern PRM_Name		 DOPactivationmenuName;
DOP_API extern PRM_ChoiceList	 DOPactivationMenu;

DOP_API extern PRM_Name		 DOPgroupName;
DOP_API extern PRM_Default	 DOPgroupDefault;
DOP_API extern PRM_Name		 DOPaffectedName;
DOP_API extern PRM_Default	 DOPaffectedDefault;
DOP_API extern PRM_Name		 DOPaffectorsName;
DOP_API extern PRM_Default	 DOPaffectorsDefault;

// These enum values must be kept in sync with the menu choices in the .C file.
enum {
    DOP_AFFECTORTYPE_NONE = 0,
    DOP_AFFECTORTYPE_LEFTTORIGHT = 1,
    DOP_AFFECTORTYPE_MUTUAL = 2
};
DOP_API extern PRM_Name		 DOPmutualaffectorName;
DOP_API extern PRM_Name		 DOPaffectortypeName;
DOP_API extern PRM_ChoiceList	 DOPaffectortypeMenu;

// These enum values must be kept in sync with the menu choices in the .C file.
enum {
    DOP_FILEMODE_AUTO = 0,
    DOP_FILEMODE_READ = 1,
    DOP_FILEMODE_WRITE = 2,
    DOP_FILEMODE_NONE = 3
};
DOP_API extern PRM_Name		 DOPfilenameName;
DOP_API extern PRM_Default	 DOPfilenameDefault;
DOP_API extern PRM_Name		 DOPfilemodeName;
DOP_API extern PRM_ChoiceList	 DOPfilemodeMenu;

DOP_API extern PRM_ChoiceList	 DOPgroupMenu;
DOP_API extern void		 DOPbuildGroupMenu(void *dop,
						   PRM_Name *names,
						   int size,
						   const PRM_SpareData *,
						   const PRM_Parm *);
DOP_API extern PRM_ChoiceList	 DOPpointgroupMenu;
DOP_API extern void		 DOPbuildPointGroupMenu(void *dop,
						   PRM_Name *names,
						   int size,
						   const PRM_SpareData *,
						   const PRM_Parm *);
			    

DOP_API extern PRM_Name		 DOPmakemutualName;

#endif


/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_FileHistoryMenu.C (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __PRM_FileHistoryMenu__
#define __PRM_FileHistoryMenu__

#include "PRM_ChoiceList.h"
#include "PRM_Name.h"
#include "PRM_Type.h"

#include <FS/FS_FileHistoryManager.h>
#include <UT/UT_Assert.h>
#include <UT/UT_String.h>


class PRM_SpareData;
class PRM_Parm;


static void
prmBuildHistoryMenu(PRM_Name *theMRUfiles, int thelistsize, 
    FS_FileHistoryType filetype)
{
    int			i, max, numfiles;
    UT_String		formattedName;
    
    FS_FileHistoryManager	*manager = FSgetFileHistoryManager();
    UT_ASSERT(manager);
    
    numfiles = manager->numFilesInList(filetype);
    
    max = (numfiles >= thelistsize) ? thelistsize-1 : numfiles;
    for (i = 0; i < max; i++)
    {
	manager->getFormattedFilenameFromList(i, filetype, formattedName);

	theMRUfiles[i].setToken((const char *)manager
		->getFilenameFromList(i, filetype));
	theMRUfiles[i].setLabel(formattedName);
    }

    theMRUfiles[i].setToken(0);
    theMRUfiles[i].setLabel(0);
}

static void
prmBuildPICMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_PIC);
}

static void
prmBuildGEOMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_GEO);
}

static void
prmBuildRAMPMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_RAMP);
}

static void
prmBuildCAPTMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_CAPT);
}

static void
prmBuildCLIPMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_CLIP);
}

static void
prmBuildPAINTMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_PAINT);
}

static void
prmBuildLUTMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_LUT);
}

static void
prmBuildCMDMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_CMD);
}

static void
prmBuildMIDIMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_MIDI);
}

static void
prmBuildTXTMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_TXT);
}

static void
prmBuildI3DMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_I3D);
}

static void
prmBuildCHANMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_CHAN);
}

static void
prmBuildSIMMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_SIM);
}

static void
prmBuildSIMDATAMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_SIMDATA);
}

static void
prmBuildDIRMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
                   const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_DIRECTORY);
}

static void
prmBuildMISCMRUList(void *, PRM_Name *theMRUfiles, int thelistsize,
    const PRM_SpareData *, const PRM_Parm *)
{
    prmBuildHistoryMenu(theMRUfiles, thelistsize, FS_FILEHISTORY_MISC);
}

static PRM_ChoiceGenFunc
prmGetHistoryCallbackFunction(PRM_Type type)
{
    PRM_ChoiceGenFunc callBackFunction;

    switch (type.getPathType())
    {
	case PRM_Type::PRM_PATH_PIC:
	    callBackFunction = prmBuildPICMRUList;
	    break;
	case PRM_Type::PRM_PATH_GEO:
	    callBackFunction = prmBuildGEOMRUList;
	    break;	
	case PRM_Type::PRM_PATH_CAPT:
	    callBackFunction = prmBuildCAPTMRUList;
	    break;
	case PRM_Type::PRM_PATH_RAMP:
	    callBackFunction = prmBuildRAMPMRUList;
	    break;
	case PRM_Type::PRM_PATH_CLIP:
	    callBackFunction = prmBuildCLIPMRUList;
	    break;
	case PRM_Type::PRM_PATH_PAINT:
	    callBackFunction = prmBuildPAINTMRUList;
	    break;
	case PRM_Type::PRM_PATH_LUT:
	    callBackFunction = prmBuildLUTMRUList;
	    break;
	case PRM_Type::PRM_PATH_CMDF:
	    callBackFunction = prmBuildCMDMRUList;
	    break;
	case PRM_Type::PRM_PATH_MIDI:
	    callBackFunction = prmBuildMIDIMRUList;
	    break;
	case PRM_Type::PRM_PATH_TXT:
	    callBackFunction = prmBuildTXTMRUList;
	    break;
	case PRM_Type::PRM_PATH_I3D:
	    callBackFunction = prmBuildI3DMRUList;
	    break;
	case PRM_Type::PRM_PATH_CHAN:
	    callBackFunction = prmBuildCHANMRUList;
	    break;
	case PRM_Type::PRM_PATH_SIM:
	    callBackFunction = prmBuildSIMMRUList;
	    break;
	case PRM_Type::PRM_PATH_SIMDATA:
	    callBackFunction = prmBuildSIMDATAMRUList;
	    break;
	case PRM_Type::PRM_PATH_DIRECTORY:
	    callBackFunction = prmBuildDIRMRUList;
	    break;
	default:
	    callBackFunction = prmBuildMISCMRUList;
	    break;
    };

    return callBackFunction;
}

PRM_ChoiceList *
prmFindHistoryMenuBuilder(PRM_Type type)
{
    return new PRM_ChoiceList((PRM_ChoiceListType)PRM_CHOICELIST_REPLACE,
				prmGetHistoryCallbackFunction(type));
}

bool
prmIsHistoryMenuBuilder(PRM_ChoiceList *choice_list, PRM_Type type)
{
    return (choice_list && choice_list->getChoiceGenerator() &&
	    choice_list->getChoiceGenerator()
			    == prmGetHistoryCallbackFunction(type));
}

#endif

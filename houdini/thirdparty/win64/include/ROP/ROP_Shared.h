/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP library (C++)
 *
 * COMMENTS:	Shared parameters available for ROP use
 *
 */

#ifndef __ROP_Shared_h__
#define __ROP_Shared_h__

#include "ROP_API.h"
class PRM_Name;
class PRM_Default;
class PRM_ChoiceList;
class PRM_Range;
class PRM_SpareData;

// Useful for IFD type renderers
ROP_API extern PRM_Name ROPdorender;
ROP_API extern PRM_Name ROPrenderDialog;
ROP_API extern PRM_Name	ROPtres;		// Resolution override
ROP_API extern PRM_Name	ROPres;			// Resolution values

ROP_API extern PRM_Name	ROPmkpath;		// Create Intermediate Dirs
ROP_API extern PRM_Name	ROPoutputPicture;

// Useful for geometry type renderers
ROP_API extern PRM_Name		 ROPxformType;		// Transform space
ROP_API extern PRM_ChoiceList	 ROPxformTypeMenu;

ROP_API extern PRM_Default	 ROPfieldDefault;	// Fields default
ROP_API extern PRM_Default	 ROPresDefault[];	// Resolution default


// Meta-data for image formats
ROP_API extern PRM_Name		 ROPmetaArtist;
ROP_API extern PRM_Name		 ROPmetaComment;
ROP_API extern PRM_Name		 ROPmetaHostname;
ROP_API extern PRM_Name		 ROPmetaMplayDir;
ROP_API extern PRM_Name		 ROPmetaMplayLabel;
ROP_API extern PRM_Name		 ROPmetaMplayGamma;
ROP_API extern PRM_Name		 ROPmetaJPEGQuality;
ROP_API extern PRM_Name		 ROPmetaTiffCompression;
ROP_API extern PRM_Name		 ROPmetaExrCompression;
ROP_API extern PRM_Name		 ROPmetaPngTgaPremultiply;

ROP_API extern PRM_Default	 ROPmetaMplayDirDefault;
ROP_API extern PRM_Default	 ROPmetaMplayGammaDefault;
ROP_API extern PRM_Default	 ROPmetaJPEGQualityDefault;
ROP_API extern PRM_Default       ROPmetaTiffCompressionDefault;
ROP_API extern PRM_Default       ROPmetaExrCompressionDefault;
ROP_API extern PRM_Default       ROPmetaPngTgaPremultiplyDefault;

ROP_API extern PRM_ChoiceList    ROPmetaMplayDirMenu;
ROP_API extern PRM_ChoiceList    ROPmetaTiffCompressionMenu;
ROP_API extern PRM_ChoiceList    ROPmetaExrCompressionMenu;
ROP_API extern PRM_ChoiceList    ROPmetaPngTgaPremultiplyMenu;


// Obsolete templates:
ROP_API extern PRM_Template 	ROPcop2Obsolete[];

ROP_API extern PRM_SpareData	ropTakeAlways;
ROP_API extern PRM_Name		ROPtakeName;
ROP_API extern PRM_Default	ROPtakeDef;
ROP_API extern PRM_ChoiceList   ROPtakeMenu;

ROP_API extern PRM_Template	ROPbaseTemplate[];
ROP_API extern PRM_Template	ROPscriptTemplate[];

ROP_API extern PRM_Name		 ROPmetaPngPremultiply;
ROP_API extern PRM_Default       ROPmetaPngPremultiplyDefault;
ROP_API extern PRM_ChoiceList    ROPmetaPngPremultiplyMenu;

#endif

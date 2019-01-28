/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_SpuLauncher.h ( RU Library, C++)
 *
 * COMMENTS:
 *	Provides common code features for running an SPU algorithm on a tile.
 */
#ifndef RU_SpuLauncher_H
#define RU_SpuLauncher_H

#ifdef CELLBE

class TIL_TileList;
class SPUT_ThreadSet;

#include <libspe2.h>
#include <SPU/SPU_Shared.h>
#include <SPUT/SPUT_SpuLauncher.h>

class RU_SpuLauncher : public SPUT_SpuLauncher
{
public:
	        RU_SpuLauncher() {} 
    virtual    ~RU_SpuLauncher() {}

    // Returns the points to the tiles' data in the rp,gb,bp,p4 pointers. Some
    // pointers can be NULL if you're not interested in them.
    void		setOutput(TIL_TileList *tiles,
				  SPU_DataType &type,
				  SPU_HostPtr *rp,
				  SPU_HostPtr *gp = NULL,
				  SPU_HostPtr *bp = NULL,
				  SPU_HostPtr *p4 = NULL);
};

#endif // CELLBE
#endif // RU_SpuLauncher_H

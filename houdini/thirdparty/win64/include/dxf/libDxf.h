/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	libDxf.h of libdxf.a
 *
 * COMMENTS:    Include file for libdxf.a
 *
 * 
*/

#ifndef _DXF_H_
#define _DXF_H_

#include "dxf_API.h"



typedef struct DXFHandle {
    char		*filename;   /* input dxf file */
    int			first;       /* first time reading it? */

    /* Data used by dxf read and write routines */
    void		*body;       /* pointer to body structure */
    double 		angle_res;   /* angle resolution to use */
    unsigned int 	keep_caps;   /* retain the end elements in extrusions */
    unsigned int	block_info;  /* print block insert data to screen */
    unsigned int	use_colour;  /* use pdp colour for display */
    char		**layers;    /* layers to extract */
    int			toc;   	     /* get table of contents */
    int			num_layers;  /* number of layers input */
    int			output_blocks;  /* divide layers into blocks */
} DXFHandle;

dxf_API extern DXFHandle	 *DxfOpen(char *filename, double angle_res,
					  int keep_caps, int block_info,
					  char **layers, int num_layers,
					  int use_colour, int useBlocks);
dxf_API extern char		**DxfToc(DXFHandle *in_dxf);
dxf_API extern int		  DxfClose(DXFHandle *in_dxf);

#endif

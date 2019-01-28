/*
 *
 *  NAME:	array.h
 *  LIBRARY:    libARR
 *
 *  COMMENTS:	useful declarations
 *
 */

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "ARR_API.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

enum Array_Type 
{
    ARRAY_TYPE_BCHAN		= 0x200,
    ARRAY_TYPE_CHAN 		= 0x300
};

typedef struct	_anode
		{
		int	ad;		/* array descriptor		*/
		int	type;		/* channel type			*/
		int	flag;		/* multi-purpose flag		*/
		float	u;		/* number of columns		*/
		float	v;		/* number of rows		*/
		float	**a;		/* array contents		*/
		struct _anode *prev;
		struct _anode *next;
		} ArrayNode, *ArrayNodePointer;

#ifdef NO_PROTOTYPES

ARR_API extern float	**ChanReadAscii(); /* read ascii channel file	*/
ARR_API extern float	**ChanReadBin();   /* read binary channel file	*/
ARR_API extern float   ArrayCountU();
ARR_API extern float   ArrayCountV();
ARR_API extern float   ArrayGetValue();
ARR_API extern float   ArrayGetIValue();
ARR_API extern float	ArrayFrameGetV();
ARR_API extern float **ArrayGetArray();
ARR_API extern ArrayNode *ArrayRead();
ARR_API extern ArrayNode *ArrayGetPointer();

#else
/*
 *  Source File: array.c
 */
ARR_API int ArrayType(int ad);
ARR_API float **ArrayGetArray(int ad);
ARR_API int ArrayGetFrame(int ad, int iv);
ARR_API float ArrayFrameGetV(int ad, int frame);
ARR_API float ArrayCountU(int ad);
ARR_API float ArrayCountV(int ad);
ARR_API float ArrayGetValue(int ad, float u, float v);
ARR_API float ArrayGetIValue(int ad, int iu, int iv);
ARR_API float ArrayPutIValue(int ad, int iu, int iv, float val);
ARR_API int ArrayFree(int ad);
ARR_API int ArrayAlloc(int rows, int cols);
ARR_API ArrayNodePointer ArrayGetPointer(int ad);
/*
 *  Source File: array_io.c
 */
ARR_API int ArrayLoad(char *fname);
ARR_API ArrayNode *ArrayRead(FILE *fp, int type);
ARR_API int ArraySave(int ad, char *fname, int type);
ARR_API int ArrayWrite(FILE *fp, ArrayNode *ap, int type);
/*
 *  Source File: channel_io.c
 */
ARR_API float **ChanReadAscii(FILE *fp, int *cols, int *rows);
ARR_API float **ChanReadBin(FILE *fp, int *cols, int *rows);
ARR_API int ChanWriteAscii(FILE *fp, float **a, int cols, int rows);
ARR_API int ChanWriteBin(FILE *fp, float **a, int cols, int rows);
/*
 *  End of generated source.
 */

#endif /* NO_PROTOTYPES */

#ifdef __cplusplus
}
#endif

#endif

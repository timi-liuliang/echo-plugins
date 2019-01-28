/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VM_SPUMath.h ( RAY Library, C++)
 *
 * COMMENTS:	A class to handle spu-ized operations below the
 *		accessibility level of SPUT.
 */

#ifndef __VM_SPUMath__
#define __VM_SPUMath__

#include "VM_API.h"
#include <SYS/SYS_Types.h>

// This enum must match with SPU_VM_OPCODE in SPU_Fluid.h
typedef enum
{
    VM_SPU_ADD,
    VM_SPU_MUL,
    VM_SPU_SUB,
    VM_SPU_DIV,
    VM_SPU_SQRT,
    VM_SPU_MADD,
    VM_SPU_SCALEOFFSET,
    VM_SPU_DOT,
} VM_SPUOp;

class VM_API VM_SPUMath
{
public:
    virtual	~VM_SPUMath() {}

    // Return true when it is possible to use spu operations for the given
    // vector size.
    virtual bool	canRun(int n) const = 0;
	       
    virtual void	arrayOp(VM_SPUOp op,
				fpreal32 *d,
				const fpreal32 *a,
				const fpreal32 *b,
				const fpreal32 *c, int n) = 0;
    virtual void	scalarOp(VM_SPUOp op,
				fpreal32 *d,
				const fpreal32 *a,
				const fpreal32 *b,
				fpreal32 c, int n) = 0;
    virtual void	scalarOp(VM_SPUOp op,
				fpreal32 *d,
				const fpreal32 *a,
				fpreal32 b,
				fpreal32 c, int n) = 0;
    virtual fpreal32	dot(const fpreal32 *a,
			    const fpreal32 *b, int n) = 0;

    virtual void	multVec(fpreal32 *result,
				const fpreal32 *v,
				const int32 *offsets,
				const void *cells,
				int bits, int nrows, int ncols) = 0;
};

#endif

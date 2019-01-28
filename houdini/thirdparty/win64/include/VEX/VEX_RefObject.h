/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_RefObject.h ( VEX Library, C++)
 *
 * COMMENTS:	A runtime reference-counted type for VEX.  The reference
 *		count is initialized to 0.
 */

#ifndef __VEX_RefObject__
#define __VEX_RefObject__

#include "VEX_API.h"
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_ValArray.h>
#include "VEX_PodTypes.h"

template <VEX_Precision PREC> class VEX_SymbolTable;

class VEX_Assemble;
using VEX_AssemblePtr = UT_IntrusivePtr<VEX_Assemble>;

// A custom reference counted type definition
class VEX_API VEX_RefType
{
public:
    VEX_RefType(const UT_StringHolder &name);

    const UT_StringHolder	&getName() const	{ return myName; }

    // Only for memory debugging
    static void	clearRefTypes();

private:
    UT_StringHolder	myName;
};

// A reference counted runtime object
class VEX_API VEX_RefObject
    : public UT_IntrusiveRefCounter<VEX_RefObject>
{
public:
	     VEX_RefObject();
    virtual ~VEX_RefObject();

    virtual const char	*className() const = 0;

    int			 getRefCount()	{ return use_count(); }

    virtual bool	isEqual(const VEX_RefObject &rhs) const = 0;
    virtual void	dump(UT_WorkBuffer &buf, int indent = 0) const = 0;
    void		dump() const;

    /// Flatten parameter values into per-type arrays.  If you've already
    /// called duplicate(), the result of flatten() will produce data
    /// arrays that match by index with the original object.  Traversal is
    /// preorder.
    virtual void	 flatten(UT_ValArray<VEX_RefObject *> &objs,
				 UT_ValArray<UT_Vector3 *> &vectors,
				 UT_ValArray<float *> &floats) = 0;

    /// Find a method by name, and return the shader and arguments
    /// associated with it.  If the method does not exist, return 0.
    virtual VEX_AssemblePtr	getMethod(const char *name,
					const VEX_SymbolTable<VEX_32> *&args);
    virtual VEX_AssemblePtr	getMethod(const char *name,
					const VEX_SymbolTable<VEX_64> *&args);

private:
};

typedef UT_IntrusivePtr<VEX_RefObject>		VEX_ObjectHandle;

#endif

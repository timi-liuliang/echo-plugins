/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	evvector.C ( EXPR Library, C++)
 *
 * COMMENTS:
 */

#ifndef __EX_Vector_h__
#define __EX_Vector_h__

#include "EXPR_API.h"
class ev_Matrix;

#include <SYS/SYS_Types.h>
#include <UT/UT_VectorTypes.h>

//
// Class definition for array data types
//

class EXPR_API ev_Vector {
public:
     ev_Vector(int size=1);
     ev_Vector(const ev_Vector &src);
    ~ev_Vector();

    int		 getSize() const		{ return (int)myArray[0]; }
    fpreal	 getValue(int idx) const;
    fpreal	 fastGet(int idx) const		{ return myArray[idx+1]; }
    void	 setValue(int idx, fpreal val);
    void	 fastSet(int idx, fpreal val)	{ myArray[idx+1] = val; }

    void	 copy(const ev_Vector &src);
    void	 add(const ev_Vector &src);
    void	 sub(const ev_Vector &src);
    void	 times(fpreal scalar);
    void	 times(const ev_Matrix &src);

    void	 getVector3(UT_Vector3R &v) const;
    void	 getVector4(UT_Vector4R &v) const;
    void	 setVector3(const UT_Vector3R &v);
    void	 setVector4(const UT_Vector4R &v);

    int		 castFrom(const char *str);
    int		 castFrom(fpreal val);
    int		 castFrom(ev_Matrix &from);
    int		 castTo(ev_Matrix &to) const;
    int		 castToFloat(fpreal &rval) const;
    char	*castToString() const;

    // Returns 1 if it's a valid vector string
    int		 scan(const char *str);

    fpreal	 operator()(int idx) const	{ return fastGet(idx); }
    fpreal	 operator[](int idx) const	{ return getValue(idx); }
    void	 grow(int newsize);

private:

    fpreal	*myArray;
};

#endif

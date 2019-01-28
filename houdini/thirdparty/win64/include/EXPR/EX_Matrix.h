/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	EX_Matrix.C ( EXPR Library, C++)
 *
 * COMMENTS:
 */

#ifndef __EX_Matrix_h__
#define __EX_Matrix_h__

#include "EXPR_API.h"

#include <SYS/SYS_Types.h>
#include <UT/UT_VectorTypes.h>

class ev_Vector;

//
// Class definition for array data types
//

class EXPR_API ev_Matrix {
public:
     ev_Matrix(int rows=1, int cols=1);
     ev_Matrix(const ev_Matrix &src);
    ~ev_Matrix();

    int		 getRows() const		{ return myRowCount; }
    int		 getCols() const		{ return myColCount; }
    fpreal	 getValue(int r, int c) const;
    fpreal	 fastGet(int r, int c) const	{ return myRowVals[r][c]; }
    void	 setValue(int r, int c, fpreal val);

    void	 copy(const ev_Matrix &src);
    void	 add(const ev_Matrix &src);
    void	 sub(const ev_Matrix &src);
    void	 times(fpreal scalar);
    void	 times(const ev_Matrix &src);

    int		 castFrom(const char *str);
    int		 castFrom(fpreal val);
    int		 castFrom(const ev_Vector &from);
    int		 castTo(ev_Vector &to) const;
    int		 castToFloat(fpreal &rval) const;
    char	*castToString() const;

    void	 getMatrix3(UT_Matrix3R &v, int overwrite=1) const;
    void	 getMatrix4(UT_Matrix4R &v, int overwrite=1) const;
    void	 setMatrix3(const UT_Matrix3R &v);
    void	 setMatrix4(const UT_Matrix4R &v);

    void	 identity(int size);

    // Returns 1 if it's a valid matrix string
    int		 scan(const char *str);

    fpreal	 operator()(int r, int c) const	{ return getValue(r, c); }
    void	 grow(int rows, int cols)
		 {
		     if (rows != myRowCount) setRows(rows);
		     if (cols != myColCount) setCols(cols);
		 }
    void	 setRows(int rows);
    void	 setCols(int cols);

private:
    int		  myRowCount, myColCount;
    fpreal	**myRowVals;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Two variable polyomials of arbitrary degree (C++)
 *
 * COMMENTS:	
 *
 */

#ifndef	__UT_Polynomial2_H__
#define	__UT_Polynomial2_H__

#include "UT_API.h"
#include <stdlib.h>
#include <iosfwd>
#include "UT_Matrix.h"

#define UT_BASIS_NONE 0
#define UT_BASIS_BEZIER 1
#define UT_BASIS_POWER 2
#define UT_BASIS_LINOP 3	// Linear operator, not a polynomial.



class UT_API UT_Polynomial2 {
public:
    UT_Polynomial2();
    UT_Polynomial2(int udeg, int vdeg);
    ~UT_Polynomial2();

    // Allocate space:
    void		 init(int udeg, int vdeg);

    // Enlarge/reduce the polynomial's range:
    void		 resize(int udeg, int vdeg);

    // Makes it the unit polynomial 
    void		 makeIdentity();
    void		 zero();

    // Swaps u & v.
    void		 transpose();

    // These create conversion functions, which are linear operators
    // to allow conversion between the bases.
    // Uses current size of the poly to determine degree.
    // As this is a linear operator, both u & v size should be equal
    void		 makeBezBasisOp();
    void		 makePowBasisOp();
    // This = op * this
    void		 applyOpLeft(const UT_Polynomial2 &op);
    // this = this * op.
    void		 applyOpRight(const UT_Polynomial2 &op);

    // Polynomial functions.  Note that they are commutative under
    // multiplication.
    // Result is stored in this.
    void		 add(const UT_Polynomial2 &poly);
    void		 mult(const UT_Polynomial2 &poly);
    void		 mult(fpreal scale);
    // this = this(polyu, polyv)
    void		 compose(const UT_Polynomial2 &polyu,
				 const UT_Polynomial2 &polyv,
				 fpreal tol = 1e-5);
    
    void		 setBasis(int newbasis) { myBasisType = newbasis; }
    int			 getBasis() const { return myBasisType; }

    int			 degreeU() const { return myMatrix.getNRH(); }
    int			 degreeV() const { return myMatrix.getNCH(); }

    // Trys to reduce degree as far as possible.
    void		 reduceDegree(fpreal tol = 1e-5);

    // Convert this to the specified type.
    UT_Polynomial2 	*convertToBezier() const;
    UT_Polynomial2	*convertToPower() const;
    // Copy this.
    UT_Polynomial2	*copy() const;

    float		&operator()(int u, int v) { return myMatrix(u,v); }
    fpreal		 operator()(int u, int v) const
					    { return myMatrix(u, v); }

    UT_Polynomial2	&operator=(const UT_Polynomial2 &p);

private:
    UT_MatrixF		myMatrix;
    int			myBasisType;
};

#endif

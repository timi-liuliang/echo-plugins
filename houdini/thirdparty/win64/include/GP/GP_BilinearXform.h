/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	Bi-linear domain transformation class.
 * 
 */

#ifndef __GP_BilinearXform_h__
#define __GP_BilinearXform_h__

#include "GP_API.h"
#include "GP_Xform.h"
#include <iosfwd>

class GP_API GP_BilinearXform : public GP_Xform
{
public:
    // Class c-tors and d-tor.
		 GP_BilinearXform(const UT_BoundingRect *brect = 0);
		 GP_BilinearXform(const GP_BilinearXform &xform);
		 GP_BilinearXform(const GP_Point &a1, const GP_Point &a2,
				  const GP_Point &a3, const GP_Point &a4,
				  const GP_Point &b1, const GP_Point &b2,
				  const GP_Point &b3, const GP_Point &b4);
    virtual	~GP_BilinearXform(void);

    // Apply this transformation to the data forward or backward:
    // Return 0 if OK and -1 if error (eg. can't invert).
    virtual int 	 forward (GP_Point	 &p) const;
    virtual int 	 forward (GP_PointMatrix &m) const;
    virtual int 	 backward(GP_Point	 &p) const;
    virtual int 	 backward(GP_PointMatrix &m) const;

    // Set the transformation to identity:
    virtual void	 reset(void);

    // Add a translation by delta to this transform:
    virtual void	 translate(float dx, float dy);

    // Compose us with the given transformation. Return 0 if we cannot compress
    // the composition. Otherwise return a pointer to the transformation, which
    // will be us or a new xform of a different type.
    virtual GP_Xform	*compose(const GP_Xform &xform,
				 const UT_BoundingRect * = 0);
    virtual GP_Xform	*composeInverse(const GP_Xform &xform,
				 const UT_BoundingRect * = 0);

    // Copy your data from the other guy if both xforms have the same type.
    // Return 0 if same type, else -1.
    virtual int 	 copyFrom(const GP_Xform &xform);

    // Return an inverse transformation. The first method allocates
    // space for the object and return 0 if it fails somewhere. The second
    // method returns the inverse in the reference provided as long as it's
    // the appropriate type, and returns 0. If the xform is not the right type,
    // or the inverse can't be computed, it returns -1.
    virtual GP_Xform	*inverse(void);
    virtual int		 inverse(GP_Xform &);

    // Each derived class has its own name and id:
    virtual const char	*getName(void) const;
    virtual unsigned int getType(void) const;
    virtual const char	*getJSONToken(void) const;

    // I/O functions returning 0 if OK and -1 otherwise.
    virtual int		 save(std::ostream &os, int binary = 0) const;
    virtual bool	 load(UT_IStream &is);

    virtual bool	 save(UT_JSONWriter &w) const;
    virtual bool	 load(UT_JSONParser &p, UT_JSONParser::traverser &it);

    // Assignment operator defined for a transform of the same type:
    GP_BilinearXform	&operator=(const GP_BilinearXform &xform);


protected:
    // The guts of the mapping process. Return 0 of OK, else -1:
    int 		 mapForward (GP_Point &data) const;
    int 		 mapBackward(GP_Point &data) const;

    // Find the root(s) of a simple equation:
    float		 findRoot (const GP_Point &p,
			    const GP_Point &p1, const GP_Point &p2, 
			    const GP_Point &p3, const GP_Point &p4) const;

private:
    GP_Point	myOriginal[4]; // 0 is upper-left corner, 1 is upper-right, etc
    GP_Point	myMapped  [4]; 
};

#endif

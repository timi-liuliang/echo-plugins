/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Domain pasting Library (C++)
 *
 * COMMENTS:	General domain transformation class.
 *		GP_Xform and GP_XformHandle
 * 
 */

#ifndef __GP_Xform_h__
#define __GP_Xform_h__

#include "GP_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_RefMatrix.h>
#include <UT/UT_BoundingRect.h>
#include <UT/UT_JSONParser.h>
#include <iosfwd>

typedef	UT_Vector2		GP_Point;
typedef	UT_RefMatrix<GP_Point>	GP_PointMatrix;


#define	GP_LINEAR_XFORM		0x00001
#define	GP_BILINEAR_XFORM	0x00002

#define	GP_LINEAR_XFORM_N	"Linear"
#define	GP_BILINEAR_XFORM_N	"Bilinear"

#define	GP_LINEAR_XFORM_JSON_TOKEN	"linear"
#define	GP_BILINEAR_XFORM_JSON_TOKEN	"bilinear"


class GP_API GP_Xform
{
public:
    // Class c-tor and d-tor.
		 GP_Xform(void) { myRefCount = 0; }
    virtual	~GP_Xform(void);

    // Apply this transformation to the data forward or backward:
    // Return 0 if OK and -1 if error (eg. can't invert).
    virtual int 	 forward (GP_Point	 &p) const = 0;
    virtual int 	 forward (GP_PointMatrix &m) const = 0;
    virtual int 	 backward(GP_Point	 &p) const = 0;
    virtual int 	 backward(GP_PointMatrix &m) const = 0;

    // Set the transformation to identity:
    virtual void	 reset(void) = 0;

    // Add a translation by delta to this transform:
    virtual void	 translate(float dx, float dy) = 0;

    // Compose us with the given transformation. Return 0 if we cannot compress
    // the composition. Otherwise return a pointer to the transformation, which
    // will be us or a new xform of a different type.
    virtual GP_Xform	*compose(const GP_Xform &xform,
				 const UT_BoundingRect *brect = 0) = 0;
    virtual GP_Xform	*composeInverse(const GP_Xform &xform,
				 const UT_BoundingRect *brect = 0) = 0;

    // Copy your data from the other guy if both xforms have the same type.
    // Return 0 if same type, else -1.
    virtual int 	 copyFrom(const GP_Xform &xform) = 0;

    // Return an inverse transformation. The first method allocates
    // space for the object and return 0 if it fails somewhere. The second
    // method returns the inverse in the reference provided as long as it's
    // the appropriate type, and returns 0. If the xform is not the right type,
    // or the inverse can't be computed, it returns -1.
    virtual GP_Xform	*inverse(void)		= 0;
    virtual int		 inverse(GP_Xform &)	= 0;

    // Each derived class has its own name and id:
    virtual const char	*getName(void) const = 0;
    virtual unsigned int getType(void) const = 0;
    virtual const char	*getJSONToken(void) const = 0;

    // I/O functions returning 0 if OK and -1 otherwise.
    virtual int		 save(std::ostream &os, int binary = 0) const = 0;
    virtual bool	 load(UT_IStream &is) = 0;

    virtual bool	 save(UT_JSONWriter &w) const = 0;
    virtual bool	 load(UT_JSONParser &p, UT_JSONParser::traverser &it)=0;

    // Bump the reference count up or down:
    int			 addReference(void)	{ return ++myRefCount; }
    int			 delReference(void)	{ return --myRefCount; }

    // A new entity of the exact type and contents as ourselves:
    GP_Xform		*copy(void) const	{ return newSpecies(*this); }

    // Generate a new xform of the species specified in the name or type.
    // The method that takes another xform also copies its contents.
    static GP_Xform	*newSpecies(const char *name);
    static GP_Xform	*newSpecies(unsigned int type);
    static GP_Xform	*newSpecies(const GP_Xform &xform);
    static GP_Xform	*newSpeciesJSON(const char *name);

    // Read an xform from the input stream:
    static GP_Xform	*read(UT_IStream &is);
    static GP_Xform	*read(UT_JSONParser &p);

    // Save an identity transformation:
    static int		 saveIdentity(std::ostream &os, int binary = 0);
    static bool		 saveIdentity(UT_JSONWriter &w);


protected:
    // Save the header part (name or type):
    int			 saveHeader(std::ostream &os, int binary = 0) const;
    // Save the JSON type field
    bool		 saveType(UT_JSONWriter &w) const;

private:
    int		myRefCount; // reference counter for handle
};



class GP_API GP_XformHandle
{
public:
    // C-tors and d-tor.
    GP_XformHandle(GP_Xform *xform = 0) : myXform(xform)
    {
	if (myXform)
	    myXform->addReference();
    }
    GP_XformHandle(const GP_XformHandle &handle) : myXform(handle.myXform)
    {
	if (myXform)
	    myXform->addReference();
    }
   ~GP_XformHandle(void)
    {
	if (myXform && !myXform->delReference())
	    delete myXform;
    }
    
    // Assign the contents of another handle to us:
    GP_XformHandle	&operator=(const GP_XformHandle &h)
			 {
			     if (this != &h)
			     {
				 if (myXform && !myXform->delReference())
				     delete myXform;
				 myXform = h.myXform;
				 if (myXform)
				     myXform->addReference();
			     }
			     return *this;
			 }

    // Assign a free floating xform to us. This does a deep copy of xform
    // as long as xform is not identical to myXform, so neither is xform
    // affected, nor do we rely on it to stay alive:
    GP_XformHandle	&operator=(const GP_Xform &xform)
			 {
			     if (myXform != &xform)
			     {
				 // First try to copy if same type:
				 if (!myXform || myXform->copyFrom(xform))
				 {
				     if (myXform && !myXform->delReference())
					 delete myXform;

				     myXform = xform.copy();
				     myXform->addReference();
				 }
				 else // myXform exists and copyFrom failed
				 {
				     if (!myXform->delReference())
					 delete myXform;
				     myXform = xform.copy();
				     myXform->addReference();
				 }
			     }
			     return *this;
			 }

    // I/O functions returning 0 if OK and -1 otherwise.
    int			 save(std::ostream &os, int binary = 0) const
			 {
			     return myXform ? myXform->save(os,binary)
					    : GP_Xform::saveIdentity(os,binary);
			 }

    bool		 load(UT_IStream &is)
			 {
			     if (myXform && !myXform->delReference())
				 delete myXform;
			     myXform = GP_Xform::read(is);
			     if (myXform)
				 myXform->addReference();
			     return myXform != NULL;
			 }

    bool		 save(UT_JSONWriter &w) const
			 {
			     return myXform ? myXform->save(w)
					    : GP_Xform::saveIdentity(w);
			 }

    bool		 load(UT_JSONParser &p)
			 {
			     if (myXform && !myXform->delReference())
				 delete myXform;
			     myXform = GP_Xform::read(p);
			     if (myXform)
				 myXform->addReference();
			     return myXform != NULL;
			 }

    // Compose my xform with another handle's. We don't do this directly
    // through the GP_Xform because the pointers might change.
    void		 compose(const GP_XformHandle  &handle,
				 const UT_BoundingRect *brect = 0)
			 {
			     if (handle.myXform)
			     {
				 if (myXform)
				 {
				     GP_Xform *newxf;
				     newxf = myXform->compose(*handle.myXform,
							      brect);

				     if (newxf != myXform)
				     {
					 if (!myXform->delReference())
					     delete myXform;
					 myXform = newxf;
					 myXform->addReference();
				     }
				 }
				 else // we don't have an xform
				 {
				     myXform = handle.myXform->copy();
				     myXform->addReference();
				 }
			     }
			 } // end compose()

    void		 composeInverse(const GP_XformHandle  &handle,
				 const UT_BoundingRect *brect = 0)
			 {
			     if (handle.myXform)
			     {
				 if (myXform)
				 {
				     GP_Xform *newxf;
				     newxf = myXform->composeInverse(
						 *handle.myXform, brect);

				     if (newxf != myXform)
				     {
					 if (!myXform->delReference())
					     delete myXform;
					 myXform = newxf;
					 myXform->addReference();
				     }
				 }
				 else // we don't have an xform
				 {
				     myXform = handle.myXform->copy();
				     myXform->addReference();
				 }
			     }
			 } // end compose()

    // Grab the contents, i.e. the transformation:
    GP_Xform		*operator->(void) const { return myXform; }
    GP_Xform		*operator* (void) const { return myXform; }

    // Check if this handle contains a transformation:
    int			 isDefined(void) const	{ return myXform != 0; }


private:
    GP_Xform	*myXform;
};

#endif

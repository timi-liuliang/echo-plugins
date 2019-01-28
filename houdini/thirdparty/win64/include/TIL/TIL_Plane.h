/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Plane.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	Defines the information for a single plane of an image (Red, Depth, 
 *	Alpha, etc). Does not contain any actual image data.
 */
#ifndef TIL_PLANE_H
#define TIL_PLANE_H

#include "TIL_API.h"
#include <UT/UT_SmallObject.h>
#include <iosfwd>
#include <string.h>

#include "TIL_Defines.h"

class TIL_API TIL_Plane :
    public UT_SmallObject<TIL_Plane,
			  UT_SMALLOBJECT_CLEANPAGES_OFF, 250,
			  UT_SMALLOBJECT_THREADSAFE_ON>
{
public:
    TIL_Plane(const char *name, TIL_DataFormat format,
		const char *n1=0,
		const char *n2=0,
		const char *n3=0,
		const char *n4=0);
    TIL_Plane(const TIL_Plane &);
    TIL_Plane();
    ~TIL_Plane();

    int64		getMemoryUsage(bool inclusive) const;

    // WARNING: The assignment operator does NOT preserve the constant
    //          flags.
    TIL_Plane &		operator=(const TIL_Plane &);

    // equality based on name ONLY. matches() does exact vector/name/type
    // equality (but no array size comparison). 
    inline int		operator==(const TIL_Plane &eq) const
			{ return (strcmp(eq.getName(), getName()) == 0); }
    
    inline int		operator!=(const TIL_Plane &eq) const
			{ return (strcmp(eq.getName(), getName()) != 0); }
    
    bool		matches(const TIL_Plane &) const;
    bool		isCompatible(const TIL_Plane &) const;
    
    // name of this plane. must be unique within a sequence. should only be
    // called by TIL_Sequence, to avoid different planes with the same name.
    const char *	getName() const { return myName; }
    void	        setName(const char *name);

    bool		isAlphaPlane() const;
    bool		isColorPlane() const;
    bool		isPointPlane() const;
    bool		isNormalPlane() const;
    bool		isDepthPlane() const;
    bool		isGeometricPlane() const;

    // the data format of this plane (int8, float32, etc)
    TIL_DataFormat	getFormat() const { return (TIL_DataFormat)myFormat; }
    void		setFormat(TIL_DataFormat format);

    // the number of layers this plane has (1 normally). Different from
    // the vector size in that you can have 4 layers of a vector3. 
    int			getArraySize() const     { return myArraySize; }
    void		setArraySize(int n);

    // Vector size of the data: RGB = 3, RGBA = 4, R = 1.
    int			getVectorSize() const    { return myVectorSize; }
    void		setVectorSize(int n);

    // vector components names. 
    inline const char *	getSubName(int i) const
			{ return (i>=0 && i<myVectorSize) ? mySubName[i] : 0; }

    void		setSubName(const char *subname, int i);
    int			hasElement(const char *planename);
   
    // returns whether the vector element is 'enabled'. If it is,
    // COP2_Node::inputTile() and similar functions will cook and return the
    // tile corresponding to the element. Otherwise, it will be passed
    // through. setScoped sets all the components to enabled or not.
    // isScoped returns true if ALL the components are scoped, false otherwise
    inline int		getPlaneMask(int i) const
			{ return (i>=0 && i<myVectorSize) ? myPlaneMask[i]:0; }

    void		setPlaneMask(int enable, int i);
    void		copyPlaneMask(const TIL_Plane &plane);
    inline void		setScoped(int enable)
		{ for(int i=myVectorSize; i--;) myPlaneMask[i]=(char)enable; }

    bool		isScoped() const;
    bool		isPartiallyScoped() const;
    bool		scopeMatches(const TIL_Plane &plane) const;

    // returns whether the array element i is enabled, similar to getPlaneMask
    int			getArrayMask(int i) const;
    void		setArrayMask(int start, int end, int step);

    // methods for dealing with constant planes
    bool		isConstantPlane() const;
    bool		isConstantComponent(int i) const;
    bool		isAnyComponentConstant() const;
    void		setConstantFlag(int i, bool enable);

    // black white/point stuff.
    int			usesBlackWhitePoints() const;
    void		getBlackWhitePoints(unsigned int &b,
					    unsigned int &w) const
			{ b = myBlackPoint; w = myWhitePoint; }
    void		getBlackWhitePoints(float &b, float &w) const;
    void		setBlackWhitePoints(unsigned int b, unsigned int w);

    void		debugPrint(std::ostream &os) const;
    int64		getMemSize() const;

    unsigned		getHash() const;

private:
    const char *	myName;
    unsigned short	myArraySize;
    unsigned char	myFormat;
    unsigned char	myVectorSize;
    
    const char *	mySubName[PLANE_MAX_VECTOR_SIZE];

    // masking.
    signed char		myPlaneMask[PLANE_MAX_VECTOR_SIZE];
    int			myArrayMaskStart;
    int			myArrayMaskEnd;
    int			myArrayMaskStep;

    // members for dealing with constant (color and alpha) planes.
    char		myConstantFlag[PLANE_MAX_VECTOR_SIZE];

    // black/white points.
    unsigned int	myBlackPoint;
    unsigned int	myWhitePoint;
    unsigned int	myNameAlloc  :1,
			myComp0Alloc :1,
			myComp1Alloc :1,
			myComp2Alloc :1,
			myComp3Alloc :1;
};


#endif

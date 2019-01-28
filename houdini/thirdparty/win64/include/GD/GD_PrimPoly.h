/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:	polygon definition
 * 
 */

#ifndef __GD_PrimPoly_H__
#define __GD_PrimPoly_H__

#include "GD_API.h"
#include "GD_Face.h"
#include "GD_PrimType.h"
#include <iosfwd>

class GD_Detail;
class UT_MemoryCounter;

class GD_API GD_PrimPoly : public GD_Face
{
public:
    GD_PrimPoly(GD_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
	: GD_Face(d, offset)
    { }
    virtual ~GD_PrimPoly() {}

    /// Report memory usage
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    virtual const GA_PrimitiveJSON	*getJSON() const;

    virtual const GA_PrimitiveDefinition &getTypeDef() const;

    // Build a new polygon and insert it into the given detail.
    static GD_PrimPoly	*build(GD_Detail *d, int npts, int closed = 0,
			       int appendPts = 1);

    virtual unsigned	 getOrder() const;

    // Evaluate one point (when du=0), or the du-th derivative.
    // Return 0 if successful, and -1 otherwise.
    virtual int		evaluate(float u, UT_Vector3 &pos,
				 unsigned du=0, int uOffset=-1) const;
    virtual bool	evaluate(GA_Offset vtx, GA_AttributeRefMap &h,
				fpreal u, uint du=0, int uoffset=-1) const;

    // Build a trim loop (which will be open unless the face is closed). The
    // loop is build every time you call this method, so it's expensive. You
    // must free the loop yourself when you are done with it. The trim pieces
    // are generated in the [ustart,ustop] or [ustop,ustart] interval, where
    // ustart and ustop are parametric values.
    virtual GD_TrimLoop	*trimLoop(float ustart, float ustop) const;
    virtual int		breakCount() const;

protected:
    /// All subclasses should call this method to register the poly intrinsics.
    /// @see GA_AttributeIntrinsic
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveDefinition &definition)
			{ return GD_Face::registerIntrinsics(definition); }

    // We don't need to save anything other than what's in a face
    virtual bool	 savePrivateH9(std::ostream &, bool binary) const;
    virtual bool	 loadPrivateH9(UT_IStream &);

private:
    int 		 create(int npts, int closed = 0, int appendPts = 1);

    friend std::ostream	&operator<<(std::ostream &os, const GD_PrimPoly &d)
			{
			    d.saveH9(os, 0,
				     GD_Primitive::theEmptySaveAttribs,
				     GD_Primitive::theEmptySaveAttribs);
			    return os;
			}

    friend class GD_PrimitiveFactory;

    static GA_PrimitiveDefinition *theDefinition;
};
#endif

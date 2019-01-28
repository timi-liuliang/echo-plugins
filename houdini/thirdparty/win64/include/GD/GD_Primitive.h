/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 */

#ifndef __GD_Primitive_H__
#define __GD_Primitive_H__

#include "GD_API.h"
#include <iosfwd>
#include <UT/UT_BoundingRect.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_VectorTypes.h>
#include <GA/GA_Primitive.h>
#include <GA/GA_ElementGroup.h>

class UT_BoundingSphere;
class GD_Detail;

class GD_API GD_Primitive : public GA_Primitive
{
public:
    	     GD_Primitive(GD_Detail *d, GA_Offset offset=GA_INVALID_OFFSET);

    virtual GD_Primitive *copy(int preserve_shared_pts = 0) const;
    virtual int		  copy(const GD_Primitive &src, int ptoffset) = 0;

    // Evaluate the position or the derivatives at (u,v), where u and v MUST
    // be in [0,1]. Return 0 if OK and -1 otherwise.
    virtual int		 evaluatePoint(UT_Vector3 &pos, float u,
				       unsigned du = 0) const;

    virtual GA_PrimCompat::TypeMask	 getPrimitiveId() const;

    GD_Detail		*getParent() const;

    // Return the barycenter of the part that visible (inside the surf domain).
    // 1 if found, 0 otherwise.
    virtual int		 visibleBaryCenter(float &uctr, float &vctr) const;

    // Transform the primitive according to the given matrix:
    virtual void	 transform(const UT_Matrix3 &);

    // Base class virtuals:
    virtual bool	 isDegenerate() const;
    virtual int		 getBBox(UT_BoundingBox *bbox) const;
    virtual void	 addToBSphere(UT_BoundingSphere *bsphere) const;
    virtual void	 getBRect(UT_BoundingRect *brect) const;
    virtual UT_Vector3	 baryCenter() const;
    virtual UT_Vector3	 computeNormal() const;
    virtual void	 reverse();
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	 addPointRefToGroup(GA_PointGroup &grp) const;
#endif
    virtual int		 detachPoints(GA_PointGroup &grp);
#if !GA_PRIMITIVE_VERTEXLIST
    virtual GA_Size	 getVertexCount() const;
    virtual bool	 vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void *data = 0) const = 0;
#endif

    /// Convience objects to pass as arguments to saveH9()/loadH9().
    static const UT_Array<GA_AttribSaveDataH9> &theEmptySaveAttribs;

protected:
    /// All subclasses should call this method to register the primitive
    /// intrinsics.
    /// @see GA_AttributeIntrinsic
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveDefinition &definition)
			{ return GA_Primitive::registerIntrinsics(definition); }

private:
    friend std::ostream	&operator<<(std::ostream &os, const GD_Primitive &d)
			{
			    // TODO: Use d.jsonSave(os);?
			    d.saveH9(os, 0,
				     GD_Primitive::theEmptySaveAttribs,
				     GD_Primitive::theEmptySaveAttribs);
			    return os;
			}
    void		jsonSave(std::ostream &os) const;
};
#endif

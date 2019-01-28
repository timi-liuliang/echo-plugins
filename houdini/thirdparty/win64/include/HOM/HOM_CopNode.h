/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_CopNode_h__
#define __HOM_CopNode_h__

#include "HOM_API.h"
#include "HOM_BinaryString.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Node.h"
#include "HOM_Track.h"

SWIGOUT(%rename(CopNode) HOM_CopNode;)

class HOM_API HOM_CopNode : virtual public HOM_Node
{
public:
    HOM_CopNode()
    : HOM_NetworkMovableItem(HOM_networkItemType::Node)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_CopNode(const HOM_CopNode &cop_node)
    : HOM_Node(cop_node),
      HOM_NetworkMovableItem(cop_node)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_CopNode()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual bool isBypassed() = 0;
    virtual void bypass(bool on) = 0;
    virtual bool isDisplayFlagSet() = 0;
    virtual void setDisplayFlag(bool on) = 0;
    virtual bool isRenderFlagSet() = 0;
    virtual void setRenderFlag(bool on) = 0;
    virtual bool isTemplateFlagSet() = 0;
    virtual void setTemplateFlag(bool on) = 0;
    virtual bool isCompressFlagSet() = 0;
    virtual void setCompressFlag(bool on) = 0;

    virtual std::vector<std::string> planes() = 0;
    virtual std::vector<std::string> components(const char *plane) = 0;
    virtual HOM_EnumValue &depth(const char *plane) = 0;
    virtual int maskInputIndex() = 0;

    SWIGOUT(%kwargs getPixelByUV);
    virtual std::vector<float> getPixelByUV(const char *plane,
	    double u, double v, const char *component=NULL,  
	    bool interpolate=true) = 0;

    SWIGOUT(%kwargs getPixelHSVByUV);
    virtual std::vector<float> getPixelHSVByUV(
	    double u, double v, bool interpolate=true) = 0;

    SWIGOUT(%kwargs getPixelLuminanceByUV);
    virtual float getPixelLuminanceByUV(
	    double u, double v, bool interpolate=true) = 0;

    SWIGOUT(%kwargs allPixels);
    virtual std::vector<float> allPixels(
	    const char *plane="C", const char *component=NULL,
	    bool interleaved=true, double time=-1.0) = 0;
    SWIGOUT(%kwargs allPixelsAsString);
    virtual HOM_BinaryString allPixelsAsString(
	    const char *plane="C", const char *component=NULL,
	    bool interleaved=true, double time=-1.0, HOM_EnumValue *depth=NULL) = 0;

    SWIGOUT(%kwargs setPixelsOfCookingPlane);
    virtual void setPixelsOfCookingPlane(
	    const std::vector<float> &values,
	    const char *component=NULL, bool interleaved=true,
	    bool flip_vertically=false) = 0;
    SWIGOUT(%ignore opaqueSetPixelsOfCookingPlaneFromString);
    virtual void opaqueSetPixelsOfCookingPlaneFromString(
	    const char *values, int64 length,
	    const char *component=NULL, bool interleaved=true,
	    HOM_EnumValue *depth=NULL, bool flip_vertically=false) = 0;

    SWIGOUT(%kwargs imageBounds);
    virtual std::vector<int> imageBounds(const char *plane="C") = 0;
    virtual int xRes() = 0;
    virtual int yRes() = 0;

    virtual double sequenceStartFrame() = 0;
    virtual double sequenceEndFrame() = 0;
    virtual double sequenceFrameLength() = 0;

    virtual bool isSingleImage() = 0;

    virtual void saveImage(const char *file_name, 
	    const std::vector<int> &frame_range = std::vector<int>()) = 0;

    virtual bool hasMetaData(const char *metadata_name) = 0;
    
    virtual int getMetaDataInt(const char *metadata_name, int index = 0) = 0;

    virtual double getMetaDataFloat(const char *metadata_name, int index = 0) = 0;

    virtual std::string getMetaDataString(const char *metadata_name) = 0;

    virtual std::vector<int> getMetaDataIntArray(const char *metadata_name) = 0;

    virtual std::vector<double> getMetaDataFloatArray(const char *metadata_name) = 0;

#ifdef SWIG
%extend
{
    %kwargs setPixelsOfCookingPlaneFromString;
    void setPixelsOfCookingPlaneFromString(
	    InterpreterObject values, const char *component=NULL,
	    bool interleaved=true, HOM_EnumValue *depth=NULL,
	    bool flip_vertically=false) = 0
    {
	HOM_PyBuffer buffer(values);
	self->opaqueSetPixelsOfCookingPlaneFromString(
	    (const char *)buffer.myData, buffer.myLength,
	    component, interleaved, depth, flip_vertically);
    }
}
#endif
};

#endif

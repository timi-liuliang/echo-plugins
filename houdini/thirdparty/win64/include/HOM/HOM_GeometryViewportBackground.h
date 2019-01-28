/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_GeometryViewportBackground_h__
#define __HOM_GeometryViewportBackground_h__

#include "HOM_API.h"
#include "HOM_EnumValue.h"
#include "HOM_Module.h"
#include "HOM_Defines.h"

SWIGOUT(%rename(GeometryViewportBackground) HOM_GeometryViewportBackground;)


class HOM_API HOM_GeometryViewportBackground
{
public:
    HOM_GeometryViewportBackground()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_GeometryViewportBackground(const HOM_GeometryViewportBackground &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_GeometryViewportBackground()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual void	setImageFile(const std::string &file_path) = 0;
    virtual std::string imageFile() = 0;
    
    virtual void	setImageCOP(const std::string &op_path) = 0;
    virtual std::string imageCOP() = 0;

    virtual void sourceFromCOP(bool use_cop) = 0;
    virtual bool isCOPSource() = 0;
    virtual bool isFileSource() = 0;

    virtual void	setImageSource(const std::string &file_or_op_path) = 0;
    virtual std::string imageSource() = 0;
    
    virtual void setAutoPlace(bool enable) = 0;
    virtual bool autoPlace() = 0;

    virtual void setImageOffset(const std::vector<double> &offset) = 0;
    virtual std::vector<double> imageOffset() = 0;
    
    virtual void setImageScale(const std::vector<double> &scale) = 0;
    virtual std::vector<double> imageScale() = 0;
    
    virtual void setBrightness(double intensity) = 0;
    virtual double brightness() = 0;
};

#endif

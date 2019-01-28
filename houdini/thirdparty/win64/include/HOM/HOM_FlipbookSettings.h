/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_FlipbookSettings_h__
#define __HOM_FlipbookSettings_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_ObjNode.h"
#include "HOM_EnumValue.h"

SWIGOUT(%rename(FlipbookSettings) HOM_FlipbookSettings;)

class HOM_API HOM_FlipbookSettings
{
public:
    HOM_FlipbookSettings()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_FlipbookSettings(const HOM_FlipbookSettings &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_FlipbookSettings()
    { HOM_DESTRUCT_OBJECT(this) }

    //virtual HOM_FlipbookSettings &operator=(const HOM_FlipbookSettings &) = 0;

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject stash;)
    virtual HOM_FlipbookSettings *stash() = 0;

    virtual void copy(const HOM_FlipbookSettings &from_settings) = 0;

    virtual void outputToMPlay(bool out_mplay) = 0;
    virtual bool outputToMPlay() = 0;

    virtual void leaveFrameAtEnd(bool end) = 0;
    virtual bool leaveFrameAtEnd() = 0;

    virtual void output(const char *out_name) = 0;
    virtual std::string output() = 0;

    virtual void sessionLabel(const char *label) = 0;
    virtual std::string sessionLabel() = 0;

    virtual void visibleObjects(const char *visible) = 0;
    virtual std::string visibleObjects() = 0;

    virtual void visibleTypes(const HOM_EnumValue &flipbookObjectType) = 0;
    virtual HOM_EnumValue &visibleTypes() = 0;

    virtual void frameRange(const std::vector<double> &range) = 0;
    virtual std::vector<double> frameRange() = 0;

    virtual void frameIncrement(int increment) = 0;
    virtual int frameIncrement() = 0;

    virtual void beautyPassOnly(bool enable) = 0;
    virtual bool beautyPassOnly() = 0;

    virtual void renderAllViewports(bool all_views) = 0;
    virtual bool renderAllViewports() = 0;

    virtual void appendFramesToCurrent(bool enable) = 0;
    virtual bool appendFramesToCurrent() = 0;

    virtual void scopeChannelKeyframesOnly(bool enable) = 0;
    virtual bool scopeChannelKeyframesOnly() = 0;
    
    virtual void blockEditing(bool enable) = 0;
    virtual bool blockEditing() = 0;

    virtual void initializeSimulations(bool enable) = 0;
    virtual bool initializeSimulations() = 0;

    virtual std::string audioFilename() = 0;
    virtual void audioFilename(const char *audio_file) = 0;

    virtual void   audioFrameStart(double start) = 0;
    virtual double audioFrameStart() = 0;

    virtual void audioTimeOffset(double time) = 0;
    virtual double audioTimeOffset() = 0;

    virtual void backgroundImage(const char *image_file) = 0;
    virtual std::string backgroundImage() = 0;

    virtual void overrideGamma(bool override) = 0;
    virtual bool overrideGamma() = 0;

    virtual void gamma(double gamma) = 0;
    virtual double gamma() = 0;
    
    virtual void overrideLUT(bool override) = 0;
    virtual bool overrideLUT() = 0;

    virtual void LUT(const char *lut_file) = 0;
    virtual std::string LUT() = 0;

    virtual void antialias(const HOM_EnumValue &flipbookAntialias) = 0;
    virtual HOM_EnumValue &antialias() = 0;

    virtual void useMotionBlur(bool enable) = 0;
    virtual bool useMotionBlur() = 0;

    virtual void motionBlurSegments(int frames) = 0;
    virtual int  motionBlurSegments() = 0;

    virtual void motionBlurFrameRange(const HOM_EnumValue &
							 flipbookMotionBlurBias) = 0;
    virtual HOM_EnumValue &motionBlurFrameRange() = 0;

    virtual void shutterFromCamera(bool use_camera) = 0;
    virtual bool shutterFromCamera() = 0;

    virtual void shutter(double frame_fraction) = 0;
    virtual double shutter() = 0;
    
    
    virtual void useDepthOfField(bool enable) = 0;
    virtual bool useDepthOfField() = 0;

    virtual void depthOfFieldFromCamera(bool use_camera) = 0;
    virtual bool depthOfFieldFromCamera() = 0;

    virtual void depthOfFieldQuality(double num_images) =0;
    virtual double depthOfFieldQuality() =0;

    virtual void focusDistance(double distance) = 0;
    virtual double focusDistance() = 0;

    virtual void aperture(double aperture) = 0;
    virtual double aperture() = 0;

    virtual void fStop(double f_stop) = 0;
    virtual double fStop() = 0;

    virtual void outputZoom(int zoom) = 0;
    virtual int outputZoom() = 0;

    virtual void useResolution(bool use_res) =0;
    virtual bool useResolution() = 0;

    virtual void resolution(const std::vector<int> &image_resolution) = 0;
    virtual std::vector<int> resolution() = 0;
    
    virtual void cropOutMaskOverlay(bool crop_mask) = 0;
    virtual bool cropOutMaskOverlay() = 0;
};

#endif

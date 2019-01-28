//
// Copyright 2016 Pixar
// Copyright 2018 Illumination Research Pte Ltd.
// Authors: J Cube Inc (Marco Pantaleoni, Bo Zhou, Paolo Berto Durante)
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#ifndef HDNSI_RENDER_PASS_H
#define HDNSI_RENDER_PASS_H

#include "pxr/pxr.h"

#include "pxr/imaging/hd/renderPass.h"
#include "pxr/imaging/hd/sprim.h"
#include "pxr/imaging/hdNSI/renderParam.h"

#include "pxr/base/gf/matrix3d.h"
#include "pxr/base/gf/matrix4d.h"

#include <nsi.hpp>
#include <ndspy.h>

PXR_NAMESPACE_OPEN_SCOPE

/// \class HdNSIRenderPass
///
/// HdRenderPass represents a single render iteration, rendering a view of the
/// scene (the HdRprimCollection) for a specific viewer (the camera/viewport
/// parameters in HdRenderPassState) to the current draw target.
///
/// This class does so by raycasting into the NSI scene.
///
class HdNSIRenderPass final : public HdRenderPass {
public:
    /// Renderpass constructor.
    ///   \param index The render index containing scene data to render.
    ///   \param collection The initial rprim collection for this renderpass.
    ///   \param scene The NSI scene to raycast into.
    HdNSIRenderPass(HdRenderIndex *index,
                    HdRprimCollection const &collection,
                    HdNSIRenderParam *renderParam);

    /// Renderpass destructor.
    virtual ~HdNSIRenderPass();

    // -----------------------------------------------------------------------
    // HdRenderPass API

    /// Determine whether the sample buffer has enough samples.
    ///   \return True if the image has enough samples to be considered final.
    virtual bool IsConverged() const override { return false; };

protected:

    // -----------------------------------------------------------------------
    // HdRenderPass API

    /// Draw the scene with the bound renderpass state.
    ///   \param renderPassState Input parameters (including viewer parameters)
    ///                          for this renderpass.
    ///   \param renderTags Which rendertags should be drawn this pass.
    virtual void _Execute(HdRenderPassStateSharedPtr const& renderPassState,
                          TfTokenVector const &renderTags) override;

    /// Update internal tracking to reflect a dirty collection.
    virtual void _MarkCollectionDirty() override {}

private:

    // -----------------------------------------------------------------------
    // Internal API

    // Class used to retrieve the pixel from renderer.
    class DspyImageHandle {
    public:

        int _width, _height;

        int _originalSizeX, _originalSizeY;

        int _originX, _originY;

        int _numFormats;

        std::vector<uint8_t> _buffer;
    };

    static std::map<HdNSIRenderPass *, DspyImageHandle *> _imageHandles;

    // Display Driver - Open callback function.
    static PtDspyError _DspyImageOpen(PtDspyImageHandle *phImage,
                                      const char *driverName,
                                      const char *fileName,
                                      int width, int height,
                                      int paramCount,
                                      const UserParameter *parameters,
                                      int numFormats,
                                      PtDspyDevFormat formats[],
                                      PtFlagStuff *flagStuff);

    // Display Driver - Query callback function.
    static PtDspyError _DspyImageQuery(PtDspyImageHandle hImage,
                                       PtDspyQueryType type,
                                       int dataLen,
                                       void *data);

    // Display Driver - Data callback function.
    static PtDspyError _DspyImageData(PtDspyImageHandle hImage,
                                      int xMin, int xMaxPlusOne,
                                      int yMin, int yMaxPlusOne,
                                      int entrySize,
                                      const unsigned char *cdata);

    // Display Driver - Close callback function.
    static PtDspyError _DspyImageClose(PtDspyImageHandle hImage);

    // The width of the viewport we're rendering into.
    unsigned int _width;
    // The height of the viewport we're rendering into.
    unsigned int _height;

    // A handle to the render param.
    HdNSIRenderParam *_renderParam;

    // Our camera-related handles.
    void _CreateNSICamera();

    std::string _cameraXformHandle;
    std::string _cameraShapeHandle;

    std::string _screenHandle;
    std::string _outputLayerHandle;
    std::string _outputDriverHandle;

    // Our headlight handle.
    void _CreateNSIHeadLight();

    std::string _headlightXformHandle;
    std::string _headlightShapeHandle;
    std::string _headlightGeoAttrsHandle;
    std::string _headlightShaderHandle;

    // Our environment light handles.
    void _CreateNSIEnvironmentLight();

    std::string _envlightXformHandle;
    std::string _envlightShapeHandle;
    std::string _envlightGeoAttrsHandle;
    std::string _envlightShaderHandle;
    std::string _envlightFileShaderHandle;
    std::string _envlightCoordShaderHandle;

    // Update the perspective camera parameters.
    void _UpdateNSICamera();

    // Status of the 3Delight renderer.
    enum RenderStatus {
        Stopped,
        Running
    };
    RenderStatus _renderStatus;

    // The view matrix
    GfMatrix4d _viewMatrix;
    GfMatrix4d _projMatrix;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // HDNSI_RENDER_PASS_H

//
// Copyright 2016 Pixar
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
#ifndef HUSD_TOKENS_H
#define HUSD_TOKENS_H

#include "HUSD_API.h"
#include <pxr/pxr.h>
#include <pxr/base/tf/staticTokens.h>

PXR_NAMESPACE_OPEN_SCOPE

struct HusdHdPrimTypeTokensType {
    HUSD_API HusdHdPrimTypeTokensType();

    const TfToken sprimCamera;

    const TfToken sprimGeometryLight;

    const TfToken bprimHoudiniFieldAsset;

    const TfToken openvdbAsset;
};
extern HUSD_API TfStaticData<HusdHdPrimTypeTokensType> &HusdHdPrimTypeTokens();


/// Tokens for light parameters
struct HusdHdLightTokensType
{
    HUSD_API HusdHdLightTokensType();

    // Light parms
    const TfToken attenStart;
    const TfToken attenType;
    const TfToken attenDist;
    const TfToken coneAngle;
    const TfToken coneDelta;
    const TfToken coneRolloff;
    const TfToken diffuse;
    const TfToken specular;

    // Shadow parms
    const TfToken shadowIntensity;

    // Values
    const TfToken physical;
    const TfToken halfDistance;
    const TfToken activeRadiusEnable;
    const TfToken activeRadius;
    const TfToken shadowType;
    const TfToken shadowOff;
    const TfToken projectMap;
    const TfToken projectAngle;
    const TfToken clipNear;
    const TfToken clipFar;
    
    // Other tokens in the hdLight itself:
    // HdTokens->color
    // HdLightTokens->intensity
    // HdLightTokens->exposure
};
extern HUSD_API TfStaticData<HusdHdLightTokensType> &HusdHdLightTokens();


struct HusdHdPrimvarTokensType
{
    HUSD_API HusdHdPrimvarTokensType();

    // instancing primvars
    const TfToken translate;
    const TfToken rotate;
    const TfToken scale;
    const TfToken instanceTransform;
    const TfToken viewLOD;
    const TfToken uv;
};
extern HUSD_API TfStaticData<HusdHdPrimvarTokensType> &HusdHdPrimvarTokens();

struct HusdHdMaterialTokensType
{
    HUSD_API HusdHdMaterialTokensType();

    // mat types
    const TfToken usdPreviewMaterial;
    const TfToken usdPrimvarReader;
    const TfToken usdUVTexture;

    // preview mat parms
    const TfToken bias;
    const TfToken diffuseColor;
    const TfToken emissiveColor;
    const TfToken specularColor;
    const TfToken clearcoat;
    const TfToken clearcoatRoughness;
    const TfToken displacement;
    const TfToken file;
    const TfToken ior;
    const TfToken metallic;
    const TfToken occlusion;
    const TfToken opacity;
    const TfToken roughness;
    const TfToken scale;
    const TfToken useSpecWorkflow;
    const TfToken wrapS;
    const TfToken wrapT;
};
extern HUSD_API TfStaticData<HusdHdMaterialTokensType> &HusdHdMaterialTokens();


PXR_NAMESPACE_CLOSE_SCOPE

#endif //HUSD_TOKENS_H


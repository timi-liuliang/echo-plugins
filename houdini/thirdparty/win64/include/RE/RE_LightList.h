/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_LightList.h ( RE Library, C++)
 *
 * COMMENTS:
 *    A list of lights which keeps track of those shaders which have already
 *    had them bound as buffer objects. This is not intended to persist over
 *    multiple redraws.
 */
#ifndef RE_LightList_h
#define RE_LightList_h

class RE_Shader;
class RE_Render;
class RE_Light;

#include <UT/UT_ValArray.h>
#include <UT/UT_Map.h>

#include "RE_API.h"
#include "RE_Types.h"
#include "RE_UniformBlock.h"

class RE_API RE_LightList
{
public:
     RE_LightList(int max_lights);
    ~RE_LightList();

    // requires lights to be re-added if the max is lowered under the current
    // # of lights.
    void	setMaxLights(int max);

    void	setLights(UT_ValArray<RE_Light *> &lights,
			  int light_offset = 0,
			  bool include_zeroed_lights = false);
    void	addLight(RE_Light *light);

    int		getNumLights() const   { return myLights.entries(); }
    RE_Light   *getLight(int i) const  { return myLights(i); }

    void	enableLights(const UT_ValArray<RE_Light *> &lights);
    void	enableHeadlight();

    bool	isLightEnabled(int index) const;

    // This will also set the RE_UNIFORM_LIGHT_MASK uniform
    void	bindForShader(RE_Render *r, RE_Shader *sh) const;
    void	bindMaskForShader(RE_Render *r) const;

private:
    int			    myUniqueId;
    int			    mySerial;
    UT_Map<RE_Light *, int> myLightTable;
    UT_ValArray<RE_Light *> myLights;
    int			    myMaxLights;
    int			    myLightMask;
    bool		    myUseHeadlight;

    // modified by bindForShader() only
    mutable RE_UniformBlockHandle   myLightBlockGL4;
};

inline bool
RE_LightList::isLightEnabled(int index) const
{
    return (myLightMask & (1<<index));
}

#endif

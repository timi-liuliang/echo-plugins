/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_HDASection_h__
#define __HOM_HDASection_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_EnumValue.h"
#include "HOM_Module.h"
#include "HOM_IterableList.h"
#include "HOM_PtrOrNull.h"
#include <string>
class HOM_HDADefinition;

SWIGOUT(%rename(HDASection) HOM_HDASection;)

class HOM_API HOM_HDASection
{
public:
    HOM_HDASection()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_HDASection(const HOM_HDASection &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_HDASection()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_HDASection> hda_section) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_HDASection> hda_section) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject definition;)
    virtual HOM_HDADefinition *definition() = 0;

    virtual std::string name() = 0;
    SWIGOUT(%kwargs contents);
    virtual HOM_BinaryString contents(
	HOM_EnumValue &compression_type=HOM_compressionType::NoCompression) = 0;
    SWIGOUT(%kwargs setContents);
    virtual void setContents(
	const std::string &contents, 
	HOM_EnumValue &compression_type=HOM_compressionType::NoCompression) = 0;
    virtual int size() = 0;
    virtual int modificationTime() = 0;
    virtual void destroy() = 0;
};

#endif


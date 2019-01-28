/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Info_h__
#define __HUSD_Info_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_ArrayStringSet.h>

class HUSD_TimeCode;
class UT_Options;

class HUSD_API HUSD_Info
{
public:
    class SchemaInfo {
	public:
				 SchemaInfo(const UT_StringHolder &name,
					const UT_StringHolder &alias)
				     : myName(name),
				       myAlias(alias)
				 { }
				 SchemaInfo(const UT_StringHolder &name)
				     : myName(name),
				       myAlias(name)
				 { }

	    bool		 operator<(const SchemaInfo &other) const
				 {
				     if (myAlias == other.myAlias)
					 return myName < other.myName;

				     return myAlias < other.myAlias;
				 }

	    UT_StringHolder	 myName;
	    UT_StringHolder	 myAlias;
    };

			 HUSD_Info();
			 HUSD_Info(HUSD_AutoAnyLock &lock);
			~HUSD_Info();

    static bool		 isArrayValueType(const UT_String &valueType);
    static bool		 isPrimvarName(const UT_String &name);
    static void		 getPrimitiveKinds(UT_StringArray &kinds);
    static bool		 reload(const UT_String &filepath, bool recursive);

    bool		 isStageValid() const;
    bool		 getStageRootLayer(UT_StringHolder &identifier) const;
    bool		 getSourceLayers(UT_StringArray &names,
				UT_StringArray &identifiers,
				bool include_empty) const;

    bool		 getSchemaInfo(UT_Array<SchemaInfo> &schemas,
				bool concrete_types_only) const;

    bool		 getVariantSets(const UT_StringRef &primpath,
				UT_StringArray &vset_names) const;
    bool		 getVariants(const UT_StringRef &primpath,
				const UT_StringRef &variantset,
				UT_StringArray &vset_names) const;
    UT_StringHolder	 getVariantSelection(const UT_StringRef &primpath,
				const UT_StringRef &variantset) const;

    bool		 getCollections(const UT_StringRef &primpath,
				bool recursive,
				UT_StringArray &collection_paths) const;

    UT_StringHolder	 getAncestorOfKind(const UT_StringRef &primpath,
				const UT_StringRef &kind) const;

    bool		 isValidPrim(const UT_StringRef &primpath,
				const UT_StringRef &prim_type =
				    UT_StringHolder::theEmptyString) const;
    UT_StringHolder	 getPrimType(const UT_StringRef &primpath) const;
    UT_StringHolder	 getAutoParentPrimKind(
				const UT_StringRef &primpath) const;

    static const UT_StringHolder &getTransformAttribName();
    static const UT_StringHolder &getTimeVaryingAttribName();
    void		 getAttributeNames(const UT_StringRef &primpath,
				UT_ArrayStringSet &attrib_names) const;
    void		 extractAttributes(const UT_StringRef &primpath,
				const UT_ArrayStringSet &which_attribs,
				const HUSD_TimeCode &tc,
				UT_Options &values) const;

private:
    HUSD_AutoAnyLock	*myAnyLock;
};

#endif


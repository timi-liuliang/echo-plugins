/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GDT Library. (Geo Delta)
 *
 * COMMENTS:	GDT_TokenList is an abstract class for a list of attribute
 *		tokens.
 *
 */

#ifndef __GDT_TokenList_h__
#define __GDT_TokenList_h__

#include "GDT_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>
#include <GA/GA_Types.h>

class GA_AttributeDict;
class GA_AttributeFilter;
class GA_Attribute;
class GA_Defaults;
class GA_Range;
class GA_AIFEditDeltaMap;
class GDT_Attribute;
class GDT_JSON;
class GEO_Detail;
class UT_String;
class UT_IStream;
class UT_Options;
class UT_JSONWriter;
class UT_JSONParser;

class GDT_API GDT_TokenList
{
public:

    GDT_TokenList();
    GDT_TokenList(const GDT_TokenList &other);
    virtual ~GDT_TokenList() = 0;

    virtual int64	 getMemoryUsage(bool inclusive=false) const;
   
    // Apply the stored deltas to the given gdp
    virtual void	 applyDeltas(GEO_Detail &gdp, bool inverse=0) const = 0;

    // Zero (but don't destroy) the attribute deltas
    virtual void	 zeroDeltas();
    // Destroy the attribute delta lists
    virtual void	 clearDeltas();
    // Destroy the attribute delta lists and attributes
    virtual void	 destroy();

    
    // Make sure the attributes in the dictionary exist in this
    // token list, in preparation for attribute changes using
    // beginAttributeChange/endAttributeChange.  Any begin/end calls
    // will be working on elements with the given attribute dictionary.
    void		 setAttribDict(const GA_AttributeDict &dict,
				       const GA_AttributeFilter &filter);

    // Refresh attributes added from setAttribDict.
    // This function assumes that your geometry topology has not changed,
    // but attributes may have been added, deleted, or changed.  Observe
    // that this function will NOT add missing attribute tokens.
    void		 refreshAttribDict(const GA_AttributeDict &dict,
					   const GA_AttributeFilter &filter);

    // Begin and end a change to the element specified by index.
    // The delta is computed and stored based on the attributes in the
    // dictionary given in the call to addAttributesFromDict.
    void		 beginAttributeChange(GA_Offset element);
    // Begin and end a change to a range of elements.
    // The deltas for all the elements in the range are computed and stored
    // based on the attributes in the dictionary given in the call to
    // addAttributesFromDict.
    void		 beginAttributeChange(const GA_Range &elements);
    void		 endAttributeChange(const GA_AIFEditDeltaMap &map);

    // Get the attribute dictionary associated with this list
    virtual const GA_AttributeDict  &getAttributeDict(
						const GEO_Detail &gdp) const=0;

    // Returns true if this list will have an effect on the gdp if its deltas
    // are applied, false otherwise.
    // This is useful because if you need to know if the list
    // has anything worth saving or applying, then knowing the number of
    // attributes isn't enough (all the attributes might be empty).
    virtual bool       	       hasChange() const;

    // I/O Methods
    virtual bool	       legacyLoadAttribs(UT_IStream &is, uint size);
    bool		       saveAttribs(UT_JSONWriter &w,
					   const GDT_JSON &json) const;
    bool		       loadAttribs(UT_JSONParser &p,
					   const GDT_JSON &json);

    // Searches through myAttributes and finds the attribute named
    // oldname and sets its name to newname.
    void                       renameAttribute(const UT_String &oldname,
			       const UT_String &newname);

protected:
    // Called by subclasses in function applyDeltas
    void		 applyAttributeDeltas(GEO_Detail &gdp,
					      const GA_AIFEditDeltaMap &map,
					      bool inverse = false) const;

    // Called by subclasses to scale and apply our stored attribute deltas
    void		 applyScaledAttributeDeltas(GEO_Detail &gdp,
						    fpreal64 scale,
						    const GA_AIFEditDeltaMap &,
						    const UT_Int64Array *subset,
						    bool inverse = false) const;

    // Called by subclasses in function mergeDeltas
    void	 	 mergeAttributeDeltas(const GDT_TokenList &other,
					      bool inverse = false);

    // Returns the index in the token list that is currently associated with
    // the specified attribute, and -1 if none.  There may be several tokens
    // in the list associated with a given attribute name, but only one will
    // be bound to an attribute at any given time.  Only call this method if
    // the attribute binding is up to date.
    int			 getBoundAttributeIndex(const UT_String &name) const;

    const GA_Attribute	*getBoundAttribute(int i) const;

    // Adds an attribute and returns its index in the array
    unsigned int	 appendAttribute(const UT_String &name, 
					 const UT_String &type,
					 const UT_Options &creation_options,
					 const GA_Defaults &defaults);
    unsigned int	 appendAttribute(const GA_Attribute &attrib);

    GDT_Attribute 	*attribs(unsigned int i) { return myAttributes(i); }
    const GDT_Attribute *attribs(unsigned int i) const 
						{ return myAttributes(i); }
    int			 numAttribs() const { return myAttributes.entries(); }

private:
    // Attribute deltas.
    UT_ValArray<GDT_Attribute *>	 myAttributes;

    // Temporary attribute references for use during beginAttributeChange()/
    // endAttributeChange() blocks.  We don't use GA_ROAttributeRefs here as
    // those increment reference counts on the GA_Attribute objects, and we
    // have no API for undoing the setAttribDict() call.
    UT_ValArray<const GA_Attribute *>	 myAttributeRefs;
};
#endif

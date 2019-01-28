/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_AttributeIndexPairs.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_AttributeIndexPairs__
#define __GEO_AttributeIndexPairs__

#include "GEO_API.h"
#include "GEO_Detail.h"
#include <GA/GA_Types.h>
#include <SYS/SYS_Types.h>

class GEO_API GEO_AttributeCapturePath
{
public:
    GEO_AttributeCapturePath()
	: myAttrib(0), myIndexPair(0)
    {
    }

    GEO_AttributeCapturePath(const GEO_Detail *gdp,
	GEO_Detail::CaptureType type = GEO_Detail::CAPTURE_BONE)
    {
	reset(gdp, type);
    }

    void reset(const GEO_Detail *gdp,
	       GEO_Detail::CaptureType type = GEO_Detail::CAPTURE_BONE);

    bool isValid() const { return myAttrib && myIndexPair; }
    int getNumPaths() const;
    const char *getPath(int i) const;
    int getIndex(const char *path) const;
    void clear(void);

protected:
    const GA_Attribute *myAttrib;
    const GA_AIFIndexPair *myIndexPair;
};

class GEO_API GEO_RWAttributeCapturePath : public GEO_AttributeCapturePath
{
public:
    GEO_RWAttributeCapturePath(
	    GEO_Detail *gdp,
	    GEO_Detail::CaptureType type = GEO_Detail::CAPTURE_BONE)
	: GEO_AttributeCapturePath(gdp, type)
    {
    }
    void setPath(int i, const char *path) const;
};

/// @brief This class provides a convenient abstraction of the contents of an
/// index pair attribute for a single element.
///
/// It is important to note that this class does not perform any sort of bounds
/// checking.
///
/// @warning This is being used internally by Side Effects to simplify forward
/// compatibility issues.  The lifetime of this class is not well known and
/// code should not rely on any methods herein.
class GEO_API GEO_AttributeIndexPairs
{
public:
    /// No Default constructor.  An instance must always be bound.
    /// @pre ref.isValid() == true
    GEO_AttributeIndexPairs(GA_Offset elem_off,
		const GA_RWAttributeRef &ref) { bind(elem_off, ref); }
    GEO_AttributeIndexPairs(GA_Offset elem_off, GA_Attribute *attrib,
                            const GA_AIFIndexPair *aifindexpair)
    { bind(elem_off, attrib, aifindexpair); }

    /// Destructor
    ~GEO_AttributeIndexPairs() { /* NOP */ }

    /// Bind a element
    /// @pre ref.isValid() == true
    /// @{
    void bind(GA_Offset elem_off, GA_Attribute *attrib,
              const GA_AIFIndexPair *aifindexpair)
    {
        myOffset = elem_off;
        myAttribute = attrib;
        myIndexPair = aifindexpair;
    }
    void	 bind(GA_Offset elem_off, const GA_RWAttributeRef &ref);
    /// @}

    int32	 getEntryIndex(int n) const;
    fpreal32	 getEntryValue(int n, int component=0) const;
    void	 getEntry(int n, int32 &index, fpreal32 &value) const
			 {
			     index = getEntryIndex(n);
			     value = getEntryValue(n);
			 }

    void	 setEntryIndex(int n, int32 index);
    void	 setEntryValue(int n, fpreal32 value, int component=0);
    void	 setEntry(int n, int32 index, fpreal32 value)
			 {
			     setEntryIndex(n, index);
			     setEntryValue(n, value);
			 }

    void	 offsetEntryValue(int n, fpreal32 o, int component=0)
			 { setEntryValue(n, getEntryValue(n, component) + o, component); }
    void	 scaleEntryValue(int n, fpreal32 s, int component=0)
			 { setEntryValue(n, getEntryValue(n, component) * s, component); }

private:
    /// Copy constructor - not available (copying not permitted)
    GEO_AttributeIndexPairs(const GEO_AttributeIndexPairs &);

    /// Assignment operator - not available (copying not permitted)
    GEO_AttributeIndexPairs &operator=(const GEO_AttributeIndexPairs &);

    GA_Attribute *myAttribute;
    const GA_AIFIndexPair *myIndexPair;
    GA_Offset myOffset;
};

/// @brief This class provides a convenient abstraction of the contents of an
/// index pair attribute for a single const element.
///
/// @warning This is being used internally by Side Effects to simplify forward
/// compatibility issues.  The lifetime of this class is not well known and
/// code should not rely on any methods herein.
class GEO_API GEO_ConstAttributeIndexPairs
{
public:
    /// No Default constructor.  An instance must always be bound.
    /// @pre ref.isValid() == true
    GEO_ConstAttributeIndexPairs(GA_Offset elem_off,
		const GA_ROAttributeRef &ref) { bind(elem_off, ref); }
    GEO_ConstAttributeIndexPairs(
        GA_Offset elem_off, const GA_Attribute *attrib,
        const GA_AIFIndexPair *aifindexpair)
    { bind(elem_off, attrib, aifindexpair); }

    /// Destructor
    ~GEO_ConstAttributeIndexPairs() { /* NOP */ }

    /// Bind a element
    /// @pre ref.isValid() == true
    /// @{
    void bind(GA_Offset elem_off, const GA_Attribute *attrib,
              const GA_AIFIndexPair *aifindexpair)
    {
        myOffset = elem_off;
        myAttribute = attrib;
        myIndexPair = aifindexpair;
    }
    void	 bind(GA_Offset elem_off, const GA_ROAttributeRef &ref);
    /// @}

    int32	 getEntryIndex(int n) const;
    fpreal32	 getEntryValue(int n, int component=0) const;
    void	 getEntry(int n, int32 &index, fpreal32 &value) const
			{ index = getEntryIndex(n); value = getEntryValue(n); }

private:
    /// Copy constructor - not available (copying not permitted)
    GEO_ConstAttributeIndexPairs(const GEO_ConstAttributeIndexPairs &);

    /// Assignment operator - not available (copying not permitted)
    GEO_ConstAttributeIndexPairs &operator=(const GEO_ConstAttributeIndexPairs &);

    const GA_Attribute *myAttribute;
    const GA_AIFIndexPair *myIndexPair;
    GA_Offset myOffset;
};

#endif

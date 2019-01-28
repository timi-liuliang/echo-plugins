/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFFileH9.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface for .geo/.bgeo file I/O
 */

#ifndef __GA_AIFFileH9__
#define __GA_AIFFileH9__

#include "GA_API.h"

#include "GA_AttributeType.h"
#include "GA_Types.h"

#include <UT/UT_String.h>
#include <UT/UT_VectorTypes.h>

#include <iosfwd>


class GA_Attribute;
class GA_Defaults;
class GA_AIFTuple;
class GA_AIFSharedStringTuple;
class GA_AIFIndexPair;
class UT_IStream;


/// @brief Attribute Interface for file I/O
/// 
/// This class provides an interface used to save and load attributes to and
/// from files.
///
/// In H9, the attributes stored their data interleaved for each element. GA
/// prefers to process data in arrays rather than interleaved.  This class
/// contains SaveInfo and LoadInfo which help with the
/// interleaving/de-interleaving process.
///
class GA_API GA_AIFFileH9 : public GA_AIFBase 
{
public:
    typedef GA_AIFDefinition<GA_AIFFileH9>	Definition;

	     GA_AIFFileH9() {}
    virtual ~GA_AIFFileH9() {}

    /// Data types stored in Houdini 9 data files
    enum AttribType
    {
	ATTRIB_FLOAT	= 0,
	ATTRIB_INT	= 1,
	ATTRIB_STRING	= 2,
	ATTRIB_MIXED	= 3,	// Impossible to save and load
	ATTRIB_INDEX	= 4,	// Index of strings
	ATTRIB_VECTOR	= 5	// Same as float, but gets transformed
				//  when geometry is transformed.
				//  the vector type is always 3 floats
    };

    /// Attribute type modifiers stored in Houdini 9 data files
    enum AttribTypeInfo
    {
	ATTRIB_INFO_NONE       = 0,    // no info available
	ATTRIB_INFO_INDEX_PAIR = 1,    // data are index-value pairs
	ATTRIB_NUM_INFO                // sentinel
    };

    // TODO: Support saving multiple H9 attributes when a single one will not
    //       capture all the data.
    /// Class used to cache information during the H9 save process
    class GA_API SaveInfo
    {
	public:
	    SaveInfo()
		: myType(ATTRIB_MIXED), myTypeInfo(ATTRIB_INFO_NONE),
		  myTupleSize(0), myComponent(0), myTupleAIF(0),
		  myStringTupleAIF(0), myIndexPairAIF(0)
	    {}
	    SaveInfo(const SaveInfo &src)
		: myType(src.myType),
		  myTypeInfo(src.myTypeInfo),
		  myTupleSize(src.myTupleSize),
		  myStringMap(src.myStringMap),
		  myComponent(src.myComponent),
		  myTupleAIF(src.myTupleAIF),
		  myStringTupleAIF(src.myStringTupleAIF),
		  myIndexPairAIF(src.myIndexPairAIF)

	    {}
	    SaveInfo(AttribType type, AttribTypeInfo type_info, int tuple_size,
		     const GA_AIFTuple *tuple, const GA_AIFSharedStringTuple *sindex,
		     const GA_AIFIndexPair *ipair, int component)
		: myType(type), myTypeInfo(type_info), myTupleSize(tuple_size),
		  myComponent(component), myTupleAIF(tuple),
		  myStringTupleAIF(sindex), myIndexPairAIF(ipair)
	    {}
	    SaveInfo	&operator=(const SaveInfo &src)
			{
			    myType = src.myType;
			    myTypeInfo = src.myTypeInfo;
			    myTupleSize = src.myTupleSize;
			    myStringMap = src.myStringMap;
			    myComponent = src.myComponent;
			    myTupleAIF = src.myTupleAIF;
			    myStringTupleAIF = src.myStringTupleAIF;
			    myIndexPairAIF = src.myIndexPairAIF;
			    return *this;
			}

	    AttribType		 getType() const { return myType; }
	    AttribTypeInfo	 getTypeInfo() const { return myTypeInfo; }
	    int			 getTupleSize() const { return myTupleSize; }
	    const UT_IntArray	&getStringMap()	const { return myStringMap; }
	    UT_IntArray		&getStringMap()	{ return myStringMap; }
	    int			 getComponent() const { return myComponent; }

	    const GA_AIFTuple		*getTupleAIF() const
						{ return myTupleAIF; }
	    const GA_AIFSharedStringTuple	*getStringTupleAIF() const
						{ return myStringTupleAIF; }
	    const GA_AIFIndexPair	*getIndexPairAIF() const
						{ return myIndexPairAIF; }
	private:
	    AttribType		 myType;
	    AttribTypeInfo	 myTypeInfo;
	    int			 myTupleSize;
	    UT_IntArray		 myStringMap;
	    int			 myComponent;

	    const GA_AIFTuple		*myTupleAIF;
	    const GA_AIFSharedStringTuple	*myStringTupleAIF;	
	    const GA_AIFIndexPair	*myIndexPairAIF;
    };

    // TODO: Consolidate with SaveInfo?
    /// Class used to cache information during the H9 save process
    class LoadInfo
    {
	public:
	    LoadInfo()
		: myType(ATTRIB_MIXED), myTypeInfo(ATTRIB_INFO_NONE),
		  myTupleSize(0), myTupleAIF(0), myStringTupleAIF(0),
		  myIndexPairAIF(0)
	    { }
	    LoadInfo(const LoadInfo &src)
		: myType(src.myType),
		  myTypeInfo(src.myTypeInfo),
		  myTupleSize(src.myTupleSize),
		  myStringMap(src.myStringMap),
		  myTupleAIF(src.myTupleAIF),
		  myStringTupleAIF(src.myStringTupleAIF),
		  myIndexPairAIF(src.myIndexPairAIF)

	    { }
	    LoadInfo(AttribType type, AttribTypeInfo type_info, int tuple_size,
		     const GA_AIFTuple *tuple, const GA_AIFSharedStringTuple *sindex,
		     const GA_AIFIndexPair *ipair)
		: myType(type), myTypeInfo(type_info), myTupleSize(tuple_size),
		  myTupleAIF(tuple), myStringTupleAIF(sindex),
		  myIndexPairAIF(ipair)
	    { }
	    ~LoadInfo()
	    { }

	    AttribType		 getType() const { return myType; }
	    AttribTypeInfo	 getTypeInfo() const { return myTypeInfo; }
	    int			 getTupleSize() const { return myTupleSize; }
	    const UT_IntArray	&getStringMap()	const { return myStringMap; }
	    UT_IntArray		&getStringMap()	{ return myStringMap; }

	    const GA_AIFTuple		*getTupleAIF() const
						{ return myTupleAIF; }
	    const GA_AIFSharedStringTuple	*getStringTupleAIF() const
						{ return myStringTupleAIF; }
	    const GA_AIFIndexPair	*getIndexPairAIF() const
						{ return myIndexPairAIF; }
	    LoadInfo	&operator=(const LoadInfo &src)
			{
			    myType = src.myType;
			    myTypeInfo = src.myTypeInfo;
			    myTupleSize = src.myTupleSize;
			    myStringMap = src.myStringMap;
			    myTupleAIF = src.myTupleAIF;
			    myStringTupleAIF = src.myStringTupleAIF;
			    myIndexPairAIF = src.myIndexPairAIF;
			    return *this;
			}
	private:
	    AttribType		 myType;
	    AttribTypeInfo	 myTypeInfo;
	    int			 myTupleSize;
	    UT_IntArray		 myStringMap;

	    const GA_AIFTuple		*myTupleAIF;
	    const GA_AIFSharedStringTuple	*myStringTupleAIF;	
	    const GA_AIFIndexPair	*myIndexPairAIF;
    };

    /// Create save information for a given GA attribute
    virtual SaveInfo	 getSaveInfo(const GA_Attribute *attrib, int component=0) const;

    /// Save the attribute defaults to the output stream.
    /// @param attrib	The attribute to save
    /// @param info	SaveInfo class helper
    /// @param os	The output stream
    /// @param binary	Save as ASCII (false) or binary (true)
    virtual bool	 saveDefaults(const GA_Attribute *attrib,
				      const SaveInfo &info, std::ostream &os,
				      bool binary) const;

    /// Save the string table to the output stream.  This method should only 
    /// be called when getSaveInfo(attrib).getType() == ATTRIB_INDEX.
    /// @param attrib	The attribute
    /// @param os	The output stream
    /// @param binary	Save as ASCII (false) or binary (true)
    ///	@param stringmap A mapping of string handles to their in-order indices.
    virtual bool	 saveStrings(const GA_Attribute *attrib,
				     std::ostream &os,
				     bool binary,
				     UT_IntArray &stringmap) const;

    /// Save the element data for a given attribute
    virtual bool	 saveData(const GA_Attribute *attrib,
				  const SaveInfo &info, GA_Offset offset,
				  std::ostream &os, bool binary) const;

    /// Create load information for an H9 attribute
    virtual LoadInfo	 getLoadInfo(const GA_Attribute *attrib,
				     AttribType type, AttribTypeInfo type_info,
				     int tuple_size) const;

    /// Load the attribute data for a given element
    virtual bool	 loadData(GA_Attribute *attrib, UT_IStream &is,
				  const LoadInfo &info, GA_Offset offset) const;

    /// Call once finished loading the attribute data for all the elements to
    /// perform any clean up or finalization that may be needed.
    virtual bool	 finishLoad(GA_Attribute *attrib,
				    const LoadInfo &info) const;

    /// Skip data unsupported data in the input stream
    static bool		 skipData(UT_IStream &is, const LoadInfo &info);

    /// Query the definition for this custom AIF.  The first call will register
    /// this AIF in a thread-safe manner.
    static Definition	 getDefinition();

protected:
    bool		 saveDefaultsImpl(const GA_Defaults *defs,
					  const SaveInfo &info,
					  std::ostream &os,
					  bool binary, char lead_ch = ' ')
					const;
};

/// Class used to map the GA attribute into a form for H9 geometry files
class GA_API GA_AttribSaveDataH9
{
public:
    typedef GA_AIFFileH9::SaveInfo	SaveInfo;

    GA_AttribSaveDataH9() : myAttrib(0), myFileAIF(0), myIndexPairAIF(0) {}
    GA_AttribSaveDataH9(const GA_AttribSaveDataH9 &src)
	: myName(src.myName, 1),
	  myAttrib(src.myAttrib),
	  myFileAIF(src.myFileAIF),
	  myIndexPairAIF(src.myIndexPairAIF),
	  myIndex(src.myIndex),
	  mySet(src.mySet),
	  myInfo(src.myInfo)
    { }
    GA_AttribSaveDataH9(const GA_Attribute *attrib, const GA_AIFFileH9 *aif, const char *name, int component=0)
	: myName(name, 1), myAttrib(attrib), myFileAIF(aif), myIndexPairAIF(0), myIndex(), mySet(0), myInfo(aif->getSaveInfo(attrib, component))
    { }
    GA_AttribSaveDataH9(const GA_Attribute *attrib, const GA_AIFIndexPair *aif, int idx, int s);

    GA_AttribSaveDataH9	&operator=(const GA_AttribSaveDataH9 &src)
			{
			    myName.harden(src.myName);
			    myAttrib = src.myAttrib;
			    myFileAIF = src.myFileAIF;
			    myIndexPairAIF = src.myIndexPairAIF;
			    myIndex = src.myIndex;
			    mySet = src.mySet;
			    myInfo = src.myInfo;
			    return *this;
			}

    const SaveInfo		&getInfo() const { return myInfo; }
    const char			*getName() const { return myName; }
    bool			 saveData(
				    GA_Offset offset,
				    std::ostream &os,
				    bool binary) const;
    bool			 saveStrings(
				    std::ostream &os,
				    bool binary) const;
    bool			 saveDefaults(
				    std::ostream &os,
				    bool binary) const;

private:
    UT_String			 myName;
    const GA_Attribute		*myAttrib;
    const GA_AIFFileH9		*myFileAIF;
    const GA_AIFIndexPair	*myIndexPairAIF;
    int				 myIndex;
    int				 mySet;
    SaveInfo		 	 myInfo;
};

/// Class used to map H9 geometry files to a form used by GA
class GA_API GA_AttribLoadDataH9
{
public:
    typedef GA_AIFFileH9::LoadInfo	LoadInfo;

    GA_AttribLoadDataH9() : myAttrib(0), myFileAIF(0)
    { }
    GA_AttribLoadDataH9(const GA_AttribLoadDataH9 &src)
	: myAttrib(src.myAttrib),
	  myFileAIF(src.myFileAIF),
	  myInfo(src.myInfo)
    { }
    GA_AttribLoadDataH9(GA_Attribute *attrib, const GA_AIFFileH9 *aif,
			const LoadInfo &info)
	: myAttrib(attrib), myFileAIF(aif), myInfo(info)
    { }
    ~GA_AttribLoadDataH9()
    { }

    GA_AttribLoadDataH9	&operator=(const GA_AttribLoadDataH9 &src)
			{
			    myAttrib = src.myAttrib;
			    myFileAIF = src.myFileAIF;
			    myInfo = src.myInfo;
			    return *this;
			}

    GA_Attribute		*getAttribute() const { return myAttrib; }
    const GA_AIFFileH9		*getFileAIF() const { return myFileAIF; }
    const LoadInfo		&getInfo() const { return myInfo; }
    UT_IntArray			&getStringMap()	{ return myInfo.getStringMap();}

private:
    GA_Attribute		*myAttrib;
    const GA_AIFFileH9		*myFileAIF;
    LoadInfo		 	 myInfo;
};

#endif

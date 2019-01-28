/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OTLDefinition.h ( OTL Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __OP_OTLDefinition__
#define __OP_OTLDefinition__

#include "OP_API.h"
#include "OP_Node.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_ValArray.h>
#include <iosfwd>

class UT_String;
class UT_WorkBuffer;


// In addition to alphanumeric and '_' characters, these are the characters
// that are legal in the operator type name.
// '.' - allowed for version numbering
// ':' - used for namespaces
// '/' - used for namespaces (scoping within another operator, which requires
//       a slash for "table/opname" specification)
#define OTL_LEGAL_OPTYPE_NAME_CHARS	".:/"


class OP_API OP_OTLDefinition
{
public:
	     OP_OTLDefinition();

    OP_OTLDefinition(const OP_OTLDefinition &definition) = default;
    OP_OTLDefinition &operator=(const OP_OTLDefinition &src) = default;

    const UT_StringHolder &getName() const
			 { return myName; }
    void		 setName(const UT_StringHolder &name)
			 { myName = name; }
    const UT_StringHolder &getLabel() const
			 { return myLabel; }
    void		 setLabel(const UT_StringHolder &label)
			 { myLabel = label; }
    const UT_StringHolder &getPath() const
			 { return myPath; }
    void		 setPath(const UT_StringHolder &path)
			 { myPath = path; }
    const UT_StringHolder &getIcon() const
			 { return myIcon; }
    void		 setIcon(const UT_StringHolder &icon)
			 { myIcon = icon; }
    const UT_StringHolder &getAuthor() const
			 { return myAuthor; }
    void		 setAuthor(const UT_StringHolder &author)
			 { myAuthor = author; }
    const UT_StringHolder &getCopyright() const
			 { return myCopyright; }
    void		 setCopyright(const UT_StringHolder &copyright)
			 { myCopyright = copyright; }
    const UT_StringHolder &getOpTableName() const
			 { return myOpTableName; }
    void		 setOpTableName(const UT_StringHolder &optablename)
			 { myOpTableName = optablename; }
    const UT_StringHolder &getIndexPath() const
			 { return myIndexPath; }
    void		 setIndexPath(const UT_StringHolder &indexpath)
			 { myIndexPath = indexpath; }
    const UT_StringHolder &getIndexFile() const
			 { return myIndexFile; }
    void		 setIndexFile(const UT_StringHolder &indexfile)
			 { myIndexFile = indexfile; }
    const UT_StringHolder &getLicenseInfo() const
			 { return myLicenseInfo; }
    void		 setLicenseInfo(const UT_StringHolder &licenseinfo)
			 { myLicenseInfo = licenseinfo; }
    const UT_StringHolder &getExtraInfo() const
			 { return myExtraInfo; }
    void		 setExtraInfo(const UT_StringHolder &extrainfo)
			 { myExtraInfo = extrainfo; }
    const UT_StringHolder &getUserInfo() const
			 { return myUserInfo; }
    void		 setUserInfo(const UT_StringHolder &userinfo)
			 { myUserInfo = userinfo; }
    int			 getMinInputs() const
			 { return myMinInputs; }
    void		 setMinInputs(int inputs)
			 { myMinInputs = inputs; }
    int			 getMaxInputs() const
			 { return myMaxInputs; }
    void		 setMaxInputs(int inputs)
			 { myMaxInputs = inputs; }
    int			 getMaxOutputs() const
			 { return myMaxOutputs; }
    void		 setMaxOutputs(int outputs)
			 { myMaxOutputs = outputs; }
    int			 getModTime() const
			 { return myModTime; }
    void		 setModTime(int modtime)
			 { myModTime = modtime; }
    bool		 getIsSubnet() const
			 { return myIsSubnet; }
    void		 setIsSubnet(bool issubnet)
			 { myIsSubnet = issubnet ? 1 : 0; }
    bool		 getIsPython() const
			 { return myIsPython; }
    void		 setIsPython(bool ispython)
			 { myIsPython = ispython ? 1 : 0; }
    bool		 getIsDummyDefinition() const
			 { return myIsDummyDefinition; }
    void		 setIsDummyDefinition(bool isdummy)
			 { myIsDummyDefinition = isdummy ? 1 : 0; }
    OP_OTLLicenseType	 getCachedLicenseType() const
			 { return myCachedLicenseType; }
    void		 setCachedLicenseType(OP_OTLLicenseType type)
			 { myCachedLicenseType = type; }

    /// Return if this is an internally defined operator
    bool		 isInternal() const
			 { return myPath == theInternalPath.asRef(); }

    /// Mark as internal
    void		 setPathToInternal()
			 { setPath(theInternalPath.asHolder()); }

    // Handle options that appear in index files (-icon, -inputs, etc.).
    int			 handleOption(int argc, char *argv[]);
    // Concatenates the op table name and the op name to get a unique name.
    void		 getTableAndName(UT_WorkBuffer &name) const;
    void		 getTableAndName(UT_String &name) const;

    // Converts the table name into an OP_OpTypeId.
    OP_OpTypeId		 getOpTypeId() const;

    bool			 load(UT_IStream &is,
				      UT_StringHolder *errors = nullptr);
    bool			 save(std::ostream &os);
    // Writes a text description that is easily readable by the user.
    void			 writeTextDescription(UT_WorkBuffer &buffer,
				      const char *indent = 0) const;
    // Writes a text description based on the format string. The format
    // string is a series of characters. Each character represents one
    // piece of information. The information is output on a single line.
    void			 writeFormattedText(UT_WorkBuffer &buffer,
				      const char *format) const;
    // Read in a text description written out as above. We return false if an
    // invalid or malformed description is found. We return true if we read in
    // a good description _or_ if we reach the end of the stream before even
    // starting a description.
    bool			 readTextDescription(UT_IStream &is);

    /// Returns an official name of the library section that contains 
    /// the HDA definition eg "Object/hda", or "artistA::Object/hda::2.0".
    /// See UT_OpUtils::getOpTypeDefSectionName() for more details.
    /// @{
    void		 getDefinitionSectionName(UT_String &sectname) const;
    void		 getDefinitionSectionName(UT_WorkBuffer &sectname) const;
    /// @}

    /// Returns an official file path that should be used for a given
    /// definition, eg "oplib:/Object/hda?Object/hda".
    void		 constructDefinitionPath(UT_String &path) const;

    /// Utility function for converting a string to a modification time.
    static int		 getModTimeFromString(const char *str);

    /// Returns true if the name is a valid operator name that can be used for
    /// an OP_Operator instance.
    static bool		 isValidOperatorName(const char *name);

    /// Returns true if op_name includes a namespace and that namesapce
    /// matches the given scope_opname, e.g., op_name of "Object/geo::op" 
    /// matches scope_name of "Object/geo".
    /// If op_name does not include a namespace, it is allowed everywhere,
    /// so it will match any scope, thus this function will return true.
    /// Returns false otherwise.
    static bool		 isMatchingNamespaceScope( const char *op_name,
					    const char *scope_opname );

private:
    UT_StringHolder		 myName;
    UT_StringHolder		 myLabel;
    UT_StringHolder		 myPath;
    UT_StringHolder		 myIcon;
    UT_StringHolder		 myAuthor;
    UT_StringHolder		 myCopyright;
    UT_StringHolder		 myOpTableName;
    UT_StringHolder		 myIndexPath;
    UT_StringHolder		 myIndexFile;
    UT_StringHolder		 myLicenseInfo;
    UT_StringHolder		 myExtraInfo;
    UT_StringHolder		 myUserInfo;
    int32			 myMinInputs;
    int32			 myMaxInputs;
    int32			 myMaxOutputs;
    int32			 myModTime;
    char			 myIsSubnet;
    char			 myIsPython;
    char			 myIsDummyDefinition;
    OP_OTLLicenseType		 myCachedLicenseType;

    // Path value that marks definition as isInternal()
    static const UT_StringLit	 theInternalPath;
};

typedef UT_ValArray<OP_OTLDefinition *> OP_OTLDefinitionArray;

#endif


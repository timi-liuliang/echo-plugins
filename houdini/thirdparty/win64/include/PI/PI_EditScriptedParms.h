/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_EditScriptedParms.h ( OP Library, C++)
 *
 * COMMENTS:	Provides a data structure for holding all the information
 * 		required to edit the parameters of a PI_ScriptOperator.
 */

#ifndef __PI_EditScriptedParms__
#define __PI_EditScriptedParms__

#include "PI_API.h"
#include <UT/UT_Color.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ValArray.h>
#include <CH/CH_ExprLanguage.h>
#include <PRM/PRM_Type.h>
#include <OP/OP_Node.h>
#include <iosfwd>

class PRM_Template;
class PRM_SpareData;
class PRM_Parm;
class PRM_Range;
class PRM_ChoiceList;
class PRM_ScriptImports;
class OP_Operator;
class PI_EditScriptedParms;
class pi_GroupData;

#define PI_MAX_SCRIPT_PARM_SIZE	16
#define PI_FOLDER_DEFAULTNAME	"folder0"

typedef enum {
    PI_MENU_NORMAL = 0,
    PI_MENU_MINI = 1,
    PI_MENU_JOIN = 2,
    PI_MENU_REPLACE = 3,
    PI_MENU_TOGGLE = 4
} PI_EditScriptedParmMenuType;

typedef enum {
    PI_MENU_NONE = 0,
    PI_MENU_ITEMS = 1,
    PI_MENU_SCRIPT = 2
} PI_EditScriptedParmMenuEnable;

typedef enum {
    PI_FOLDER_COLLAPSIBLE,
    PI_FOLDER_SIMPLE,
    PI_FOLDER_TAB,
    PI_FOLDER_RADIO,
    PI_FOLDER_IMPORT,
    PI_FOLDER_MULTILIST,
    PI_FOLDER_MULTISCROLL,
    PI_FOLDER_MULTISWITCHER,
    PI_FOLDER_ENDOFFOLDER,
    PI_FOLDER_NOTAFOLDER
} PI_EditScriptedParmFolderType;

typedef UT_SymbolMap<UT_StringArray *> PI_BuiltMultiParmInfo;

class PI_API PI_EditScriptedParmMenu
{
public:
    bool	isValidChoice() const
		{
		    return myToken.isstring() || myLabel.isstring();
		}

    UT_StringHolder	myToken;
    UT_StringHolder	myLabel;
};

class PI_API PI_EditScriptedParm
{
public:
     PI_EditScriptedParm();
     PI_EditScriptedParm(const PRM_Template &tplate, OP_Node *node,
			 bool init_auto_links=true);
     PI_EditScriptedParm(PI_EditScriptedParmFolderType ftype);
     PI_EditScriptedParm(const PI_EditScriptedParm &src);
    ~PI_EditScriptedParm();

    /// Assignment operator.
    const PI_EditScriptedParm &    operator=(const PI_EditScriptedParm &src);

    void		 setType(const UT_StringHolder &type)
			 {
			     myType = type;
			     myCachedTypeIndex = -2;
			 }
    const UT_StringHolder &getType() const { return myType; }

    void		 changeTypeToRampColor();
    void		 changeTypeToRampFloat();

    // Copy default parm values from the current values on the given parameter
    // starting at the specified subindex on that parameter (mapping it to 0
    // on this parameter).
    void		 copyDefaultValuesFromParm(PRM_Parm *parm, int subidx);

    // Resets the autolink values from one or more channels of this parm
    // to the set of currently existing links. Setting subidx to -1 causes
    // all channels to reset their autolinks.
    void		 setAutolinksToCurrentLinks(OP_Node *node, int subidx);

    // Take the incoming list of parm or channel names and add those parms
    // or channels to our autolink values in an intelligent way. The
    // links array will be filled with the name of the parameter each of the
    // parms was linked to (or an emtpy string if no link was made for that
    // parm).
    void		 addParametersToAutolink(const UT_StringArray &parms,
						 const char *nodepath,
						 bool linkinvisibles,
						 PI_EditScriptedParms *owner,
						 UT_StringArray &errors);

    void		 setRange(const PRM_Range &range);

    int			 save(std::ostream &os, int indent, OP_Operator *op,
			      bool validate_join,
			      const PI_EditScriptedParm *nextparm,
			      bool in_import_block);
    int			 saveSelectors(std::ostream &os, const char *tokeni,
				       OP_Operator *op);
    void		 saveSpareData(std::ostream &os,
				       const char *tokeni,
				       bool skip_import_data,
				       bool first_group_page);
    void		 saveSpareValue(std::ostream &os, const char *tokeni,
					const char *spare_token);
    void		 saveConditional(std::ostream &os, int indent,
					 const char *name,
					 UT_StringHolder &conditional);
    // Debug dump of data
    void		 dump(std::ostream &os);

    int			 getNMenu() const  { return myMenu.entries(); }
    void		 addMenu(
			    const UT_StringHolder &token = UT_StringHolder(),
			    const UT_StringHolder &label = UT_StringHolder());
    void		 moveMenu(int idx);
    void		 destroyMenu(int idx);
    int			 checkNullMenu();
    void		 clearMenuItems();
    void		 clearCachedChannels();

    void		 setMenu(const PRM_ChoiceList *menu,
				PRM_TypeExtended extended_menu_type);

    // Get our parm type index from our string type for use when building
    // channels, or applying autolinks. It will return -1 for import blocks
    // and tab folder which do not allow any of this sort of activity.
    // The return value can be used to interface to theParmTypes array ONLY!
    int			 getParmTypeIdxForChannels() const;
    // Set the parameter size in a safe way.
    void		 setSize(int size);

    // Accessor to get the list of channel names.  This method
    // updates the channel name cache, if necessary, and returns a constant
    // reference directly to the cache in an attempt to avoid the overhead of
    // building a UT_StringArray.
    const UT_StringArray &getChannelNames() const;

    // Get list of channel names for particular potential new parm name, using
    // the internal channel token cache.
    const UT_StringArray &getChannelNames(const UT_StringRef &new_parm) const;

    // Copy all the spare data for the specified folder index from the given
    // reference data.  The index is the vector index of this folder in the
    // corresponding switcher parameter.
    void		 extractGroupSpareData(const PRM_SpareData *ref,
					       int index);

    // Merges the provided spare data into our spare data.
    void		 mergeSpareData(const PRM_SpareData *srcsparedata);

    const char		*getSpareValue(const char *token) const;
    void		 setSpareValue(const char *token, const char *value);
    void		 copySpareValue(const char *token,
					const PRM_SpareData &ref);

    const char		*getCallbackData() const;
    void		 setCallbackData(const char *value);
    CH_ScriptLanguage	 getCallbackLanguage() const;
    void		 setCallbackLanguage(CH_ScriptLanguage language);

    const char		*getOpFilter() const;
    void		 setOpFilter(const char *value);
    void		 setOpFilter(const PRM_SpareData *spareptr);
    const char		*getRManType() const;
    void		 setRManType(const char *value);
    int			 getMultiStartOffset() const;
    void		 setMultiStartOffset(int value);
    void		 setRampColorType(UT_ColorType color_type);
    UT_ColorType	 getRampColorType() const;
    const char		*getRampBasisVar() const;
    void		 setRampBasisVar(const char *value);
    const char		*getRampKeysVar() const;
    void		 setRampKeysVar(const char *value);
    const char		*getRampValuesVar() const;
    void		 setRampValuesVar(const char *value);
    const char		*getImportSource() const;
    void		 setImportSource(const char *value);
    const char		*getImportToken() const;
    void		 setImportToken(const char *value);
    const char		*getImportMask() const;
    void		 setImportMask(const char *value);
    bool		 getImportEnable() const;
    void		 setImportEnable(bool value);
    bool		 getExportDisable() const;
    void		 setExportDisable(bool value);
    bool		 getUnquotedFlag() const;
    void		 setUnquotedFlag(bool value);
    bool		 getIsGroupStart() const;
    bool		 getIsGroupEnd() const;
    bool		 getIsGroupParm() const;
    bool		 getIsMultiParm() const;
    bool		 getIsRampParm() const;
    bool		 getIsRampParmColor() const;
    bool		 getIsRampParmFloat() const;
    bool		 getIsFileParm() const;
    bool		 getIsBasicStringParm() const;
    bool		 getIsButtonParm() const;

    bool		 getIsColorParm() const;
    PRM_ColorType	 getColorType() const;
    void		 setColorType(PRM_ColorType color_type);
    bool		 getColorWheel() const;
    void		 setColorWheel(bool value);
    bool		 getColorDynamic() const;
    void		 setColorDynamic(bool value);

    bool		 getIsKeyValueDictParm() const;
    const char		*getKeyValueDictKeyLabel() const;
    void		 setKeyValueDictKeyLabel(const char *label);
    const char		*getKeyValueDictValueLabel() const;
    void		 setKeyValueDictValueLabel(const char *label);
    bool		 getKeyValueDictUseChooser() const;
    void		 setKeyValueDictUseChooser(bool use_chooser);
    const char		*getKeyValueDictChooserLabel() const;
    void		 setKeyValueDictChooserLabel(const char *label);
    const char		*getKeyValueDictChooserCallback() const;
    void		 setKeyValueDictChooserCallback(const char *callback);

    const char		*getRampBasisDefault() const;
    void		 setRampBasisDefault(const char *value);
    bool		 getRampShowControlsDefault() const;
    void		 setRampShowControlsDefault(bool value);

    void		 clearRampDefaults();
    bool		 hasRampDefaultValue() const;
    void		 setRampDefaultFromParm(const PRM_Parm &ramp_parm);

    const char		*getFileChooserMode() const;
    void		 setFileChooserMode(const char *value);

    const char		*getFileChooserPattern() const;
    void		 setFileChooserPattern(const char *value);

    const char		*getButtonIcon() const;
    void		 setButtonIcon(const char *value);

    const char		*getScriptAction() const;
    void		 setScriptAction(const char *value);
    const char		*getScriptActionHelp() const;
    void		 setScriptActionHelp(const char *value);
    const char		*getScriptActionIcon() const;
    void		 setScriptActionIcon(const char *value);

    // This utility method returns string constants.
    static const char	*getScriptType(const PRM_Type &ptype,
				       PRM_TypeExtended etype,
				       PRM_MultiType mtype, int size);

    static bool		 parmTypeUsesOpFilter(const char *type);

    static const char	*getExportDisableToken();
    static bool		 isSwitcherLevelSpareToken(const char *token);

    static PI_EditScriptedParmFolderType	 getSwitcherFolderType(
						    const PRM_Template &tplate);
    static PI_EditScriptedParmFolderType	 getSwitcherFolderType(
						    const PRM_Type &type,
						    const PRM_SpareData *spare);

    UT_StringHolder			 myName;
    UT_StringHolder			 myLabel;
    bool				 myUseLabel;
    UT_StringHolder			 myClass;
    UT_StringHolder			 myCategory;
    int					 myGroupID;
    bool				 myInvisible;
    int					 myExport;
    int					 mySize;
    UT_StringHolder			 myDefaults[PI_MAX_SCRIPT_PARM_SIZE];
    CH_StringMeaning			 myDefaultsStringMeaning[
					    PI_MAX_SCRIPT_PARM_SIZE];
    int					 myRangeEnable;
    float				 myRange[2];
    bool				 myRangeLock[2];
    PI_EditScriptedParmMenuEnable	 myMenuEnable;
    PI_EditScriptedParmMenuType		 myMenuType;
    bool				 myUseMenuToken;
    PI_EditScriptedParmFolderType	 myFolderType;
    UT_StringHolder			 myMenuScript;
    CH_ScriptLanguage			 myMenuScriptLanguage;
    int					 myDoAutolink[PI_MAX_SCRIPT_PARM_SIZE];
    UT_StringHolder			 myAutolink[PI_MAX_SCRIPT_PARM_SIZE];
    UT_StringHolder			 myOldAutolink[PI_MAX_SCRIPT_PARM_SIZE];
    UT_StringHolder			 myConditional[NB_PRM_CONDTYPES];
    UT_StringHolder			 myTabConditional[NB_PRM_CONDTYPES];
    UT_StringHolder			 myHelpText;
    UT_ValArray<PI_EditScriptedParmMenu> myMenu;
    PRM_SpareData			*mySpareData;
    bool				 myJoinNextFlag;
    bool				 myTabBreak;
    bool				 myReserved;

    class PI_API PI_ParmType {
	public:
	    const char			*myLabel;
	    const char			*myDSToken;
	    const PRM_Type		&myType;
	    const char			*myIcon;
	    int				 myExport;
	    int				 mySize;
	    int				 myMenu;
	    int				 myString;
	    int				 myRange;
	    float			 myMin, myMax;
	    const char			*myDefault;
	    const char			*mySuffix[PI_MAX_SCRIPT_PARM_SIZE];
	    int				 myUseFloatDefs;
    };
    static PI_ParmType			 theParmTypes[];

private:
    void		 defaultInit();
    void		 initMenu(const PRM_ChoiceList *menu,
				PRM_TypeExtended extended_menu_type);

    void		 rebuildChannelNameCache(
				const UT_StringRef &parm_name,
				int parmtypeidx) const;

    UT_StringHolder			 myType;
    mutable int				 myCachedTypeIndex;

    // A mutable cache for the channel names last built for this parameter,
    // along with the settings last used to build it.
    mutable UT_StringArray		 myCachedChannelNames;
    mutable UT_StringHolder		 myChannelCacheToken;
    mutable int				 myChannelCacheTypeIndex;

};

typedef UT_ValArray<PI_EditScriptedParm *> PI_EditScriptedParmArray;

class PI_API PI_EditScriptedParms
{
public:
			 PI_EditScriptedParms();
			 // Use this constructor to edit spare parameters.
			 // If skip_parm_vops is true, then the parameters
			 // coming from Parameter VOPs (ramp, etc) will
			 // not be included in the list, otherwise they will be.
			 // If allow_switch_rename is true then the
			 // switcher (folder) names will be changed to a uniuqe
			 // name, so that they are not marked as reserved,
			 // otherwise, if false, such names will remain
			 // unchanged (usually renaming is desirable, because
			 // having reserved switchers confuses things and 
			 // makes them uneditable, but other times all
			 // parameters should keep their original names).
			 PI_EditScriptedParms(OP_Node *node,
					      bool add_reserved_parms,
					      bool init_auto_links,
					      bool skip_parm_vops  = false,
					      bool allow_switch_rename = true);
			 // Use this constructor to edit DA parameters.
			 // You can optionally include spare parameters
			 // anyway but with this constructor, DA parameters
			 // are not treated as reserved parameters.
			 PI_EditScriptedParms(OP_Operator *op,
					      OP_Node *node,
					      bool add_reserved_parms,
					      bool spareparms=false,
					      bool skip_parm_vops=false);
			 // Use this constructor to edit optype override
			 // parameters.  Base and DA parameters are treated as
			 // reserved parameters.
			 PI_EditScriptedParms(OP_Operator *op,
					      bool add_reserved_parms,
					      bool skip_parm_vops=false);
			 // Get parameters from a dialog script.
			 // The node is just a reference used to determine
			 // what parameters should be reserved.
			 PI_EditScriptedParms(OP_Node *node,
					      UT_IStream &is,
					      bool spareparms,
					      bool skip_reserved,
					      bool init_auto_links,
					      bool fix_invalid_joins);
			 // Get parameters from a array of parm templates.
			 // The node is just a reference used to determine
			 // what parameters should be reserved.
			 PI_EditScriptedParms(OP_Node *node,
					      const PRM_Template *tplate,
					      bool spareparms,
					      bool skip_reserved,
					      bool init_auto_links);
			 // Copy constructor.
			 PI_EditScriptedParms(const PI_EditScriptedParms &src);
			~PI_EditScriptedParms();

    // Merge the contents of another PI_EditScriptedParms into this one.
    // The only change to the parms being merged is to resolve conflicts
    // with existing parm names, and ensure no reserved parm names are used.
    // The new parms are always added to the end of the current parms.  The
    // reserved flag from the source parms is preserved, so be careful when
    // merging parameters from different sources.
    void			 mergeParms(const PI_EditScriptedParms &src);

    // Remove all our parameters.
    void			 clearParms();

    // Save the parms in dialog script format.
    int				 save(std::ostream &os, UT_String &warnings,
				      bool validate_joins);

    // Dump the parameters, in a meaningless format, but useful for debugging.
    void			 dump(std::ostream &os);

    // Compile these parameters, which means eliminating all information
    // about the parameters that is not absolutely required for proper
    // functioning.
    void			 compile();

    // Get our individual parms.
    PI_EditScriptedParmArray	&getParms() { return myParms; }
    int				 getNParms() const;
    PI_EditScriptedParm		*getParm(int i);
    int				 getParmIndexWithName(const UT_StringRef &name);
    PI_EditScriptedParm		*getParmWithName(const UT_StringRef &name);
    int				 getParmIndex(PI_EditScriptedParm *p);
    int				 getFolderIndexWithLabel(
					const UT_StringRef &label);
    int				 getFolderIndexWithName(
					const UT_StringRef &name);
    PI_EditScriptedParm		*getFolderWithLabel(
					const UT_StringRef &label);
    PI_EditScriptedParm		*getFolderWithName(
					const UT_StringRef &name);

    // Remove all folder group parameters that do not contain any
    // regular parameters (not folder groups and not separators).
    void			 removeEmptyFolders();

    // Given a list of folder names, return the index of the start of the
    // innermost folder, or -1 if there is no such folder.
    int				 getNestedFolderIndex(
				    const UT_StringArray &folder_labels);

    // Given the index of a parameter, return the parms corresponding to the
    // containing folders.
    void			 getContainingFolders(
				    int parm_index,
				    UT_ValArray<PI_EditScriptedParm *> &result);

    // Find a parm that already has a particular autolink.
    PI_EditScriptedParm		*getParmWithAutolink(const char *channelpath,
						     const char *nodepath,
						     int *linkedsubidx = 0);

    // For a group start parm or group end parm, find the opposite end of
    // that group. Otherwise return -1.
    int				 getMatchingGroupParm(int parm) const;

    // Returns true if the group parameter specified is the first page
    // in a series of folders.
    bool			 getIsFirstPage(int parm) const;

    // Returns the position of the group parameter that contains the specified
    // parameter. If there is no parent group parameter, -1 is returned.
    int				 getParentGroupParm(int parm);

    // Get information about the menu associated with a particular parameter.
    int				 getNMenu(int parm);
    PI_EditScriptedParmMenu	*getMenu(int parm, int i);
    const char			*getMenuScript(int parm);

    // Test if a parameter can be moved from srcidx to dstidx. For example,
    // a folder start token is not allowed to be moved beyond a corresponding
    // end token. Returns true if parm move is allowed, false otherwise.
    bool		 isParmMoveAllowed(int first, int last, int offset);

    // Add, remove, and reorder parameters.
    bool		 updateNode();
    void		 addParm(PI_EditScriptedParm *parm);
    bool		 moveParms(int first, int last, int offset,
				    bool remove_invalid_groups=true);
    void		 moveParms(const UT_IntArray &movingparms, int destpos,
				    bool remove_invalid_groups=true);

    // Adds parms for the multiparm templates attached to the specified
    // parm. This function works recursively if the multiparm templates
    // contain other multiparms.
    void		 insertMultiParmTemplates(const PRM_Template *tplate,
						  const char *parentparmname,
						  const OP_Node *linktonode,
						  const char *nameprefix,
						  const char *labelprefix,
						  int insertat);

    // Apply a permutation to a block of parameters.  This method only does
    // sanity checking on the permutation itself, and not on the parameters
    // themselves, so use it with caution.  For example, don't screw up the
    // order of group begin and end entries.
    //
    // The permutation array must consist of a permutation of the indicies
    // [first..(first + permutation.entries() - 1)].
    //
    // The operation applied is basically:
    //   new_parms(first+i) = old_parms(permutation(i))
    void		 permuteParms(int first,
				      const UT_IntArray &permutation);

    // If we are told to remove a group start or end parm, we automatically
    // remove the opposite end of that group.  Returns the number of parms
    // removed.
    int			 removeParms(int first, int last, bool remove_invalid_joins=true);

    // joins cannot occur over folder boundaries, or at the end of the parm
    // list.
    void		 removeInvalidJoins();
 
    // Copy default parm values from the current values on the specified node.
    void		 copyDefaultsFromNode(OP_Node *node);

    int			 checkReservedName(const char *name) const;
    bool		 containsReservedParm() const;
    void		 initializeReserveNames(OP_Parameters *node);
    void		 initializeReserveFlags();
    void		 clearReserveNamesAndFlags();
    bool		 makeSafeParmName(PI_EditScriptedParm *parm,
					  UT_String &parmname,
					  const UT_StringSet *extrareserved,
					  bool checkexistingparms);
    bool		 makeSafeParmName(PI_EditScriptedParm *parm,
					  UT_StringHolder &parmname,
					  const UT_StringSet *extrareserved,
					  bool checkexistingparms);

    // Set all autolink values for all parameters to the current set of
    // links.
    void		 setAutolinksToCurrentLinks();

    // Take the list of parm or channel names and add those parms or channels
    // to the autolink fields of the appropriate PI_EditScriptedParm. The
    // links array will be filled with the name of the parameter each of the
    // parms was linked to (or an emtpy string if no link was made for that
    // parm).
    void		 addParametersToAutolinks(PI_EditScriptedParm &destparm,
					const UT_StringArray &parms,
					const char *nodepath,
					bool linkinvisibles,
					PI_EditScriptedParmArray &changedparms,
					UT_StringArray &errors);

    // Makes sure that none of our parm names contain too many hashes for
    // the multiparm instance level they are at. This is called right before
    // saving. Extra hashes are converted to underscores.
    void		 fixParmNamesForInstanceLevel();

    // Turn the auto link values from all our parms into channel references
    // on the appropriate nodes. The function is not const because it actually
    // changes the auto link values.
    void		 applyAutoLinks(UT_StringArray &errors,
					OP_NodeList &changednodes,
					UT_IntArray &changedparms);

    // The applyAutoLinks call happens before the node's templates are
    // updated. So after updating the templates, we need to force any
    // changed nodes to rebuild their dependencies and recook.
    void		 updateNodesFromApplyAutoLinks(
					OP_NodeList &changednodes,
					UT_IntArray &changedparms);

    // This function is called whenever a node's name is changed. It goes
    // through all our parm's auto links and changes any references from the
    // old node name to the new node name.
    bool		 updateAutoLinksForNameChange(
					const UT_String &refnodepath,
					const UT_String &oldpath,
					const UT_String &newpath);

    // Gets the node that is associated with these parms. This value is
    // used for finding and setting parameter links, and detecting parm
    // name conflicts.
    OP_Node		*getNode() const;

    // These functions are used to control the table of multiparm link
    // information used by piGetCurrentLinks.
    static void			 buildMultiparmInfo(OP_Node *refnode);
    static void			 clearMultiparmInfo();
    static void			 lockMultiparmInfo();
    static void			 releaseMultiparmInfo();
    static const PI_BuiltMultiParmInfo &getBuiltMultiparmInfo();

    // Adds additional links so that dstparm has the additional links to the
    // srcchannel. They must be both ramp parameters of the same type
    // designated by is_color_ramp.
    static void		addRampAutoLinks(
				OP_Node *dstnode,
				PRM_Parm *dstparm,
				const char *dstchannel,
				const char *srcrelpath,
				const char *srcchannel,
				bool is_color_ramp,
				UT_String &error);
    /// Deletes ramp references added via addRampAutoLinks()
    static void		deleteRampAutoLinks(
				OP_Node *dstnode,
				PRM_Parm *dstparm,
				const char *dstchannel,
				bool is_color_ramp,
				UT_String &error,
				bool also_revert = false);

private:
    // Returns a pointer to the base parm templates, which is the myBase
    // member if editing HDA parms, or the operator templates if editing
    // spare parameters.
    const PRM_Template	*getBaseParmTemplates() const;
    // Returns a pointer to the base parm template (if any) that matches
    // the supplied parameter name.
    const PRM_Template	*getBaseParmTemplate(const char *parmname) const;

    // This utility function helps makeSafeParmName generate a list of all
    // multiparm instance parms in this PI_EditScriptedParms.
    void		 buildMParmInst(PI_EditScriptedParm *skipparm,
					UT_StringArray &mparminst,
					UT_IntArray &mparmlvl) const;

    // This utility function figures out the depth of multiparm nesting
    // for the specified parm.
    int			 getMultiParmInstanceLevel(
					PI_EditScriptedParm *parm) const;

    // Utility function for makeSafeParmName which recursively checks a
    // PRM_Template array (and any multiparm templates in that array) for
    // conflicting parm names.
    bool		 conflictsWithTemplate(const PRM_Template *tplate,
					const UT_StringArray &mparmtplates,
					const UT_StringArray &channels,
					const UT_StringArray &mparminst,
					const UT_IntArray &mparmlvl) const;

    void		 createImportGroups(int offset,
					UT_Array<pi_GroupData> &gstack,
					const PRM_ScriptImports *imports,
					int &import_index);
    void		 createImportGroupsBeforeGroupEnd(
					int offset, int depth,
					int switcher_index, int folder,
					const PRM_ScriptImports *imports,
					int &import_index);
    void		 createParms(const PRM_Template *tplates,
					const PRM_ScriptImports *imports,
					bool skip_reserved,
					bool init_auto_links = true,
					bool skip_parm_vops  = false,
					bool allow_switch_rename = true);
    void		 createParms(UT_ValArray<const PRM_Template *> &tplates,
					const PRM_ScriptImports *imports,
					bool skip_reserved,
					bool init_auto_links = true,
					bool skip_parm_vops  = false,
					bool allow_switch_rename = true);
    void		 removeInvalidGroups(int startpos);

    void		 saveGroupHeader(std::ostream &os, int index,
					bool first_page,
					int &glevel,
					int &import_depth,
					bool &in_import_block);
    void		 saveGroupFooter(std::ostream &os, int index,
					int &glevel,
					int &import_depth,
					bool &in_import_block);
    void		 saveImportHeader(std::ostream &os, int index,
					int &glevel,
					int &import_depth,
					bool &in_import_block);
    void		 saveImportFooter(std::ostream &os, int index,
					int &glevel,
					int &import_depth,
					bool &in_import_block);
    void		 saveMultiParmHeader(std::ostream &os, int index,
					int &glevel,
					int &import_depth,
					bool &in_import_block);
    void		 saveMultiParmFooter(std::ostream &os, int index,
					int &glevel,
					int &import_depth,
					bool &in_import_block);

    PI_EditScriptedParmArray	 myParms;
    UT_StringSet		 myReservedParmNames;
    OP_Operator			*myOp;
    int				 myNodeId;
    bool			 myIsEditingSpareParms;
    bool			 myIsEditingOverrideParms;
};


// ============================================================================
// Abstracts edited parameters that may come from different nodes.
class PI_API PI_EditScriptedParmsAggregate
{
public:
	     PI_EditScriptedParmsAggregate();
    virtual ~PI_EditScriptedParmsAggregate();

    /// @{ Methods corresponding to the PI_EditScriptedParms class.
    int			    getNParms() const;
    PI_EditScriptedParm	*   getParm(int parm_idx);
    OP_Node *		    getNode(int parm_idx);
    int			    getMatchingGroupParm(int parm_idx) const;
    int			    removeParms(int first_parm_idx, int last_parm_idx);
    /// @}
   
    /// Gets list containing i-th parameter.
    PRM_ParmList *	    getParmList(int parm_idx);

    /// Copies the member parameters to the given parms.
    void		    copyToParms( PI_EditScriptedParms &parms ) const;

protected:
    /// @{ Node and parm management methods for subclasses.
    void	appendToAggregate(OP_Node *node, PI_EditScriptedParms* parms);
    int			    getAggregateCount() const;
    OP_Node *		    getAggregateNode(int src_idx ) const;
    PI_EditScriptedParms*   getAggregateParms(int src_idx ) const;
    /// @}

private:
    int		getParmIndex( int node_idx, int parm_sub_idx ) const;
    void	getNodeAndParmIndices( int &node_idx, int &parm_sub_idx,
			int parm_idx ) const;

private:
    // Nodes the parameters come from.
    UT_ValArray<OP_Node *>		mySrcNodes;

    /// Each entry contains parms from a corresponding OP in mySrcNodes list.
    UT_ValArray<PI_EditScriptedParms*>	mySrcParms;
};

#endif


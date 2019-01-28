/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_DopDescription_h__
#define __SIM_DopDescription_h__

#include "SIM_API.h"
#include <UT/UT_String.h>

class PRM_ParmList;
class PRM_Template;
class OP_Node;
class OP_Operator;

/// This class is used to create a new DOP type based on a SIM_Data subclass.
/// This class stores the information that describes the automatic DOP that
/// should be created to represent this data type. An automatic DOP outputs
/// an instance of a SIM_Data subclass to be attached to one or more objects.
class SIM_API SIM_DopDescription
{
public:
    /// Returns the live data input.  Note that this unfortuantely
    /// does not often correspond to the wire index!
    /// -1 means all inputs are enabled.  -2 means none of them are.
    /// 0 or more mean only that input is enabled.
    typedef int (*SIM_ComputeLiveInput)(const OP_Node *node, double time);

    /// Callback to handle any obsolete parameters.
    typedef void (*SIM_ObsoleteParmResolver)(OP_Node *node,
                                             PRM_ParmList *obsolete_parms);

    /// Callback to update old nodes to the latest version.
    typedef void (*SIM_SyncNodeVersion)(OP_Node *node, const char *old_version,
                                        const char *cur_version,
                                        bool *node_deleted);

    /// Callback to provide a custom version string for the operator.
    typedef bool (*SIM_GetOpVersion)(const OP_Operator &op, UT_String &version);

    /// Callback to hide or disable parameters.
    typedef bool (*SIM_UpdateParmsFlags)(OP_Node *node);

    /// The SIM_DopDescription constructor.
    /// All features of this class are set on construction and cannot be
    /// altered once set. Generally this class will be created in a static
    /// member function of some SIM_Data subclass and used when registering
    /// the data factory. See the various data access functions for a
    /// desription of what each constructor argument means.
			 SIM_DopDescription(bool createdop,
					    const char *dopname,
					    const char *dopenglish,
					    const char *dataname,
					    const char *datatypename,
					    const PRM_Template *templates);
    /// The SIM_DopDescription destructor.
			~SIM_DopDescription();

    /// Returns true if an automatic DOP should be created.
    /// Many SIM_Data subclasses are for internal use only and do not want
    /// to be created explicitly by users. In that case this value should be
    /// set to false, in which case all other values will be ignored.
    bool			 getCreateDop() const;
    /// Returns the name of the automatic DOP operator type.
    /// This name must be unique among DOP operator types.
    const UT_String		&getDopName() const;
    /// Returns the descriptive name for the DOP operator type.
    /// This is the value that appears in toolbars to represent the automatic
    /// DOP operator type created from this DOP description.
    const UT_String		&getDopEnglish() const;
    /// Returns the name of the data within the simulation object.
    /// This name should be a unique value that specifies the name under
    /// which this data is stored in each simulation object.
    const UT_String		&getDataName() const;
    /// Returns the name of the SIM_Data subclass type.
    /// This is the name under which the SIM_Data subclass is registered.
    /// This value is used to create the new data.
    const UT_String		&getDataTypeName() const;
    /// Returns the name of the SIM_Data to be created if the node is
    /// bypassed.  If none is given, the data will be deleted.
    const char			*getBypassDataTypeName() const { return myBypassDataTypeName; }
    void			 setBypassDataTypeName(const char *name) { myBypassDataTypeName.harden(name); }
    /// Returns an array of PRM_Templates for setting options.
    /// The values from each parameter are placed into a SIM_Options, which
    /// is then sent to the SIM_Data::setParameters() function to set up
    /// the SIM_Data class based on the parameter values.
    const PRM_Template		*getTemplates() const;
    /// Returns if this data should have no grey inputs, but instead only
    /// green inputs so it has to go through an apply stage.
    bool			 getPureApplyData() const { return myPureApplyData; }
    void			 setPureApplyData(bool pureapply) { myPureApplyData = pureapply; }

    /// Set the "lowest common denominator" data type name. This is the
    /// data type that DOP nodes compare existing data against to see if the
    /// requested data already exists and is of a "compatible" type. This
    /// affects whether the DOP node uses its "set initial" parameters or
    /// only its "set always" parameters. This defaults to the actual data
    /// type name.
    void			 setLCDDataTypeName(const char *lcddatatype);
    /// Set the "lowest common denominator" data type name.
    const UT_String		&getLCDDataTypeName() const;

    /// Set the Share Data parameter default. The default if off.
    void			 setDefaultShareData(int share);
    /// Get the Share Data parameter default.
    int				 getDefaultShareData() const;
    /// Set the Unique Data Name parameter default. The default is off.
    void			 setDefaultUniqueDataName(int unique);
    /// Get the Unique Data Name parameter default.
    int				 getDefaultUniqueDataName() const;
    /// Set whether each parm gets an update menu. The default is on.
    void			 setCreatePerParmUpdateMenus(bool createmenus)
				 { myCreatePerParmUpdateMenus = createmenus; }
    /// get whether each parm gets an update menu. 
    bool			 getCreatePerParmUpdateMenus() const
				 { return myCreatePerParmUpdateMenus; }

    /// Set the templates to use for controlling guide geometry.
    void			 setGuideTemplates(const PRM_Template *tplates);
    /// Get the templates to use for controlling guide geometry.
    const PRM_Template		*getGuideTemplates() const;

    /// Set the templates to use for obsolete parameters.
    void			 setObsoleteTemplates(PRM_Template *tplates);
    /// Get the templates to use for obsolete parameters.
    PRM_Template		*getObsoleteTemplates() const;
    /// Set the callback to handle any obsolete parameters.
    void                         setObsoleteParmResolver(SIM_ObsoleteParmResolver resolver);
    /// Launch the callback to handle the obsolete parameters.
    void                         resolveObsoleteParms(OP_Node *node,
                                                      PRM_ParmList *obsolete_parms) const;
    /// Set the callback to update nodes to the latest version.
    void                         setSyncNodeVersion(SIM_SyncNodeVersion callback);
    /// Launch the callback to update nodes to the latest version.
    void                         syncNodeVersion(OP_Node *node, const char *old_version,
                                                 const char *cur_version, bool *node_deleted) const;
    /// Set the callback to provide a custom version string for the operator.
    void                         setOpVersionCallback(SIM_GetOpVersion callback);
    /// Launch the callback to provide a custom version string for the operator.
    bool                         getOpVersion(const OP_Operator &op, UT_String &version) const;

    /// Set the callback to hide or disable parameters.
    void                         setUpdateParmsFlags(SIM_UpdateParmsFlags callback);
    /// Run the callback to hide or disable parameters.
    bool                         updateParmsFlags(OP_Node *node) const;

    bool			 getInitialTemplateFlag() const
				 { return myInitialTemplateFlag; }
    void			 setInitialTemplateFlag(bool flagval)
				 { myInitialTemplateFlag = flagval; }

    int				 computeLiveInput(int defval, const OP_Node *node, double time) const
    {
	if (!myLiveInputComputer)
	    return defval;
	return (*myLiveInputComputer)(node, time);
    }
    void			 setLiveInputComputer(SIM_ComputeLiveInput computer) { myLiveInputComputer = computer; }

    void			 getDescriptiveNameParm(UT_String &parm) const { parm = myDescriptiveNameParm; }
    void			 setDescriptiveNameParm(const char *parm) { myDescriptiveNameParm.harden(parm); }

private:
    /// Disallow using the copy constructor.
				 SIM_DopDescription(const SIM_DopDescription &);
    /// Disallow using the assignment operator.
    const SIM_DopDescription	&operator=(const SIM_DopDescription &);

    bool			 myCreateDop;
    const UT_String		 myDopName;
    const UT_String		 myDopEnglish;
    const UT_String		 myDataName;
    const UT_String		 myDataTypeName;
    UT_String		 	 myBypassDataTypeName;
    const PRM_Template		*myTemplates;
    const PRM_Template		*myGuideTemplates;
    PRM_Template		*myObsoleteTemplates;
    SIM_ObsoleteParmResolver     myObsoleteParmResolver;
    SIM_SyncNodeVersion          mySyncNodeVersionCallback;
    SIM_GetOpVersion             myGetOpVersionCallback;
    SIM_UpdateParmsFlags         myUpdateParmsCallback;
    UT_String			 myLCDDataTypeName;
    int				 myDefaultShareData;
    int				 myDefaultUniqueDataName;
    bool			 myCreatePerParmUpdateMenus;
    bool			 myPureApplyData;
    bool			 myInitialTemplateFlag;
    SIM_ComputeLiveInput	 myLiveInputComputer;
    UT_String			 myDescriptiveNameParm;
};

#endif


/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_UserOption.h ( GR Library, C++)
 *
 * COMMENTS:
 *      A GR_UserOption is a custom display option which can be toggled on/off
 *      and contain any arbitrary settings.
 */

#ifndef __GR_UserOption__
#define __GR_UserOption__

#include "GR_API.h"
#include <UT/UT_String.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>
#include "GR_Defines.h"

// Option tokens for all GR_UserOption subclasses.
#define GR_LABEL_TOKEN		"label"
#define GR_ICON_TOKEN		"icon"
#define GR_VISIBILITY_TOKEN	"visibility"
#define GR_ATTRIB_TOKEN		"attrib"
#define GR_GROUP_TOKEN		"group"
#define GR_CLASS_TOKEN		"class"
#define GR_OVERRIDECOLOR_TOKEN	"overridecolor"
#define GR_COLOR_TOKEN		"color"
#define GR_SELCOLOR_TOKEN	"selcolor"
#define GR_STYLE_TOKEN		"style"
#define GR_FLAGS_TOKEN		"flags"

// Option tokens for GR_VectorOption.
#define GR_SCALE_TOKEN		"scale"
#define GR_ARROW_TOKEN		"arrow"
// obsolete tokens
#define GR_FADE_TOKEN		"fade"

// Option tokens for GR_GenericOption
#define GR_STRING_TOKEN		"string"

class GR_OptionTemplate;
class GR_UserDecoration;
class GU_Detail;
class UT_Options;

enum GR_UserOptionType
{
    GR_USER_OPTION_GENERIC,
    GR_USER_OPTION_VECTOR,
    GR_USER_OPTION_TEXT,
    GR_USER_OPTION_SCENE,
    GR_USER_OPTION_VISUALIZER
};

class GR_API GR_UserOptionParm
{
public:
    const char	*myName;
    int		 myType;
};

class GR_API GR_UserOption
{
public:
    GR_UserOption(const char *name, const char *label,
		  GR_OptionTemplate *entry);

    virtual 	~GR_UserOption();

    const char	*getName() const { return (const char *) myName; }
    void	 setName(const char *name) { myName.harden(name); }
    const char	*getLabel() const { return (const char *) myLabel; }
    void	 setLabel(const char *label) { myLabel.harden(label); }
    const char	*getIconName() const { return (const char *) myIconName; }
    void	 setIconName(const char *name);
    GR_DecorVisibility getVisibility() const { return myVisibility; }
    void	 setVisibility(GR_DecorVisibility v) { myVisibility = v; }

    // Changing the state of this option will trigger a geometry refinement
    // if 'ref_req' is true. This is primarily used by filter-style render
    // hooks. This can only be called during setup of the option.
    void	 setRefineRequired(bool ref_req) { myRefineRequired=ref_req; }
    bool	 isRefineRequired() const { return myRefineRequired; }

    // Hide the option from the Display Options dialog. Should only be called
    // during option setup.
    void	 setHidden(bool hide) { myHidden = hide; }
    bool	 isHidden() const { return myHidden; }

    GR_OptionTemplate	*getTemplate() const { return myTemplate; }

    // The version count is incremented whenever the settings of the
    // option are changed.
    int			 getVersion() const { return myVersion; }
    void		 bumpVersion() { myVersion++; }

    virtual GR_UserOption	*duplicate() const = 0;

    // Class I/O methods.  They return 0 if successful, -1 if not.
    virtual int			 saveCommand(std::ostream &os) const;
    virtual bool		 loadCommand(UT_IStream &is);

    virtual bool		 save(std::ostream &os, int indent) const;
    virtual bool		 load(UT_IStream &is);

    // Methods to manipulate the internal settings of the option.  The apply
    // method returns a boolean to indicate whether the settings have changed.
    virtual void		 querySettings(UT_Options &options) const = 0;
    virtual bool		 applySettings(const UT_Options &options) = 0;

    virtual const char		*getAttribute() const { return NULL; }
    virtual int			 getAttributeType() const { return 0; }
    virtual fpreal		 getScale() const { return 1.0; }
    virtual bool		 getArrowTip() const { return false; }
    virtual bool		 getOverrideColor() const { return false; }
    virtual UT_Vector3		 getColor() const { return UT_Vector3(); }
    virtual UT_Vector3		 getSelectColor() const{return UT_Vector3();}
    virtual GR_UserOptionType	 getOptionType() const
				 { return GR_USER_OPTION_GENERIC; }

    // A utility method to output settings as a single line where each 
    // setting is saved in the form:
    //     <token> ( <value1>, [<value2>...] ).
    // Return 0 if successful, -1 if not.
    // TODO: Take templates to specify order as UT_Options is sorted!
    static int			 saveSettings(std::ostream &os,
				      const UT_Options &options, bool for_cmd);
			
    // A utility method to extract settings from an input stream where each
    // settings is stored in the form:
    //     <token> ( <value1>, [<value2>...] )
    //
    // A terminal token can be specified to stop the processing of the stream
    // when it is encountered.
    static bool			 loadSettings(UT_IStream &is, 
				      const GR_UserOptionParm *parm_templates,
				      UT_Options &options, bool for_cmd,
				      const char *terminal_token = 0);

protected:
    GR_UserOption(const GR_UserOption &src);

    virtual const GR_UserOptionParm	*getSettingTemplates() const  = 0;

    static void			 getDefaultColor(UT_Vector3 &clr);
    static void			 getDefaultSelColor(UT_Vector3 &clr);

    // Utility methods to query a specified setting.  The return value is used
    // to indicate the setting differs from the passed in value.
    static bool			 querySetting(const UT_Options &options,
					      const char *name, bool &v);
    static bool			 querySetting(const UT_Options &options,
					      const char *name, float &v);
    static bool			 querySetting(const UT_Options &options,
					      const char *name, UT_String &v);
    static bool			 querySetting(const UT_Options &options,
					      const char *name, UT_Vector3 &v);
    static bool			 querySetting(const UT_Options &options,
    					      const char *name, exint &i);

private:
    UT_String			 myName;
    UT_String			 myLabel;
    UT_String			 myIconName;
    GR_OptionTemplate		*myTemplate;
    GR_DecorVisibility		 myVisibility;
    bool			 myRefineRequired;
    bool			 myHidden;

    int				 myVersion;
};

#endif

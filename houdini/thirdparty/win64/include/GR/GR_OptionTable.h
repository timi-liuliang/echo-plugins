/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_OptionTable.h ( GR Library, C++)
 *
 * COMMENTS:
 *     This class holds the table of all available option types as well as
 *     the instances of all currently active options.
 */

#ifndef __GR_OptionTable__
#define __GR_OptionTable__

#include "GR_API.h"
#include <UT/UT_SymbolTable.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Notifier.h>

class GR_OptionTemplate;
class GR_UserOption;

class GR_API GR_OptionTableEvent
{
public:
    enum GR_OptionTableEventType
    {
	OPTION_ADDED,
	OPTION_DELETED,
	OPTION_CHANGED
    };

    GR_OptionTableEvent( GR_OptionTableEventType type, GR_UserOption *option,
			 int id )
	: myEventType(type), myOption(option), myOptionId(id) {}

    GR_OptionTableEventType	 myEventType;
    GR_UserOption		*myOption;
    int				 myOptionId;
};

class GR_API GR_OptionTable
{
public:
		 GR_OptionTable();
    virtual 	~GR_OptionTable();

    // The responsibility to delete a template resides with this method.
    void	 addTemplate(GR_OptionTemplate *tplate);

    // Query the templates currently present in the table.
    int		 getNumTemplates() const;
    GR_OptionTemplate *getTemplate(int i) const;

    // Query the options currently instantiated.
    int		 getNumOptions() const { return myOptions.entries(); }
    const GR_UserOption	*getOption(int i) const { return myOptions(i); }

    GR_UserOption	*createOption(const char *type, const char *name,
				      const char *label = NULL);
    bool		 destroyOption(const char *name);
    int			 destroyOptionsByPattern(const char *pattern);
    bool		 renameOption(const char *old_name,
				      const char *new_name, bool notify);
    void		 optionChanged(const char *name);

    GR_UserOption	*getOption(const char *name);
    int			 getOptionId(const char *name) const;
    GR_UserOption	*getOptionById(int unique_id)
			 {
			     if( unique_id >= 0 && unique_id <
				     myOptionsByID.entries() )
				 return myOptionsByID(unique_id);
			     return  0;
			 }
    void		 getOptionsByPattern(const char *pattern,
					     UT_ValArray<GR_UserOption *> &l);

    /// Returns a notifier that distributes events when options are added,
    /// removed or changed.
    UT_NotifierImpl<GR_OptionTableEvent &>
			&getEventNotifier() { return myEventNotifier; }

    // Method to override the default definition file name.  In order for the
    // default definitions to be loaded from this file, this must be called
    // before the first instance of this class is created.
    static void	 overrideDefinitionFile(const char *filename);

    void	 saveDefaultOptionDefinitions();
    void	 loadDefaultOptionDefinitions();

    /// Return true if the save/load was successful, and false otherwise.
    bool	 saveOptions(std::ostream &os, int indent = 0) const;
    bool	 loadOptions(UT_IStream &is);

private:
    void	 addFactoryTemplates();

protected:
    UT_ValArray<GR_OptionTemplate *>	myTemplates;
    UT_SymbolMap<GR_OptionTemplate *>	myTemplateLUT;

    UT_ValArray<GR_UserOption *>	myOptions;
    UT_SymbolMap<exint>			myOptionsLUT;

    // A table to allow fast lookup of options by the unique id.
    UT_ValArray<GR_UserOption *>	myOptionsByID;

    // Object that notifies about option changes.  The int argument
    // means absolutely nothing.
    UT_NotifierImpl<GR_OptionTableEvent &>	myEventNotifier;
};

GR_API extern GR_OptionTable *GRgetOptionTable();

#endif


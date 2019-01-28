/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        PI_SettingList.h
 *
 * COMMENTS:
 *	This class maintains a list of settings that control how a PI
 *	behaves.  Each setting is named.  The corresponding value may
 *	be a bool, int, fpreal, UT_String, or UT_Vector[2/3/4].  The version
 *	of the overloaded getSetting()/appendSetting() method that you
 *	call determines the value type.
 *
 *	There may only be one type of setting value per name.  Appending
 *	a setting that has the same name as an existing setting is not
 *	allowed.
 *
 *	If no setting with the given name exists, getSetting() will return
 *	the supplied default setting.
 */

#ifndef __PI_SettingList_h__
#define __PI_SettingList_h__

#include "PI_API.h"
#include <iosfwd>
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>

class UT_String;
class pi_Setting;

// This enumerated type in used internally, but must be declared in this
// header because some private methods take these values as arguments.  A
// number of helper classes also use this type.  It could go in the public
// part of the header, but it's too cumbersome to type
// PI_SettingList::PI_SETTING_FLOAT, so it's not in the header.
enum pi_SettingType
{
    PI_SETTING_FLOAT,
    PI_SETTING_STRING,
    PI_SETTING_VECTOR2,
    PI_SETTING_VECTOR3,
    PI_SETTING_VECTOR4
};

class PI_API PI_SettingList
{
public:
    explicit PI_SettingList();
    ~PI_SettingList();

    // These methods are called by a PI when it loads its settings.
    void		 getSetting(const char	     *setting_name,
				    int		     &setting,
				    int		      default_value) const;
    void		 getSetting(const char	     *setting_name,
				    bool	     &setting,
				    bool	      default_value) const;
    void		 getSetting(const char	     *setting_name,
				    fpreal	     &setting,
				    fpreal	      default_value) const;
    void		 getSetting(const char	     *setting_name,
				    UT_String	     &setting,
				    const char	     *default_value) const;
    void		 getSetting(const char	     *setting_name,
				    UT_Vector2R	     &setting,
				    const UT_Vector2R &default_value) const;
    void		 getSetting(const char	     *setting_name,
				    UT_Vector3R	     &setting,
				    const UT_Vector3R &default_value) const;
    void		 getSetting(const char	     *setting_name,
				    UT_Vector4R	     &setting,
				    const UT_Vector4R &default_value) const;

    // These methods are called by a PI when it saves its settings.
    void		 appendSetting(const char	*setting_name,
				       int		 setting);
    void		 appendSetting(const char	*setting_name,
				       bool		 setting);
    void		 appendSetting(const char	*setting_name,
				       fpreal		 setting);
    void		 appendSetting(const char	*setting_name,
				       const char	*setting);
    void		 appendSetting(const char	*setting_name,
				       const UT_Vector2R &setting);
    void		 appendSetting(const char	*setting_name,
				       const UT_Vector3R &setting);
    void		 appendSetting(const char	*setting_name,
				       const UT_Vector4R &setting);

    // This method is used to remove a specific setting from the list.
    void		 removeSetting(const char	*setting_name);

    // When a PI reads a setting it will be marked as read.  This method
    // will return a string containing all unread (ie. unrecognized) settings.
    bool		 findUnreadSettings(UT_String &unread_settings) const;

    // The resource manager uses the == operator to see if settings differ from
    // the default settings.  Note that this operator is not const and does not
    // take a reference to a const object because it may sort the two lists.
    bool		 operator==(PI_SettingList &setting_list);
    void		 sort();

    PI_SettingList	&operator=(const PI_SettingList &setting_list);

    // Clear the list or check if it's empty.
    void		 clear();
    bool		 isEmpty() const { return !mySettings.entries(); }

    // These methods convert the settings to and from ascii data.
    void		 loadFromSettingString(const char *setting_string);
    void		 generateSettingString(std::ostream &os) const;
    void		 generateSettingString(UT_String &str) const;

    friend std::ostream	&operator<<(std::ostream &os,
				    const PI_SettingList &setting_list)
			{
			    setting_list.generateSettingString(os);
			    return os;
			}

private:
    // Find a setting with a given name and type.
    pi_Setting		*findSetting(const char *setting_name,
				     pi_SettingType setting_type) const;

    // The setting parser needs to be a friend so it can call findSetting
    // and append to mySettings.
    friend class pi_SettingParser;

    // Data:

    UT_ValArray<pi_Setting *>	 mySettings;
    bool			 mySortedFlag;
};

#endif

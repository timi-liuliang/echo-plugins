/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DM_ModifierKeys.h ( DM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DM_ModifierKeys__
#define __DM_ModifierKeys__

#include "DM_API.h"

#include <UT/UT_ValArray.h>

class UI_Event;
class UT_WorkBuffer;
class UT_StringHolder;

enum class DM_ModifierKeyConstraint : short
{
    FloorXY,
    FloorH
};

/// @brief Definition of a modifier key context
///
/// A modifier key context consists of a list of modifier key states, each
/// mapping to an integer identifier.  A UI_Event can then be mapped to the
/// appropriate identifier by its myAltFlags member.
class DM_API DM_ModifierKeys
{
public:
    /// Returns the identifier for the modifier matching the event, if any.
    /// In the event of no matching modifier, a value of -1 is returned.
    int		 findMatch(const UI_Event *event) const;

    /// Returns whether the event matches the specified modifier identifier.
    /// An event with multiple modifier keys set can match multiple modifier
    /// identifiers when the matched modifiers are proper subsets.
    bool	 match(const UI_Event *event, int modifier_id) const;

    /// Append a modifier.  The order these modifiers are appended is what
    /// determines their relative priorities.  When modifier_key_flags is
    /// supplied, that value overrides any loaded from the configuration files.
    ///
    /// TODO: Specify whether the modifier can be combined with other modifiers
    ///       or must stand alone.
    void	 append(int modifier_id);
    void	 append(int modifier_id, short modifier_key_flags);
    void	 append(const DM_ModifierKeys &src_context);

    /// Call after appending any modifier(s).
    void	 update();

    void	 clear()
			{ myModifiers.clear(); }
    exint	 entries() const
			{ return myModifiers.entries(); }

    /// Query the description string for the specified modifier.  Returns
    /// false if the modifier is not found or is in conflict with a higher
    /// priority modifier.
    bool	 resolveDescription(int modifier_id,
				    UT_WorkBuffer &buffer) const;

    /// Register a new modifier token and return the corresponding identifier
    /// to use in instances of this class.
    static int	 registerModifier(const char *modifier_token);

    /// Add and register a new modifier definition along with its assigment keys. 
    /// Returns a new modifier id or -1 if the operation failed.
    static int	 addModifier(const char *modifier_token, 
			    UT_Array<UT_StringHolder> const& key_assigments);

    /// Remove a modifier previously added with addModifier
    static void	 removeModifier(const char* modifier_token);

private:
    struct ModifierInfo
    {
	ModifierInfo(int id, int key_flags) : myId(id), myKeyFlags(key_flags)
	{}

	int	 myId;
	int	 myKeyFlags;
    };

    UT_ValArray<ModifierInfo>	 myModifiers;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Styler.h (STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Styler__
#define __STY_Styler__

#include "STY_API.h"
#include "STY_Types.h"
#include "STY_StylerMatch.h"
#include "STY_StyleSheet.h"
#include <UT/UT_Array.h>

class STY_MaterialProperties;
class STY_Results;
class STY_ResultsFilter;
class STY_Subject;
class STY_SubjectGroup;
class STY_Style; 
class STY_StylerGroup;
class STY_TargetMatchStatus;

/// Styler represents a state of style sheets after a sequence of 
/// "transformations" that pruned some old entries given the styled subjects
/// and added new style sheets from new sources.
/// These transformations are performed with clone() methods.
/// After that, at any point, any relevant overrides can be obtained
/// with getResults() method. 
/// Eg, a new style sheet can be added to the original styler, which returns
/// a newly cloned styler that holds its entries, then these entries
/// can be matched to the subject with clone() method, which will fully match 
/// certain entries, partially match some other ones, and reject the remaining
/// ones, returning a newly cloned styler that encodes this match state; 
/// finally getResults() can be called on the styler that was cloned last
/// to obtain overrides for the subject with which the it was cloned.
class STY_API STY_Styler
{
public:
    /// Styler with an empty tree node. Serves as the root of a tree.
    STY_Styler();

    /// Convenience constructor for non-empty root of the tree.
    explicit STY_Styler(const STY_StyleSheetHandle &stylesheet);

    /// Destructor.
    ~STY_Styler();

    /// Returns true if this styler contains any information at all. This
    /// can be either styles or style sheet references.
    bool hasAnyData() const
		{ return myData && (myData->myMatches.entries() > 0 ||
				    myData->myStyleSheets.entries() > 0); }

    /// Returns true if this styler may produce override values at this level
    /// or for some child subject. This is different than calling getResults
    /// and checking for values because getResults requires a full match in
    /// order to return a value. This is also slightly more inclusive than
    /// hasAnyData because we don't care about referenced style sheets.
    bool hasStyles() const
		{ return myData && (myData->myMatches.entries() > 0); }

    /// Returns true if this styler has any non-full matches at this level. In
    /// other words, different child subjects may produce different sets of
    /// overrides. If this styler has only full matches (and the child subjects
    /// don't add any styles) then regardless of what child subjects are
    /// applied, all the styles will match.
    bool hasAnyPartialMatchStyles() const
		{
		    if (myData)
			for (auto &&match : myData->myMatches)
			    if (!match->isFullMatch())
				return true;

		    return false;
		}

    /// Returns true if this styler has at least one stylesheet, and the last
    /// stylesheet is marked with the "final" flag, meaning no further styles
    /// should be processed.
    bool isSolo() const;

    /// Returns a styler that reflects style sheets entries after processing 
    /// them with the given subject. 
    /// The returned styler will contain only the entries whose targets were 
    /// matched by the given subject, and potentially some new entries 
    /// supplied by the subject for itself and its children (if the subject
    /// exists in some hierarchical structure).
    /// This method can be thought of as a "transformation" of the original
    /// styler, with the help of the given subject (which prunes some
    /// old entries and provides some new ones), that results in a new styler
    /// that reflects the new state.
    STY_Styler cloneWithSubject(
		const STY_Subject &subject,
		UT_WorkBuffer *errors = NULL) const;

    /// Adds a new style sheet to the styler.
    /// Conceptually, the styler pushes the new style sheet onto its own
    /// style sheet stack. By design, the entries from that new style sheet 
    /// have lower priority than the entries from the style sheets already 
    /// on the stack (which allows parents to reconfigure children).
    /// Thus, the overrides from the new style sheet will be used only if
    /// old style sheets don't specify these overrides.
    /// The resulting values can be querried with getResults().
    STY_Styler cloneWithAddedStyleSheet(
		const STY_StyleSheetHandle &stylesheet) const;

    /// Combines this styler with another and returns the result.
    /// The provided styler can be essentially concatenated with the current
    /// styler by providing an empty push_target. Or the provided styler can
    /// be treated as a styler of sub-components of this styler by providing
    /// a push_target that indicates which sub-components should be affected
    /// by the provided styler.
    STY_Styler cloneWithAddedStyler(
		const STY_Styler &styler,
		const STY_TargetHandle &push_target = STY_TargetHandle()) const;

    /// Returns a new styler with all styles that override the given
    /// categories and properties removed.
    STY_Styler cloneWithOverridesRemoved(
		const STY_OverrideNames *extra_override_names,
		bool remove_full_matches) const;

    /// Obtains overrides given the current state of the styler.
    /// This method returns overrides from entries whose targets have been 
    /// previously fully matched by the subjects in the subject hierarchy.
    /// It is implemented as a wrapper around the STY_StylerGroup interface.
    /// Whenever possible, the Group interface should be used directly to
    /// process multiple style sheets in a single pass.
    /// 
    /// @param filter		An object that decides what kind of override
    ///	    values the caller is interested in. It filters out overrides
    ///	    that are not needed, and allows in the ones that the caller
    ///	    wants to obtain. Eg, the caller may want only the overrides
    ///	    in the 'materialParameters' category.
    void getResults(
		STY_Results &results,
		const STY_ResultsFilter &filter) const;

    /// Obtains a material defined inside a style sheet.
    /// Returns true if a material by the given name was defined in a style
    /// sheet, or false otherwise.
    bool getMaterialDefinition(
		STY_MaterialProperties &material_properties,
		const char *material_name) const;

    /// Get errors generated parsing the style sheet.
    void getErrors(UT_WorkBuffer &errors) const;

    /// Saves the style sheet represented by this styler as a JSON string.
    void saveStyleSheet(UT_StringHolder &stylesheet,
			bool verbose = false,
			bool path = false) const;

    /// These are for DefaultClearer below, for use by UT::ArrayMap and
    /// UT::ArraySet.
    /// @{
    SYS_FORCE_INLINE
    void clear()
    {
        myData.reset();
    }
    SYS_FORCE_INLINE
    bool isClear()
    {
        return myData != nullptr;
    }
    /// @}

private:
    /// Internally, the styler represents a style sheet stack. All style
    /// sheets that define this stack are referenced to prevent deletion of
    /// data that we need (the STY_Style objects held in each
    /// STY_StylerMatchHandle). The style sheets also hold the scripts and
    /// material definitions used by the style sheet entries. The style
    /// sheet entries we keep have been pruned to ensure that they either
    /// apply directly to the subject to which this Styler refers, or may
    /// match a child of that subject.
    class STY_StylerData :
	public UT_IntrusiveRefCounter<STY_StylerData>
    {
    public:
	/// The underlying style sheets that may be referenced by the matches
	/// stored in this styler (ie, myMatches).
	UT_Array<STY_StyleSheetHandle>	myStyleSheets;

	/// List of active entries that affect subjects, that were passed 
	/// as an argument to clone methods, or their children.
	/// This is a list of entries successfully matched so far when cloning
	/// stylers. This list references the underlying style sheet entries,
	/// but contains its own version of targets. 
	UT_Array<STY_StylerMatchHandle>	myMatches;
    };
    typedef UT_IntrusivePtr<STY_StylerData> STY_StylerDataHandle;

    /// Helper for adding a new style sheet to an existing one. Takes an
    /// optional subject that is used for filtering self targets. Any
    /// non-self target entries are left intact, allowing subject children
    /// to match them, and allowing getResults() for subject to skip them.
    /// Modifies this Styler, so should only be used during the construction
    /// of a new styler.
    void addStyler(const STY_Styler &styler,
	    const STY_Subject *self,
	    STY_StylerGroupMatchHandle *group_matches = NULL,
	    int subject_index = -1);

    /// Helper for adding a new style sheet to an existing one. Takes an
    /// optional target that is used to add a level of targeting to all
    /// styles on the added style sheet. This can be used to compose a bunch
    /// of lower level (e.g. primitive attribute style sheets) into a higher
    /// level (e.g. object primitive) style sheet.
    void addStyler(const STY_Styler &add_styler,
	    const STY_TargetHandle &push_target = STY_TargetHandle());

    /// Helper function for pruning a styler for a subject. Generates the new
    /// closure when a matching entry is found.
    STY_TargetMatchClosureHandle
	    getNewClosure(const STY_StylerMatch &match,
		    const STY_Subject &subject,
		    const STY_TargetMatchStatus &match_status) const;

    /// Prunes the current style sheet stack, getting rid of any entries 
    /// that don't match the subject. Modifies this Styler, so should only
    /// be used during the construction of a new styler. The match_map and
    /// closure_map parameters tell this function to output the results of
    /// all candidate match attempts to these arrays. They are used by our
    /// array-based processing of STY_StylerGroup.
    void pruneWithSubject(
	    const STY_Subject &subject,
	    STY_StylerGroupMatchHandle *group_matches = NULL,
	    int subject_index = -1);

    /// Prunes any overrides that match the provided category/name pairs.
    /// This may involve removing whole style entries, or just some of the
    /// overrides from a style entry.
    void pruneOverrides(
	    const STY_OverrideNames *extra_override_names,
	    bool remove_full_matches);

    /// Styler data. 
    /// Ultimately, STY_Styler is a convenience wrapper around this pointer.
    /// Using smart pointers is convenient to pass stylers around by value,
    /// which provides a much nicer interface for cloning stylers.
    STY_StylerDataHandle		myData;

    /// This class needs access to our internal functions for implementation
    /// of efficient array-based styler methods.
    friend class			STY_StylerGroup;
    friend class			sty_PruneWithSubjectTask;
};

namespace UT {
template<>
struct DefaultClearer<STY_Styler>
{
    SYS_FORCE_INLINE
    static void clear(STY_Styler&v) { v.clear(); }
    SYS_FORCE_INLINE
    static bool isClear(STY_Styler &v) { return v.isClear(); }
    SYS_FORCE_INLINE
    static void clearConstruct(STY_Styler *p) { new((void *)p) STY_Styler(); }

    static const bool clearNeedsDestruction = false;
};
}

#endif


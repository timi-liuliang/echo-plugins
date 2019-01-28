/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Template.h (Parameter Library)
 *
 * COMMENTS:
 *	PRM_Template is the class which contains all the static data
 *	relating to a parameter. It encapsulates the characteristics
 *	of the parameter, such as type, default values, range, etc.
 *	A PRM_Template is used in conjunction with a PRM_Instance to 
 *	define a PRM_Parm. A PRM_Instance is to a PRM_Template as
 *	an object is to a class in C++.
 */

#ifndef __PRM_Template__
#define __PRM_Template__

#include "PRM_API.h"
#include "PRM_Callback.h"
#include "PRM_ConditionalType.h"
#include "PRM_Name.h"
#include "PRM_Type.h"
#include <UT/UT_Assert.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>

#define PRM_TYPE_IS_SWITCHER(type) (type.isSwitcher())
#define PRM_IS_JOIN(parm) (parm.getTypeExtended() == PRM_TYPE_TOGGLE_JOIN || \
		       parm.getTypeExtended() == PRM_TYPE_JOIN_PAIR)

class UT_WorkBuffer;
class PRM_ChoiceList;
class PRM_Conditional;
class PRM_ConditionalBase;
class PRM_Range;
class PRM_Default;
class PRM_Template;
class PRM_SpareData;

PRM_API extern PRM_ChoiceList PRMtoggleMenu;

class PRM_API PRM_Template
{
public:
    // An export level is a hint to say if a parm should be displayed or not.
    // By default, all parms have minimum export level, which means that
    // they are only displayed if someone asks for a dialog with the minimum
    // export level.  Creating a dialog with a higher export level will only
    // contain those parameters with higher export levels.
    enum PRM_Export
    {
	PRM_EXPORT_MIN,
	PRM_EXPORT_MED,
	PRM_EXPORT_MAX,
	PRM_EXPORT_TBX		// for toolboxes
    };

    enum
    {
	// This is maximum vector size for parameters. This is because we keep
	// per component flags in uint32 variables. For myAutoScopeField, we
	// use the high-bit to keep track of whether it's initialized so the
	// actual limit is 31, not 32.
	MAX_VECTOR_SIZE = 31
    };

    // No extended type given and no export level given.
			  PRM_Template(PRM_Type	thetype = PRM_LIST_TERMINATOR,
				int			 thevectorsize = 1, 
				PRM_Name		*thenameptr = 0,
				PRM_Default		*thedefaults = 0,
				PRM_ChoiceList		*thechoicelistptr = 0,
				PRM_Range		*therangeptr = 0,
				PRM_Callback		 thecallbackfunc = 0,
				PRM_SpareData		*thespareptr = 0,
				int			 theparmgroup = 1,
				const char		*thehelptext = 0,
				PRM_ConditionalBase	*thecondptr = 0);

    // No extended type given but export level given.
			  PRM_Template(PRM_Type		 thetype,
				PRM_Export		 theexportlevel,
				int			 thevectorsize = 1, 
				PRM_Name		*thenameptr = 0,
				PRM_Default		*thedefaults = 0,
				PRM_ChoiceList		*thechoicelistptr = 0,
				PRM_Range		*therangeptr = 0,
				PRM_Callback		 thecallbackfunc = 0,
				PRM_SpareData		*thespareptr = 0,
				int			 theparmgroup = 1,
				const char		*thehelptext = 0,
				PRM_ConditionalBase	*thecondptr = 0);

    // Extended type given but no export level given.
			  PRM_Template(PRM_Type		 thetype,
				PRM_TypeExtended	 thetype_ext,
				int			 thevectorsize = 1, 
				PRM_Name		*thenameptr = 0,
				PRM_Default		*thedefaults = 0,
				PRM_ChoiceList		*thechoicelistptr = 0,
				PRM_Range		*therangeptr = 0,
				PRM_Callback		 thecallbackfunc = 0,
				PRM_SpareData		*thespareptr = 0,
				int			 theparmgroup = 1,
				const char		*thehelptext = 0,
				PRM_ConditionalBase	*thecondptr = 0);

    // Extended type given and export level given.
			  PRM_Template(PRM_Type		 thetype,
				PRM_TypeExtended	 thetype_ext,
				PRM_Export		 theexportlevel,
				int			 thevectorsize = 1, 
				PRM_Name		*thenameptr = 0,
				PRM_Default		*thedefaults = 0,
				PRM_ChoiceList		*thechoicelistptr = 0,
				PRM_Range		*therangeptr = 0,
				PRM_Callback		 thecallbackfunc = 0,
				PRM_SpareData		*thespareptr = 0,
				int			 theparmgroup = 1,
				const char		*thehelptext = 0,
				PRM_ConditionalBase	*thecondptr = 0);
    
    // Dynamic multi-parm type contructors:
    
    // Notes: - the export level cannot be PRM_EXPORT_TBX (they won't fit)
    //        - the vector size is the number of shown instances (Scroll type)
    //          or the number of instances per tab (Switcher type).
    //        - the default is the number of default instances (int)
    //        - the range is the allowable range of instances, min zero.
    //	      - the template list is a list of templates to use as a guide
    //	        to create new instances (a template, but I didn't want to use
    //		that word again). Names will have a number suffixed to them
    //	        depending on the instance number ("foo1") or %d may be used to
    //		specify where the number is put ("foo%dbar"). The template list
    //		otherwise should look exactly the same as a normal list.
    //        - the start offset is where we start counting our instances,
    //          by default from 1.
			  PRM_Template(PRM_MultiType	 thetype,
				PRM_Template		*thetemplates,
				PRM_Export		 theexportlevel,
				fpreal			 multisize,
				PRM_Name		*thenameptr,
				PRM_Default		*thedefaultsize = 0,
				PRM_Range		*thedefaultrange = 0,
				PRM_SpareData		*thespareptr = 0,
				const char		*thehelptext = 0,
				PRM_ConditionalBase	*thecondptr = 0,
				PRM_Callback		 thecallbackfunc = 0);
    // no export option.
			  PRM_Template(PRM_MultiType	 thetype,
				PRM_Template		*thetemplates,
				fpreal			 multisize,
				PRM_Name		*thenameptr,
				PRM_Default		*thedefaultsize = 0,
				PRM_Range		*thedefaultrange = 0,
				PRM_SpareData		*thespareptr = 0,
				const char		*thehelptext = 0,
				PRM_ConditionalBase	*thecondptr = 0,
				PRM_Callback		 thecallbackfunc = 0);

			  PRM_Template(const PRM_Template &src);
			 ~PRM_Template();

    int64		 getMemoryUsage(bool inclusive) const;

    const PRM_Template	 &operator=(const PRM_Template &src);

    void		 initialize(PRM_Type thetype,
				PRM_TypeExtended thetype_ext,
				PRM_Export theexportlevel,
				int thevectorsize,
				PRM_Name *thenameptr,
				PRM_Default *thedefaults,
				PRM_ChoiceList *thechoicelistptr,
				PRM_Range *therangeptr,
				PRM_Callback thecallbackfunc,
				PRM_SpareData *thespareptr,
				int theparmgroup,
				const char *thehelptext,
				PRM_ConditionalBase *thecondptr);
    
    void		initMulti(PRM_MultiType	thetype,
				PRM_Template *thetemplates,
				PRM_Export theexportlevel,
				fpreal multisize,
				PRM_Name *thenameptr,
				PRM_Default *thedefaultsize,
				PRM_Range *thedefaultrange,
				PRM_Callback thecallbackfunc,
				PRM_SpareData *thespareptr,
				const char *thehelptext,
				PRM_ConditionalBase *thecondptr);

    // This method is effectively an assignment operator which copies everything
    // from the source except for the vector size and defaults.
    void		  assign(const PRM_Template &src, int thevectorsize,
				PRM_Default *thedefaults);

    const PRM_Type	 &getType() const	{ return myType; }
    PRM_MultiType	  getMultiType() const	{ return myMultiType; }
    bool		  isMultiType() const;
    bool		  isRampType() const;
    bool		  isRampTypeColor() const;
    PRM_TypeExtended	  getTypeExtended() const { return myTypeExtended; }
    int			  getVectorSize() const	{ return myVectorSize; }
    fpreal		  getMultiSize() const  { return myMultiSize; }
    void		  setNamePtr(PRM_Name *name) { myNamePtr = name; }
    PRM_Name		 *getNamePtr()		{ return myNamePtr; }
    const PRM_Name	 *getNamePtr() const	{ return myNamePtr; }
    PRM_SpareData	 *getSparePtr()		{ return mySparePtr; }
    const PRM_SpareData	 *getSparePtr() const	{ return mySparePtr; }
    PRM_SpareData	 *getOrCreateSparePtr();
    void		  setSparePtr(PRM_SpareData *spare)
			    { mySparePtr = spare; }
    PRM_Conditional	 *getConditionalPtr(PRM_ConditionalType type);
    const PRM_Conditional *getConditionalPtr(PRM_ConditionalType type) const;
    PRM_ConditionalBase *getConditionalBasePtr()
			    { return myConditionalPtr; }
    const PRM_ConditionalBase *getConditionalBasePtr() const
			    { return myConditionalPtr; }
    PRM_Template &setConditionalBasePtr(PRM_ConditionalBase *conditional)
			    { myConditionalPtr = conditional; return *this; }
    
    PRM_Conditional SYS_DEPRECATED(12.0) *getDisablePtr()
			  { return getConditionalPtr(PRM_CONDTYPE_DISABLE); }
    const PRM_Conditional SYS_DEPRECATED(12.0) *getDisablePtr() const	
			  { return getConditionalPtr(PRM_CONDTYPE_DISABLE); }
    const PRM_ChoiceList *getChoiceListPtr() const
			  {
			      const PRM_ChoiceList	*list;
			      if (!myChoiceListPtr &&
				      (myType & PRM_TYPE_ORDINAL) &&
				      (myType & PRM_TYPE_TOGGLE))
				  list = &PRMtoggleMenu;
			      else
				  list = myChoiceListPtr;
			      return list;
			  }
    PRM_ChoiceList	 *getChoiceListPtr()	
			  {
			      PRM_ChoiceList	*list;
			      if (myChoiceListPtr == nullptr
				      && myType.isOrdinalType()
				      && myType.hasOrdinalType(
					    PRM_Type::PRM_ORD_TOGGLE))
				  list = &PRMtoggleMenu;
			      else
				  list = myChoiceListPtr;
			      return list;
			  }
    void		  setChoiceListPtr(PRM_ChoiceList *list)
			  {
			      myChoiceListPtr = list;
			  }
    const PRM_Range	 *getRangePtr()	const	{ return myRangePtr; }
    PRM_Range	 	 *getRangePtr()		{ return myRangePtr; }
    PRM_Callback	  getCallback() const	{ return myCallbackFunc; }
    void		  setCallback(PRM_Callback callback)
						{ myCallbackFunc = callback; }
    const UT_StringHolder &getHelpText() const	{ return myHelpText; }
    unsigned int	  getAutoScopeField()
			  {
			    if (myAutoScopeField & (1<<(unsigned)myVectorSize))
				setAutoScopeField();
			    return myAutoScopeField;
			  }

    const PRM_Template   *getMultiParmTemplate() const
			  { return myMultiParmTemplate; }
    PRM_Template	 *getMultiParmTemplate() 
			  { return myMultiParmTemplate; }
    void		  setMultiParmTemplate(PRM_Template *tplate)
			  {
			      UT_ASSERT(!tplate || !tplate->isRampType());
			      myMultiParmTemplate = tplate;
			  }
    int			  getMultiStartOffset() const;

    /// matchMultiInstance() tests the given name and returns true if it
    /// might be an instance parm of the multiparm token (with # characters).
    /// - If the result is true, then the parsed instance index (relative to
    ///   start_offset) is returned.
    /// - If the index in the name is less that start_offset, then it will be
    ///   rejected.
    /// - For nested multiparms, the instance_index array contains an array of
    ///   the parent multiparm instance indices that you want to match against.
    ///   If it is nullptr, then those checks will be skipped.
    static bool		  matchMultiInstance(const char *name,
					     const char *multi_name,
					     int start_offset,
					     int &index,
					     const UT_IntArray *instance_index);

    PRM_Default		 *getDefault(int index)
			  {
			      return (myDefaults
				      && index >= 0 && index < myVectorSize)
				  ? myDefaults[index].get() : nullptr;
			  }
    const PRM_Default	 *getDefault(int index) const
			  {
			      return (myDefaults
				      && index >= 0 && index < myVectorSize)
				  ? myDefaults[index].get() : nullptr;
			  }
    PRM_Default		 *getFactoryDefaults() const
			  {
			      return myFactoryDefaults;
			  }
    void		  setFactoryDefaults(PRM_Default *defaults)
			  {
			      myFactoryDefaults = defaults;
			  }
    void		  restoreFactoryDefaults();
    void		  setFloatDefault(int index, fpreal v);
    void		  setOrdinalDefault(int index, int v);
    void		  setStringDefault(int index, const char *v);
    void		  setDefault(int index, const PRM_Default &d);

    // This function sets the PRM_TYPE_NORESIM flag on an existing parameter.
    // This is an odd operation because it is changing the type of a parameter
    // when it already exists. But setting this flag doesn't really change
    // the fundamental type of the parameter. This capability is used in
    // DOP_Table.C for marking parameters related to guide geometry as not
    // requiring a refresh.
    void		  setNoResim(bool noresim);
    void		  setNoCook(bool nocook);
    /// Set the no-cook flag based on the "cook_dependent" spare data.  If the
    /// force flag is set, the no-cook flag will be set to true if the spare
    /// data doesn't exist.
    /// @return The value of the nocook flag
    bool		  setNoCook(const PRM_SpareData *spare,
					bool force=false);
    void		  setBaseParm(bool baseparm);
    void		  setInvisible(bool invisible);
    bool		  getInvisible() const
			  { return (myType & PRM_TYPE_INVISIBLE) != 0; }
    void		  setJoinNext(bool joinnext);
    bool		  getJoinNext() const
			    { return (myType & PRM_TYPE_JOIN_NEXT) != 0; }
    void		  setLabelNone(bool labelnone);
    bool		  getLabelNone() const
			    { return (myType & PRM_TYPE_LABEL_NONE) != 0; }

    void		  setTypeExtended(PRM_TypeExtended type_ext)
			    { myTypeExtended = type_ext; }

    PRM_Export		  exportLevel() const
			  { return (PRM_Export)myExportLevel; }
    void		  setExportLevel(PRM_Export level)
			  { myExportLevel = (unsigned char)level; }
    int			  getParmGroup() const { return (int)myParmGroup; }

    // findSubIndex returns -1 if no parameter is found
    int			  findSubIndex(const UT_StringRef &thechannelname) const
			  {
			      if (!myChannelNames)
				  buildChannelNames();

			      for (int i = 0; i < myVectorSize; i++)
			      {
				  if (myChannelNames[i] == thechannelname)
				      return i;
			      }
			      return -1;
			  }

    const UT_StringHolder *getChannelTokens() const
			  {
			      if (!myChannelNames)
				  buildChannelNames();
			      return myChannelNames.get();
			  }
    void		  getChannelToken(UT_String &s, int vi) const
			  {
			      if (!myChannelNames)
				  buildChannelNames();

			      if (vi >= 0 && vi < myVectorSize)
			      {
				  s = myChannelNames[vi].buffer();
			      }
			      else
			      {
				  UT_ASSERT(!"ERROR: Out of range");
				  s = "";
			      }
			  }

    const UT_StringHolder &getChannelToken(int vi) const
			  {
			      if (!myChannelNames)
				  buildChannelNames();

			      if (vi >= 0 && vi < myVectorSize)
				  return myChannelNames[vi];

			      UT_ASSERT(!"ERROR: Out of range");
			      return UT_StringHolder::theEmptyString;
			  }

    bool		  hasChannelToken(const UT_StringRef &s, int vi) const
			  {
			      if (!myChannelNames)
				  buildChannelNames();

			      UT_ASSERT_P(vi >= 0 && vi < myVectorSize);
			      return myChannelNames[vi] == s;
			  }
    void		  getChannelLabel(UT_String &thestrref,
				int theindex) const;
    void		  getToken(UT_String &thestrref) const
			  {
			    thestrref = myNamePtr->getToken();
			  }
    void		  getLabel(UT_String &thestrref) const
			  {
			    thestrref = myNamePtr->getLabel();
			  }

    int			  getExpressionFlag() const
			  {
			    return myNamePtr->getExpressionFlag();
			  }

    //
    // Retrieve as strings instead of UT_Strings.
    const char		 *getToken() const
			  {
			    return myNamePtr->getToken();
			  }
    const UT_StringRef	 &getTokenRef() const
			  {
			    return myNamePtr->getTokenRef();
			  }
    const char		 *getLabel() const
			  {
			    return myNamePtr->getLabel();
			  }
    unsigned		  getTokenHash() const
			  {
			    return myNamePtr->getHash();
			  }

    // A utility function to count the number of templates in a list.  When
    // counting a list for inclusion into a switcher, nested switchers (and all
    // their templates) only count as a single template.  When for_switcher is
    // true, the count will include switchers (and all their nested templates)
    // as a single entity.  When for_switcher is false, the function will
    // return the total size of the template. Similarly for toggle joins &
    // parm joins with 'for_join'.
    static int			 countTemplates(
					const PRM_Template *templates,
					bool for_switcher = false,
					bool for_joins = false,
					bool for_rows = false);
    // Finds an index of a template that matches a given token. Returns -1 if
    // not found.
    static int			 getTemplateIndexByToken(
					const PRM_Template *templates,
					const char * token);

    // Returns a pointer to the template right after the PRM_SWITCHER
    // template that is passed in.
    static const PRM_Template	*getEndOfSwitcher(
					const PRM_Template *templates);
    // Returns the number of switcher and non-switcher parms contained
    // within another switcher.
    static void			 getSwitcherStats(
					const PRM_Template *templates,
					UT_IntArray &numswitchersonpages,
					UT_IntArray &numnonswitchersonpages);

    static bool			 getEnclosingSwitcherFolder(
					const PRM_Template *tplates,
					const int index,
					int &switcher, int &folder);

    // Returns the highest export level for any template in the list.
    static PRM_Export		 getHighestExportLevel(
					const PRM_Template *tplates);

    // Return whether our choicelist is the one used automatically when none
    // is specified by the user.
    bool			 isAutomaticallyGeneratedChoiceList() const;

    // Find the first folder, if any, with the specified label in a switcher
    // template.  Returns -1 if none exists.
    int				 findSwitcherFolderWithLabel(
					const char *label) const;

    unsigned int	  getAutoSelectField()
			  {
			    if (myAutoSelectField & (1<<(unsigned)myVectorSize))
				setAutoSelectField();
			    return myAutoSelectField;
			  }
private:
    void			 clearOurData();
    void			 buildChannelNames() const;
    void			 setAutoScopeField();
    void			 setAutoSelectField();
    
    using PRM_DefaultPtr = UT_UniquePtr<PRM_Default>;

    PRM_Type		 	 myType;
    PRM_MultiType		 myMultiType;
    PRM_TypeExtended	 	 myTypeExtended;
    int			 	 myVectorSize;
    PRM_Name		 	*myNamePtr;
    UT_UniquePtr<PRM_DefaultPtr[]> myDefaults;
    PRM_Default		 	*myFactoryDefaults;
    PRM_ChoiceList	 	*myChoiceListPtr;
    PRM_Range		 	*myRangePtr;
    PRM_Callback	 	 myCallbackFunc;
    PRM_SpareData	 	*mySparePtr;
    PRM_ConditionalBase		*myConditionalPtr;
    unsigned char	 	 myExportLevel;
    unsigned char	 	 myParmGroup;
    unsigned char		 myChoiceListOwnedByMe;
    unsigned int		 myAutoScopeField;
    unsigned int		 myAutoSelectField;
    PRM_Template		*myMultiParmTemplate;
    fpreal			 myMultiSize;
    UT_StringHolder		 myHelpText;
    mutable UT_UniquePtr<UT_StringHolder[]>
				 myChannelNames;    // just a cache
};

#endif

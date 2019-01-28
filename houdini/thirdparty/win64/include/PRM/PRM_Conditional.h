/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Conditional.h ( PRM Library, C++)
 *
 * COMMENTS:	Conditional information based on a conditional string.
 */

#ifndef __PRM_Conditional__
#define __PRM_Conditional__

#include "PRM_API.h"

#include "PRM_ConditionalType.h"

#include <UT/UT_IntArray.h>
#include <UT/UT_Map.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_Pair.h>
#include <UT/UT_Variadic.h>
#include <map>

class PRM_Parm;
class PRM_ParmList;
class PRM_Conditional;

enum PRM_ConditionalGroupType
{
    PRM_CONDGROUPTYPE_NONE,	// Not a group
    PRM_CONDGROUPTYPE_SINGLE,	// Single group of PRM_Conditional(s)
    PRM_CONDGROUPTYPE_SWITCHER	// Switcher group with a main group + tab groups
};

enum PRM_ConditionalOperator
{
    PRM_CONDOP_EQ,
    PRM_CONDOP_NE,
    PRM_CONDOP_GE,
    PRM_CONDOP_LE,
    PRM_CONDOP_GT,
    PRM_CONDOP_LT,
    PRM_CONDOP_REGEX_EQ,
    PRM_CONDOP_REGEX_NE
};

/// A container class for a set of AND clauses, representing a single brace
/// group (e.g "{ foo == 0 bar == 1}"). If one condition inside this class
/// fails, the whole conditional clause fails. A list of PRM_ConditionalInfos
/// can be stringed together for an OR behavior (e.g. "{foo==0} {bar==1}").
class PRM_API PRM_ConditionalInfo
{
public:
				 PRM_ConditionalInfo()
				 { }
				 PRM_ConditionalInfo(PRM_ConditionalInfo &other)
				 {
				     operator=(other);
				 }
				 
				 ~PRM_ConditionalInfo()
				 { }

    int64			 getMemoryUsage(bool inclusive) const
    {
        int64 mem = 0;
        mem += myControlParms.getMemoryUsage(false);
        mem += myControlValues.getMemoryUsage(false);
        mem += myTestOperators.getMemoryUsage(false);
        return mem;
    }

    // Create a new instance of the conditional information for parameters
    // contained within multi-parms.
    PRM_ConditionalInfo		*instance(const UT_IntArray &index_list) const;

    const PRM_ConditionalInfo	&operator=(const PRM_ConditionalInfo &src)
				 {
				     myControlParms = src.myControlParms;
				     myControlValues = src.myControlValues;
				     myTestOperators = src.myTestOperators;

				     return *this;
				 }

    void			 clear()
				 {
				     myControlParms.clear();
				     myControlValues.clear();
				     myTestOperators.clear();
				 }
    void			 addCondition(const UT_StringHolder &parm,
					      const UT_StringHolder &value,
					      PRM_ConditionalOperator op)
				 {
				    myControlParms.append(parm);
				    myControlValues.append(value);
				    myTestOperators.append(op);
				 }

    const UT_StringArray	&getControlParms() const
				 { return myControlParms; }
    const UT_StringArray	&getControlValues() const
				 { return myControlValues; }
    const UT_Array<PRM_ConditionalOperator> &getTestOperators() const
				 { return myTestOperators; }

private:
    UT_StringArray		 myControlParms;
    UT_StringArray		 myControlValues;
    UT_Array<PRM_ConditionalOperator> myTestOperators;
};

/// Base class for PRM_Conditional*. It allows passing either PRM_Conditional,
/// PRM_ConditionalGroup, or PRM_ConditionalSwitcher to a PRM_Template without
/// casting.
class PRM_API PRM_ConditionalBase
{
public:
    virtual ~PRM_ConditionalBase() {}

    // Allows us to avoid an expensive dynamic_cast. See:
    // http://tinodidriksen.com/2010/04/14/cpp-dynamic-cast-performance/
    virtual PRM_ConditionalGroupType groupType() const = 0;
    
    // Create a new conditional for use within multi-parms.  Since the
    // conditional information may be dependent on the index of the multiparm,
    // we need to replace any #'s in the conditional infos with the actual
    // index.
    virtual PRM_ConditionalBase	*instance(
				    const UT_IntArray &index_list)const = 0;

    virtual PRM_Conditional	 *getConditional(PRM_ConditionalType type) = 0;
    virtual const PRM_Conditional*getConditional(
					PRM_ConditionalType type) const = 0;

    virtual int64		 getMemoryUsage(bool inclusive) const = 0;
};


/// This class allow setting a single conditional type for a PRM_Template.
/// If multiple conditional types are required, use PRM_ConditionalGroup.
class PRM_API PRM_Conditional : public PRM_ConditionalBase
{
public:
				 PRM_Conditional(PRM_ConditionalType type =
							PRM_CONDTYPE_DISABLE);
				 PRM_Conditional(const PRM_Conditional &other);
    explicit			 PRM_Conditional(const char *conditional,
						 PRM_ConditionalType type =
							PRM_CONDTYPE_DISABLE);
    virtual 			~PRM_Conditional();

    virtual int64		 getMemoryUsage(bool inclusive) const;

    virtual PRM_ConditionalGroupType groupType() const
				 { return PRM_CONDGROUPTYPE_NONE; }
    PRM_ConditionalType	 	 getType() const
				 {
				     return myType;
				 }
				 
    static const char 		*getName(PRM_ConditionalType type,
					 bool label);
    const char 			*getName(bool label) const;

    PRM_ConditionalBase		*instance(const UT_IntArray &index_list) const;
    PRM_Conditional	 	*getConditional(PRM_ConditionalType type);
    const PRM_Conditional	*getConditional(PRM_ConditionalType type) const;
    
    int				 entries() const;
    const PRM_ConditionalInfo	*operator()(int index) const;
    const PRM_Conditional	&operator=(const PRM_Conditional &src);

    void			 clear();
    PRM_ConditionalInfo		*createInfo();

    void			 getStringFromList(
				    UT_StringHolder &str,
				    const UT_StringArray *validparms = nullptr
				    ) const;
    void			 getStringFromList(UT_String &str) const;
    void			 getStringFromList(UT_String &str,
				    const UT_StringArray &validparms) const;
    bool			 setListFromString(const char *conditional,
				    UT_StringHolder &error);

    // Verifies the provided string is in proper conditional format.
    // Proper format is:
    // All non-whitespace is enclosed in { }.
    // Inside brackets, each parm name has a value paired to it.
    // All open quotes have matching close quotes.
    static bool		 verifyConditionalString(const char *conditional,
						 UT_StringHolder &error);

    static bool		 getOperatorFromString(const char *testoperator,
					       PRM_ConditionalOperator &op);
    
    static bool		 isValidOperator(const char *testoperator);

    bool 		 eval(const PRM_Parm &prm,
                              const PRM_ParmList &pl,
			      const UT_StringArray *skip_parms) const;

private:
    static bool		 parseOperatorString(const char *&testoperator,
					     PRM_ConditionalOperator &op);

    static bool 	 parseConditionalString(const char *str,
						PRM_Conditional *cond,
						UT_StringHolder &error);
    
    void		 getStringFromList(UT_WorkBuffer &str,
			                   const UT_StringArray *validparms) const;

    void                 evalFunction(const PRM_Parm &prm,
                                      const PRM_ParmList &pl,
                                      const UT_StringRef &function,
                                      UT_String &result) const;

    UT_ValArray<PRM_ConditionalInfo *>	 myConditionalInfos;
    PRM_ConditionalType		 myType;
};

typedef UT_Pair<const char *, PRM_ConditionalType> PRM_ConditionalGroupItem;
typedef UT_VariadicT<PRM_ConditionalGroupItem>	PRM_ConditionalGroupArgs;

/// This class allow setting multiple conditionals for a PRM_Template.
class PRM_API PRM_ConditionalGroup : public PRM_ConditionalBase
{
public:
				 /// 
    PRM_ConditionalGroup(const PRM_ConditionalGroupArgs &args);

    PRM_ConditionalGroup()
    { }

    PRM_ConditionalGroup( const PRM_ConditionalGroup &other);
				
    virtual 			~PRM_ConditionalGroup();

    virtual int64		 getMemoryUsage(bool inclusive) const;

    virtual PRM_ConditionalGroupType groupType() const
				 { return PRM_CONDGROUPTYPE_SINGLE; }

    virtual PRM_ConditionalBase	*instance(const UT_IntArray &index_list) const;

    void			 addConditional(const PRM_Conditional &cond);
    void			 addConditional(const char *conditional,
						PRM_ConditionalType type);
    PRM_Conditional	 	*getConditional(PRM_ConditionalType type);
    const PRM_Conditional	*getConditional(PRM_ConditionalType type) const;
    
    void			 clear();
protected:
    void			 instance(PRM_ConditionalGroup *group,
					  const UT_IntArray &index_list) const;
    
private:
    void			 addConditional(PRM_Conditional *cond);
    
    typedef UT_SortedMap<PRM_ConditionalType, PRM_Conditional *> prm_ConditionalMap;

    prm_ConditionalMap		myConditionals;
};

class PRM_API PRM_ConditionalSwitcherItem
{
public:
    PRM_ConditionalSwitcherItem(int index, const char *conditional, PRM_ConditionalType type)
	: myIndex(index)
	, myConditional(conditional)
	, myType(type)
    {
    }
    int			 myIndex;	// Tab index
    const char		*myConditional;
    PRM_ConditionalType	 myType;
};
typedef UT_VariadicT<PRM_ConditionalSwitcherItem>	PRM_ConditionalSwitcherArgs;

/// This class can be passed to the PRM_Template constructor when constructing
/// a template for a folder switcher. This allows the caller to set a different
/// conditional for each folder.
class PRM_API PRM_ConditionalSwitcher : public PRM_ConditionalGroup
{
public:
    PRM_ConditionalSwitcher()
    { }
				 
    PRM_ConditionalSwitcher(const PRM_ConditionalSwitcherArgs &args);
    virtual ~PRM_ConditionalSwitcher();

    virtual int64		 getMemoryUsage(bool inclusive) const;

    virtual PRM_ConditionalGroupType groupType() const
				 { return PRM_CONDGROUPTYPE_SWITCHER; }
    virtual PRM_ConditionalBase	*instance(const UT_IntArray &index_list) const;

    PRM_Conditional		*getTabConditional(
				    int index,
				    PRM_ConditionalType type) const;
    PRM_ConditionalBase		*getTabConditional(int index) const;

    void			 setTabConditional(
				    int index,
				    const PRM_ConditionalBase &cond );
    void			 addTabConditional(
				    int index,
				    const PRM_Conditional &conditional);

    void			 clear();
private:
    typedef UT_SortedMap<int, PRM_ConditionalBase *> prm_SwitcherMap;
    prm_SwitcherMap		 myTabConditionals;
};


typedef SYS_DEPRECATED(12.0) PRM_Conditional		PRM_DisableList;
typedef SYS_DEPRECATED(12.0) PRM_ConditionalInfo	PRM_DisableInfo;


#endif

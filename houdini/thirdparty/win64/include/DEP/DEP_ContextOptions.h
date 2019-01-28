/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DEP_ContextOptions.h (DEP Library, C++)
 */

#ifndef __DEP_CONTEXTOPTIONS_H__
#define __DEP_CONTEXTOPTIONS_H__

#include "DEP_API.h"
#include "DEP_ContextOptionsFwd.h"
#include <UT/UT_COW.h>
#include <UT/UT_ConcurrentSet.h>
#include <UT/UT_Options.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringSet.h>
#include <UT/UT_TBBSpinLock.h>
#include <UT/UT_UniquePtr.h>
#include <SYS/SYS_Types.h>

typedef UT_ConcurrentSet<UT_StringHolder>	DEP_ContextOptionDeps;
typedef UT_UniquePtr<DEP_ContextOptionDeps>	DEP_ContextOptionDepsPtr;
typedef UT_TBBSpinLock				DEP_ContextOptionDepsLock;
typedef UT_TBBSpinLock::Scope			DEP_ContextOptionDepsLockScope;

class DEP_API DEP_ContextOptions
{
public:
				 DEP_ContextOptions();
				~DEP_ContextOptions();

    int				 getNumOptions() const;
    bool			 hasOption(const UT_StringRef &opt) const;
    const UT_OptionEntry	*getOptionEntry(
					const UT_StringRef &opt) const;
    bool			 isOptionAtDefault(
					const UT_StringRef &opt) const;

    UT_Options::iterator	 begin() const;
    UT_Options::iterator	 end() const;

    void			 setOption(const UT_StringHolder &opt,
					UT_OptionEntry *value,
					bool is_default = false);
    void			 setOptionS(const UT_StringHolder &opt,
					const UT_StringHolder &value,
					bool is_default = false);
    void			 setOptionF(const UT_StringHolder &opt,
					fpreal64 value,
					bool is_default = false);
    void			 setOptionI(const UT_StringHolder &opt,
					int64 value,
					bool is_default = false);
    void			 removeOption(const UT_StringHolder &opt);
    void			 clear();

private:
    void			 setOptionIsAtDefault(
					const UT_StringHolder &opt,
					bool is_default);

    UT_Options			 myOptions;
    UT_StringSet		 myOptionsAtDefaults;
};

DEP_API extern const DEP_ContextOptionDeps &
DEPgetContextOptionDepsFromPtr(
	const DEP_ContextOptionDepsPtr &ptr);
DEP_API extern DEP_ContextOptionDeps &
DEPcreateContextOptionDepsFromPtr(
	DEP_ContextOptionDepsPtr &ptr,
	DEP_ContextOptionDepsLock *lock = nullptr);

#endif // __DEP_CONTEXTOPTIONS_H__


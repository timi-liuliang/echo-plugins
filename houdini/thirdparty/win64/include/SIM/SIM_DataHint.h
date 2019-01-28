/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_DataHint.h ( SIM Library, C++)
 *
 * COMMENTS:	Holds information about what sort of simulation data
 * 		relationships are likely valid, and which are likely not.
 */

#ifndef __SIM_DataHint__
#define __SIM_DataHint__

#include "SIM_API.h"
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_SymbolTable.h>

class UT_StringArray;
class SIM_Data;
class SIM_Object;
class SIM_Engine;
class SIM_DataHint;
class SIM_RootData;

typedef UT_ValArray<SIM_DataHint *>	 SIM_DataHintArray;

class SIM_API SIM_DataHint
{
public:
    static void		 addHint(const UT_StringHolder &parentdatatype,
				 const UT_StringHolder &expectedchildtype,
				 const UT_StringHolder &expectedchildname,
				 int maxcount);
    static void		 getAllDataHints(UT_StringArray &parentdatatypes,
					 UT_StringArray &expectedchildtypes,
					 UT_StringArray &expectedchildnames,
					 UT_IntArray &maxcounts);
    static void		 getAllSubdataTypes(const UT_StringRef &parentdatatype,
					    UT_StringArray &subdatatypes,
					    UT_IntArray &maxcountindices,
					    UT_IntArray &maxcounts);
    static void		 getAllSubdataTypes(const UT_StringRef &parentdatatype,
					    UT_StringArray &subdatatypes,
					    UT_IntArray &maxcountindices,
					    UT_IntArray &maxcounts,
					    SIM_Engine &work_engine);
    static int		 getSubdataNumInputs(const UT_StringRef &datatype,
					    SIM_Engine &work_engine);
    static void		 verifySimulationData(const SIM_Engine &engine);
    static void		 destroyAllDataHints();

    static void		 addAlias(const UT_StringHolder &datatype,
				  const UT_StringHolder &alias);
    static const UT_StringHolder
			&getDataTypeFromAlias(const UT_StringRef &alias);
    static void		 getAllDataTypeAliases(UT_StringArray &aliases,
					       UT_StringArray &datatypes);

private:
			 SIM_DataHint(const UT_StringHolder &parentdatatype,
				      const UT_StringHolder &expecteddatatype,
				      const UT_StringHolder &expecteddataname,
				      int maxcount);
    virtual		~SIM_DataHint();

    bool		 getDataIsExpected(const SIM_Data &child,
					   const UT_StringRef &childname) const;

    static void		 processHintsForData(const SIM_Engine &engine,
					     const SIM_RootData &rootdata,
					     const SIM_Data &parentdata);
    static void		 destroyDataHints(void *);

    UT_StringHolder	 myParentDataType;
    UT_StringHolder	 myExpectedDataType;
    UT_StringHolder	 myExpectedDataName;
    int			 myMaxCount;

    typedef UT_SymbolMap<SIM_DataHintArray *> SIM_DataHintTable;
    static SIM_DataHintArray	*theMasterHintArray;
    static SIM_DataHintTable 	*theHintTable;
    static UT_SymbolMap<UT_StringHolder> theDataTypeAliases;
};

#endif

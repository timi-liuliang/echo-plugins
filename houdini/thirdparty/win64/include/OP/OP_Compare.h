/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	OPUI_ShowChanges.h
*
* COMMENTS:	Dialog that launches a comparison of an operator type
*		instance against a saved version.
*
*/

#ifndef __OP_Compare_h__
#define __OP_Compare_h__

#include "OP_API.h"
#include <UT/UT_MIME.h>
#include <UT/UT_Options.h>

class OP_Node;
class OP_Network;
class OP_OTLLibrary;
class OP_OTLDefinition;
class OP_CompareData;

class OP_API OP_Compare
{
public:
    OP_Compare();
    virtual ~OP_Compare();



    typedef enum
    {
	ModeHipFile,
	ModeHDANetwork,
	ModeHDA
    } DialogMode;

    typedef enum
    {
	SourceUnknown,
	SourceHipFile,
	SourceHDANetwork,
	SourceHDANetworkCache,
	SourceHDA,
	SourceNetwork,
	SourceBackup,
	SourceCurrentScene,
	SourceLastSaved
    } CompareSource;

    void		 setOptions(const UT_Options &options)
    {
	myOptions = options;
	prepareFilters();
    }
    const UT_Options	&getOptions() const
    {
	return myOptions;
    }

    void		 getCompareFilters(UT_WorkBuffer &inclusions,
					   UT_WorkBuffer &exclusions,
					   UT_MIME::TextFilterList &text_filters) const;
    void		 prepareFilters();

    void		 prepareNetworkFile(const UT_StringHolder &source_file,
					    UT_StringHolder &filename) const;

    void		 prepareHDA(const UT_StringHolder &source_file,
				    UT_StringHolder &filename) const;

    void		 prepareHDANetwork(const UT_StringHolder &source_file,
					   const UT_StringHolder &table_and_name,
					   UT_StringHolder &filename) const;

    void		 prepareNetwork(OP_Network *network,
					UT_StringHolder &filename) const;

private:
    UT_Options		 myOptions;
    OP_CompareData	*myCompareData;
};

#endif

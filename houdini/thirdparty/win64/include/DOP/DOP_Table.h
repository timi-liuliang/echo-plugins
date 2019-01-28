/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP library (C++)
 *
 * COMMENTS:	Create DOPs of different types
 *
 */

#ifndef __DOP_Table__
#define __DOP_Table__

#include "DOP_API.h"

class OP_OperatorTable;
class SIM_DopDescription;
class UT_StringArray;
class SIM_Engine;

#include <OP/OP_Operator.h>

DOP_API void
DOP_createAutomaticOperator(OP_OperatorTable &table,
			const SIM_DopDescription &dopdescription,
			const UT_StringArray &solvertypes,
			const UT_StringArray &reltypes,
			SIM_Engine &engine,
			OP_Constructor constructor = 0,
			const char *child_table_name = 0);


#endif

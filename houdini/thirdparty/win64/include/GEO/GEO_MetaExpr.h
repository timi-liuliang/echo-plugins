/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO library (C++)
 *
 * COMMENTS:	Meta expressions used to construct the meta primitive
 *		blending tree.  There are operator nodes & primitive nodes.
 *
 */

#ifndef __GEO_MetaExpr_h__
#define __GEO_MetaExpr_h__

#include "GEO_API.h"

#include <UT/UT_Array.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_VectorTypes.h>
#include <TS/TS_Expression.h>
#include <GA/GA_Types.h>

class UT_MemoryCounter;
class GA_Range;
class GEO_Detail;
class GEO_Primitive;
class GA_PrimitiveGroup;
class GA_ROAttributeRef;
class GA_AttributeRefMap;
class GA_WeightedSum;

class GEO_API GEO_MetaExpression
{
public:
    GEO_MetaExpression()
        : myExpr()
    {}

    GEO_MetaExpression(const GEO_Detail *gdp,
        const GA_PrimitiveGroup *primgroup = NULL)
        : myExpr()
    { preCompute(gdp, primgroup); }

    GEO_MetaExpression(const GEO_Detail *gdp,
        const GA_Range &prim_range)
        : myExpr()
    { preCompute(gdp, prim_range); }

    ~GEO_MetaExpression();

    /// Compute memory usage (includes all shared memory)
    int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    TS_MetaExpressionPtr	preCompute(const GEO_Detail *gdp,
					const GA_PrimitiveGroup *prims=nullptr);
    TS_MetaExpressionPtr	preCompute(const GEO_Detail *gdp,
					const GA_Range &prim_range);

    /// Create a TS metaball expression given the gdp
    /// NOTE: ref() has not been called on the TS_MetaExpression returned.
    static TS_MetaExpressionPtr	 getExpression(const GEO_Detail *gdp);

    /// Create a TS metaball expression given the gdp and primitive range
    /// NOTE: ref() has not been called on the TS_MetaExpression returned.
    static TS_MetaExpressionPtr	 getExpression(const GEO_Detail *gdp,
					const GA_Range &prim_range,
					UT_Array<const GEO_Primitive *> *p=nullptr);

    const TS_MetaExpressionPtr	&getTSExpression() const { return myExpr; }

    // These assume that you've already done the preCompute!
    void		 computeAttributes(const UT_Vector3 &pos,
					GA_Offset result, 
					const GA_AttributeRefMap &map) const;
    void                 sumPointAttributes(const UT_Vector3 &pos,
                                            GA_Offset resultptoff,
                                            const GA_AttributeRefMap &map,
                                            GA_WeightedSum &sum) const;


    // Compute a single attribute.  Unlike the two methods above, this
    // routine normalizes the result to the sum of metaball weights.
    void		 computeNormalizedAttributes(const UT_Vector3 &pos,
					GA_Offset result,
					const GA_AttributeRefMap &map) const;
    void		 computeAttribute(const UT_Vector3 &pos, float *data,
					  const GA_ROAttributeRef &ref,
					  int size) const;

    UT_Array<const GEO_Primitive *>	  &getList() { return myPrimitives; }
    const UT_Array<const GEO_Primitive *> &getList() const { return myPrimitives; }

    /// @{
    /// Serialize into an integer array.  Nodes in the graph are stored as:
    /// @code
    ///	   id operator count primlist
    /// @endcode
    /// Where the id can be used to reference a sub-expression in other
    /// expressions.  The first id is based on the number of primitives in the
    /// gdp.
    /// For example:  "sum(1, max(0, 1), 2)", in a gdp with 10 primitives
    /// will be stored as: @code
    ///	   [ TS_SERIALIZE_MAGIC 10 TS_OP_MAX 2 0 1 11 TS_SUM 3 1 10 2 ]
    /// @endcode
    static void		serialize(const TS_MetaExpression *expr,
				    UT_Int32Array &opcodes,
				    exint nprimitives)
			    { return serialize_T<int32>(expr, opcodes, nprimitives); }
    static void		serialize(const TS_MetaExpression *expr,
				    UT_Int64Array &opcodes,
				    exint nprimitives)
			    { return serialize_T<int64>(expr, opcodes, nprimitives); }
    /// @}
    /// @{
    /// Create an expression for the given serialization
    static bool	unserialize(UT_WorkBuffer &expr, const UT_Int32Array &opcodes)
		{
		    return unserialize_T<int32>(expr, opcodes.getRawArray(),
			    opcodes.entries());
		}
    static bool	unserialize(UT_WorkBuffer &expr, const UT_Int64Array &opcodes)
		{
		    return unserialize_T<int64>(expr, opcodes.getRawArray(),
			    opcodes.entries());
		}
    static bool	unserialize(UT_WorkBuffer &expr, const int32 *array, exint size)
		{ return unserialize_T<int32>(expr, array, size); }
    static bool	unserialize(UT_WorkBuffer &expr, const int64 *array, exint size)
		{ return unserialize_T<int64>(expr, array, size); }
    /// @}

private:
    template <typename INT_T>
    static void		serialize_T(const TS_MetaExpression *expr,
				    UT_ValArray<INT_T> &opcodes,
				    exint nprimitives);
    template <typename INT_T>
    static bool		unserialize_T(UT_WorkBuffer &expr,
				    const INT_T *opcodes,
				    exint entries);

#ifdef INTEL_COMPILER
	GEO_MetaExpression(const GEO_MetaExpression&) { }
#endif
	
private:
    TS_MetaExpressionPtr		myExpr;
    UT_Array<const GEO_Primitive *>	myPrimitives;
};

#endif

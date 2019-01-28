/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_TupleUtils.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_TupleUtils__
#define __GA_TupleUtils__

#include "GA_API.h"

/// @brief Collection of useful functions for working with tuple data.
///
/// This class is a collection of common functions (static methods) useful
/// for working with tuple data.

class GA_API GA_TupleUtils 
{
public:
    // componentAsRange() and componentWSizeAsRange() are convenience methods
    // for many operations taking a component or a tuple size argument with an
    // optional component.
    // Rather than performing the logic again and again, you can simply write:
    //     int i, end;
    //     for (i = componentAsRange(input_comp, tuple_size, end); i < end; i++)
    //     { .... }
    // or
    //     int i, end;
    //     for (i = componentWSizeAsRange(input_comp, tuple_size,
    //					  input_tuple_size, end);
    //          i < end; i++)
    //     { .... }
    //
    static inline int	componentAsRange(int component, int tuple_size,
					 int &end)
		{
		    if (component < 0)
		    {
			end = tuple_size;
			return 0;
		    }
		    end = component+1;
		    return (end <= tuple_size) ? component : end;
		}
    static inline int	sizeAsRange(int size, int tuple_size, int &end)
		{
		    end = SYSmin(tuple_size, size);
		    return 0;
		}
    static inline int	componentWSizeAsRange(int component, int size,
					      int tuple_size, int &end)
		{
		    if (component < 0)
		    {
			end = SYSmin(tuple_size, size);
			return 0;
		    }
		    end = component+1;
		    return (end <= tuple_size) ? component : end;
		}
    static inline int	startWSizeAsRange(int start, int size,
					  int tuple_size, int &end)
		{
		    end = SYSmin(tuple_size, start+size);
		    return start;
		}
};

#endif

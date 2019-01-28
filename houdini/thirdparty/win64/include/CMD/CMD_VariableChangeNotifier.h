/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CMD_VariableChangeNotifier.h (CMD  Library, C++)
 *
 * COMMENTS:
 *		This class implements a notification center for global 
 *		variables in Houdini. When a variable value changes, the
 *		CMD_VariableChangeNotifier is informed of that fact, 
 *		and then it takes over the process of further notification
 *		distribution.
 */

#ifndef __CMD_VariableChangeNotifier__
#define __CMD_VariableChangeNotifier__

#include "CMD_API.h"

class CMD_API CMD_VariableChangeNotifier
{
public:
			 CMD_VariableChangeNotifier();
    virtual		~CMD_VariableChangeNotifier();

    /// Informs the variable change notifier object that a given variable
    /// value has changed. The notifier will trigger an appropriate event.
    virtual void	variableValueChanged( const char * var_name ) = 0;
};

#endif


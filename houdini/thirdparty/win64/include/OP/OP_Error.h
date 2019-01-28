#ifndef __OP_Error__
#define __OP_Error__

#include <UT/UT_ErrorManager.h>

typedef UT_ErrorSeverity OP_ERROR;

enum OP_ErrorCodes
{
    //
    //  Cooking errors

    OP_ERR_CHAN_ERROR = 0,	// 0: Evaluation of a channel failed
				// Get the error from the channel lib
    OP_ERR_INVALID_SRC,		// 1: No source or source has error
    OP_ERR_NUMSRC_TOO_FEW,	// 2: Incorrect number of sources
    OP_ERR_NUMSRC_TOO_MANY,	// 3: Incorrect number of sources
    OP_ERR_RECURSION,		// 4: Recursion into OP cooking
    OP_ERR_INTERRUPTED,		// 5: Cook was interrupted

    //
    //  File I/O errors

    OP_ERR_UNKNOWN_OP,		// 6: Unknown operator on load
    OP_ERR_CORRUPT_FILE,	// 7: Corrupt file, data wasn't what we expected
    OP_ERR_MISSING_PARENT,	// 8: Missing parent of node to load
    OP_ERR_BAD_PARENT,		// 9: Bad parent (i.e. parent is not a network)

    OP_ERR_BAD_NODETYPE,	// 10: Bad node type (warning)
    OP_ERR_OPSAVE_NOT_HIP,	// 11: File was created by opsave, not mwrite.

    OP_ERR_BAD_INTRINSIC_BIN_PACKET,// 12: warning
    OP_ERR_BAD_INTRINSIC_TOKEN,	// 13: warning

    OP_BAD_OPINPUT_READ,	// 14: Bad operator-input 
    OP_ERROR_SAVE_INTRINSIC_FAILED,// 15: Operator Save Intrinsic data failed
    OP_ERROR_OPERATOR_SAVE_FAILED, // 16: Operator Save failed

    OP_LOAD_UNKNOWN_BINARY_FLAG,// 17: 
    OP_LOAD_UNKNOWN_ASCII_FLAG,	// 18: 
    OP_BAD_FLAG_FORMAT,		// 19: 
    OP_ERR_UNIX_CMD,		// 20: Unix command execution failed
    OP_ERR_UNIX_GET,		// 21: Read from unix command failed

    OP_MACRO_IS_EMPTY,		// 22:
    OP_MACRO_COOK_FAILED,	// 23:
    OP_MACROS_ONLY,		// 24: Operation is only valid in a macro/sub-net
    OP_MACRO_INVALID,		// 25: Operation is not valid in a macro/sub-net

    OP_WARNING_RENAME_EXPORT,	// 26: Rename operation changed export

    OP_BAD_CBGROUP_VERSION,	// 27: Clipboard group load had bad version #
    OP_BAD_CBGROUP_DEFINITION,	// 28: Error in clipboard group file

    OP_NO_NETWORK,		// 29: No network set for %s
    OP_NOTHING_TO_COPY,		// 30: Nothing to copy.
    OP_COPY_FAILED,		// 31: Copy failed
    OP_NOTHING_TO_PASTE,	// 32: Nothing to paste.

    OP_ERR_PERMISSION,		// 33: Permission error

    OP_ERR_ANYTHING,		// 34: %s

    OP_ERR_LOAD_COOK,		// 35: Error loading cooked data

    OP_WARN_BAD_CHREF,		// 36: Bad channel reference
    OP_WARN_RECURSIVE_CHREF,	// 37: Recursive channel reference

    OP_WARN_COULDNT_DELETE,	// 38: Couldn't delete a particular node

    OP_WARN_ERROR_SYNCING,	// 39: Error syncing child with its definition.

    OP_WARN_COULDNT_DELETE_NETBOX, // 40: Couldn't delete a particular netbox
    OP_ERROR_NETBOX_SAVE_FAILED,   // 41: Network Box Save failed
    OP_WARN_STAMP_TOKEN_CONFLICT,  // 42: Potential stamp conflict: %s. 
    OP_WARN_PARAM_DEPRECATED,	// 43: Use of param() is deprecated.
    OP_WARN_DELETE_CHANNEL,	// 44: Deleted an unknown channel.
    OP_WARN_SPARE_CHANNEL,	// 45: Made spare parm for an unknown channel.

    OP_WARN_DUMMY_DEFINITION,	// 46: Node is using an incomplete definition.
    OP_WARN_DUMMY_CHILD,	// 47: "%s" is using incomplete definition.
    OP_WARN_DUMMY_OPERATORS,	// 48: Operators using incomplete definitions:%s

    OP_PYTHON_ERROR,		// 49: Python error: %s.

    OP_LOADING_LOCKED,		// 50: Ignoring data for locked node: %s

    OP_ERROR_POSTIT_LOAD_FAILED,   // 51: Post-it Note load failed
    OP_ERROR_POSTIT_SAVE_FAILED,   // 52: Post-it Note save failed

    OP_WARN_BAD_OPREF,		// 53: Bad node reference

    OP_ERR_TRANSFORM,		// 54: Error cooking transform of %s

    OP_ERR_IGNORED_EXTRA_INPUTS,    // 55: Ignored extraneous inputs on load

    OP_ERR_INVALID_LICENSE,	// 56: Asset has invalid license

    OP_WARN_LOAD_OP_CONFLICT,	// 57: Loaded node with conflicting name

    OP_ERR_RECURSIVE_NODETYPE,	// 58: Recursive node type (nested assets)

    OP_WARN_COULDNT_DELETE_POSTIT, // 59: Couldn't delete a particular postit
    OP_WARN_COULDNT_DELETE_DOT, // 60: Couldn't delete a particular dot

    OP_ERROR_DOT_LOAD_FAILED,   // 61: Dot load failed

    OP_UNKNOWN_CONTEXT_OPTION,	// 62: Expression queried unknown context option

    OP_ERR_NUM_ERROR_MSGS	// sentinel
};

#endif

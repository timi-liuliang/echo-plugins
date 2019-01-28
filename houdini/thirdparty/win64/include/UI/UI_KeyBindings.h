/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UI_KeyBindings.h ( UI Library, C++)
 *
 * COMMENTS:
 *	All apps and directories that define key bindings should include us.
 */
#ifndef __UI_KeyBindings_h__
#define __UI_KeyBindings_h__

#define			 COMMAND_KEY_BIT		0x40000000
#define			   OTHER_KEY_BIT		0x20000000
#define			   ARROW_KEY_BIT		0x10000000
#define			FUNCTION_KEY_BIT		0x08000000
#define			     ALT_KEY_BIT		0x04000000
#define			    CTRL_KEY_BIT		0x02000000
#define			   SHIFT_KEY_BIT		0x01000000
#define			CONFLICT_KEY_BIT		0x00800000	// Special bit for the hotkey manager
#define	       CONFLICT_ANCESTOR_KEY_BIT		0x00400000	// Special bit for the hotkey manager
#define	     CONFLICT_DESCENDANT_KEY_BIT		0x00200000	// Special bit for the hotkey manager
#define			  CHARACTER_BITS		0x001FFFFF	// Unicode range.
#define			    SPECIAL_BITS		(~CHARACTER_BITS)
#define			   MODIFIER_BITS		(SHIFT_KEY_BIT|CTRL_KEY_BIT|ALT_KEY_BIT|COMMAND_KEY_BIT)

#define KEY_IS_ALTED(key)		((key) &      ALT_KEY_BIT)
#define KEY_IS_CTRLED(key)		((key) &     CTRL_KEY_BIT)
#define KEY_IS_SHIFTED(key)		((key) &    SHIFT_KEY_BIT)
#define KEY_IS_COMMANDED(key)		((key) &  COMMAND_KEY_BIT)

#define KEY_IS_ARROW(key)		((key) &    ARROW_KEY_BIT)
#define KEY_IS_OTHER(key)		((key) &    OTHER_KEY_BIT)
#define KEY_IS_FUNCTION(key)		((key) & FUNCTION_KEY_BIT)
#define KEY_CHAR_ONLY(key)		((key) &   CHARACTER_BITS)
#define KEY_SPECIAL_ONLY(key)		((key) &     SPECIAL_BITS)
#define KEY_NO_CONFLICT(key)		((key) & ~(CONFLICT_KEY_BIT | CONFLICT_ANCESTOR_KEY_BIT | CONFLICT_DESCENDANT_KEY_BIT))
#define KEY_HAS_MODIFIERS(key)		(((key) & MODIFIER_BITS)!=0)

#define ALT_KEY(key)			((key) |      ALT_KEY_BIT)
#define CTRL_KEY(key)			((key) |     CTRL_KEY_BIT)
#define SHIFT_KEY(key)			((key) |    SHIFT_KEY_BIT)
#define COMMAND_KEY(key)		((key) |  COMMAND_KEY_BIT)
#define OTHER_KEY(key)			((key) |    OTHER_KEY_BIT)
#define ARROW_KEY(key)			((key) |    ARROW_KEY_BIT)
#define FUNCTION_KEY(key)		((key) | FUNCTION_KEY_BIT)

#define NO_ALT_KEY(key)			((key) &     ~ALT_KEY_BIT)
#define NO_CTRL_KEY(key)		((key) &    ~CTRL_KEY_BIT)
#define NO_SHIFT_KEY(key)		((key) &   ~SHIFT_KEY_BIT)
#define NO_COMMAND_KEY(key)		((key) & ~COMMAND_KEY_BIT)
#define NO_MODIFIER_KEY(key)		(((key) & MODIFIER_BITS)==0)


#define BACKSPACE_KEY			0x08
#define DELETE_KEY			0x7F
#define RETURN_KEY			0x0A
#define ENTER_KEY			0x0D
#define ESC_KEY				0x1B
#define TAB_KEY				0x09

#define KEYCMP(symbol)		(UI_Object::keycmp(symbol,key))
// Version of KEYCMP that suppresses the hotkey echoing to the error manager.
// UI_ErrorManager::displayHotkeyMessage() can be called later to echo a
// custom message.
#define KEYCMP_SILENT(symbol) \
		    (UI_Object::keycmp(symbol, key, UI_HotkeyEcho::theNullEcho))
// Version of KEYCMP that takes a custom UI_HotkeyEcho object to customize the
// echoing of hotkeys to the error manager.
#define KEYCMP_CUSTOM_ECHO(symbol, echo) (UI_Object::keycmp(symbol, key, echo))

//
// ------------- Hotkey definitions: ----------------
//

// Global:
#define UI_CMD_CUT			"h.cut"
#define UI_CMD_COPY			"h.copy"
#define UI_CMD_PASTE			"h.paste"
#define UI_CMD_UNDO			"h.undo"
#define UI_CMD_REDO			"h.redo"
#define UI_CMD_FIND			"h.find"

// For the UI Inputfield:
#define UI_CMD_IFIELD_EDITOR		"inputfield.editor"
#define UI_CMD_IFIELD_UNDO		"inputfield.undo"
#define UI_CMD_IFIELD_HOME		"inputfield.home"
#define UI_CMD_IFIELD_END		"inputfield.end"
#define UI_CMD_IFIELD_UP		"inputfield.up"
#define UI_CMD_IFIELD_DOWN		"inputfield.down"
#define UI_CMD_IFIELD_PREV		"inputfield.prev"
#define UI_CMD_IFIELD_NEXT		"inputfield.next"
#define UI_CMD_IFIELD_SELECT_HOME	"inputfield.select_home"
#define UI_CMD_IFIELD_SELECT_END	"inputfield.select_end"
#define UI_CMD_IFIELD_SELECT_UP		"inputfield.select_up"
#define UI_CMD_IFIELD_SELECT_DOWN	"inputfield.select_down"
#define UI_CMD_IFIELD_SELECT_PREV	"inputfield.select_prev"
#define UI_CMD_IFIELD_SELECT_NEXT	"inputfield.select_next"
#define UI_CMD_IFIELD_SELECT_ALL	"inputfield.select_all"
#define UI_CMD_IFIELD_BACKTAB		"inputfield.backtab"
#define UI_CMD_IFIELD_JUMP_TO_MATCH	"inputfield.jump_match"
#define UI_CMD_IFIELD_CONTEXT_HELP	"inputfield.context_help"
#define UI_CMD_IFIELD_PREV_WORD		"inputfield.prev_word"
#define UI_CMD_IFIELD_NEXT_WORD		"inputfield.next_word"
#define UI_CMD_IFIELD_SELECT_PREV_WORD	"inputfield.select_prev_word"
#define UI_CMD_IFIELD_SELECT_NEXT_WORD	"inputfield.select_next_word"
#define UI_CMD_IFIELD_AUTOCOMPLETE	"inputfield.autocomplete"

// For the UI Textport and UI_TextEditor:
#define UI_CMD_TXTPORT_PAGEUP		"h.pane.textport.pageup"
#define UI_CMD_TXTPORT_PAGEDOWN		"h.pane.textport.pagedown"
#define UI_CMD_TXTPORT_TOP		"h.pane.textport.top"
#define UI_CMD_TXTPORT_BOTTOM		"h.pane.textport.bottom"
#define UI_CMD_TXTPORT_FIRSTLINE	"h.pane.textport.firstline"
#define UI_CMD_TXTPORT_LASTLINE		"h.pane.textport.lastline"
#define UI_CMD_TXTPORT_SELECTPAGEUP	"h.pane.textport.selectpageup"
#define UI_CMD_TXTPORT_SELECTPAGEDOWN	"h.pane.textport.selectpagedown"
#define UI_CMD_TXTPORT_SELECTTOP	"h.pane.textport.selecttop"
#define UI_CMD_TXTPORT_SELECTBOTTOM	"h.pane.textport.selectbottom"
#define UI_CMD_TXTPORT_SELECTFIRSTLINE	"h.pane.textport.selectfirstline"
#define UI_CMD_TXTPORT_SELECTLASTLINE	"h.pane.textport.selectlastline"
#define UI_CMD_TXTPORT_EDITOR		"h.pane.textport.editor"
#define UI_CMD_TXTPORT_MATCH_PAREN	"h.pane.textport.match_paren"
#define UI_CMD_TXTPORT_SEARCH_AND_REPLACE "h.pane.textport.search_and_replace"
#define UI_CMD_TXTPORT_JUMP_TO_LINE	"h.pane.textport.jump_to_line"
#define UI_CMD_TXTPORT_COMMENT_OUT	"h.pane.textport.comment_out"
#define UI_CMD_TXTPORT_UNCOMMENT	"h.pane.textport.uncomment"

// For the UI Window
#define UI_CMD_CONTEXT_HELP		"h.context_help"

#endif

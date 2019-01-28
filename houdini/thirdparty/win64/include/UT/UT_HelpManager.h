/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_HelpManager.h
 *
 * COMMENTS:
 *	This class allows any code in Houdini to load a help file or display
 *	arbitrary text in a help pane.
 *
 *	Note that UTgetHelpManager() will return null in applications that
 *	don't support help panes.
 */

#ifndef __UT_HelpManager_h__
#define __UT_HelpManager_h__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_Assert.h"

class UT_String;
class OP_Operator;
class OP_Node;
class OP_GalleryEntry;
class BM_State;
class SHLF_Tool;
class PRM_Parm;
class PRM_ScriptParm;
class PYP_Interface;

// The help URL protocols used by the help system.
#define UT_OP_PROTOCOL_PREFIX		"operator:"
#define UT_PROP_PROTOCOL_PREFIX		"prop:"
#define UT_GALLERY_PROTOCOL_PREFIX	"gallery:"
#define UT_PARM_PROTOCOL_PREFIX		"parm:"


class UT_API UT_HelpManager
{
public:
    UT_HelpManager();
    virtual ~UT_HelpManager();

    // openContents() will display the help contents page.
    virtual void	 openContents() = 0;

    // openStartup() brings up the special welcome/essential skills page
    // of help upon startup:
    virtual void	 openStartup() = 0;

    // openFile() and displayData() are the main methods of the help manager
    // to display help.  They will ensure a help browser pane exists and
    // they will open a text/html file or display text/html data.  Note
    // that both of these methods call clearDesiredHelpLocation() when
    // they finish.  setBaseURI() will set the URI used to resolve links
    // to other files when html data is passed in with displayData().
    virtual void	 openFile(const char *path) = 0;
    virtual void	 openURL(const char *url) = 0;
    virtual void	 openHelpPath(const char *help_path) = 0;
    virtual void	 displayData(const char *data, bool is_html) = 0;

    /// Obtains a tool tip from the help page pointed to by the URL.
    virtual bool         getTooltip( const char * url, UT_String & tooltip ) =0;

    /// Obtains a tool tip for the specified parameter.
    virtual bool		 getTooltip(
				    const PRM_Parm *parm, UT_String &tooltip) 
				    = 0;
	
    /// Obtains a tool tip conained by the full help text. The url serves
    /// as a caching key.
    virtual bool        getParsedTooltip( const char * url_key,
					  const char * full_help,
					  UT_String & tooltip) = 0;

    // These methods help pop up a helpcard for a specific operator,
    // state, or tool. The help text associated with the entity can come
    // from the central help server, or if no such help exists from the
    // URL provided by the entity or from the text internally defined by the op
    // (HDA/HDK) or the tool, or from a file on disk associated with the
    // operator or state.
    virtual void         openHelpCard( OP_Node * node ) = 0;
    virtual void         openHelpCard( 
			    OP_Operator * op, const char *node_path=nullptr ) = 0;
    virtual void         openHelpCard( BM_State * state ) = 0;
    virtual void         openHelpCard( SHLF_Tool * tool ) = 0;
    virtual void	 openHelpCard( const PYP_Interface * interface ) = 0;
    virtual void         openHelpCard( OP_GalleryEntry * entry ) = 0;
    virtual void         openHelpCard( const PRM_ScriptParm * prop ) = 0;

    // Obtains a URL that points to the help server web page for the given
    // operator, state, tool, or topic id.
    virtual void	 getHelpCardURL( OP_Operator * op, UT_String & str ) =0;
    virtual void         getHelpCardURL( BM_State * state, UT_String & url ) =0;
    virtual void	 getHelpCardURL( SHLF_Tool * tool, UT_String & url ) =0;
    virtual void	 getHelpCardURL( const PYP_Interface * interface,
					 UT_String & url ) = 0;
    virtual void         getHelpCardURL( OP_GalleryEntry * entry,
					 UT_String & url ) = 0;
    virtual void	 getHelpCardURL( const PRM_ScriptParm * prop,
					 UT_String & url ) = 0;
    virtual void	 getHelpCardURL( const char *helpTopicId, 
					 UT_String & url ) = 0;
    virtual void	 getHelpCardURL( const PRM_Parm * parm, 
					 UT_String &url) = 0;

    virtual void	 setBaseURI(const char *base_uri) = 0;
    virtual const UT_String &getBaseURI() const = 0;

    // Returns true if the help manager is using an external help server.
    // Returns false otherwise.
    virtual bool getUseExternalHelp() const = 0;

    // Sets whether the help manager should use an external help server or not.
    virtual void setUseExternalHelp(bool use_external) = 0;

    // Returns the base url for the external help server.
    // Returns "" if no external help server is used.
    virtual const UT_String &getExternalHelpURL() const = 0;

    // Sets the url to the external help server.
    virtual void setExternalHelpURL(const char *ext_help_url) = 0;

    // Returns the base URL for all of Houdini's help.
    virtual const UT_String &getHelpBaseURL() = 0;

    // Returns the url for the help startup page.
    virtual const UT_String &getStartupURL() = 0;

    // By setting the desired help location, any calls to openFile() or
    // displayData() that need to create a floating pane will create it
    // near the specified location.  Typically, the location is the mouse
    // location when the help button was pressed.  Call
    // clearDesiredHelpLocation() after the call to openFile() or displayData().
    void		 setDesiredHelpLocation(fpreal x, fpreal y);
    void		 clearDesiredHelpLocation();

    // When the help is displayed in a floating pane, the floating pane gets
    // a unique id.  When there is no floating help pane, the floating help
    // pane id is 0.  This help pane id lets the op help buttons decide
    // whether to display the help for an op or close the floating help pane.
    void		 setFloatingHelpPaneId(int pane_id)
    { myFloatingHelpPaneId = pane_id; }
    int			 getFloatingHelpPaneId() const
    { return myFloatingHelpPaneId; }
    virtual void	 closeFloatingPane() = 0;

    // Does this platform support html help?  This method is static so it
    // can be called from a non-graphical program, since UTsetHelpManager()
    // may not have been called.
    static bool		 platformSupportsHtmlHelp();

    /// Concatenates the operator table name, operator type name, some
    /// extra information into a help URL.
    static void		 constructOpBasedURL( const char *protocol,
				const char *optable, const char *optype,
				const char *extra, UT_String &url);

    /// Parses the help URL into the operator table name, operator type name,
    /// extra info string, given the url and the required protocol prefix.
    /// Returns true on success, and false otherwise.
    static bool		 parseOpBasedURL( const char * url, 
				const char *protocol,
				UT_String &optable, UT_String &optype,
				UT_String &extra );

    // These methods are used by UTgetHelpManager() and UTsetHelpManager().
    // You should never have to call them directly.
    static UT_HelpManager *getHelpManager() { return ourHelpManager; }
    static void		 setHelpManager(UT_HelpManager *help_manager);

protected:
    // This method is used by subclasses to determine if and where to position
    // the help location.
    bool getDesiredHelpLocation(float &x, float &y);

private:
    static UT_HelpManager	*ourHelpManager;

    float		 myDesiredHelpLocationX;
    float		 myDesiredHelpLocationY;
    int			 myFloatingHelpPaneId;
};

static inline UT_HelpManager *UTgetHelpManager()
{ return UT_HelpManager::getHelpManager(); }

static inline void UTsetHelpManager(UT_HelpManager *help_manager)
{ UT_HelpManager::setHelpManager(help_manager); }

#endif

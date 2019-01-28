/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        OP3D_SelectorSupport.h
 *
 * COMMENTS:
 * 	OP3D_SelectorSupport is designed to provide a selector framework 
 *	for python state hosts. Classes deriving from OP3D_SelectorSupport 
 *	provides specific selector implementation e.g. OP3D_GeometrySelectorSupport
 */

#ifndef OP3D_SelectorSupport_h
#define OP3D_SelectorSupport_h

#include "OP3D_API.h"

#include <DM/DM_Defines.h>
#include <UI/UI_Object.h>
#include <UI/UI_Value.h>
#include <PI/PI_PythonState.h>

class OP3D_InputSelectorBase;
class OP3D_ScriptSelectStateInfoPtr;
class UI_Event;
class OP_Node;
class BM_Viewport;
class BM_View;
class UI_Feel;

class OP3D_API OP3D_SelectorSupport : public UI_Object
{
    public:
    using Selector = OP3D_InputSelectorBase;

    // Interface typically implemented by python state hosts to interact
    // with a selector
    struct Handler
    {
	Handler() = default;
	virtual ~Handler() = default;

	virtual OP_Node* opNode() = 0;
	virtual BM_View& selectorView() = 0;
	virtual void onStartSelector() = 0;
	virtual void onStopSelector() = 0;
	virtual void onSelection() = 0;
	virtual void updateSelectorPrompt() = 0;
    };

    OP3D_SelectorSupport(Handler* handler);
    virtual ~OP3D_SelectorSupport() = default;

    void init(PI_PythonState::Info::Selector& selector_info);

    void start();
    void stop();
    void updatePrompt();
    
    // the selector has been started and ready to use
    bool started() const;
    UI_Value& finishedValue();

    // the selector cannot be used
    bool useSelector() const;    

    int handleMouseEvent(UI_Event *event);
    int handleMouseWheelEvent(UI_Event *event);
    int handleKeyEvent(UI_Event *event, BM_Viewport &viewport);
    bool handleTransitoryKey(const UI_Event &event);
    bool handleHotkey(int key);
    OP3D_ScriptSelectStateInfoPtr& selectorInfo();

    // overridables
    virtual void handleSelection(UI_Event *) = 0;
    virtual Selector* selector() = 0;
    virtual void createSelector() = 0;
    virtual void termSelector() = 0;
    virtual void clearSelectorPtr() = 0;
    virtual void handleVolatileSelection() = 0;
    virtual int getToolboxCount() const = 0;
    virtual UI_Feel* getToolbox(int index) const = 0;

    protected:
    Handler* handler();
    Handler const* handler() const;
    void updateToolbox();

    private:
    bool			    myStartedStatus;
    UI_Value			    mySelectorFinishedValue;
    Handler*			    myHandler;
    PI_PythonState::Info::Selector* mySelectorInfo;
};

#endif // OP3D_SelectorSupport_h

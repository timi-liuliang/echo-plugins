#ifndef __RE_Visual_h__
#define __RE_Visual_h__

#include "RE_API.h"
#include "RE_Types.h"

#include <UT/UT_Lock.h>
#include <UT/UT_ThreadSpecificValue.h>

// Include this before including any QOpenGL files.
// This ensures that our OpenGL core profile API is included
// instead of the one shipped with Qt.
// TODO: We should look into moving over to Qt's OpenGL API
//	 (i.e. include QtGui/qopengl.h or maybe QtGui/QOpenGLFunctions.h
//	 instead of glcorearb.h)
#include "RE_OGL.h"

#include <QtGui/QOpenGLContext>
#include <QtGui/QWindow>
#include <QtWidgets/QWidget>

// Object names for QWidget and QWindow (drawable surface) objects used
// for OpenGL rendering.  Note that the drawable surface name must be exactly
// equal to the drawable name plus "Window" as the suffix.  This is a
// restriction of the QWidgetWindow class.
#define RE_GL_DRAWABLE_OBJ_NAME "RE_GLDrawable"
#define RE_GL_DRAWABLE_SURFACE_OBJ_NAME RE_GL_DRAWABLE_OBJ_NAME "Window"

#define RE_GL_DRAWABLE_WRAPPER_OBJ_NAME RE_GL_DRAWABLE_OBJ_NAME "Wrapper"

class RE_Server;
class RE_Window;

class RE_GLDrawable;
class QGLFormat;
class QSurfaceFormat;

class RE_GLContext : public QOpenGLContext
{
public:
    RE_GLContext() :
	myDrawable(NULL)
    {}

    ~RE_GLContext() {}

    RE_GLDrawable *drawable() const { return myDrawable; }
    void setDrawable(RE_GLDrawable *drawable) { myDrawable = drawable; }

private:
    RE_GLDrawable *myDrawable;
};

/// GL drawable for Qt5.
/// It's basically a plain Qt widget that manages a GL context.
class RE_GLDrawable : public QWidget
{
public:
    RE_GLDrawable(const QSurfaceFormat &format)
	: QWidget()
    {
	myContext.setDrawable(this);
	myContext.setFormat(format);

	init_();
    }

    RE_GLDrawable(
	const QSurfaceFormat &format, QWidget *parent, 
	QOpenGLContext *sharedContext)
	: QWidget(parent)
    {
	myContext.setDrawable(this);
	myContext.setFormat(format);
	if (sharedContext)
	    myContext.setShareContext(sharedContext);

	init_();
    }

    virtual ~RE_GLDrawable() {}

    RE_GLContext *context() { return &myContext; }

    virtual QPaintEngine *paintEngine() const { return nullptr; };
    static const char *widgetTag() { return RE_GL_DRAWABLE_OBJ_NAME; }

    /// Return the string tag used to identify whether a QWindow
    /// is used a drawable surface for Houdini.
    static const char * drawableSurfaceTag() 
	{ return RE_GL_DRAWABLE_SURFACE_OBJ_NAME; }

    /// Return the string tag used to identify whether a QWidget
    /// is the wrapper widget of a GL drawable in Houdini.
    static const char *widgetWrapperTag()
	{ return RE_GL_DRAWABLE_WRAPPER_OBJ_NAME; }

protected:
    virtual void paintEvent(QPaintEvent *event) {}
    virtual void resizeEvent(QResizeEvent *event) {}
    virtual void mousePressEvent(QMouseEvent *event)
    {
	setFocus();
	QWidget::mousePressEvent(event);
    }

private:
    void init_()
    {
	// Neat little trick to force Qt to create the platform drawable without
	// showing it.  We need the platform drawable to exist before doing
	// any OpenGL rendering.
	//
	// Note that we have to resize the window handle (surface) to keep its
	// geometry in sync with the GL widget.  This must be done *before* the
	// call to winId() or else the underlying platform window (surface)
	// will be initialized with a default size assigned by Qt.
	(void)winId();
	UT_ASSERT(windowHandle());
	windowHandle()->resize(size());
	(void)windowHandle()->winId();

	// Tag the drawable surface.
	windowHandle()->setObjectName(RE_GLDrawable::drawableSurfaceTag());

	// Set these attributes to prevent Qt from trying to fill in the
	// background with either black or white (system-dependent).
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	UT_IF_ASSERT(bool created =) myContext.create();
	UT_ASSERT(created && myContext.isValid());

	// Tag ourself so that we can be identified in the event loop.
	setObjectName(widgetTag());
	setAcceptDrops(true);
    }

    RE_GLContext myContext;
};

class RE_API RE_Visual
{
public:
    RE_Visual(RE_DisplayMode newmode);
    ~RE_Visual();

    RE_OGLContext createContext(RE_Window *win) const;
    void	 deleteContext(RE_OGLContext) const;

    RE_OGLContext grabContext(RE_Window *win, bool attach = true);
    void	returnContext(RE_Window *win, RE_OGLContext ctx);

    // isMode() returns true on an exact match. isCompatibleWithMode() returns
    // true if this visual is equal to or a superset of the features in 'match'
    bool	isMode(RE_DisplayMode match) const;
    bool	isCompatibleWithMode(RE_DisplayMode match) const;

    bool	isRGB() const;

    /// Returns the display mode used for the request to create this visual.
    RE_DisplayMode getMode() const { return myVisualMode; }

    const QSurfaceFormat *	getPixelFormat() const;
    static QOpenGLContext *	getSharedGLContext();
    static void			initDefaultGLFormat();

    static bool	isDetachedContext(RE_OGLContext);

    /// Return the tag we use to identify our GL widgets.
    /// This is useful when handling events in the event loop.
    static const char *		glWidgetTag();

    /// Return the tag we use to identify our GL widget wrappers.
    /// This is useful when handling events in the event loop.
    static const char *		glWidgetWrapperTag();

    /// Return the window widget that is bound to the specified GL widget.
    static QWidget *		ownerWindowWidget(QWidget *gl_widget);

    bool		       canSupportQuadBufferStereo(OGLDrawable drawable);

private:
    void			initPixelFormat_(RE_DisplayMode mode);
    void			createSharedGLWidget_();
    void			testGLVersions_();
    bool			canCreateGLContext_(
				    int gl_major, int gl_minor, 
				    bool use_core_profile);

    RE_DisplayMode	    myVisualMode;

    UT_Lock		    myContextLock;
    UT_Array<RE_OGLContext> myVisibleContexts;

    // This widget is for stashing away returned GL widgets by containing them
    // as children.  This causes the GL widgets to be parented at all times so
    // that their window flags do not change.
    QWidget *myStashWidget;

    QSurfaceFormat *		myPixelFormat;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Shared.h ( Object Library, C++)
 *
 * COMMENTS:
 *		These are structures that are needed by the rest of the
 *		object library.
 */

#ifndef __OBJ_Shared__
#define __OBJ_Shared__

#include "OBJ_API.h"
#include <UT/UT_Assert.h>
#include <PRM/PRM_Default.h>

// These macros are used by all objects as a simple way of copying a
// PRM_Template from one of the arrays below into their own PRM_Template
// array. The second form exports the parm to the toolbox.
class obj_SwitcherBuilder {
public:
    obj_SwitcherBuilder(PRM_Default *switcher, int maxsize)
    {
	myPrev = 0;
	mySwitcher = switcher;
	myFolder = -1;
	myMaxSize = maxsize;
	myCount = 0;
    }
    ~obj_SwitcherBuilder()
    {
	UT_ASSERT(!myPrev && theCurrentSwitcher != this);
	if (theCurrentSwitcher == this)
	    pop();
	if (myFolder >= 0 && myFolder != myMaxSize)
	{
	    UT_ASSERT(0 && "Forgot to call finish() didn't you?");
	    endFolder();
	}
    }
    void		 push()
			 {
			     UT_ASSERT(myPrev == 0);
			     myPrev = theCurrentSwitcher;
			     theCurrentSwitcher = this;
			 }
    static void		 pop()
			 {
			     UT_ASSERT(theCurrentSwitcher);
			     obj_SwitcherBuilder	*prev;
			     prev = theCurrentSwitcher->myPrev;
			     theCurrentSwitcher->myPrev = 0;
			     theCurrentSwitcher = prev;
			 }
    static void		 folder(const char *label)
			 {
			     UT_ASSERT(theCurrentSwitcher);
			     theCurrentSwitcher->startFolder(label);
			 }
    static void		 finish()
			 {
			     UT_ASSERT(theCurrentSwitcher);
			     theCurrentSwitcher->endFolder();
			     pop();
			 }
    static void		 add()
			 {
			     if (theCurrentSwitcher)
			     {
				 UT_ASSERT(theCurrentSwitcher->isValid());
				 theCurrentSwitcher->myCount++;
			     }
			 }
private:
    bool		 isValid() const
			 {
			     return myFolder >= 0 && myFolder < myMaxSize;
			 }
    void		 startFolder(const char *label)
			 {
			     if (myCount || myFolder >= 0)
			     {
				 UT_ASSERT(myCount && myFolder >= 0);
				 mySwitcher[myFolder].setOrdinal(myCount);
			     }
			     myFolder++;
			     UT_ASSERT(myFolder < myMaxSize);
			     mySwitcher[myFolder].setString(label);
			     myCount = 0;
			 }
    void		 endFolder()
			 {
			     // This is only called on the last folder
			     UT_ASSERT(myCount > 0);
			     UT_ASSERT(myFolder == myMaxSize-1);
			     mySwitcher[myFolder].setOrdinal(myCount);
			     myFolder = myMaxSize;
			 }
    static obj_SwitcherBuilder	*theCurrentSwitcher;
    obj_SwitcherBuilder		*myPrev;
    PRM_Default			*mySwitcher;
    int				 myFolder;
    int				 myCount;
    int				 myMaxSize;
};


#define SET_DECL_SWITCHER(builder, switcher)	\
	obj_SwitcherBuilder	builder(switcher, sizeof(switcher)/sizeof(PRM_Default));
#define SET_PUSH_SWITCHER(builder)	builder.push();
#define SET_FOLDER(label)		obj_SwitcherBuilder::folder(label);
#define SET_POP_SWITCHER()		obj_SwitcherBuilder::finish();
#define SET_TPLATE(src, idx) \
	theTemplate[i] = src[idx]; i++; obj_SwitcherBuilder::add();
#define SET_TPLATE_NOINC(src, idx) \
	theTemplate[i] = src[idx]; i++;
#define SET_ADDED_TPLATE()		obj_SwitcherBuilder::add();
#define SET_TPLATE_EX(src, idx) \
	theTemplate[i] = src[idx]; \
	obj_SwitcherBuilder::add(); \
	theTemplate[i].setExportLevel(PRM_Template::PRM_EXPORT_TBX); i++;

// Hide the Lookat and Path related params by default
#define SET_TPLATE_LEGACY_LOOKAT_PATH() \
	SET_TPLATE(OBJbaseTemplate, I_CONSTRAINTS_ON) \
	SET_TPLATE(OBJbaseTemplate, I_CONSTRAINTS) \
	SET_TPLATE(OBJbaseITemplate, I_LOOKAT) \
	SET_TPLATE(OBJbaseITemplate, I_LOOKUPOBJ) \
	SET_TPLATE(OBJbaseITemplate, I_LOOKUP) \
	SET_TPLATE(OBJbaseITemplate, I_PATHOBJ) \
	SET_TPLATE(OBJbaseITemplate, I_PATHROLL) \
	SET_TPLATE(OBJbaseITemplate, I_PATHPOS) \
	SET_TPLATE(OBJbaseITemplate, I_PATHUPARMTYPE) \
	SET_TPLATE(OBJbaseITemplate, I_PATHORIENT) \
	SET_TPLATE(OBJbaseITemplate, I_PATHUP) \
	SET_TPLATE(OBJbaseITemplate, I_PATHBANK)

// the 'I' version is called when old parms were already invisible
#define SET_TPLATEI_LEGACY_LOOKAT_PATH() \
	SET_TPLATE(OBJbaseTemplate, I_CONSTRAINTS_ON) \
	SET_TPLATE(OBJbaseTemplate, I_CONSTRAINTS) \
	SET_TPLATE(OBJbaseITemplate, I_LOOKAT) \
	SET_TPLATE(OBJbaseITemplate, I_LOOKUPOBJ) \
	SET_TPLATE(OBJbaseITemplate, I_LOOKUP) \
	SET_TPLATE(OBJbaseITemplate, I_PATHOBJ) \
	SET_TPLATE(OBJbaseITemplate, I_PATHROLL) \
	SET_TPLATE(OBJbaseITemplate, I_PATHPOS) \
	SET_TPLATE(OBJbaseITemplate, I_PATHUPARMTYPE) \
	SET_TPLATE(OBJbaseITemplate, I_PATHORIENT) \
	SET_TPLATE(OBJbaseITemplate, I_PATHUP) \
	SET_TPLATE(OBJbaseITemplate, I_PATHBANK)


class PRM_Template;

OBJ_API extern PRM_Template	OBJbaseObsolete[];
OBJ_API extern PRM_Template	OBJboneObsolete[];
OBJ_API extern PRM_Template	OBJdopnetObsolete[];
OBJ_API extern PRM_Template	OBJgeoObsolete[];
OBJ_API extern PRM_Template	OBJcamObsolete[];
OBJ_API extern PRM_Template	OBJfogObsolete[];
OBJ_API extern PRM_Template	OBJambientObsolete[];
OBJ_API extern PRM_Template	OBJlightObsolete[];
OBJ_API extern PRM_Template	OBJfetchObsolete[];
OBJ_API extern PRM_Template	OBJsoundObsolete[];
OBJ_API extern PRM_Template	OBJmicrophoneObsolete[];
OBJ_API extern PRM_Template	OBJhandleObsolete[];
OBJ_API extern PRM_Template	OBJblendObsolete[];

OBJ_API extern PRM_Template	OBJbaseTemplate[];
OBJ_API extern PRM_Template	OBJgeoTemplate[];
OBJ_API extern PRM_Template	OBJnullTemplate[];
OBJ_API extern PRM_Template	OBJboneTemplate[];
OBJ_API extern PRM_Template	OBJcamSwitchTemplate[];
OBJ_API extern PRM_Template	OBJstereoCameraTemplate[];
OBJ_API extern PRM_Template	OBJambientTemplate[];
OBJ_API extern PRM_Template	OBJfogTemplate[];
OBJ_API extern PRM_Template	OBJhandleTemplate[];
OBJ_API extern PRM_Template	OBJblendTemplate[];
OBJ_API extern PRM_Template	OBJsoundTemplate[];
OBJ_API extern PRM_Template	OBJmicrophoneTemplate[];
OBJ_API extern PRM_Template	OBJsubnetTemplate[];
OBJ_API extern PRM_Template	OBJfetchTemplate[];
OBJ_API extern PRM_Template	OBJextractgeoTemplate[];
OBJ_API extern PRM_Template	OBJstickyTemplate[];
OBJ_API extern PRM_Template	OBJstickyBlendTemplate[];
OBJ_API extern PRM_Template	OBJstickySharedTemplate[];
OBJ_API extern PRM_Template	OBJdopnetTemplate[];
OBJ_API extern PRM_Template	OBJrivetTemplate[];
OBJ_API extern PRM_Template	OBJmuscleTemplate[];

OBJ_API extern PRM_Template	OBJbaseITemplate[];
OBJ_API extern PRM_Template	OBJgeoITemplate[];
OBJ_API extern PRM_Template	OBJnullITemplate[];
OBJ_API extern PRM_Template	OBJboneITemplate[];
OBJ_API extern PRM_Template	OBJcamSwitchITemplate[];
OBJ_API extern PRM_Template	OBJstereoCameraITemplate[];
OBJ_API extern PRM_Template	OBJambientITemplate[];
OBJ_API extern PRM_Template	OBJfogITemplate[];
OBJ_API extern PRM_Template	OBJhandleITemplate[];
OBJ_API extern PRM_Template	OBJblendITemplate[];
OBJ_API extern PRM_Template	OBJsoundITemplate[];
OBJ_API extern PRM_Template	OBJmicrophoneITemplate[];
OBJ_API extern PRM_Template	OBJsubnetITemplate[];
OBJ_API extern PRM_Template	OBJfetchITemplate[];
OBJ_API extern PRM_Template	OBJstickyITemplate[];
OBJ_API extern PRM_Template	OBJstickySharedITemplate[];
OBJ_API extern PRM_Template	OBJstickyBlendITemplate[];
OBJ_API extern PRM_Template	OBJdopnetITemplate[];
OBJ_API extern PRM_Template	OBJrivetITemplate[];
OBJ_API extern PRM_Template	OBJmuscleITemplate[];

OBJ_API extern const char	*displaySopToken;
OBJ_API extern const char	*renderSopToken;
OBJ_API extern const char	*boundingBoxToken;
OBJ_API extern const char	*boundingSphereToken;
OBJ_API extern const char	*input1ObjectToken;
OBJ_API extern const char	*input2ObjectToken;
OBJ_API extern const char	*input3ObjectToken;
OBJ_API extern const char	*input4ObjectToken;

#endif

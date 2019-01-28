/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RAY library (C++)
 *
 * COMMENTS:	This contains styler information, along with the parent
 *		styler and subject used to create the styler. This allows
 *		updating the parent styler and automatically rebuilding the
 *		child styler.
 *
 */

#ifndef __RAY_StylerInfo_H__
#define __RAY_StylerInfo_H__

#include "RAY_API.h"
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_SharedPtr.h>
#include <STY/STY_Styler.h>
#include <STY/STY_Subject.h>
#include <GSTY/GSTY_SubjectPrimGroup.h>

class RAY_API RAY_StylerInfo
{
public:
    using SharedPrimGroup = UT_SharedPtr<GSTY_SubjectPrimGroup>;

			 RAY_StylerInfo();
			 RAY_StylerInfo(const STY_Styler &parent_styler,
				const STY_SubjectHandle &subject);
			 RAY_StylerInfo(const STY_Styler &parent_styler,
				const UT_Array<STY_SubjectHandle> &subjects);
			~RAY_StylerInfo();

    const STY_Styler	&getStyler() const;
    void		 updateStyler(const STY_Styler &new_parent_styler);
    void		 setParentAndSubject(const STY_Styler &styler,
				const STY_SubjectHandle &subject);
    void		 setParentAndSubjects(const STY_Styler &styler,
				const UT_Array<STY_SubjectHandle> &subjects);
    void		 addSubject(const STY_SubjectHandle &subject);
    void		 clear();

    void		setGroupSharingHolder(const SharedPrimGroup &g)
			    { myGroupSharingHolder = g; }

private:
    class StylerInfoData;
    class StylerInfoData
	: public UT_IntrusiveRefCounter<StylerInfoData>
    {
	public:
	    StylerInfoData();
	    ~StylerInfoData();
	    void			 rebuildStyler();

	    STY_Styler			myParentStyler;
	    UT_Array<STY_SubjectHandle> mySubjects;
	    STY_Styler			myStyler;
    };
    typedef UT_IntrusivePtr<StylerInfoData>	StylerInfoDataHandle;
    SharedPrimGroup				myGroupSharingHolder;

    void		 createUnsharedStylerInfoData()
			 {
			    if (!myData)
				myData.reset(new StylerInfoData());
			    else if (myData->use_count() > 1)
				myData.reset(new StylerInfoData(*myData));
			 }

    StylerInfoDataHandle myData;
};

#endif

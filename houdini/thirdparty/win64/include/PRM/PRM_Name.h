/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Name.h (Parameter Library)
 *
 * COMMENTS:
 *		This class is used to store names of things which may have
 *		a nice "label" name, and a "token" (more parsable) name.
 */

#ifndef __PRM_Label__
#define __PRM_Label__

#include "PRM_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Types.h>

class UT_WorkBuffer;

class PRM_API PRM_Name
{
public:

    /// Default PRM_Name constructor is a sentinel value
		 PRM_Name()
		    : myToken(UT_StringHolder::theSentinel)
		    , myLabel(UT_StringHolder::theSentinel)
		    , myFlags(0)
		 {
		 }

    ///  If thelabel is set to 0, then the thetoken is used as 
    ///  the label. Currently the flags parameter is only used for
    ///  the parm expression flag. Set theflags to 1 to make the parm
    ///  default to expression mode.
    ///  @note This constructor will only REFERENCE the given strings
    explicit	 PRM_Name(const char *thetoken,
			  const char *thelabel = nullptr,
			  int	      theflags = 0)
		     : myToken(thetoken
			 ? UT_StringHolder(UT_StringHolder::REFERENCE, thetoken)
			 : UT_StringHolder::theSentinel)
		     , myLabel(thelabel
			 ? UT_StringHolder(UT_StringHolder::REFERENCE, thelabel)
			 : UT_StringHolder::theSentinel)
		     , myFlags(theflags)
		 {
		 }

    enum PRM_NameCopy { COPY };

    /// Constructor for doing deep copies of the strings
    explicit	 PRM_Name(PRM_NameCopy,
			  const UT_StringHolder &thetoken,
			  const UT_StringHolder &thelabel,
			  int	      theflags = 0)
		     : myToken(thetoken)
		     , myLabel(thelabel)
		     , myFlags(theflags)
		 {
		 }

    /// Check if this is a sentinel
    bool	 isSentinel() const
		 {
		     return myToken.isSentinel();
		 }

    /// Set to a sentinel value
    void	 setAsSentinel()
		 {
		     myToken = UT_StringHolder::theSentinel;
		     myLabel = UT_StringHolder::theSentinel;
		 }

    /// Set to a separator value
    void	 setAsSeparator();

    const char	*getToken() const
		 {
		     return myToken.isSentinel() ? nullptr : myToken.c_str();
		 }
    const char	*getLabel() const
		 {
		    return myLabel.isSentinel() ? getToken() : myLabel.c_str();
		 }
    int		 getExpressionFlag() const	{ return myFlags; }
    unsigned	 getHash() const		{ return myToken.hash(); }

    /// Use this for hash tables, might be the sentinel value
    const UT_StringRef &getTokenRef() const	{ return myToken; }

    /// Get label UT_StringRef &, might be the sentinel value
    const UT_StringRef &getLabelRef() const	{ return myLabel; }

    /// Do a deep copy of its internal references
    void	 harden()
		 {
		    if (!myToken.isSentinel())
			myToken = UT_StringHolder(myToken);
		    if (!myLabel.isSentinel())
			myLabel = UT_StringHolder(myLabel);
		 }
    /// Sets the token, doing a deep copy
    void	 setToken(const char *s)
		 {
		     myToken = s ? UT_StringHolder(s)
				 : UT_StringHolder::theSentinel;
		 }
    /// Sets the label, doing a deep copy
    void	 setLabel(const char *s)
		 {
		     myLabel = s ? UT_StringHolder(s)
				 : UT_StringHolder::theSentinel;
		 }
    /// Sets the token and label, doing deep copies
    void	 setTokenAndLabel(const char *token, const char *label)
		 {
		     setToken(token);
		     setLabel(label);
		 }
    /// Sets the token and label, doing shallow copies.
    /// @note There's no way to use this method to set sentinels, use
    ///       setTokenAndLabel(nullptr, nullptr) instead.
    void	 setTokenAndLabelRef(const UT_StringRef &token,
				     const UT_StringRef &label)
		 {
		     myToken = token;
		     myLabel = label;
		 }

    void	 instance(const int *instance, int num);
    void	 instance(const UT_IntArray &instance_num);

    static void	 instanceToken(UT_String &token, const int *instance, int num);
    static void	 instanceToken(UT_String &token,
			       const UT_IntArray &instance_num);
    static void	 instanceToken(UT_WorkBuffer &result, const UT_StringRef &token,
			       const int *instance, int num);
    static void	 instanceToken(UT_WorkBuffer &result, const UT_StringRef &token,
			       const UT_IntArray &instance_num);

    /// Returns if this a valid menu choice
    bool	 isValidChoice() const
		 {
		    if (isSentinel())
			return false;
		    return myToken.isstring() || myLabel.isstring();
		 }

    int64 getMemoryUsage(bool inclusive) const
    {
        exint mem = inclusive ? sizeof(*this) : 0;
        mem += myToken.getMemoryUsage(false);
        mem += myLabel.getMemoryUsage(false);
        return mem;
    }

    static const char *const mySeparator;

private:
    UT_StringRef    myToken;
    UT_StringRef    myLabel;
    int		    myFlags;
};

// Overload for custom formatting of PRM_Name with UTformat.
PRM_API size_t format(char *buffer, size_t buffer_size, const PRM_Name &v);

#endif

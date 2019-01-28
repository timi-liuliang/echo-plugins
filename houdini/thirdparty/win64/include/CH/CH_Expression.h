/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Channel Library (C++)
 *
 * COMMENTS:	This might want to be moved into the utility library
 *		if possible.
 *
 * CLASSES:
 *	CH_ExprVariable:
 *		Used for looking up variable names in expressions
 *	CH_Expression:
 *		A class for evaluating expressions.
 * 
 */

#ifndef __CH_Expression_h__
#define __CH_Expression_h__

#include "CH_API.h"
#include "CH_ExprLanguage.h"
#include <EXPR/EXPR.h>
#include <UT/UT_String.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class UT_IStream;
class EV_EXPRESSION;
class PY_CompiledCode;

class CH_API CH_Expression
{
public:
    CH_Expression(const char *expr, CH_ExprLanguage language);
    ~CH_Expression();

    int64		 getMemoryUsage(bool inclusive) const;

    void		 save(std::ostream &os, int binary, bool compiled) const;
    bool		 load(UT_IStream &is);

    const char		*getExpression() const;
    void		 setExpression(const char *expr,
				       CH_ExprLanguage language);
    unsigned		 getExpressionFlag() const;
    void		 setExpressionFlag(int flag);

    CH_ExprLanguage	 getLanguage() const;
    void		 setLanguage(CH_ExprLanguage language);

    void		 unresolveLocalVars(int thread);

    unsigned		 isCompiled() const;
    bool		 isPureCompiled() const;

    /// Evaluate as a fpreal
    fpreal		 evaluate(int thread);
    /// Evaluate as a string
    void		 evaluateString(UT_String &result, int thread);

    void		 dirtyExprCache();

    int			 modified() const;
    int			 usesInValue();
    int			 usesOutValue();
    int			 usesValues();
    int			 usesSlopes();
    int			 usesAccels();
    int			 usesKnots();

    void		 buildOpDependencies(void *ref_id, int thread);
    int			 changeOpRef(const char *new_fullpath,
				     const char *old_fullpath,
				     const char *old_cwd,
				     const char *chan_name,
				     const char *old_chan_name,
				     int thread);

    void		*getSingleFunctionInstanceData(
			    EV_FUNCTION *func, int thread, UT_String &argument);

    int			 findString(const char *str, bool fullword,
				    bool usewildcards) const;
    int			 changeString(const char *from, const char *to,
				      bool fullword, int thread);

    const char		*getReferencePath() const;
    void		 setReferencePath(const char *path);

    void		 forceStringType();

    CH_Expression	&operator=(const CH_Expression &from)
			 {
			    const char	*src;
			    if ((src = from.getExpression()))
				setExpression(src, from.getLanguage());
			    return *this;
			 }
    
    operator const char *() const { return getExpression(); }

    // This static method is called to add time dependency flags to the
    // currently evaluating expression, flag it as using input slopes, etc.
    // The flags use thread-local storage, so python code making HOM calls
    // from different threads won't accidentally set each others' flags.
    static void 	 addExprFlags(int flags, int thread);

    // This method lets you get at the flags directly.  The Python sop,
    // for example, needs to know if the code set the time-dependent flag
    // when it evaluated, so it needs to clear the flags beforehand, check
    // them after the cook, and restore them.
    static int		&exprFlags();

    // THIS FUNCTION IS DEPRECATED!! Only left here for backwards
    // compatibility. The proper way to mark expression evaluation as time
    // dependent is to turn on the CH_EXPRTIME flag in the expression callback
    // using the new func_flags parameter like so: func_flags |= CH_EXPRTIME.
    //
    // Old Note: HDK users may rely on setTimeDependency(), even if we don't
    // call it directly.  It's a shortcut for addExprFlags(CH_EXPRTIME);
    static SYS_DEPRECATED(10.0) void setTimeDependency();

private:
    void changeExpr(const char *source, int thread);

    CH_ExprLanguage	 myLanguage;
    EV_EXPRESSION	*myExpr;
    PY_CompiledCode	*myPyExpr;
    UT_String		 myReferencePath;

    friend std::ostream	&operator<<(std::ostream &os, const CH_Expression &d)
			{
			    d.save(os, 0, false);
			    return os;
			}
};

inline unsigned
CH_Expression::getExpressionFlag(void) const
{
    return myExpr->getFlags();
}

inline void
CH_Expression::setExpressionFlag(int flag)
{
    myExpr->setFlags(flag);
}

inline int
CH_Expression::modified() const
{
    return !myExpr->isEvaluated();
}

#endif
